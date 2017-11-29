import csv
from matplotlib import pyplot, patches
from math import sqrt

from heapq import *

CSV_PATH = "world.csv"

VAL_TO_COLOR = {
    0: "green",
    1: "red",
    -1: "blue"
}

EDGE_COST = 1

START_POSITION = (0, 0)
END_POSITION = (19, 19)


def import_csv_as_array(csv_path):
    csv_file = open(csv_path, "rU")  # Open the file

    csv_reader = csv.reader(csv_file)  # Put it through the csv reader

    # Loop through the csv lines and append them to an array
    output_array = []
    for line in csv_reader:
        output_array.append([int(col_val) for col_val in line])

    # Delete the csv reader and close the file
    del csv_reader
    csv_file.close()

    # Return our world map array
    return output_array


def plot_grid_map(grid_map, fig_save_path=None):
    # Make the plot
    figure_object, axes_object = pyplot.subplots()

    # Plot appropriately colored rectangles for each point on the map
    for y, row in enumerate(grid_map):
        for x, col in enumerate(row):
            axes_object.add_patch(patches.Rectangle((x, y), 1, 1, fill=True, color=VAL_TO_COLOR[col]))

    # Plot some x and y dotted lines to make it nicer to view the underlying grid
    for y in range(len(grid_map)):
        axes_object.plot([0, len(grid_map[0])], [y, y], color="black", alpha=0.75, linestyle=":")

    for x in range(len(grid_map[0])):
        axes_object.plot([x, x], [0, len(grid_map)], color="black", alpha=0.75, linestyle=":")

    # Set the y limit from len(grid_map) to 0 so it matches how the file looks in terms of the map
    axes_object.set_ylim([len(grid_map), 0])
    axes_object.autoscale(enable=True, tight=True)

    # If the optional argument to save to a file is added, output that file
    if fig_save_path:
        figure_object.savefig(fig_save_path, bbox_inches="tight")

    # Show the plot
    pyplot.show()


class AStarSolver(object):
    # Directions to be used for children
    VALID_DIRECTIONS = \
        [
            [1, 0],   # E
            [0, 1],   # N
            [-1, 0],  # W
            [0, -1],  # S
        ]

    def __init__(self, world, start_position, end_position):
        # Initialize all the class variables
        self.world_map = world
        self.world_limit_x = len(self.world_map[0])
        self.world_limit_y = len(self.world_map)

        self.start_position = start_position
        self.end_position = end_position

        self.open_set = []
        self.closed_set = []

        self.g_scores = {}
        self.f_scores = {}

        self.travel_path = {}
        self.final_path = []
        self.solution_map = list(self.world_map)

    @staticmethod
    def heuristic(start_point, end_point):
        # Calculate the heuristic from point a to point b using the pythagorean theorem
        delta_x = abs(end_point[0] - start_point[0])
        delta_y = abs(end_point[1] - start_point[1])

        return sqrt(pow(delta_x, 2) + pow(delta_y, 2))

    def solve_path(self):
        # Add the starting node, plus it's initial f_cost
        self.g_scores[self.start_position] = 0
        self.f_scores[self.start_position] = self.heuristic(self.start_position, self.end_position)

        # Put the starting node into the open set as (f_score, position)
        # It needs to be in this form for heap sorting by f_score
        heappush(self.open_set, (self.f_scores[self.start_position], self.start_position))

        while self.open_set:
            # Pop off the most recent node in open set with the lowest f_score
            current_node = heappop(self.open_set)

            # Extract the current position from the node
            current_position = current_node[1]

            # If we've reached the end, break so we can compute the final path
            if current_position == self.end_position:
                break

            # Now that we've reached this node, add it to the closed set
            self.closed_set.append(current_position)

            # Loop through the cardinal directions we can move to
            for delta_x, delta_y in self.VALID_DIRECTIONS:
                # Computer the child position based on the cardinal direction and teh current position
                child_position = (current_position[0] + delta_x, current_position[1] + delta_y)

                # Compute the child's g_score with an edge cost of 1
                child_g_score = self.g_scores[current_position] + EDGE_COST

                # Check if location is in the world
                valid_x_limit = 0 <= child_position[0] < self.world_limit_x
                valid_y_limit = 0 <= child_position[1] < self.world_limit_y

                # If it's in the world, make sure the child location is not an obstacle
                valid_not_obstacle = None
                if valid_x_limit and valid_y_limit:
                    valid_not_obstacle = self.world_map[child_position[1]][child_position[0]] != 1

                # If the child is in a valid location and not an obstacle:
                if valid_x_limit and valid_y_limit and valid_not_obstacle:

                    # Skip to the next child if we've already seen this node and the current path is more costly than
                    # what we've seen previously
                    if child_position in self.closed_set and child_g_score >= self.g_scores.get(child_position, 0):
                        continue

                    # Get a list of all positions in our open set
                    open_set_positions = [x[1] for x in self.open_set]

                    # If the score is better than what we've seen, or if we've never seen this node before, add the node
                    # to our open set and add this as a potential path
                    if child_g_score < self.g_scores.get(child_position, 0) or child_position not in open_set_positions:
                        self.travel_path[child_position] = current_position  # Add this jump to the travel path
                        self.g_scores[child_position] = child_g_score  # Sets the new g_score
                        self.f_scores[child_position] = \
                            child_g_score + self.heuristic(child_position, self.end_position)  # Sets the new f_score
                        heappush(self.open_set, (self.f_scores[child_position], child_position))  # Add to open set

        # Work our way backwards from the end to find the proper path
        final_path = [self.end_position]  # Add our last hop manually so the loop below can include our start position
        current_position = self.end_position  # Set the current position to the end
        while current_position != self.start_position:  # Keep looping until we've reached the beginning of the path
            current_position = self.travel_path[current_position]  # Update the current to the last path location
            final_path.append(current_position)  # Append this location to our final array

        self.final_path = final_path[::-1]  # Now that we've found the path, reverse it so it's in order

        # This applies modifications to the world map with the solution so you can see the path when plotting
        for x, y in self.final_path:
            self.solution_map[y][x] = -1

    def get_solution_map(self):
        # Gives us the solution map once we've found a solution
        return self.solution_map


if __name__ == '__main__':
    world_map = import_csv_as_array(CSV_PATH)  # Import the map

    solver = AStarSolver(world_map, START_POSITION, END_POSITION)  # Initialize the solver
    solver.solve_path()  # Solve the path
    solution_map = solver.get_solution_map()  # Retrieve the solution map

    plot_grid_map(solution_map, "final_path.pdf")  # Plot and save the solution
