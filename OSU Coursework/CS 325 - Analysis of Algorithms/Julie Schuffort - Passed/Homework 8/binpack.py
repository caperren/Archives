from time import time
from collections import deque
from copy import copy

INPUT_FILENAME = "bin.txt"


class BinPacker(object):
    def __init__(self, bin_pack_test_case):
        self._bin_size = bin_pack_test_case["bin_capacity"]
        self._items = deque(list(bin_pack_test_case["items"]))
        self._bins = []
        self._run_time = 0

    def _solve(self):
        raise NotImplemented

    def solve_and_time(self):
        start_time = time() * 1000
        self._solve()
        self._run_time = (time() * 1000) - start_time

        return len(self._bins), self._run_time


class FirstFit(BinPacker):
    def _solve(self):
        while self._items:
            current_item = self._items.popleft()

            item_placed = False

            for bin_index in range(len(self._bins)):
                new_sum = self._bins[bin_index] + current_item

                if new_sum <= self._bin_size:
                    self._bins[bin_index] = new_sum
                    item_placed = True
                    break

            if not item_placed:
                self._bins.append(current_item)


class FirstFitDecreasing(FirstFit):
    def _solve(self):
        sorted_descending = list(self._items)
        sorted_descending.sort(reverse=True)
        self._items = deque(sorted_descending)

        super(FirstFitDecreasing, self)._solve()


class BestFit(BinPacker):
    def _solve(self):
        while self._items:
            current_item = self._items.popleft()

            min_room_left_over = self._bin_size
            best_index_sum = None
            best_index = None

            for bin_index in range(len(self._bins)):
                new_sum = self._bins[bin_index] + current_item
                bin_difference = self._bin_size - new_sum

                if 0 <= bin_difference < min_room_left_over:

                    best_index = bin_index
                    best_index_sum = new_sum
                    min_room_left_over = bin_difference

            if best_index is not None:
                self._bins[best_index] = best_index_sum
            else:
                self._bins.append(current_item)

        # print(self._bins)


def get_test_cases_from_file(filename):
    input_test_cases = []

    with open(filename, "r") as input_file:
        file_lines = input_file.readlines()

        current_line = 0
        num_test_cases = int(file_lines[current_line])

        for _ in range(num_test_cases):
            current_test_case = {}

            current_line += 1
            current_test_case["bin_capacity"] = int(file_lines[current_line])

            current_line += 1
            num_items = int(file_lines[current_line])

            current_line += 1
            items_split = file_lines[current_line].split(" ")
            current_test_case["items"] = [int(item_str) for item_str in items_split[:num_items]]

            input_test_cases.append(current_test_case)

        return input_test_cases


if __name__ == "__main__":
    test_cases = get_test_cases_from_file(INPUT_FILENAME)

    for test_index, test_case in enumerate(test_cases):
        first_fit_num_bins, first_fit_run_time = FirstFit(test_case).solve_and_time()
        first_fit_dec_num_bins, first_fit_dec_run_time = FirstFitDecreasing(test_case).solve_and_time()
        best_fit_num_bins, best_fit_run_time = BestFit(test_case).solve_and_time()

        print("Test Case {} First Fit: {}, {} ms. First Fit Decreasing {}, {} ms. Best Fit: {}, {} ms.".format(
            test_index + 1,
            first_fit_num_bins, first_fit_run_time,
            first_fit_dec_num_bins, first_fit_dec_run_time,
            best_fit_num_bins, best_fit_run_time
        ))
