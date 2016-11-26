import re
import os
import glob

INPUT_DIR = 'outputs/2'
OUTPUT_FILE = 'merge_output.txt'

def makedir_safe(path):
    """ if dir not exists, make dir. """
    if not os.path.exists(path):
        os.makedirs(path)


def sample():
    input_files = glob.glob(INPUT_DIR + "/*.txt")
    input_num = len(input_files)
    edge_map = {}    
    for input_file in input_files:
        with open(input_file, 'rb') as r:
            lines = r.readlines()
            for line in lines[1:]:
                splitted = line.split(' ')
                key = splitted[0] + " " + splitted[1]
                edge_map.setdefault(key, 0)
                edge_map[key] += float(splitted[2])

    with open(OUTPUT_FILE, 'wb') as w:
        for key, value in edge_map.iteritems():
            w.write(key + " " + str(value / input_num) + "\n")


if __name__ == "__main__":
    sample()
