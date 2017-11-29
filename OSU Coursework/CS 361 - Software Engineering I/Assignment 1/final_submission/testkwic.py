import kwic

empty_document = ""
design_words_doc = "Design is hard.\nLet's just implement."
goodbye_buddy_doc = "Hello there.\nHello there, buddy.\nHello and goodbye, buddy.\nHello is like buddy Goodbye!"
hello_buddy_periods = "Hello there.  Hello there, buddy.    Hello and goodbye, buddy. Hello is like buddy Goodbye!"

this_is_split_periods_circular_output = [
    (['This', 'is', 'something', 'split', 'by', 'periods.'], 0),
    (['is', 'something', 'split', 'by', 'periods.', 'This'], 0),
    (['something', 'split', 'by', 'periods.', 'This', 'is'], 0),
    (['split', 'by', 'periods.', 'This', 'is', 'something'], 0),
    (['by', 'periods.', 'This', 'is', 'something', 'split'], 0),
    (['periods.', 'This', 'is', 'something', 'split', 'by'], 0),
    (['Not', 'newlines.'], 1),
    (['newlines.', 'Not'], 1)
]

if __name__ == "__main__":
    # Ensure empty input gives empty output
    assert(kwic.kwic(empty_document) == ([]))

    # Ensure real input does not produce empty output
    assert(kwic.kwic(design_words_doc) != ([]))

    # Make sure it's broken into two lines
    assert(len(kwic.kwic(design_words_doc)) == 6)

    # Make sure it's broken into four line
    assert(len(kwic.kwic(goodbye_buddy_doc)) == 14)

    # Just realized I can check each individual function, so here is the check to split by periods
    assert(kwic.split_by_periods("This is something split \nby periods. Not \n newlines.") ==
           ["This is something split by periods.", " Not  newlines."])

    # This checks to make sure a sentence gets split into words properly
    assert(kwic.split_by_word_as_tuples(["This is something split by periods.", " Not  newlines."]) ==
           [(['This', 'is', 'something', 'split', 'by', 'periods.'], 0), (['Not', 'newlines.'], 1)])

    # These check to make sure that the circular shift function is rotating properly
    assert(kwic.array_circular_shift(["One", "Two", "Three", "Four", "Five"], 0) ==
           ["One", "Two", "Three", "Four", "Five"])

    assert (kwic.array_circular_shift(["One", "Two", "Three", "Four", "Five"], 2) ==
            ["Three", "Four", "Five", "One", "Two"])

    # This checks to make sure than circularly shifted versions of all sentences are correct
    assert(kwic.fill_with_circular_shifts_and_original([(['This', 'is', 'something', 'split', 'by', 'periods.'], 0),
                                                        (['Not', 'newlines.'], 1)]) ==
           this_is_split_periods_circular_output)

    # This gives a bit of a sanity check. It's making sure the circular shift works, and that the output is formatted
    # correctly...
    assert(kwic.kwic("This is something split \nby periods. Not \n newlines.", periodsToBreaks=True) ==
           this_is_split_periods_circular_output)
