INPUT_FILENAME = "data.txt"
OUTPUT_FILENAME = "merge.txt"


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
    output = []

    with open(INPUT_FILENAME, "r") as input_file:
        file_lines = input_file.readlines()

        for line in file_lines:
            line = line.strip("\n\r")
            line_items = line.split(" ")

            item_count = int(line_items[0])
            data_values = [int(item) for item in line_items[1:]]

            output.append(merge_sort(data_values, top_level=True))

    with open(OUTPUT_FILENAME, "w") as output_file:
        output_last_index = len(output) - 1

        for index, line in enumerate(output):
            line_string = " ".join([str(value) for value in line])

            output_file.write(line_string)

            if index != output_last_index:
                output_file.write("\n")
