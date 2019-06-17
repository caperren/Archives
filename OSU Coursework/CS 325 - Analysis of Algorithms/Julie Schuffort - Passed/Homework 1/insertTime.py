from random import randint
from time import time

MIN_INT = 0
MAX_INT = 10000


def insert_sort(data):
    for current_value_index in range(1, len(data)):
        current = data[current_value_index]

        while current_value_index > 0 and data[current_value_index - 1] > current:
            data[current_value_index] = data[current_value_index - 1]
            current_value_index -= 1

        data[current_value_index] = current

    return data


if __name__ == "__main__":
    start = 5000
    step = 1000
    n_values = [start + i * step for i in range(10)]
    n_generated_list = [[randint(MIN_INT, MAX_INT) for _ in range(n_value)] for n_value in n_values]

    print("n_value, time")

    for current_list in n_generated_list:
        current_length = len(current_list)

        start_time = time()
        insert_sort(current_list)
        print("{}, {}".format(current_length, time() - start_time))
