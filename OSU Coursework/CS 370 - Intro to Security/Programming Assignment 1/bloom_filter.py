#!/usr/bin/env python3
# ##### Includes #####
# System includes
import sys
import getopt
import hashlib
import random
from time import time

# ##### Global Variables #####
USAGE_STRING = "usage: ./bloom_filter.py -d dictionary.txt -i input.txt -o output3.txt output5.txt"
NUM_ARGUMENTS_CORRECT = 7

HASH_ARRAY_SIZE = 5976456  # should be a 1% false positive rate


ARGUMENT_MAPPING = {
    "dictionary": 1,
    "input": 3,
    "three_hash": 5,
    "five_hash": 6
}

AVAILABLE_HASHES = [
    "ripemd160",
    "sha256",
    "whirlpool",
    "md5",
    "DSA"
]


# ##### Bloom Filter Class #####
class BloomFilter(object):
    def __init__(self, arguments):
        super(BloomFilter, self).__init__()

        if len(arguments) != NUM_ARGUMENTS_CORRECT:
            print(USAGE_STRING)
            sys.exit(2)

        self.dictionary_path = None
        self.input_file_path = None
        self.three_hash_output_path = None
        self.five_hash_output_path = None

        self.three_hash_dictionary = {i: 0 for i in range(HASH_ARRAY_SIZE)}
        self.five_hash_dictionary = {i: 0 for i in range(HASH_ARRAY_SIZE)}

        self.dictionary_path = arguments[ARGUMENT_MAPPING["dictionary"]]
        self.input_file_path = arguments[ARGUMENT_MAPPING["input"]]
        self.three_hash_output_path = arguments[ARGUMENT_MAPPING["three_hash"]]
        self.five_hash_output_path = arguments[ARGUMENT_MAPPING["five_hash"]]

    def generate_filters(self):
        dictionary_file = open(self.dictionary_path, "r", encoding="latin-1")
        lines = dictionary_file.read().splitlines()

        print("Generating filter using \"%s\". This will take a few moments." % self.dictionary_path)

        for password in lines:
            clean_password = password.strip()
            for i in range(5):
                five_hasher = hashlib.new(AVAILABLE_HASHES[i])
                five_hasher.update(clean_password.encode())
                current_hash = int(five_hasher.hexdigest(), 16)

                self.five_hash_dictionary[current_hash % HASH_ARRAY_SIZE] = 1

            for i in range(3):
                three_hasher = hashlib.new(AVAILABLE_HASHES[i])
                three_hasher.update(clean_password.encode())
                current_hash = int(three_hasher.hexdigest(), 16)
                self.three_hash_dictionary[current_hash % HASH_ARRAY_SIZE] = 1

        print("Filter generation complete.")

        dictionary_file.close()

    def process_inputs_and_generate_outputs(self):
        input_file = open(self.input_file_path, "r", encoding="latin-1")
        lines = input_file.read().splitlines()

        output_file_three_hash = open(self.three_hash_output_path, "w")
        output_file_five_hash = open(self.five_hash_output_path, "w")

        print("Processing input file \"%s\" and writing outputs to \"%s\" and \"%s\"." %
              (self.input_file_path, self.three_hash_output_path, self.five_hash_output_path))

        for password in lines[1:]:
            in_set_three = True
            in_set_five = True

            clean_password = password.strip()
            for i in range(5):
                five_hasher = hashlib.new(AVAILABLE_HASHES[i])
                five_hasher.update(clean_password.encode())
                current_hash = int(five_hasher.hexdigest(), 16)

                if self.five_hash_dictionary[current_hash % HASH_ARRAY_SIZE] == 0:
                    in_set_five = False

            for i in range(3):
                three_hasher = hashlib.new(AVAILABLE_HASHES[i])
                three_hasher.update(clean_password.encode())
                current_hash = int(three_hasher.hexdigest(), 16)

                if self.three_hash_dictionary[current_hash % HASH_ARRAY_SIZE] == 0:
                    in_set_three = False

            output_file_three_hash.write("%s\n" % ("no" if not in_set_three else "maybe"))
            output_file_five_hash.write("%s\n" % ("no" if not in_set_five else "maybe"))

        print("Processing complete.")

        input_file.close()
        output_file_three_hash.close()
        output_file_five_hash.close()


# ##### Main #####
if __name__ == "__main__":
    bloom_filter = BloomFilter(sys.argv[1:])
    bloom_filter.generate_filters()
    bloom_filter.process_inputs_and_generate_outputs()
