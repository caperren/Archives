INPUT_FILE = "shopping.txt"
OUTPUT_FILE = "results.txt"


def shopping_solver(max_individual_weight, shopping_items):
    num_items = len(shopping_items["weights"])

    # Each element of the table is (z, (a, b)) where z is the max value, and (a, b) are the x and y
    # coordinates in the table for the parent that was used to create this value
    lookup_table = [[(0, (0, 0)) for _ in range(num_items + 1)] for _ in range(max_individual_weight + 1)]

    # Run the solver
    for current_weight in range(max_individual_weight + 1):
        for current_item in range(num_items + 1):
            last_item_index = current_item - 1

            if current_weight == 0 or current_item == 0:
                lookup_table[current_weight][current_item] = 0, (0, 0)

            elif shopping_items["weights"][last_item_index] <= current_weight:
                included_weight = current_weight - shopping_items["weights"][last_item_index]

                included = shopping_items["prices"][last_item_index] + lookup_table[included_weight][last_item_index][0]
                not_included = lookup_table[current_weight][last_item_index][0]

                if included > not_included:
                    lookup_table[current_weight][current_item] = included, (included_weight, last_item_index)

                else:
                    lookup_table[current_weight][current_item] = not_included, (current_weight, last_item_index)

            else:
                lookup_table[current_weight][current_item] = \
                    lookup_table[current_weight][last_item_index][0], (current_weight, last_item_index)

    # Trace parents to find the path
    solved_path = []

    last_weight_index, last_item_index = max_individual_weight, num_items
    current_weight_index, current_item_index = lookup_table[max_individual_weight][num_items][1]

    while [current_weight_index == 0,
           current_item_index == 0,
           last_weight_index == 0,
           last_item_index == 0] != [True, True, True, True]:
        if current_item_index != last_item_index and current_weight_index != last_weight_index:
            solved_path.append(current_item_index + 1)

        last_item_index = current_item_index
        last_weight_index = current_weight_index

        current_weight_index, current_item_index = lookup_table[current_weight_index][current_item_index][1]

    return lookup_table[max_individual_weight][num_items][0], list(reversed(solved_path))


def get_tests_from_file(filename):
    tests_from_file = []

    with open(filename, "r") as shopping_file:
        lines = shopping_file.readlines()

        line_index = 0
        num_tests = int(lines[line_index])

        for test_number in range(num_tests):
            current_test = {"items": {"weights": [], "prices": []}, "family": []}

            line_index += 1
            num_items = int(lines[line_index])

            for item_number in range(num_items):
                line_index += 1

                item = lines[line_index]
                item_split = item.split(" ")

                current_test["items"]["weights"].append(int(item_split[1]))
                current_test["items"]["prices"].append(int(item_split[0]))

            line_index += 1
            num_family = int(lines[line_index])

            for family_number in range(num_family):
                line_index += 1
                current_test["family"].append(int(lines[line_index]))

            tests_from_file.append(current_test)

    return tests_from_file


def print_and_save_results(all_testing_results):
    output_string = ""

    for test_number, current_test in enumerate(all_testing_results):
        output_string += "Test Case {}\n".format(test_number + 1)
        output_string += "Total Price {}\n".format(current_test[0])
        output_string += "Member Items:\n"

        for member_number, items in enumerate(current_test[1]):
            if items:
                output_string += "{}: {}\n".format(member_number + 1, " ".join([str(item) for item in items]) + " ")
            else:
                output_string += "{}: \n".format(member_number + 1)

        output_string += "\n"

    print(output_string, end="")

    with open(OUTPUT_FILE, "w") as output_file:
        output_file.write(output_string)


if __name__ == "__main__":
    tests = get_tests_from_file(INPUT_FILE)

    all_test_results = []

    for test in tests:
        test_sum = 0
        family_member_items = []

        for family_member in test["family"]:
            max_value, item_indexes = shopping_solver(family_member, test["items"])
            test_sum += max_value
            family_member_items.append(item_indexes)

        all_test_results.append((test_sum, family_member_items))

    print_and_save_results(all_test_results)
