def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document:
        return []

    split_into_sentences = document.splitlines()

    return [split_into_sentences]