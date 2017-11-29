def alphabetized_key(input_data):
    output_array = []
    for word in input_data[0]:
        output_array.append(word.lower())
    return output_array


def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document and not listPairs:
        return []
    elif not document and listPairs:
        return [], []

    if periodsToBreaks:
        output_array = []
        temp_sentence = ""
        document_length_zero_indexed = len(document) - 1
        for current_index, current_value in enumerate(document):
            if current_value == '.':
                if (current_index == 0) or (current_index == document_length_zero_indexed) or \
                        (document[current_index - 1].islower() and (document[current_index + 1].isspace() or
                                                                        (document[current_index + 1] == '\n'))):
                    temp_sentence += current_value
                    output_array.append(temp_sentence)
                    temp_sentence = ""
            else:
                if current_value != '\n':
                    temp_sentence += current_value
                else:
                    temp_sentence += " "

        if temp_sentence:
            output_array.append(temp_sentence)
        split_into_sentences = output_array
    else:
        split_into_sentences = document.split('\n')

    output_array = []
    index_incrementer = 0

    for sentence in split_into_sentences:
        words_array = sentence.split(" ")
        words_array = filter(None, words_array)
        output_array.append((words_array, index_incrementer))
        index_incrementer += 1

    split_into_word_tuples =  output_array

    output_array = []

    for current_tuple in split_into_word_tuples:
        for index, _ in enumerate(current_tuple[0]):
            temp_array = current_tuple[0][index:] + current_tuple[0][:index]
            output_array.append((temp_array, current_tuple[1]))

    circular_shifted_data = output_array

    if ignoreWords:
        lowered_input = []
        output_array = []

        for word in ignoreWords:
            lowered_input.append(word.lower())

        for current_tuple in circular_shifted_data:
            if current_tuple[0][0].lower().strip(".:!?,") in lowered_input:
                pass
            else:
                output_array.append(current_tuple)

        circular_shifted_data =  output_array

    sorted_array = sorted(circular_shifted_data, key=alphabetized_key)
    alphabetized_data = sorted_array

    if listPairs:
        known_pairs = {}

        char_set = ".,?!:"
        for sentence_array, _ in split_into_word_tuples:
            seen_in_sentence = set([])

            for first_word in sentence_array:
                for second_word in sentence_array:

                    first = "".join(char for char in first_word.lower() if char not in char_set)
                    second = "".join(char for char in second_word.lower() if char not in char_set)

                    if first > second:
                        temp = second
                        second = first
                        first = temp

                    if (first == second) or (first == ""):
                        continue

                    if (first, second) not in seen_in_sentence:
                        seen_in_sentence.add((first, second))

                        if (first, second) in known_pairs:
                            known_pairs[(first, second)] += 1
                        else:
                            known_pairs[(first, second)] = 1

        output_list = []

        for key in known_pairs:
            if known_pairs[key] > 1:
                output_list.append((key, known_pairs[key]))

        output_list.sort(key=alphabetized_key)

        return alphabetized_data, output_list
    else:
        return alphabetized_data
