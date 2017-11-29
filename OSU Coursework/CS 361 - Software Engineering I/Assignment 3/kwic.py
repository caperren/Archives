# Created by Corwin Perren

######################################################
########## Begin Kwic Class Implementation ###########
######################################################
class Kwic(object):
    def __init__(self, ignoreWords=None, periodsToBreaks=False):
        self.ignore_words = ignoreWords
        self.periods_to_breaks = periodsToBreaks

        self.all_sentence_tuples = []

        self.kwic_output_array = []
        self.list_pairs_output_array = []

        self.text_to_add = ""

        self.event_spec_instance = EventSpec("kwic.fsm")
        self.event_spec_instance.event("callConstructor")

    def addText(self, new_text):
        self.event_spec_instance.event("callAddText")
        self.text_to_add += " " + str(new_text)

    def index(self):
        self.event_spec_instance.event("callIndex")
        self.__process_kwic_index()
        return self.kwic_output_array

    def listPairs(self):
        self.event_spec_instance.event("callListPairs")
        self.__process_list_pairs()
        return self.list_pairs_output_array

    def reset(self):
        self.event_spec_instance.event("callReset")
        self.all_sentence_tuples = []

        self.kwic_output_array = []
        self.list_pairs_output_array = []

        self.text_to_add = []

    def print_eventspec_log(self):
        self.event_spec_instance.printLog()

    def __process_kwic_index(self):
        self.event_spec_instance.event("callProcessIndex")
        if self.text_to_add:
            if self.periods_to_breaks:
                split_into_sentences = self.__split_by_periods(self.text_to_add)
            else:
                self.event_spec_instance.event("processNewlineSplit")
                split_into_sentences = self.text_to_add.split('\n')

            split_into_word_tuples = self.__split_by_word_as_tuples(split_into_sentences)

            for sentence_tuple in split_into_word_tuples:
                self.all_sentence_tuples.append(sentence_tuple)

            circular_shifted_data = self.__fill_with_circular_shifts_and_original(split_into_word_tuples)

            if self.ignore_words:
                circular_shifted_data = self.__remove_words(circular_shifted_data, self.ignore_words)

            for current_tuple in circular_shifted_data:
                self.kwic_output_array.append(current_tuple)

            self.text_to_add = ""

            self.kwic_output_array = self.__alphabetize_tuple_list(self.kwic_output_array)
        else:
            self.event_spec_instance.event("processNoNewText")

    def __process_list_pairs(self):
        self.event_spec_instance.event("callProcessListPairs")
        if self.text_to_add:
            self.__process_kwic_index()

        self.list_pairs_output_array = self.__create_list_pairs(self.all_sentence_tuples)

    def __fill_with_circular_shifts_and_original(self, sentence_array):
        self.event_spec_instance.event("callFillCircular")
        output_array = []

        for current_tuple in sentence_array:
            for index, _ in enumerate(current_tuple[0]):
                output_array.append((self.__array_circular_shift(current_tuple[0], index), current_tuple[1]))

        return output_array

    def __alphabetize_tuple_list(self, input_array):
        self.event_spec_instance.event("callAlphabetize")
        sorted_array = sorted(input_array, key=self.__alphabetized_key)
        return sorted_array

    def __create_list_pairs(self, input_array):
        self.event_spec_instance.event("callCreateListPairs")
        known_pairs = {}

        for sentence_array, _ in input_array:
            seen_in_sentence = set([])

            for first_word in sentence_array:
                for second_word in sentence_array:
                    first, second = self.__return_ordered_words(self.__sanitize_word(first_word),
                                                                self.__sanitize_word(second_word))

                    if (first == second) or (first == ""):
                        continue

                    if (first, second) not in seen_in_sentence:
                        seen_in_sentence.add((first, second))

                        if (first, second) in known_pairs:
                            known_pairs[(first, second)] += 1
                        else:
                            known_pairs[(first, second)] = 1

        output_list = []

        for key in known_pairs:
            if known_pairs[key] > 1:
                output_list.append((key, known_pairs[key]))

        output_list.sort(key=self.__alphabetized_key)

        return output_list

    def __split_by_periods(self, document):
        self.event_spec_instance.event("callSplitPeriods")
        output_array = []
        temp_sentence = ""
        document_length_zero_indexed = len(document) - 1
        for current_index, current_value in enumerate(document):
            if current_value == '.':
                if (current_index == 0) or (current_index == document_length_zero_indexed) or \
                        (document[current_index - 1].islower() and (document[current_index + 1].isspace() or
                                                                        (document[current_index + 1] == '\n'))):
                    temp_sentence += current_value
                    output_array.append(temp_sentence)
                    temp_sentence = ""
            else:
                if current_value != '\n':
                    temp_sentence += current_value
                else:
                    temp_sentence += " "

        if temp_sentence:
            output_array.append(temp_sentence)
        return output_array

    def __split_by_word_as_tuples(self, sentence_array):
        self.event_spec_instance.event("callSplitAsTuples")

        output_array = []
        index_incrementer = 0

        for sentence in sentence_array:
            words_array = sentence.split(" ")
            words_array = filter(None, words_array)
            output_array.append((words_array, index_incrementer))
            index_incrementer += 1

        return output_array

    def __array_circular_shift(self, input_array, rotate_val):
        output_array = input_array[rotate_val:] + input_array[:rotate_val]
        return output_array

    def __alphabetized_key(self, input_data):
        output_array = []
        for word in input_data[0]:
            output_array.append(word.lower())
        return output_array

    def __remove_words(self, input_array, words):
        self.event_spec_instance.event("callRemoveWords")

        lowered_input = []
        output_array = []

        for word in words:
            lowered_input.append(word.lower())

        for current_tuple in input_array:
            if current_tuple[0][0].lower().strip(".:!?,") in lowered_input:
                pass
            else:
                output_array.append(current_tuple)

        return output_array

    def __sanitize_word(self, input_word):
        return input_word.lower().translate(None, ".,?!:")

    def __return_ordered_words(self, word_one, word_two):

        if word_one < word_two:
            return word_one, word_two
        else:
            return word_two, word_one


