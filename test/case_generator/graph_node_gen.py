#!/usr/bin/python
import sys
import getopt
import logging
import numpy as np

log = None

amount_to_gen = 100
id_min = -1000
id_max = 1000
x_coord_min = -1000
x_coord_max = 1000
y_coord_min = -1000
y_coord_max = 1000
output_filename = "nodes.txt"

def generateRandomNode():
    return "{0} {1} {2}".format(
        np.random.randint(id_min, id_max + 1),
        np.random.randint(x_coord_min, x_coord_max + 1),
        np.random.randint(y_coord_min, y_coord_max + 1)
    )

def parseArgs(argv):
    global amount_to_gen, id_min, id_max, x_coord_min, x_coord_max, y_coord_min, y_coord_max, output_filename
    try:
        opts, _ = getopt.getopt(argv, "s:n:o:xms:xmx:yms:ymx:ims:imx")
    except getopt.GetoptError:
        log.error("Invalid arguments")
        sys.exit(2)

    for opt, arg in opts:
        if (opt == "-s"):
            if (0 < int(arg) < 100):
                max_string_size = int(arg)
            else:
                log.error(
                    "Invalid string size [" + arg + "]. Must be in range [1,100]")
                sys.exit(1)
        elif (opt == "-n"):
            if (0 < int(arg)):
                amount_to_gen = int(arg)
            else:
                log.error(
                    "Invalid amount to generate [" + arg + "]. Must be in range [1,inf)")
                sys.exit(1)
        elif (opt == "-o"):
            if (str(arg).endswith(".txt")):
                output_filename = arg
            else:
                log.error("Output file must be in text format")
                sys.exit(1)
        elif (opt == "xms"):
            x_coord_min = int(arg)
        elif (opt == "xmx"):
            x_coord_max = int(arg)
        elif (opt == "yms"):
            y_coord_min = int(arg)
        elif (opt == "ymx"):
            y_coord_max = int(arg)
        elif (opt == "ims"):
            id_min = int(arg)
        elif (opt == "imx"):
            id_max = int(arg)


if __name__ == "__main__":
    log = logging.getLogger(__name__)
    sh = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter(
        '[%(asctime)s][%(name)s] %(levelname)s :: %(message)s')
    sh.setFormatter(formatter)
    log.addHandler(sh)
    log.setLevel(logging.INFO)

    log.info("Parsing arguments...")
    parseArgs(sys.argv[1:])
    log.info("Arguments parsed")
    f = open(output_filename, "w+")
    log.info("Generating graph nodes...")
    for i in range(amount_to_gen):
        f.write(generateRandomNode() + "\n")
    f.close()
    log.info("Finished generating. Output to: " + output_filename)
