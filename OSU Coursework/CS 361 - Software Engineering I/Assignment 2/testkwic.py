def split_by_periods(document):
    output_array = []
    temp_sentence = ""
    document_length_zero_indexed = len(document) - 1
    for current_index, current_value in enumerate(document):
        if current_value == '.':
            if (current_index == 0) or (current_index == document_length_zero_indexed) or \
                    (document[current_index-1].islower() and (document[current_index+1].isspace() or
                                                              (document[current_index+1] == '\n'))):
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
    return output_array


def split_by_word_as_tuples(sentence_array):
    output_array = []
    index_incrementer = 0

    for sentence in sentence_array:
        words_array = sentence.split(" ")
        words_array = filter(None, words_array)
        output_array.append((words_array, index_incrementer))
        index_incrementer += 1

    return output_array


def array_circular_shift(input_array, rotate_val):
    output_array = input_array[rotate_val:] + input_array[:rotate_val]
    return output_array


def fill_with_circular_shifts_and_original(sentence_array):
    output_array = []

    for current_tuple in sentence_array:
        for index, _ in enumerate(current_tuple[0]):
            output_array.append((array_circular_shift(current_tuple[0], index), current_tuple[1]))

    return output_array


def alphabetize_tuple_list(input_array):
    sorted_array = sorted(input_array, key=alphabetized_key)
    return sorted_array


def alphabetized_key(input_data):
    output_array = []
    for word in input_data[0]:
        output_array.append(word.lower())
    return output_array


def remove_words(input_array, words):
    lowered_input = []
    output_array = []

    for word in words:
        lowered_input.append(word.lower())

    for current_tuple in input_array:
        if current_tuple[0][0].lower().strip(".:!?,") in lowered_input:
            pass
        else:
            output_array.append(current_tuple)

    return output_array


def create_list_pairs(input_array):
    known_pairs = {}

    for sentence_array, _ in input_array:
        seen_in_sentence = set([])

        for first_word in sentence_array:
            for second_word in sentence_array:
                first, second = return_ordered_words(sanitize_word(first_word), sanitize_word(second_word))

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

    return output_list


def sanitize_word(input_word):
    char_set = ".,?!:"
    return "".join(char for char in input_word.lower() if char not in char_set)


def return_ordered_words(word_one, word_two):

    if word_one < word_two:
        return word_one, word_two
    else:
        return word_two, word_one


def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document and not listPairs:
        return []
    elif not document and listPairs:
        return [], []

    if periodsToBreaks:
        split_into_sentences = split_by_periods(document)
    else:
        split_into_sentences = document.split('\n')

    split_into_word_tuples = split_by_word_as_tuples(split_into_sentences)

    circular_shifted_data = fill_with_circular_shifts_and_original(split_into_word_tuples)

    if ignoreWords:
        circular_shifted_data = remove_words(circular_shifted_data, ignoreWords)

    alphabetized_data = alphabetize_tuple_list(circular_shifted_data)

    if listPairs:
        return alphabetized_data, create_list_pairs(split_into_word_tuples)
    else:
        return alphabetized_data

