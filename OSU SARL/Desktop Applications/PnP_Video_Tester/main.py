import sys
from PyQt4 import QtCore, QtGui, uic
import signal

from VideoCore import PickAndPlateVideo

form_class = uic.loadUiType("form.ui")[0]  # Load the UI


#####################################
# PickAndPlateWindow Class Definition
#####################################
class PickAndPlateWindow(QtGui.QMainWindow, form_class):
    def __init__(self, parent=None):
        QtGui.QMainWindow.__init__(self, parent)

        # ########## Set up QT Application Window ##########
        self.setupUi(self)  # Has to be first call in class in order to link gui form objects

        # ########## Instantiation of program classes ##########
        self.video = PickAndPlateVideo(self)

        self.video.requested_image_ready_signal.connect(self.on_image_ready_slot)
        self.video.number_embryos_detected_signal.connect(self.on_detected_embryos_number_changed_slot)

    def on_image_ready_slot(self):
        try:
            self.detection_calibration_image_preview_label.setPixmap(
                QtGui.QPixmap.fromImage(self.video.settings_and_cal_qimage))
            self.video.images_displayed = True
            #print "Got here"
        except:
            print "Failed to show"

    def on_detected_embryos_number_changed_slot(self, detected, valid, pickable):
        self.detection_num_detected_label.setText(str(detected))
        self.detection_valid_detected_label.setText(str(valid))
        self.detection_pickable_detected_label.setText(str(pickable))

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal.SIG_DFL)  # This allows the keyboard interrupt kill to work  properly
    app = QtGui.QApplication(sys.argv)  # Create the base qt gui application
    myWindow = PickAndPlateWindow()  # Make a window in this application using the pnp MyWindowClass
    myWindow.show()  # Show the window in the application

    # screenFilter = TouchScreenEventFilter()  # Still might be needed if multiple presses from touchscreen breaks stuff
    # app.installEventFilter(screenFilter)

    app.exec_()  # Execute launching of the application