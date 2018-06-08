import random
import os, shutil

NUM_READ_WRITES = 10
NUM_FILES = 100

RANDOM_PATH = "/dev/zero"
dev_random = open(RANDOM_PATH, "r")

FILES_FOLDER = "files"

file_test_structure = {
    "file": None,
    "size": None
}

ALL_FILES = []

if os.path.isdir(FILES_FOLDER):
    shutil.rmtree(FILES_FOLDER)

os.mkdir(FILES_FOLDER)

for i in range(NUM_FILES):
    temp = dict(file_test_structure)

    temp["file"] = open(FILES_FOLDER + "/" + str(i)+ ".txt", "w+")
    temp["size"] = random.randint(1000000, 10000000)
    ALL_FILES.append(temp)


for i in range(NUM_READ_WRITES):
    for j in range(NUM_FILES):
        current_file = ALL_FILES[j]
        current_file["file"].seek(0)
        current_file["file"].write(dev_random.read(current_file["size"]))

    for j in range(NUM_FILES):
        current_file = ALL_FILES[j]
        current_file["file"].seek(0)
        current_file["file"].read(current_file["size"])