######################################################
########## Begin Provided EventSpec Include ##########
######################################################
class EventSpec():
    def readfsm(self, file):
        # takes a filename, returns a finite state machine
        # fsm is (begin state, structure)
        fsm = {}
        with open(file) as f:
            s = None
            t = {}
            for l in f:
                ls = l.split()
                if (ls == []) or (ls[0][0] == "#"):
                    continue
                if ls[0] == "state:":
                    if s != None:
                        if s in fsm:
                            raise SyntaxError("Cannot define state " + s + " twice")
                        fsm[s] = t
                    s = ls[1]
                    t = {}
                elif ls[1] == "->":
                    t[ls[0]] = ls[2]
                elif ls[0] == "begin:":
                    beginState = ls[1]
                else:
                    raise SyntaxError(l + " is not a line in a finite state machine definition")
            if s != None:
                fsm[s] = t
            return (beginState, fsm)

    def __init__(self, file):
        (self.start, self.machine) = self.readfsm(file)
        self.state = self.start
        self.trace = []
        self.triggers = {}

    def onEvent(self, event, action):
        self.triggers[event] = action

    def reset(self):
        self.state = self.start
        self.trace = []

    def trace(self):
        return self.trace

    def state(self):
        return self.state

    def printLog(self):
        i = 0
        for (e, s) in self.trace:
            print "  STEP #" + str(i) + ":", e, "-->", s
            i += 1

    def event(self, event):
        try:
            self.state = self.machine[self.state][event]
            self.trace.append((event, self.state))
            if event in self.triggers:
                self.triggers[event]()
        except KeyError:
            raise RuntimeError("From state " + self.state + ", transition " + event + " is not allowed (trace: " + str(
                self.trace) + ")")
