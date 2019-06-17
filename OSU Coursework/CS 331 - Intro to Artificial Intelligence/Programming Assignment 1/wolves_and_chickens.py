import argparse
import csv
from collections import deque
from copy import copy
from queue import PriorityQueue
from time import time
import sys


class BankState(object):
    def __init__(self, chickens=0, wolves=0, has_boat=False):
        self.chickens = chickens
        self.wolves = wolves
        self.has_boat = has_boat

    def __eq__(self, other):
        chickens_eq = self.chickens == other.chickens
        wolves_eq = self.wolves == other.wolves
        has_boat_eq = self.has_boat == other.has_boat

        return chickens_eq and wolves_eq and has_boat_eq

    def __hash__(self):
        return hash((self.chickens, self.wolves, self.has_boat))

    def __str__(self):
        return "Chickens: {}, Wolves: {}, Has Boat: {}".format(self.chickens, self.wolves, self.has_boat)

    def __copy__(self):
        return type(self)(self.chickens, self.wolves, self.has_boat)

    def valid_state(self):
        return (self.wolves <= self.chickens or self.chickens == 0) and (self.wolves >= 0) and (self.chickens >= 0)


class GameState(object):
    AVAILABLE_ACTIONS = [
        {"chickens": 1, "wolves": 0},
        {"chickens": 2, "wolves": 0},
        {"chickens": 0, "wolves": 1},
        {"chickens": 1, "wolves": 1},
        {"chickens": 0, "wolves": 2},
    ]

    def __init__(self, left_bank=None, right_bank=None, total_chickens=0, total_wolves=0):
        self.left_bank = BankState() if left_bank is None else left_bank
        self.right_bank = BankState() if right_bank is None else right_bank

        self.total_chickens = total_chickens
        self.total_wolves = total_wolves

    def __eq__(self, other):
        return self.left_bank == other.left_bank and self.right_bank == other.right_bank

    def __hash__(self):
        return hash((self.left_bank, self.right_bank))

    def __str__(self):
        return "[Left Bank -> {} | Right Bank -> {}]".format(self.left_bank, self.right_bank)

    def __copy__(self):
        return type(self)(
            copy(self.left_bank), copy(self.right_bank), copy(self.total_chickens), copy(self.total_wolves)
        )

    def valid_state(self):
        banks_valid = self.left_bank.valid_state() and self.right_bank.valid_state()
        chickens_valid = (self.left_bank.chickens + self.right_bank.chickens) == self.total_chickens
        wolves_valid = (self.left_bank.wolves + self.right_bank.wolves) == self.total_wolves

        return banks_valid and chickens_valid and wolves_valid

    def apply_action(self, action, left_to_right_bank=True):
        if left_to_right_bank:
            self.right_bank.chickens += action["chickens"]
            self.right_bank.wolves += action["wolves"]
            self.right_bank.has_boat = True

            self.left_bank.chickens -= action["chickens"]
            self.left_bank.wolves -= action["wolves"]
            self.left_bank.has_boat = False
        else:
            self.right_bank.chickens -= action["chickens"]
            self.right_bank.wolves -= action["wolves"]
            self.right_bank.has_boat = False

            self.left_bank.chickens += action["chickens"]
            self.left_bank.wolves += action["wolves"]
            self.left_bank.has_boat = True

    def set_from_file(self, file_path):
        with open(file_path, "r") as game_state_file:
            read_csv = list(csv.reader(game_state_file))
            left_bank_state, right_bank_state = read_csv[0], read_csv[1]

            self.left_bank.chickens = int(left_bank_state[0])
            self.left_bank.wolves = int(left_bank_state[1])
            self.left_bank.has_boat = left_bank_state[2] == "1"

            self.right_bank.chickens = int(right_bank_state[0])
            self.right_bank.wolves = int(right_bank_state[1])
            self.right_bank.has_boat = right_bank_state[2] == "1"

            self.total_chickens = self.left_bank.chickens + self.right_bank.chickens
            self.total_wolves = self.left_bank.wolves + self.right_bank.wolves


