from random import randint
import numpy as np
from time import time


# Edit this section to change runtime parameters for solvers below
MAX_TILING_K_VALUE = 12
NUM_RAND_TILING_GAMES = 10
TILING_K_VALUES = [i for i in range(1, MAX_TILING_K_VALUE)]

MAX_RAND_COIN_VALUE = 21
NUM_RAND_COIN_GAMES = 10
COINS_K_VALUES = [i for i in range(1, MAX_RAND_COIN_VALUE)]
# End adjustment section


class TilingSolver(object):
    EMPTY_CELL = "   "
    REMOVED_CELL = " X "

    USE_POSITION = 1
    EXISTING_POSITION = 0

    CELL_OPTIONS = {
        "tl": [[USE_POSITION, USE_POSITION],
               [USE_POSITION, EXISTING_POSITION]],
        "tr": [[USE_POSITION, USE_POSITION],
               [EXISTING_POSITION, USE_POSITION]],
        "bl": [[USE_POSITION, EXISTING_POSITION],
               [USE_POSITION, USE_POSITION]],
        "br": [[EXISTING_POSITION, USE_POSITION],
               [USE_POSITION, USE_POSITION]]
    }

    def __init__(self, k_value, special_print=False, removed_cell=None):
        self.k_value = k_value
        self.removed_cell = removed_cell

        self.two_to_k = 2 ** self.k_value

        self.tile_table = []
        self.current_tile_number = "001"

        self.generate_starting_table()

        self.start_time = time()
        self.tile(self.two_to_k, self.tile_table, self.removed_cell)
        self.total_time = time() - self.start_time

        self.print_tile_table(special_print)

    def generate_starting_table(self):
        # noinspection PyTypeChecker
        self.tile_table = np.full((self.two_to_k, self.two_to_k), self.EMPTY_CELL)

        if not self.removed_cell:
            self.removed_cell = randint(0, self.two_to_k - 1), randint(0, self.two_to_k - 1)

        x, y = self.removed_cell

        # noinspection PyUnresolvedReferences
        self.tile_table[y][x] = self.REMOVED_CELL

    def print_tile_table(self, special_print):
        if not special_print:
            print("Solved k=%s in %0.5f seconds." % (self.k_value, self.total_time))
            for row in self.tile_table:
                print("|", end="")
                for column in row:
                    print(column, end="|")
                print()
            print()
        else:
            print("%s\t%s\t%f" % (self.k_value, self.two_to_k, self.total_time))

    def tile(self, size, table, missing_cell_location):
        table_min = 0
        table_half = size // 2
        table_max = size

        top_left = table[table_min: table_half, table_min: table_half]
        top_right = table[table_min: table_half, table_half: table_max]
        bottom_left = table[table_half: table_max, table_min: table_half]
        bottom_right = table[table_half: table_max, table_half: table_max]

        missing_x, missing_y = missing_cell_location

        x_option = "r" if missing_x < table_half else "l"
        y_option = "b" if missing_y < table_half else "t"

        tile_placement_option = y_option + x_option

        option = self.CELL_OPTIONS[tile_placement_option]

        if option[0][0] == self.USE_POSITION:
            table[table_half - 1][table_half - 1] = self.current_tile_number

        if option[0][1] == self.USE_POSITION:
            table[table_half - 1][table_half] = self.current_tile_number

        if option[1][0] == self.USE_POSITION:
            table[table_half][table_half - 1] = self.current_tile_number

        if option[1][1] == self.USE_POSITION:
            table[table_half][table_half] = self.current_tile_number

        self.increment_tile_number()

        if size == 2:
            return

        self.tile(table_half, top_left, self.get_missing_tile_location(table_half, top_left))
        self.tile(table_half, top_right, self.get_missing_tile_location(table_half, top_right))
        self.tile(table_half, bottom_left, self.get_missing_tile_location(table_half, bottom_left))
        self.tile(table_half, bottom_right, self.get_missing_tile_location(table_half, bottom_right))

    def increment_tile_number(self):
        self.current_tile_number = "%03d" % (int(self.current_tile_number) + 1)

    def get_missing_tile_location(self, size, table):
        for row_index in range(size):
            for column_index in range(size):
                if table[row_index][column_index] != self.EMPTY_CELL:
                    return column_index, row_index


