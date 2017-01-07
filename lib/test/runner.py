import sys
import difflib
from subprocess import call
from os import listdir, devnull
from os.path import isfile, join

def main(args):
    if args < 2:
        print "more args needed"
        return

    path = args[1]

    onlyfiles = [f for f in listdir(path) if isfile(join(path, f)) and f.endswith(".hex")]
    errors = 0
    tests = 0
    FNULL = open(devnull, 'w')
    for f in onlyfiles:
        tests += 1
        file_path = join(path, f)
        call(["python", "test/hex2bin.py", file_path]) #convert to binary
        filename = f.split('.', 1)[0].upper()
        bin_path = join(path, filename + ".bin")
        actual_out_path = join(path, filename + ".out")
        actual = open(actual_out_path, "r")
        out_path = join(path, filename + ".out1")
        gen = open(out_path, "w+")
        call(["./test-6502", bin_path], stderr=gen, stdout=FNULL) # exec test
        gen.seek(0, 0)
        diff = difflib.unified_diff(
            actual.readlines(),
            gen.readlines(),
        )
        diffs = [line for line in diff]
        if len(diffs) == 0:
            call(["rm", out_path])
        else:
            print "Error: " + filename
            errors += 1

    print "Total Tests: ", tests
    print "Total Errors: ", errors
    print "Percentage Passing: ", (1.0 - float(errors) / float(tests)) * 100, "%"


if __name__ == "__main__":
    main(sys.argv)
