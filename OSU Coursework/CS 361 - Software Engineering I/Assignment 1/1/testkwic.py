from kwic import kwic

empty_document = ""
design_words_doc = "Design is hard.\nLet's just implement."

if __name__ == "__main__":
    assert(kwic(empty_document) == [])      # Ensure empty input gives empty output
    assert(kwic(design_words_doc) != [])    # Ensure real input does not produce empty output