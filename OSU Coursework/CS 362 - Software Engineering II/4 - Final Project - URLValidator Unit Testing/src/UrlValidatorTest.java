/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.Random;

import junit.framework.TestCase;

/**
 * Performs Validation Test for url validations.
 * 
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May
 *          2011) $
 */
public class UrlValidatorTest extends TestCase {

	private boolean printStatus = false;
	private boolean printIndex = false;// print index that indicates current
										// scheme,host,port,path, query test
										// were using.

	public UrlValidatorTest(String testName) {
		super(testName);
	}

	public void testManualTest() {
		// Create new UrlValidator object
		UrlValidator urlVal = new UrlValidator(null, null,
				UrlValidator.ALLOW_ALL_SCHEMES);

		// Manual Tests:

		// These should return True
		System.out.println("The following should return true:");
		System.out.println(urlVal.isValid("http://www.amazon.com"));
		System.out.println(urlVal.isValid("http://www.google.com"));
		System.out.println(urlVal.isValid("http://www.com")); // Oddly enough
																// this is a
																// legit website
		System.out.println(urlVal.isValid("http://www.GOOGLE.com"));
		System.out.println(urlVal.isValid("HTTP://WWW.GOOGLE.COM"));
		System.out.println(urlVal.isValid("http://WWW.google.COM"));
		System.out.println(urlVal.isValid("reddit.com")); // RETURNS FALSE

		// These should return False
		System.out.println("The following should return false:");
		System.out.println(urlVal.isValid("http://www.invalid"));
		System.out.println(urlVal.isValid("reddit"));
		System.out.println(urlVal.isValid("www.reddit"));
		System.out.println(urlVal.isValid("al;skdjfals;kdjgas"));
		System.out.println(urlVal.isValid("www.:).com"));

	}

	/*
	 * The URLValidator can be split up into two disjoint sets of tests, tests
	 * that should pass and tests that should fail. This is consistent with an
	 * explanation of Input Domain Partitioning as found in an article written
	 * by Tomas Vagoun for the Annual Hawaii International Conference on System
	 * Sciences (https://goo.gl/acd0IL).
	 * 
	 * This verifies that the basics of the validator do work, and approaching
	 * this problem as black box testing is some of the best that we can do.
	 */

	public void testYourFirstPartition() {
		UrlValidator urlVal = new UrlValidator(null, null,
				UrlValidator.ALLOW_ALL_SCHEMES);

		String test;

		// This extremely ubiquitous URL is obviously completely true, and as
		// such, the validator should return true.

		test = "http://www.google.com";
		if (urlVal.isValid(test) == true) {
			System.out.println("Passed correctly at " + test + ".");
		} else {
			System.out.println("Failed incorrectly at " + test + ".");
		}

		// This test happened to fail, so I wrote my bug report about it
		test = "http://www.google.xxx";
		if (urlVal.isValid(test) == true) {
			System.out.println("Passed correctly at " + test + ".");
		} else {
			System.out.println("Failed incorrectly at " + test + ".");
		}

		// This test happened to fail, so I wrote my bug report about it
		test = "http://www.peta.xxx";
		if (urlVal.isValid(test) == true) {
			System.out.println("Passed correctly at " + test + ".");
		} else {
			System.out.println("Failed incorrectly at " + test + ".");
		}

	}

	public void testYourSecondPartition() {
		UrlValidator urlVal = new UrlValidator(null, null,
				UrlValidator.ALLOW_ALL_SCHEMES);

		String test;

		// But we can detect that the isvalid fails on a known incorrect version
		// so prove that it can still detect invalid URLs.
		test = "http://www.google.government";
		if (urlVal.isValid(test) == false) {
			System.out.println("Failed correctly at " + test + ".");
		} else {
			System.out.println("Failed incorrectly at " + test + ".");
		}

	}

