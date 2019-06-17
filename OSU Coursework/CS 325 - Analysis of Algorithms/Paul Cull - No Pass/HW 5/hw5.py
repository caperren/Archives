from time import time
from random import randint


def generate_polynomials(k_input):
    n = 2 ** k_input

    left = [randint(0, 10) for _ in range(n)]
    right = [randint(0, 10) for _ in range(n)]

    return left, right


def iterative_poly_solver(left_coefficients, right_coefficients):
    result_coefficients = [0 for _ in range(len(left_coefficients) + len(right_coefficients) - 1)]

    for left_index, left_val in enumerate(left_coefficients):
        for right_index, right_val in enumerate(right_coefficients):
            result_coefficients[left_index + right_index] = left_val * right_val + result_coefficients[left_index + right_index]
    return result_coefficients


def recursive_poly_solver(left_coefficients, right_coefficients):
    if len(left_coefficients) == 1 and len(right_coefficients) == 1:
        return [left_coefficients[0] * right_coefficients[0]]

    halfway = len(left_coefficients) // 2

    left_lower_half = left_coefficients[:halfway]
    left_higher_half = left_coefficients[halfway:]

    right_lower_half = right_coefficients[:halfway]
    right_higher_half = right_coefficients[halfway:]

    sum_low_high_left = [0 for _ in range(len(left_lower_half))]
    sum_low_high_right = [0 for _ in range(len(right_lower_half))]

    for i in range(len(left_lower_half)):
        sum_low_high_left[i] = left_lower_half[i] + left_higher_half[i]
        sum_low_high_right[i] = right_lower_half[i] + right_lower_half[i]

    low_left_low_right_multiply = recursive_poly_solver(left_lower_half, right_lower_half)
    sums_multiple = recursive_poly_solver(sum_low_high_left, sum_low_high_right)
    high_left_high_right_multiply = recursive_poly_solver(left_higher_half, right_higher_half)

    combined = [0 for _ in range(len(left_coefficients) + len(right_coefficients) - 1)]

    for i in range(len(left_coefficients) - 1):
        combined[i] += low_left_low_right_multiply[i]
        combined[i + halfway] += sums_multiple[i] - low_left_low_right_multiply[i] - high_left_high_right_multiply[i]
        combined[i + 2 * halfway] += high_left_high_right_multiply[i]

    return combined


if __name__ == '__main__':
    for i in range(20):
        left, right = generate_polynomials(i)

        start = time()
        iterative_poly_solver(left, right)
        print("%s\t%s" % (2 ** i, time()-start))

    # for i in range(10):
    #     left, right = generate_polynomials(i)
    #         print(recursive_poly_solver(left[0], right[1]))
