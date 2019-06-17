import re
from math import log
from os.path import exists
from os import remove

TRAINING_SET_FILENAME = "trainingSet.txt"
TEST_SET_FILENAME = "testSet.txt"

PREPROCESSED_TRAIN_OUTPUT_FILENAME = "preprocessed_train.txt"
PREPROCESSED_TEST_OUTPUT_FILENAME = "preprocessed_test.txt"
RESULTS_OUTPUT_FILENAME = "results.txt"

POSITIVE_LABEL = 1
NEGATIVE_LABEL = 0

VALID_LABELS = [NEGATIVE_LABEL, POSITIVE_LABEL]


# ##################################################
# ########## Functions for pre-processing ##########
# ##################################################


def get_lines_in_file(filename):
    with open(filename, "r") as current_file:
        return current_file.readlines()


def line_to_word_list_and_class(current_line):
    split_by_tabs = current_line.split("\t")
    review_sentence = split_by_tabs[0]

    review_sentence = review_sentence.strip()  # Remove beginning and ending whitespace
    review_sentence = re.sub(r"[^a-zA-Z ]", "", review_sentence)  # Strip everything but a-z, A-Z, and spaces
    review_sentence = review_sentence.lower()  # Make everything lowercase

    sentence_words = review_sentence.split(" ")  # Split sentence into words
    sentence_words = [word for word in sentence_words if word != ""]  # Remove empty strings
    sentence_words.sort()  # Put everything in alphabetical order

    return sentence_words, int(split_by_tabs[1])


def create_vocabulary_from_sentences_with_classes(raw_sentences):
    vocabulary_set = set()

    for current_sentence, _ in raw_sentences:
        for word in current_sentence:
            vocabulary_set.add(word)

    vocabulary_list = list(vocabulary_set)
    vocabulary_list.sort()

    vocabulary_indexer = {word: index for index, word in enumerate(vocabulary_list)}

    return vocabulary_list, vocabulary_indexer


def get_features_from_vocab_and_sentence(vocab_info, sentence_and_class):
    vocab_list, vocab_indexer = vocab_info
    sentence_words, sentence_class = sentence_and_class

    features = [0 for _ in range(len(vocab_list))]
    features.append(sentence_class)

    for word in sentence_words:
        if word in vocab_indexer:
            features[vocab_indexer[word]] = 1

    return features


def get_all_features_from_vocab_and_sentences(vocab_info, sentences):
    all_features = []

    for sentence in sentences:
        all_features.append(get_features_from_vocab_and_sentence(vocab_info, sentence))

    return all_features


def write_post_processed_data_to_file(vocab_info, features, filename):
    vocab_list, _ = vocab_info

    vocab_header = ",".join(vocab_list + ["classlabel"])

    with open(filename, "w+") as output_file:
        output_file.write(vocab_header + "\n")

        for feature in features:
            feature_output_string = ",".join([str(value) for value in feature])
            output_file.write(feature_output_string + "\n")


# ###################################################
# ########## Functions for naive bayes net ##########
# ###################################################


def train_net(vocabulary, features):
    vocab_info, vocab_mapping = vocabulary

    p_label = {}
    p_label_count = {}
    p_word_given_label = {
        word: {} for word in vocab_info
    }

    # Calculate p(CD)
    for label in VALID_LABELS:
        num_features_with_label = [feature[-1] for feature in features].count(label)
        num_features_total = len(features)
        p_label[label] = log(num_features_with_label / num_features_total)
        p_label_count[label] = num_features_with_label

    # Calculate p(word | label) for all words
    for cd_label in VALID_LABELS:
        for word in vocab_info:
            word_index = vocab_mapping[word]

            num_featuers_with_word_and_label = len([
                feature for feature in features
                if feature[-1] == cd_label and feature[word_index]
            ])

            p_word_given_label[word][cd_label] = log(
                (num_featuers_with_word_and_label + 1) / (p_label_count[cd_label] + len(VALID_LABELS))
            )

    return p_word_given_label, p_label


