from kwic import kwic

empty_document = ""
design_words_doc = "Design is hard.\nLet's just implement."
goodbye_buddy_doc = "Hello there.\nHello there, buddy.\nHello and goodbye, buddy.\nHello is like buddy Goodbye!"
hello_buddy_periods = "Hello there.  Hello there, buddy.    Hello and goodbye, buddy. Hello is like buddy Goodbye!"

hello_buddy_periods_output = [(["Hello", "there.", "Hello", "there,", "buddy.", "Hello", "and", "goodbye,", "buddy.",
                                "Hello", "is", "like", "buddy", "Goodbye!"], 0)]

hello_buddy_word_tuples_output = [(["Hello", "there."], 0),
                                  (["Hello", "there,", "buddy."], 1),
                                  (["Hello", "and", "goodbye,", "buddy."], 2),
                                  (["Hello", "is", "like", "buddy", "Goodbye!"], 3)]

if __name__ == "__main__":
    # Ensure empty input gives empty output
    assert(kwic(empty_document) == [])

    # Ensure real input does not produce empty output
    assert(kwic(design_words_doc) != [])

    # Make sure it's broken into two lines
    assert(len(kwic(design_words_doc)) == 2)

    # Make sure it's broken into four line
    assert(len(kwic(goodbye_buddy_doc)) == 4)

    # Make sure array contains same elements from kwic vs test output
    assert(any(x in kwic(hello_buddy_periods) for x in hello_buddy_periods_output))

    # Make sure arrays contain the same elements from kwic vs test, even with periods for breaks...
    assert(any(x in kwic(hello_buddy_periods, periodsToBreaks=True) for x in hello_buddy_word_tuples_output))