class Node(object):
    def __init__(self, parent_node, game_state, path_cost, depth):
        self.parent_node = parent_node
        self.game_state = game_state
        self.depth = depth
        self.path_cost = path_cost

    def __str__(self):
        return "{} | {} | {}".format(self.game_state, self.depth, self.path_cost)

    def __hash__(self):
        return hash((self.parent_node, self.game_state, self.depth, self.path_cost))

    def __lt__(self, other):
        return hash(self) < hash(other)


class GameSolver(object):
    def __init__(self, start_state, goal_state):
        self.start_state = start_state
        self.goal_state = goal_state

        self.expanded_nodes_count = 0

        self.fringe = deque()
        self.solution = None

    def solution_string(self):
        solutions_strings = deque()

        sentinel = self.solution
        while sentinel is not None:
            solutions_strings.appendleft(str(sentinel.game_state))
            sentinel = sentinel.parent_node

        return "\n".join(solutions_strings)

    def show_and_save_solution(self, file_path):
        if solver.solution:
            solution_string = self.solution_string()

            print(solution_string)

            with open(file_path, "w") as output_file:
                output_file.write(solution_string)
                output_file.write("\n")

            print()
            print("{} nodes were expanded for a solution with a depth of {}".format(
                solver.expanded_nodes_count,
                solver.solution.depth)
            )
        else:
            print("No solution found in {} expanded nodes".format(solver.expanded_nodes_count))

    @staticmethod
    def valid_states_from_current_node(current_node):
        valid_states = []

        current_state = current_node.game_state
        left_to_right_bank = current_state.left_bank.has_boat

        for action in current_state.AVAILABLE_ACTIONS:
            test_state = copy(current_state)
            test_state.apply_action(action, left_to_right_bank)
            if test_state.valid_state():
                valid_states.append(test_state)

        return valid_states

    def is_goal(self, node):
        return node.game_state == self.goal_state

    def expand(self, node):
        pass

    def solve(self):
        pass


class BFSGameSolver(GameSolver):
    def expand(self, current_node):
        successors = deque()

        new_path_cost = current_node.path_cost + 1
        new_depth = current_node.depth + 1

        for state in self.valid_states_from_current_node(current_node):
            successors.append(Node(current_node, state, new_path_cost, new_depth))

        return successors

    def solve(self):
        closed = set()

        self.fringe.append(Node(None, self.start_state, 0, 0))

        while True:
            if not self.fringe:
                self.solution = None
                return

            current_node = self.fringe.popleft()
            current_state = current_node.game_state

            if self.is_goal(current_node):
                self.solution = current_node
                return

            self.expanded_nodes_count += 1

            if current_state not in closed:
                closed.add(current_state)
                self.fringe += self.expand(current_node)


class DFSGameSolver(GameSolver):
    def expand(self, current_node):
        successors = deque()

        new_path_cost = current_node.path_cost + 1
        new_depth = current_node.depth + 1

        for state in reversed(self.valid_states_from_current_node(current_node)):
            successors.append(Node(current_node, state, new_path_cost, new_depth))

        return successors

    def solve(self):
        closed = set()

        self.fringe.append(Node(None, self.start_state, 0, 0))

        while True:
            if not self.fringe:
                self.solution = None
                return

            current_node = self.fringe.pop()
            current_state = current_node.game_state

            if self.is_goal(current_node):
                self.solution = current_node
                return

            self.expanded_nodes_count += 1

            if current_state not in closed:
                closed.add(current_state)
                self.fringe += self.expand(current_node)


class IDDFSGameSolver(GameSolver):
    def __init__(self, start_state, goal_state, starting_depth=1):
        super(IDDFSGameSolver, self).__init__(start_state, goal_state)

        self.current_depth = starting_depth
        self.max_depth = 1000000

    def expand(self, current_node):
        successors = deque()

        if current_node.depth == self.current_depth:
            return successors

        new_path_cost = current_node.path_cost + 1
        new_depth = current_node.depth + 1

        for state in reversed(self.valid_states_from_current_node(current_node)):
            successors.append(Node(current_node, state, new_path_cost, new_depth))

        return successors

    def solve(self):
        closed = set()

        start_state_node = Node(None, self.start_state, 0, 0)
        self.fringe.append(start_state_node)

        while True:
            if not self.fringe:
                if self.current_depth != self.max_depth:
                    self.current_depth += 1
                    closed = set()
                    self.fringe.append(Node(None, self.start_state, 0, 0))
                else:
                    self.solution = None
                    return

            current_node = self.fringe.pop()
            current_state = current_node.game_state

            if self.is_goal(current_node):
                self.solution = current_node
                return

            self.expanded_nodes_count += 1

            if current_state not in closed:
                closed.add(current_state)
                self.fringe += self.expand(current_node)


