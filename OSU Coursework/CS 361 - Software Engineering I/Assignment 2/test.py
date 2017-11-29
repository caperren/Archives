import time
import kwic as kwic_original
import fastkwic as kwic_fast
import testkwic as kwic_test

num_tests = 1

test_original_kwic = True
print_original_kwic = False

test_fast_kwic = True
print_fast_kwic = False

test_test_kwic = True
print_test_kwic = False

design_words_doc = "Design is hard.\nLet's just implement."
goodbye_buddy_doc = "Hello there.\nHello there, buddy.\nHello and goodbye, buddy.\nHello is like buddy Goodbye!"
hello_buddy_periods = "Hello there.  Hello there, buddy.    Hello and goodbye, buddy. Hello is like buddy Goodbye!"
letters_and_stuff = "It's very nice to be footloose. \nWith just a toothbrush and a comb.\n"

open_file = open("test_documents/chesterton_short.txt", "r")
file_as_lines = open_file.readlines()
file_as_string = ""

for line in file_as_lines:
    file_as_string += line

del file_as_lines
open_file.close()

input_document = file_as_string

if __name__ == "__main__":
    original_output = None
    fast_output = None
    test_output = None

    original_times = []
    fast_times = []
    test_times = []

    for i in range(num_tests):
        if test_original_kwic:
            print "\nTesting kwic.py"
            start_time = time.time()
            # original_output = kwic_original.kwic(input_document)
            original_output = kwic_original.kwic(input_document, listPairs=True)
            if print_original_kwic:
                print original_output
            total = time.time() - start_time
            original_times.append(total)
            print "kwic.py took " + str(total) + " seconds."

        if test_fast_kwic:

            print "\nTesting fastkwic.py"
            start_time = time.time()
            # fast_output = kwic_fast.kwic(input_document)
            fast_output = kwic_fast.kwic(input_document, listPairs=True)
            if print_fast_kwic:
                print fast_output
            total = time.time() - start_time
            fast_times.append(total)
            print "fastkwic.py took " + str(total) + " seconds."

        if test_test_kwic:

            print "\nTesting testkwic.py"
            start_time = time.time()
            # test_output = kwic_test.kwic(input_document)
            test_output = kwic_test.kwic(input_document, listPairs=True)
            if print_test_kwic:
                print test_output
            total = time.time() - start_time
            test_times.append(total)
            print "testkwic.py took " + str(total) + " seconds."

    print "\nOriginal == Fast: " + str(original_output == fast_output)
    print "Original == Test: " + str(original_output == test_output)
    print "Test == Fast: " + str(test_output == fast_output)
    print "\n\n"
    if test_original_kwic:
        print "Original Avg: " + str(sum(original_times)/ float(len(original_times)))

    if test_fast_kwic:
        print "Fast Avg: " + str(sum(fast_times) / float(len(fast_times)))

    if test_test_kwic:
        print "Test Avg: " + str(sum(test_times) / float(len(test_times)))

