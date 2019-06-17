from random import randint
from time import time


MIN_INT = 0
MAX_INT = 10000


def stooge_sort(data, start_index=0, end_index=None):
    if end_index is None:
        end_index = len(data) - 1

    data_length = end_index - start_index

    if data[start_index] > data[end_index]:
        data[start_index], data[end_index] = data[end_index], data[start_index]

    if data_length > 1:
        split_point = (end_index - start_index + 1) // 3

        stooge_sort(data, 0, end_index - split_point)
        stooge_sort(data, start_index + split_point, end_index)
        stooge_sort(data, 0, end_index - split_point)

    return data


if __name__ == "__main__":
    start = 4
    step = 1
    n_values = [start + i * step for i in range(10)]
    n_generated_list = [[randint(MIN_INT, MAX_INT) for _ in range(n_value)] for n_value in n_values]

    print("n_value, time")

    for current_list in n_generated_list:
        current_length = len(current_list)

        start_time = time()
        stooge_sort(current_list)
        print("{}, {}".format(current_length, time() - start_time))
