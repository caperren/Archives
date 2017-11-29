from kwic import kwic

empty_document = ""
design_words_doc = "Design is hard.\nLet's just implement."
goodbye_buddy_doc = "Hello there.\nHello there, buddy.\nHello and goodbye, buddy.\nHello is like buddy Goodbye!"

if __name__ == "__main__":
    assert(kwic(empty_document) == [])      # Ensure empty input gives empty output
    assert(kwic(design_words_doc) != [])    # Ensure real input does not produce empty output

    assert(len(kwic(design_words_doc)[0]) == 2)          # [(), ()] Make sure it's broken into two lines
    assert(len(kwic(goodbye_buddy_doc)[0]) == 4)          # [(), ()] Make sure it's broken into four lines
