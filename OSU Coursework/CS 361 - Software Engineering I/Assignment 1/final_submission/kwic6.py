def split_by_periods(document):
    output_array = []

    sentence_array_temp = ""

    for current_char in document:
        if current_char != "\n":
            sentence_array_temp += current_char

        if current_char == ".":
            output_array.append(sentence_array_temp)
            sentence_array_temp = ""

    if sentence_array_temp:
        output_array.append(sentence_array_temp)

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


def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document:
        return [], []

    if periodsToBreaks:
        split_into_sentences = split_by_periods(document)
    else:
        split_into_sentences = document.splitlines()

    split_into_word_tuples = split_by_word_as_tuples(split_into_sentences)

    output_tuple = (fill_with_circular_shifts_and_original(split_into_word_tuples), [])

    return output_tuple
