from kwic import kwic

empty_document = ""
design_words_doc = "Design is hard.\nLet's just implement."
goodbye_buddy_doc = "Hello there.\nHello there, buddy.\nHello and goodbye, buddy.\nHello is like buddy Goodbye!"
hello_buddy_periods = "Hello there.  Hello there, buddy.    Hello and goodbye, buddy. Hello is like buddy Goodbye!"

if __name__ == "__main__":
    # Ensure empty input gives empty output
    assert(kwic(empty_document) == [])

    # Ensure real input does not produce empty output
    assert(kwic(design_words_doc) != [])

    # Make sure it's broken into two lines
    assert(len(kwic(design_words_doc)[0]) == 2)

    # Make sure it's broken into four line
    assert(len(kwic(goodbye_buddy_doc)[0]) == 4)

    # Make sure line with just periods only shows up as one normally
    assert(len(kwic(hello_buddy_periods)[0]) == 1)

    # Make sure it's broken into four lines once it's broken by periods instead
    assert(len(kwic(hello_buddy_periods, periodsToBreaks=True)[0]) == 4)