class AStarGameSolver(GameSolver):
    def __init__(self, start_state, goal_state):
        super(AStarGameSolver, self).__init__(start_state, goal_state)

        self.fringe = PriorityQueue()

        self.left_to_right_chickens = self.goal_state.left_bank.chickens < self.goal_state.right_bank.chickens
        self.left_to_right_wolves = self.goal_state.left_bank.wolves < self.goal_state.right_bank.wolves

    def astar_heuristic(self, current_node):
        current_state = current_node.game_state

        if self.left_to_right_chickens:
            min_moves_chickens = (self.goal_state.right_bank.chickens - current_state.right_bank.chickens) / 2.0
        else:
            min_moves_chickens = (self.goal_state.left_bank.chickens - current_state.left_bank.chickens) / 2.0

        if self.left_to_right_wolves:
            min_moves_wolves = (self.goal_state.right_bank.wolves - current_state.right_bank.wolves) / 2.0
        else:
            min_moves_wolves = (self.goal_state.left_bank.wolves - current_state.left_bank.wolves) / 2.0

        return min_moves_chickens + min_moves_wolves

    def astar_evaluator(self, current_node):
        return self.astar_heuristic(current_node) + current_node.path_cost

    def expand(self, current_node):
        new_path_cost = current_node.path_cost + 1
        new_depth = current_node.depth + 1

        for state in self.valid_states_from_current_node(current_node):
            new_node = Node(current_node, state, new_path_cost, new_depth)
            evaluated_priority = self.astar_evaluator(new_node)
            self.fringe.put((evaluated_priority, new_node))

    def solve(self):
        closed = set()

        self.fringe.put((1, Node(None, self.start_state, 0, 0)))

        while True:
            if not self.fringe:
                self.solution = None
                return

            _, current_node = self.fringe.get()
            current_state = current_node.game_state

            if self.is_goal(current_node):
                self.solution = current_node
                return

            self.expanded_nodes_count += 1

            if current_state not in closed:
                closed.add(current_state)
                self.expand(current_node)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Solves the chicken and wolves game")

    parser.add_argument("initial_state_file", help="Filename of the state file in the current directory")
    parser.add_argument("goal_state_file", help="Filename of the goal state file in the current directory")
    parser.add_argument("mode", choices=["bfs", "dfs", "iddfs", "astar"], help="Mode to run the solver with")
    parser.add_argument("output_file", help="Desired filename of the output file in the current directory")
    args = parser.parse_args()

    print("Starting chickens and wolves game solver\n")

    sys.setrecursionlimit(10000)

    starting_state = GameState()
    ending_state = GameState()

    starting_state.set_from_file(args.initial_state_file)
    ending_state.set_from_file(args.goal_state_file)

    if args.mode == "bfs":
        print("Running bfs on start_state {} with goal {}".format(starting_state, ending_state))
        solver = BFSGameSolver(starting_state, ending_state)
    elif args.mode == "dfs":
        print("Running dfs on start_state {} with goal {}".format(starting_state, ending_state))
        solver = DFSGameSolver(starting_state, ending_state)
    elif args.mode == "iddfs":
        print("Running iddfs on start_state {} with goal {}".format(starting_state, ending_state))
        solver = IDDFSGameSolver(starting_state, ending_state)
    else:
        print("Running astar on start_state {} with goal {}".format(starting_state, ending_state))
        solver = AStarGameSolver(starting_state, ending_state)

    print()

    start_time = time()
    solver.solve()
    end_time = time() - start_time

    solver.show_and_save_solution(args.output_file)

    print("Solve ran in {} seconds".format(end_time))