	public void testIsValid() {
		// Print out that we're starting
		System.out.println("Starting programmatically generated tests!");

		// Make an instance of the url validator class
		UrlValidator url_validator = new UrlValidator(null, null,
				UrlValidator.ALLOW_ALL_SCHEMES);

		// Instantiate the random number generator
		Random random_generator = new Random();

		// Read in result pair arrays from the files that contain the test input
		ResultPair[] test_schemes = get_result_pairs_from_file("test_files/test_schemes.txt");
		ResultPair[] test_authorities = get_result_pairs_from_file("test_files/test_authorities.txt");
		ResultPair[] test_ports = get_result_pairs_from_file("test_files/test_ports.txt");
		ResultPair[] test_paths = get_result_pairs_from_file("test_files/test_paths.txt");

		// Set up some variables for keeping track of suceeded/failed tests.
		int match_count = 0;
		int fail_count = 0;

		// Main loop that will run the programmatic tests for so many times
		for (int i = 0; i < 100000; i++) {
			// Set up variables for another run through the loop
			String full_url_string = "";
			Boolean expected_validity = true;
			int current_rand = -1;

			// Get a random scheme, concatenate its string with the full url,
			// and bitwise and its validity result so the result becomes false
			// if necessary
			current_rand = random_generator.nextInt(test_schemes.length);
			full_url_string += test_schemes[current_rand].item;
			expected_validity &= test_schemes[current_rand].valid;

			// Get a random authority, concatenate its string with the full url,
			// and bitwise and its validity result so the result becomes false
			// if necessary
			current_rand = random_generator.nextInt(test_authorities.length);
			full_url_string += test_authorities[current_rand].item;
			expected_validity &= test_authorities[current_rand].valid;

			// Get a random port, concatenate its string with the full url,
			// and bitwise and its validity result so the result becomes false
			// if necessary
			current_rand = random_generator.nextInt(test_ports.length);
			full_url_string += test_ports[current_rand].item;
			expected_validity &= test_ports[current_rand].valid;

			// Get a random path, concatenate its string with the full url,
			// and bitwise and its validity result so the result becomes false
			// if necessary
			current_rand = random_generator.nextInt(test_paths.length);
			full_url_string += test_paths[current_rand].item;
			expected_validity &= test_paths[current_rand].valid;

			// Get the result of isValid from the url validator
			Boolean actual_validity = url_validator.isValid(full_url_string);

			// Make a boolean that tells us whether the expected and actual
			// validities match
			Boolean validities_match = (actual_validity == expected_validity);

			if (validities_match) {
				// If they match, increment our success counter.
				match_count++;

			} else {
				// Otherwise, print the failed url, and increment our fail count
				// Here, I also have another call to isValid that can be
				// uncommented for easy debugging of the failed url
				System.out.println("Failed on url: " + full_url_string);
				url_validator.isValid(full_url_string);
				fail_count++;
			}

		}

		// Print that the test is over
		System.out.println("Programmatically generated tests complete!");

		// Print the number of successes
		System.out.println(match_count + " tests succeeded!");

		// If there are failures, print the number of failures and a large
		// warning.
		if (fail_count > 0) {
			System.out.println("SOME TESTS FAILED!!!!");
			System.out.println(fail_count + " tests failed!");
		}

	}

	private ResultPair[] get_result_pairs_from_file(String filename) {
		// Create some file reading classes
		FileReader file_reader = null;
		BufferedReader buffered_reader = null;

		// Get the number of lines that are in the file
		int num_lines = get_num_lines_in_file(filename);

		// Try and open the file reader on the file or handle exceptions
		try {
			file_reader = new FileReader(filename);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		// Put this reader into the buffered reader to make it easy to work with
		buffered_reader = new BufferedReader(file_reader);

		// Create a result pair array with the correct number of entries
		ResultPair[] pairs = new ResultPair[num_lines];

		// Variable to hold the current line from the reader
		String current_line = null;

		// Do and initial read to start off, or handle exceptions
		try {
			current_line = buffered_reader.readLine();
		} catch (IOException e1) {
			e1.printStackTrace();
		}

		// For the number of lines in the file, loop
		for (int i = 0; i < num_lines; i++) {
			// Encapsulate in try block to hand exceptions
			try {
				// Split the current line by tabs, so there will be two indexes,
				// one with the string, and one saying whether it's valid
				String[] split_by_tab = current_line.split("\\t");

				// Set the name to the first index
				String name = split_by_tab[0];

				// Create and empty bool for validity
				Boolean is_valid = null;

				// Check the validity string in index one, and set the validity
				// boolean accordingly
				if (split_by_tab[1].equals("valid")) {
					is_valid = true;
				} else {
					is_valid = false;
				}

				// Set the current index of the result pair array to a new
				// result pair with the name and validity
				pairs[i] = new ResultPair(name, is_valid);

				// Read a new line for the next run through the loop
				current_line = buffered_reader.readLine();
			} catch (IOException e) {
				e.printStackTrace();
			}

		}

		// Now we're done, so close the file or handle exceptions
		try {
			buffered_reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		// Return our new array of pairs
		return pairs;
	}

	private int get_num_lines_in_file(String filename) {
		// Create a line number reader
		LineNumberReader line_reader = null;

		// Make a line reader and handle exceptions
		try {
			line_reader = new LineNumberReader(new FileReader(
					new File(filename)));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		// Skip to the end of the file or handle exceptions
		try {
			line_reader.skip(Long.MAX_VALUE);
		} catch (IOException e) {
			e.printStackTrace();
		}

		// Get the number of lines the reader has read. The extra one makes it
		// one indexed.
		int number_of_lines = line_reader.getLineNumber() + 1;

		// Close the file, or handle exceptions
		try {
			line_reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		// Return the number of lines we found
		return number_of_lines;
	}
}
