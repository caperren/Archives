INPUT_FILENAME = "data.txt"
OUTPUT_FILENAME = "insert.txt"


def insert_sort(data):
    for current_value_index in range(1, len(data)):
        current = data[current_value_index]

        while current_value_index > 0 and data[current_value_index - 1] > current:
            data[current_value_index] = data[current_value_index - 1]
            current_value_index -= 1

        data[current_value_index] = current

    return data


if __name__ == "__main__":
    output = []

    with open(INPUT_FILENAME, "r") as input_file:
        file_lines = input_file.readlines()

        for line in file_lines:
            line = line.strip("\n\r")
            line_items = line.split(" ")

            data_values = [int(item) for item in line_items[1:]]

            output.append(insert_sort(data_values))

    with open(OUTPUT_FILENAME, "w") as output_file:
        output_last_index = len(output) - 1

        for index, line in enumerate(output):
            line_string = " ".join([str(value) for value in line])

            output_file.write(line_string)

            if index != output_last_index:
                output_file.write("\n")
