INPUT_FILENAME = "data.txt"
OUTPUT_FILENAME = "stooge.out"


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
    output = []

    with open(INPUT_FILENAME, "r") as input_file:
        file_lines = input_file.readlines()

        for line in file_lines:
            line = line.strip("\n\r")
            line_items = line.split(" ")

            item_count = int(line_items[0])
            data_values = [int(item) for item in line_items[1:]]

            output.append(stooge_sort(data_values))

    with open(OUTPUT_FILENAME, "w") as output_file:
        output_last_index = len(output) - 1

        for index, line in enumerate(output):
            line_string = " ".join([str(value) for value in line])

            output_file.write(line_string)

            if index != output_last_index:
                output_file.write("\n")
