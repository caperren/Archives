"""
    This file contains the PickAndPlateVideo sub-class as part of the Framework Class
    This class handles initializing and interfacing with the
"""

__author__ = "Corwin Perren"
__copyright__ = "None"
__credits__ = [""]
__license__ = "GPL (GNU General Public License)"
__version__ = "0.1 Alpha"
__maintainer__ = "Corwin Perren"
__email__ = "caperren@caperren.com"
__status__ = "Development"

# This file is part of "Pick And Plate".
#
# "Pick And Plate" is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# "Pick And Plate" is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with "Pick And Plate".  If not, see <http://www.gnu.org/licenses/>.

#####################################
# Imports
#####################################
# Python native imports
from PyQt4 import QtCore
import cv2
import numpy
import qimage2ndarray
from math import isnan, pow, sqrt
from datetime import datetime


class PickAndPlateVideo(QtCore.QThread):

    requested_image_ready_signal = QtCore.pyqtSignal()

    number_embryos_detected_signal = QtCore.pyqtSignal(int, int, int)

    def __init__(self, main_window):
        QtCore.QThread.__init__(self)

        # ########## Reference to highest level window ##########
        self.main_window = main_window

        # ########## Thread flags ##########
        self.not_abort_flag = True

        self.min_thresh = 0
        self.x_res = 0
        self.y_res = 0
        self.x_center = 0
        self.y_center = 0
        self.crop_dim_half = 0
        self.usable_offset = 95

        self.raw_frame = None

        self.images_displayed = False

        self.settings_and_cal_qimage = None
        self.keypoints = None

        self.current_params = cv2.SimpleBlobDetector_Params()

        self.raw_frame = cv2.imread('chorionated/two/chor2.png', cv2.IMREAD_COLOR)

        # rows, cols, _unused = self.raw_frame.shape
        #
        # cv2.line(self.raw_frame, (0, rows / 2), (cols, rows / 2), (0, 255, 255), 2)
        # cv2.line(self.raw_frame, (cols / 2, 0), (cols / 2, rows), (0, 255, 255), 2)
        #
        # rotation_matrix = cv2.getRotationMatrix2D((cols / 2, rows / 2), 2, 1)
        # self.raw_frame = cv2.warpAffine(self.raw_frame, rotation_matrix, (cols, rows))

        # ########## Make signal/slot connections ##########
        self.connect_signals_to_slots()

        # ########## Start timer ##########
        self.start()

    def connect_signals_to_slots(self):
        pass

    def run(self):
        while self.not_abort_flag:
            self.show_detection_calibration()

    def show_detection_calibration(self):
        self.setup_blob_params()

        frame = cv2.cvtColor(self.raw_frame, cv2.COLOR_RGB2GRAY)
        #return_val, frame = cv2.threshold(frame, self.min_thresh, 255, cv2.cv.CV_THRESH_BINARY)

        mask_frame = numpy.zeros((self.y_res, self.x_res), numpy.uint8)
        cv2.circle(mask_frame, (self.x_center, self.y_center), (self.crop_dim_half - self.usable_offset), 255, -1)

        frame = cv2.bitwise_and(frame, mask_frame)

        frame = self.masked_detect_and_overlay(frame, frame)

        frame = self.crop_image(frame)



        try:
            self.images_displayed = False
            resized = cv2.resize(frame, (1000, 1000))

            # cv2.line(resized, (0, 1000 / 2), (1000, 1000 / 2), (255, 0, 0), 1)
            # cv2.line(resized, (1000 / 2, 0), (1000 / 2, 1000), (255, 0, 0), 1)

            self.settings_and_cal_qimage = self.convert_to_qimage(resized)
            self.requested_image_ready_signal.emit()

            while not self.images_displayed:
                if not self.not_abort_flag:
                    break
                # print "Waiting"
                self.msleep(10)
        except:
            print "Failed to convert"

    def masked_detect_and_overlay(self, input_frame, overlay_frame):
        detector = cv2.SimpleBlobDetector(self.current_params)
        self.keypoints = detector.detect(input_frame)
        #output_frame = cv2.drawKeypoints(overlay_frame, self.keypoints, color=(255, 0, 0))

        # #### Test algorithm ######
        detected_clean = []
        valid_total = 0
        pickable_total = 0

        output_frame = cv2.cvtColor(overlay_frame, cv2.COLOR_GRAY2BGR)

        valid_embryos = self.get_valid_embryos(self.keypoints)
        pickable_embryos = self.get_pickable_embryos(valid_embryos)

        output_frame = self.draw_found(output_frame, valid_embryos, pickable_embryos)

        valid_total = len(valid_embryos)
        pickable_total = len(pickable_embryos)
        self.number_embryos_detected_signal.emit(len(self.keypoints), valid_total, pickable_total)

        return output_frame

    @staticmethod
    def draw_found(frame, valid, pickable):

        for embryo in valid:
            cv2.circle(frame, (int(embryo[0]), int(embryo[1])), int(embryo[2]), (255, 0, 0), -1, cv2.CV_AA)

        for embryo in pickable:
            cv2.circle(frame, (int(embryo[0]), int(embryo[1])), int(embryo[2]), (0, 255, 0), -1, cv2.CV_AA)

        return frame

    def get_valid_embryos(self, keypoints):
        valid = []

        for point in self.keypoints:
                if (not isnan(point.pt[0])) and (not isnan(point.pt[1])) and (not isnan(point.size)):
                    valid.append([point.pt[0], point.pt[1], point.size])

        return valid

    def get_pickable_embryos(self, valid):
        pickable = []
        min_between_embryos = self.main_window.detection_seperation_embryos_min_spin_box.value()
        max_embryo_dia = self.main_window.detection_embryo_size_max_spin_box.value()
        min_embryo_dia = self.main_window.detection_embryo_size_min_spin_box.value()

        for embryo in valid:
            x = embryo[0]
            y = embryo[1]
            dia = embryo[2]

            if (dia >= min_embryo_dia) and (dia <= max_embryo_dia):
                found_too_close = False
                for comp_embryo in valid:
                    c_x = comp_embryo[0]
                    c_y = comp_embryo[1]
                    c_dia = comp_embryo[2]

                    if (x == c_x) and (y == c_y):
                        pass
                    elif (sqrt(pow(abs(x-c_x), 2) + pow(abs(y-c_y), 2))-(dia/2)-(c_dia/2)) < min_between_embryos:
                        found_too_close = True
                        break
                if not found_too_close:
                    pickable.append(embryo)

        return pickable

    def setup_blob_params(self):

        min_blob_dist = self.main_window.detection_min_distance_between_blobs_spin_box.value()
        min_repeat = self.main_window.detection_min_repeatability_spin_box.value()
        blob_thresh_min = self.main_window.detection_blob_threshold_min_spin_box.value()
        blob_thresh_max = self.main_window.detection_blob_threshold_max_spin_box.value()
        blob_thresh_step = self.main_window.detection_blob_threshold_step_spin_box.value()
        blob_color_en = self.main_window.detection_blob_color_group_box.isChecked()
        blob_color_val = self.main_window.detection_blob_color_spin_box.value()
        blob_area_en = self.main_window.detection_blob_area_group_box.isChecked()
        blob_area_min = self.main_window.detection_blob_area_min_spin_box.value()
        blob_area_max = self.main_window.detection_blob_area_max_spin_box.value()
        blob_circ_en = self.main_window.detection_blob_circularity_group_box.isChecked()
        blob_circ_min = self.main_window.detection_blob_circularity_min_spin_box.value()
        blob_circ_max = self.main_window.detection_blob_circularity_max_spin_box.value()
        blob_conv_en = self.main_window.detection_blob_convexity_group_box.isChecked()
        blob_conv_min = self.main_window.detection_blob_convexity_min_spin_box.value()
        blob_conv_max = self.main_window.detection_blob_convexity_max_spin_box.value()
        blob_inertia_en = self.main_window.detection_blob_inertia_group_box.isChecked()
        blob_inertia_min = self.main_window.detection_blob_inertia_min_spin_box.value()
        blob_inertia_max = self.main_window.detection_blob_inertia_max_spin_box.value()

        self.current_params.minDistBetweenBlobs = min_blob_dist
        self.current_params.minRepeatability = min_repeat
        self.current_params.minThreshold = blob_thresh_min
        self.current_params.maxThreshold = blob_thresh_max
        self.current_params.thresholdStep = blob_thresh_step

        if blob_color_en:
            self.current_params.filterByColor = True
            self.current_params.blobColor = blob_color_val
        else:
            self.current_params.filterByColor = False

        if blob_area_en:
            self.current_params.filterByArea = True
            self.current_params.minArea = blob_area_min
            self.current_params.maxArea = blob_area_max
        else:
            self.current_params.filterByArea = False

        if blob_circ_en:
            self.current_params.filterByCircularity = True
            self.current_params.minCircularity = blob_circ_min
            self.current_params.maxCircularity = blob_circ_max
        else:
            self.current_params.filterByCircularity = False

        if blob_conv_en:
            self.current_params.filterByConvexity = True
            self.current_params.minConvexity = blob_conv_min
            self.current_params.maxConvexity = blob_conv_max
        else:
            self.current_params.filterByConvexity = False

        if blob_inertia_en:
            self.current_params.filterByInertia = True
            self.current_params.minInertiaRatio = blob_inertia_min
            self.current_params.maxInertiaRatio = blob_inertia_max
        else:
            self.current_params.filterByInertia = False

        self.min_thresh = self.main_window.detection_min_binary_threshold_spin_box.value()

        self.x_res = 1920
        self.y_res = 1080
        self.x_center = 958
        self.y_center = 559
        self.crop_dim_half = 548/2
        self.usable_offset = 83

    @staticmethod
    def convert_to_qimage(input_matrix):
        return qimage2ndarray.array2qimage(input_matrix)

    def crop_image(self, input_matrix):
        x1 = self.x_center - self.crop_dim_half
        x2 = self.x_center + self.crop_dim_half
        y1 = self.y_center - self.crop_dim_half
        y2 = self.y_center + self.crop_dim_half

        cropped = input_matrix[y1:y2, x1:x2]

        return cropped

    def images_displayed_slot(self):
        self.images_displayed = True