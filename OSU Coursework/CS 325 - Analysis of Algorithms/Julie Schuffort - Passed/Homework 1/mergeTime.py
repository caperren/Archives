from random import randint
from time import time

MIN_INT = 0
MAX_INT = 10000


def merge_sort(data, top_level=False):
    data_length = len(data)

    if data_length > 1:
        list_midpoint = data_length // 2

        left_data = data[:list_midpoint]
        left_data_length = len(left_data)

        right_data = data[list_midpoint:]
        right_data_length = len(right_data)

        merge_sort(left_data)
        merge_sort(right_data)

        left_data_index = 0
        right_data_index = 0
        final_data_index = 0

        while left_data_index < left_data_length and right_data_index < right_data_length:
            if left_data[left_data_index] < right_data[right_data_index]:
                data[final_data_index] = left_data[left_data_index]
                left_data_index += 1
            else:
                data[final_data_index] = right_data[right_data_index]
                right_data_index += 1

            final_data_index += 1

        while left_data_index < left_data_length:
            data[final_data_index] = left_data[left_data_index]
            left_data_index += 1
            final_data_index += 1

        while right_data_index < right_data_length:
            data[final_data_index] = right_data[right_data_index]
            right_data_index += 1
            final_data_index += 1

    if top_level:
        return data


if __name__ == "__main__":
    start = 200000
    step = 250000
    n_values = [start + i * step for i in range(10)]
    n_generated_list = [[randint(MIN_INT, MAX_INT) for _ in range(n_value)] for n_value in n_values]

    print("n_value, time")

    for current_list in n_generated_list:
        current_length = len(current_list)

        start_time = time()
        merge_sort(current_list, top_level=True)
        print("{}, {}".format(current_length, time() - start_time))
