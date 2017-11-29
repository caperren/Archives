#!/usr/bin/env python

import rospy
import math
import tf
from tf.transformations import euler_from_quaternion
import message_filters
from pprint import pprint
import time
# The laser scan message
from sensor_msgs.msg import LaserScan

# The odometry message
from nav_msgs.msg import Odometry

# the velocity command message
from geometry_msgs.msg import Twist

# instantiate global variables "globalOdom"
globalOdom = Odometry()

# global pi - this may come in handy
pi = math.pi


def angle_between_points((x_1, y_1), (x_2, y_2)):
    return math.atan2(y_2 - y_1, x_2 - x_1)


# method to control the robot
def callback(scan, odom):
    # the odometry parameter should be global
    global globalOdom
    globalOdom = odom

    # make a new twist message
    command = Twist()

    # Fill in the fields.  Field values are unspecified 
    # until they are actually assigned. The Twist message 
    # holds linear and angular velocities.
    command.linear.x = 0.0
    command.linear.y = 0.0
    command.linear.z = 0.0
    command.angular.x = 0.0
    command.angular.y = 0.0
    command.angular.z = 0.0

    # get goal x and y locations from the launch file
    goalX = rospy.get_param('hw2/goalX', 0.0)
    goalY = rospy.get_param('hw2/goalY', 0.0)
    
    # find current (x,y) position of robot based on odometry
    currentX = globalOdom.pose.pose.position.x
    currentY = globalOdom.pose.pose.position.y

    # find current orientation of robot based on odometry (quaternion coordinates)
    xOr = globalOdom.pose.pose.orientation.x
    yOr = globalOdom.pose.pose.orientation.y
    zOr = globalOdom.pose.pose.orientation.z
    wOr = globalOdom.pose.pose.orientation.w

    # find orientation of robot (Euler coordinates)
    (roll, pitch, yaw) = euler_from_quaternion([xOr, yOr, zOr, wOr])

    # find currentAngle of robot (equivalent to yaw)
    # now that you have yaw, the robot's pose is completely defined by (currentX, currentY, currentAngle)
    currentAngle = yaw

    # find laser scanner properties (min scan angle, max scan angle, scan angle increment)
    maxAngle = scan.angle_max
    minAngle = scan.angle_min
    angleIncrement = scan.angle_increment

    # find current laser angle, max scan length, distance array for all scans, and number of laser scans
    currentLaserTheta = minAngle
    maxScanLength = scan.range_max 
    distanceArray = scan.ranges
    numScans = len(distanceArray)

    # ######### My Modifications ##########
    # Get the angle from the robot to the goal position
    desired_angle = angle_between_points((currentX, currentY), (goalX, goalY))

    # Precision variables
    angle_offset = 0.05  # Handles how much deviance from our desired angle we can be
    goal_precision = 0.1  # Sets how close to the goal is determined to be the actual goal
    precision = 0.02  # Sets how forcefully we try to maintain a path

    min_theta = desired_angle - angle_offset  # Sets the min theta to be within from settings above
    max_theta = desired_angle + angle_offset  # Same but for max theta

    abs_min_theta = abs(currentAngle - min_theta)  # Get the absolute value of how far off we are, in regards to limits
    abs_max_theta = abs(currentAngle - max_theta)  # Same as previous

    # If we've reached the goal, within our goal precision, stop the rover from moving
    if abs(goalY - currentY) < goal_precision and abs(goalX - currentX) < goal_precision:
        return

    # Otherwise, make the rover move forward always
    command.linear.x = 0.35

    # Check if our current angle is within our precision for direction
    # This is what sets the rover heading towards the goal
    if abs_min_theta > precision and abs_max_theta > precision:
        # If we're not, turn left or right depending on which limit we're closer to
        if abs_max_theta > abs_min_theta:
            command.angular.z = 0.25  # Counter-clockwise
        else:
            command.angular.z = -0.25  # Clockwise

    # Sets up obstacle avoidance
    num_scans = 400  # Set the number of samples around the center-forward of the rover's vision
    threshold = 1.0  # Set a threshold for how close objects are allowed to be
    results = ["Good" for _ in range(num_scans)]  # Set up an array for the number of scans, all set to good
    middle_point = numScans / 2  # Get the middle point for the scan information we have

    # Get our limits for upper and lower scan boundaries based on the scan inputs and num_scans
    lower_range = middle_point - (num_scans / 2)
    upper_range = middle_point + (num_scans / 2)

    position = 0  # Variable to keep track of the results array position

    # Go through our scans in the range, and mark any that are out of bounds.
    for current_scan in range(lower_range, upper_range):
        if distanceArray[current_scan] < threshold:
            results[position] = "Bad"
        position += 1

    # Split the results up into measurements from the left vs right side of the rover, counting how many sensor
    # readings on each side were too close to an object
    right_count = results[: (len(results) / 2) - 10].count("Bad")
    left_count = results[(len(results) / 2) + 10:].count("Bad")

    # Debug statements while testing
    # print time.time()
    # print "Left: %s Right: %s" % (left_count, right_count)

    # Threshold for how many positions on one side of the rover need to be too close to an object before avoiding
    count_thresh = 20

    # How forcefully to turn the rover one direction or another when it is trying to avoid obstacles
    increment = 0.8

    # If either side of the rover has too many counts for an object being too closer
    if left_count > count_thresh or right_count > count_thresh:
        # Turn the rover left or right, depending on which side has more counts than the other
        if left_count and right_count:
            if left_count > right_count or right_count == left_count:
                command.angular.z += -increment
            else:
                command.angular.z += increment
        elif left_count:
            command.angular.z += -increment
        elif right_count:
            command.angular.z += increment

    # Send the commands
    pub.publish(command)

# main function call
if __name__ == "__main__":
    # Initialize the node
    rospy.init_node('lab2', log_level=rospy.DEBUG)

    # subscribe to laser scan message
    sub = message_filters.Subscriber('base_scan', LaserScan)

    # subscribe to odometry message    
    sub2 = message_filters.Subscriber('odom', Odometry)

    # synchronize laser scan and odometry data
    ts = message_filters.TimeSynchronizer([sub, sub2], 10)
    ts.registerCallback(callback)

    # publish twist message
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)

    # Turn control over to ROS
    rospy.spin()