class ODDSolver(object):
    GOOD_COIN_WEIGHT = randint(1, 100)
    BAD_COIN_WEIGHT_MIN = GOOD_COIN_WEIGHT + 1
    BAD_COIN_WEIGHT_MAX = 5 * BAD_COIN_WEIGHT_MIN

    HEAVIER = "heavier"
    LIGHTER = "lighter"
    EQUAL = "equal"

    def __init__(self, k_value, special_print=False):
        self.k_value = k_value

        self.three_to_k = 3 ** self.k_value

        self.coins = []

        self.setup_coins()

        start_left_range = 0, self.three_to_k // 3
        start_mid_range = self.three_to_k // 3, (self.three_to_k * 2) // 3
        start_right_range = (self.three_to_k * 2) // 3, self.three_to_k

        self.start_time = time()
        self.bad_coin_at_position = self.run_solver(start_left_range, start_mid_range, start_right_range)
        self.total_time = time() - self.start_time

        self.print_result(special_print)

    def setup_coins(self):
        self.coins = np.full(self.three_to_k, self.GOOD_COIN_WEIGHT)

        self.coins[randint(0, self.three_to_k - 1)] = randint(self.BAD_COIN_WEIGHT_MIN, self.BAD_COIN_WEIGHT_MAX)

    def run_solver(self, left_range, mid_range, right_range):
        left_mid = self.weigh(left_range, mid_range)
        mid_right = self.weigh(mid_range, right_range)
        left_right = self.weigh(left_range, right_range)

        heaviest = None

        if left_mid != self.EQUAL:
            heaviest = left_range if left_mid == self.HEAVIER else mid_range

        if mid_right != self.EQUAL:
            heaviest = mid_range if mid_right == self.HEAVIER else right_range

        if left_right != self.EQUAL:
            heaviest = left_range if left_right == self.HEAVIER else right_range

        if left_range[-1] - left_range[0] == 1 or \
                mid_range[-1] - mid_range[0] == 1 or \
                right_range[-1] - right_range[0] == 1:
            return heaviest[0]

        range_min = heaviest[0]
        range_max = heaviest[-1]
        range_diff = range_max - range_min

        start_left_range = range_min, (range_diff // 3) + range_min
        start_mid_range = (range_diff // 3) + range_min, ((2 * range_diff) // 3) + range_min
        start_right_range = ((2 * range_diff) // 3) + range_min, range_max

        return self.run_solver(start_left_range, start_mid_range, start_right_range)

    def weigh(self, left_range, right_range):
        left_sum = self.coins[left_range[0]: left_range[1]].sum()
        right_sum = self.coins[right_range[0]: right_range[1]].sum()

        if left_sum > right_sum:
            return self.HEAVIER
        elif left_sum < right_sum:
            return self.LIGHTER
        else:
            return self.EQUAL

    def print_result(self, special_print):
        if not special_print:
            print("Solved odd coin problem with k=%s in %0.5f seconds. Bad coin at position %s." %
                  (self.k_value, self.total_time, self.bad_coin_at_position))
        else:
            print("%s\t%s\t%f" % (self.k_value, self.three_to_k, self.total_time))


if __name__ == '__main__':
    for current_k in TILING_K_VALUES:
        for _ in range(NUM_RAND_TILING_GAMES):
            TilingSolver(current_k, True)

    print()

    for current_k in COINS_K_VALUES:
        for _ in range(NUM_RAND_COIN_GAMES):
            ODDSolver(current_k, True)
