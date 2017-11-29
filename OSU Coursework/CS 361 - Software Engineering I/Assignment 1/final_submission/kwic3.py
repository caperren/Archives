def kwic(document, listPairs=False, ignoreWords=None, periodsToBreaks=False):
    if not document:
        return []

    if periodsToBreaks:
        split_into_sentences = document.split(".")
    else:
        split_into_sentences = document.splitlines()

    return [split_into_sentences]
