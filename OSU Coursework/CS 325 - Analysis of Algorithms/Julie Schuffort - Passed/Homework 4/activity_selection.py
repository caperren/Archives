INPUT_FILE = "act.txt"


def greedy_activity_selector(activity_set):
    activity_num_index = 0
    activity_start_index = 1
    activity_finish_index = 2

    activity_set.sort(key=lambda activity: activity[activity_start_index], reverse=True)

    selections = [activity_set[0][activity_num_index]]
    comparison_activity_index = 0

    for i in range(1, len(activity_set)):
        if activity_set[i][activity_finish_index] <= activity_set[comparison_activity_index][activity_start_index]:
            selections.append(activity_set[i][activity_num_index])
            comparison_activity_index = i

    return list(reversed(selections))


def get_activity_sets(input_filename):
    file_activity_sets = []

    with open(input_filename, "r") as input_file:
        lines = input_file.readlines()
        last_index = len(lines) - 1

        line_index = -1

        while line_index != last_index:
            current_set = []
            line_index += 1
            num_activities = int(lines[line_index])

            for activity_number in range(num_activities):
                line_index += 1

                activity = lines[line_index]
                activity_split = activity.split(" ")

                current_set.append(list((int(value) for value in activity_split)))

            file_activity_sets.append(current_set)

    return file_activity_sets


if __name__ == "__main__":
    activity_sets = get_activity_sets(INPUT_FILE)

    for set_number, current_activity_set in enumerate(activity_sets):
        selected_activities = greedy_activity_selector(current_activity_set)

        print("Set {}".format(set_number + 1))
        print("Number of activities selected = {}".format(len(selected_activities)))
        print("Activities: {}".format(" ".join([str(activity) for activity in selected_activities])))
        print()
