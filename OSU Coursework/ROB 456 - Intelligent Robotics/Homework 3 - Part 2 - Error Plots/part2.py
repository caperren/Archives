import csv
import matplotlib.pyplot as plotter

base_path = "/home/caperren/catkin_ws/src/rob456_hw3/logging_outputs"
odom_path = base_path + "/" + "odomlogfile.txt"
slam_path = base_path + "/" + "slamlogfile.txt"

if __name__ == '__main__':
    odom_file = open(odom_path, "r")
    slam_path = open(slam_path, "r")

    odom_csv = csv.reader(odom_file, delimiter=',')
    slam_csv = csv.reader(slam_path, delimiter=',')

    odom_time = []
    odom_x_error = []
    odom_y_error = []
    odom_theta_error = []

    for line in odom_csv:
        odom_time.append(line[0])
        odom_x_error.append(line[1])
        odom_y_error.append(line[2])
        odom_theta_error.append(line[3])

    slam_time = []
    slam_x_error = []
    slam_y_error = []
    slam_theta_error = []

    for line in slam_csv:
        slam_time.append(line[0])
        slam_x_error.append(line[1])
        slam_y_error.append(line[2])
        slam_theta_error.append(line[3])

    # Plot for odometry
    figure_object_odom, axes_object_odom = plotter.subplots()
    axes_object_odom.plot(odom_time, odom_x_error, label="X Error")
    axes_object_odom.plot(odom_time, odom_y_error, label="Y Error")

    axes_theta = axes_object_odom.twinx()
    axes_theta.plot(odom_time, odom_theta_error, label="Theta Error", color="r")
    axes_theta.legend()

    axes_object_odom.set_title("Odometery Error Graphs")
    axes_object_odom.set_xlabel("Time")
    axes_object_odom.set_ylabel("Error")
    axes_object_odom.legend(loc=4)
    axes_object_odom.autoscale(enable=True, axis='x', tight=True)

    figure_object_odom.savefig("odom_error_graphs.pdf", bbox_inches="tight")

    # Plot for slam
    figure_object_slam, axes_object_slam = plotter.subplots()
    axes_object_slam.plot(slam_time, slam_x_error, label="X Error")
    axes_object_slam.plot(slam_time, slam_y_error, label="Y Error")

    axes_theta = axes_object_slam.twinx()
    axes_theta.plot(slam_time, slam_theta_error, label="Theta Error", color="r")
    axes_theta.legend()
    # axes_theta.plot(slam_time, slam_theta_error, label="Theta Error")

    axes_object_slam.set_title("Slam Error Graphs")
    axes_object_slam.set_xlabel("Time")
    axes_object_slam.set_ylabel("Error")
    axes_object_slam.legend(loc=4)
    axes_object_slam.autoscale(enable=True, axis='x', tight=True)

    figure_object_slam.savefig("slam_error_graphs.pdf", bbox_inches="tight")

    plotter.show()