def test_net_on_features(vocabulary, features, probabilities):
    vocab_info, vocab_mapping = vocabulary
    p_word_given_label, p_label = probabilities

    results = []

    for feature in features:
        word_indices = [index for index, value in enumerate(feature[:-1]) if value]
        feature_probabilities = {}

        for label in VALID_LABELS:
            feature_probabilities[label] = p_label[label]

            for index in word_indices:
                feature_probabilities[label] += p_word_given_label[vocab_info[index]][label]

        feature_result = max(feature_probabilities, key=lambda x: feature_probabilities[x])
        results.append(feature_result)

    return results


# ##########################################
# ########## Functions for output ##########
# ##########################################
# ##########################################


def clean_output_file(filename):
    if exists(filename):
        remove(filename)


def evaluate_and_export_results(training_filename, testing_filename, output_filename, vocabulary, features, results):
    vocab_info, vocab_mapping = vocabulary

    total_count = len(results)
    correct_count = 0
    incorrect = []

    for row_index, result in enumerate(results):
        current_feature = features[row_index]

        feature_class_actual = current_feature[-1]

        if feature_class_actual == result:
            correct_count += 1
        else:
            words = [vocab_info[index] for index, value in enumerate(current_feature[:-1]) if value]
            incorrect.append((row_index, words, feature_class_actual, result))

    correct_percent = (correct_count / total_count) * 100.0

    results_string = "####################################################################\n"
    results_string += "Trained with file: {}\nTested with file: {}\n\n".format(training_filename, testing_filename)
    results_string += "Correct reviews: {}\nIncorrect reviews: {}\n".format(correct_count,
                                                                            len(incorrect))
    results_string += "Correct: {:.1f}%\n\n".format(correct_percent)

    for row_index, words, feature_class_actual, result in incorrect:
        results_string += "Failed on file row index {} with expected value {} and result {}.\n".format(
            row_index, feature_class_actual, result
        )

    results_string += "####################################################################\n\n\n"

    with open(output_filename, "a") as output_file:
        output_file.write(results_string)


# ##########################
# ########## Main ##########
# ##########################
if __name__ == "__main__":
    # ##### Pre-processing #####
    # Get vocabulary and training features
    training_file_lines = get_lines_in_file(TRAINING_SET_FILENAME)

    training_sentences_raw = [line_to_word_list_and_class(line) for line in training_file_lines]
    vocabulary_info = create_vocabulary_from_sentences_with_classes(training_sentences_raw)

    training_features = get_all_features_from_vocab_and_sentences(vocabulary_info, training_sentences_raw)

    # Get test features from existing vocabulary
    test_file_lines = get_lines_in_file(TEST_SET_FILENAME)
    test_sentences_raw = [line_to_word_list_and_class(line) for line in test_file_lines]
    test_features = get_all_features_from_vocab_and_sentences(vocabulary_info, test_sentences_raw)

    # Output pre-processed files
    write_post_processed_data_to_file(vocabulary_info, training_features, PREPROCESSED_TRAIN_OUTPUT_FILENAME)
    write_post_processed_data_to_file(vocabulary_info, test_features, PREPROCESSED_TEST_OUTPUT_FILENAME)

    # ##### Bayes net processing #####
    bayes_net_probabilities = train_net(vocabulary_info, training_features)

    training_results = test_net_on_features(vocabulary_info, training_features, bayes_net_probabilities)
    test_results = test_net_on_features(vocabulary_info, test_features, bayes_net_probabilities)

    # ##### Output results #####
    clean_output_file(RESULTS_OUTPUT_FILENAME)

    evaluate_and_export_results(TRAINING_SET_FILENAME,
                                TRAINING_SET_FILENAME,
                                RESULTS_OUTPUT_FILENAME,
                                vocabulary_info,
                                training_features,
                                training_results)

    evaluate_and_export_results(TRAINING_SET_FILENAME,
                                TEST_SET_FILENAME,
                                RESULTS_OUTPUT_FILENAME,
                                vocabulary_info,
                                test_features,
                                test_results)
