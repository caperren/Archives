import argparse
from collections import deque


class Wrestler(object):
    def __init__(self, name):
        self._name = name
        self._rivals = deque()
        self._parent = None
        self.depth = 0

    def __repr__(self):
        return "{}: {}".format(self._name, [rival.name for rival in self._rivals])

    @property
    def name(self):
        return self._name

    @property
    def rivals(self):
        return self._rivals

    def add_rival(self, rival):
        self._rivals.append(rival)


def wrestler_bfs(wrestlers):
    seen = set()
    bfs_babyfaces = []
    bfs_heels = []

    for wrestler in wrestlers:
        current_wrestlers = deque([wrestler])
        while current_wrestlers:
            current_wrestler = current_wrestlers.popleft()
            if current_wrestler.name not in seen:
                seen.add(current_wrestler.name)
                wrestlers_to_expand = deque(current_wrestler.rivals)

                if current_wrestler.depth % 2 == 0:
                    should_append = True
                    for rival_wrestler in wrestlers_to_expand:
                        if rival_wrestler.name in bfs_babyfaces:
                            should_append = False
                            break
                    if should_append:
                        bfs_babyfaces.append(current_wrestler.name)
                else:
                    should_append = True
                    for rival_wrestler in wrestlers_to_expand:
                        if rival_wrestler.name in bfs_heels:
                            should_append = False
                            break
                    if should_append:
                        bfs_heels.append(current_wrestler.name)

                for rival_wrestler in wrestlers_to_expand:
                    rival_wrestler.depth = current_wrestler.depth + 1

                current_wrestlers += wrestlers_to_expand

    return bfs_babyfaces, bfs_heels


def get_wrestler_info_from_file(filename):
    found_wrestlers = deque()
    found_wrestler_indexes = {}

    with open(filename, "r") as input_file:
        lines = input_file.readlines()

        current_line = 0
        file_num_wrestlers = int(lines[current_line])

        for i in range(file_num_wrestlers):
            current_line += 1
            wrestler_name = lines[current_line].strip("\n")

            found_wrestlers.append(Wrestler(wrestler_name))
            found_wrestler_indexes[wrestler_name] = len(found_wrestlers) - 1

        current_line += 1
        num_rivalries = int(lines[current_line])

        for _ in range(num_rivalries):
            current_line += 1
            rival_1_name, rival_2_name = lines[current_line].strip("\n").split(" ")

            rival_1_index = found_wrestler_indexes[rival_1_name]
            rival_2_index = found_wrestler_indexes[rival_2_name]

            found_wrestlers[rival_1_index].add_rival(found_wrestlers[rival_2_index])
            found_wrestlers[rival_2_index].add_rival(found_wrestlers[rival_1_index])

        return found_wrestlers


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Tool to determine which wrestlers are babyfaces or heels")

    parser.add_argument("input_filename", help="Filename of wrestler information to use")
    args = parser.parse_args()

    all_wrestlers = get_wrestler_info_from_file(args.input_filename)

    babyfaces, heels = wrestler_bfs(all_wrestlers)

    num_wrestlers = len(all_wrestlers)
    num_assigned = len(babyfaces) + len(heels)

    if num_wrestlers == num_assigned:
        print("Yes")
        print("Babyfaces: {}".format(" ".join(babyfaces)))
        print("Heels: {}".format(" ".join(heels)))
    else:
        print("No")
