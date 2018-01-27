#!/usr/bin/env python3

from time import time


def run_hanoi_algorithm(hanoi_function, num_disks, print_steps=False, print_stack=False):
    stacks = get_starting_stack(num_disks)

    if print_stack:
        pretty_print_stacks(stacks)

    hanoi_function(len(stacks["left"]), stacks, "left", "middle", "right", print_steps, print_stack)


def pretty_print_stacks(hanoi_stacks):
    to_print = "Left:%s\tMiddle:%s\tRight:%s" % (hanoi_stacks["left"], hanoi_stacks["middle"], hanoi_stacks["right"])
    print(to_print.expandtabs(40))


def get_starting_stack(num_start_disks):
    stacks = {
        "left": [i for i in range(1, num_start_disks + 1)],
        "middle": [],
        "right": []
    }

    return stacks


def move_disk(hanoi_stacks, starting_post, ending_post, print_steps=False, print_stack=False):
    hanoi_stacks[ending_post].insert(0, hanoi_stacks[starting_post].pop(0))

    if print_steps:
        print("Moved disk from %s to %s" % (starting_post, ending_post))

    if print_stack:
        pretty_print_stacks(hanoi_stacks)


def hanoi_recursive(height, hanoi_stacks, starting_post, temp_post, ending_post, print_steps=False, print_stack=False):
    if height > 0:
        hanoi_recursive(height - 1, hanoi_stacks, starting_post, ending_post, temp_post, print_steps, print_stack)

        move_disk(hanoi_stacks, starting_post, ending_post, print_steps, print_stack)

        hanoi_recursive(height - 1, hanoi_stacks, temp_post, starting_post, ending_post, print_steps, print_stack)


def make_legal_move(height, hanoi_stacks, starting_post, ending_post, print_steps=False, print_stack=False):
    if len(hanoi_stacks["right"]) == height or \
            (len(hanoi_stacks[starting_post]) == 0 and len(hanoi_stacks[ending_post]) == 0):
        return
    elif len(hanoi_stacks[starting_post]) == 0:
        larger = starting_post
        smaller = ending_post
    elif len(hanoi_stacks[ending_post]) == 0:
        larger = ending_post
        smaller = starting_post
    else:
        larger = starting_post if hanoi_stacks[starting_post][0] > hanoi_stacks[ending_post][0] else ending_post
        smaller = starting_post if larger != starting_post else ending_post

    move_disk(hanoi_stacks, smaller, larger, print_steps, print_stack)


def hanoi_iterative(height, hanoi_stacks, starting_post, temp_post, ending_post, print_steps=False, print_stack=False):
    while len(hanoi_stacks["right"]) != height:
        if height % 2 == 0:
            make_legal_move(height, hanoi_stacks, starting_post, temp_post, print_steps, print_stack)
            make_legal_move(height, hanoi_stacks, starting_post, ending_post, print_steps, print_stack)
        else:
            make_legal_move(height, hanoi_stacks, starting_post, ending_post, print_steps, print_stack)
            make_legal_move(height, hanoi_stacks, starting_post, temp_post, print_steps, print_stack)

        make_legal_move(height, hanoi_stacks, temp_post, ending_post, print_steps, print_stack)


if __name__ == "__main__":
    # ##### Variables for Later #####
    recursive_timings = [[], []]
    iterative_timings = [[], []]
    num_timing_iterations = 25

    # ##### Problem 1 #####
    print("CS 325 - Homework 2 - Corwin Perren")

    print("\n\n########## Problem 1 ##########")
    print("Recursive and iterative version of hanoi made...")
    # Recursive and iterative version made. See the two immediate functions above.

    # ##### Problem 2 #####
    print("\n\n########## Problem 2 ##########")

    # Recursive 3 disks
    print("### Hanoi Recursive - 3 Disks - Printed Steps ###")
    run_hanoi_algorithm(hanoi_recursive, 3, True, False)

    # Recursive 4 disks
    print("\n\n### Hanoi Recursive - 4 Disks - Printed Steps ###")
    run_hanoi_algorithm(hanoi_recursive, 4, True, False)

    # Iterative 3 disks
    print("\n\n### Hanoi Iterative - 3 Disks - Printed Steps ###")
    run_hanoi_algorithm(hanoi_iterative, 3, True, False)

    # Iterative 4 disks
    print("\n\n### Hanoi Iterative - 4 Disks - Printed Steps ###")
    run_hanoi_algorithm(hanoi_iterative, 4, True, False)

    # Verified that these are the correct steps

    # ##### Problem 3 #####
    print("\n\n########## Problem 3 ##########")
    print("### 4 Disks - Print Out Successive Contents from Recursion ###")
    run_hanoi_algorithm(hanoi_recursive, 4, False, True)

    # ##### Problem 4 #####
    print("\n\n########## Problem 4 ##########")
    print("### Recursive Timings For Small n ###")
    for current_n in range(1, num_timing_iterations):
        start_time = time()
        run_hanoi_algorithm(hanoi_recursive, current_n)
        run_time = time() - start_time

        recursive_timings[0].append(current_n)
        recursive_timings[1].append(run_time)
        print("%s disks: %.10f seconds" % (current_n, run_time))

    print("### Iterative Timings For Small n ###")
    for current_n in range(1, num_timing_iterations):
        start_time = time()
        run_hanoi_algorithm(hanoi_iterative, current_n)
        run_time = time() - start_time

        iterative_timings[0].append(current_n)
        iterative_timings[1].append(run_time)
        print("%s disks: %.10f seconds" % (current_n, run_time))

    # ##### Problem 5 #####
    print("\n\n########## Problem 5 ##########")
    print("### Plots shown below, or printouts for spreadsheet if run from code ###")

    for i in range(len(recursive_timings[0])):
        print("%s\t%s" % (recursive_timings[0][i], recursive_timings[1][i]))

    for i in range(len(iterative_timings[0])):
        print("%s\t%s" % (iterative_timings[0][i], iterative_timings[1][i]))

    # ##### Problem 6 #####
    print("\n\n########## Problem 6 ##########")
    print("Recursive C Value is approximately equal to 7.42E-07")
    print("Iterative C Value is approximately equal to 1.13E-06")

    # ##### Problem 7 #####
    print("\n\n########## Problem 7 ##########")
    print("The recursive algorithm will be faster for large n.")

    # ##### Problem 8 #####
    print("\n\n########## Problem 8 ##########")
    print("The recursive algorithm would take approximately 434027 years for 64 disks.")
    print("The iterative algorithm would take approximately 660985 years for 64 disks.")

    # ##### Problem 9 #####
    print("\n\n########## Problem 9 ##########")
    print("The recursive algorithm could solve approximately 29 disks in ten minutes.")
    print("The iterative algorithm could solve approximately 28 disks in ten minutes.")
