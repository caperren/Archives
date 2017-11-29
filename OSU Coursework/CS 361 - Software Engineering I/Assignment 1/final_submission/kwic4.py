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


def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document:
        return []

    if periodsToBreaks:
        split_into_sentences = split_by_periods(document)
    else:
        split_into_sentences = document.splitlines()

    return split_into_sentences
