#!/usr/bin/python
import sys, getopt, logging, string
import numpy as np

log = None

charset = string.printable
character_sets = {
    "ALL": string.printable,
    "ALL_ASCII": string.ascii_letters,
    "UPPER_ASCII": string.ascii_uppercase,
    "LOWER_ASCII": string.ascii_lowercase,
    "DIGITS": string.digits
}
amount_to_gen = 100
max_string_size = 32
output_filename = "strings.txt"

def generateRandomString():
    return "".join(np.random.choice(list(charset), np.random.randint(1, max_string_size + 1)))

def parseArgs(argv):
    global charset, amount_to_gen, max_string_size, output_filename
    try:
        opts, _ = getopt.getopt(argv, "c:s:n:o:")
    except getopt.GetoptError:
        log.error("Invalid arguments")
        sys.exit(2)

    for opt, arg in opts:
        if (opt == "-c"):
            try:
                charset = character_sets[arg.upper()]
            except:
                log.error("Invalid charset: " + str(arg))
                sys.exit(1)
        elif (opt == "-s"):
            if (0 < int(arg) < 100):
                max_string_size = int(arg)
            else:
                log.error("Invalid string size [" + arg + "]. Must be in range [1,100]")
                sys.exit(1)
        elif (opt == "-n"):
            if (0 < int(arg)):
                amount_to_gen = int(arg)
            else:
                log.error("Invalid amount to generate [" + arg + "]. Must be in range [1,inf)")
                sys.exit(1)
        elif (opt == "-o"):
            if (str(arg).endswith(".txt")):
                output_filename = arg
            else:
                log.error("Output file must be in text format")
                sys.exit(1)

if __name__ == "__main__":
    log = logging.getLogger(__name__)
    sh = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter('[%(asctime)s][%(name)s] %(levelname)s :: %(message)s')
    sh.setFormatter(formatter)
    log.addHandler(sh)
    log.setLevel(logging.INFO)

    log.info("Parsing arguments...")
    parseArgs(sys.argv[1:])
    log.info("Arguments parsed")
    f = open(output_filename, "w+")
    log.info("Generating strings...")
    for i in range(amount_to_gen):
        f.write(generateRandomString() + "\n")
    f.close()
    log.info("Finished generating. Output to: " + output_filename)
