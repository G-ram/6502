import sys

def main(args):
    if len(args) < 2:
        print "Not enough args supplied"
        return

    filename = args[1]
    hex = open(filename, 'r')
    hex = ''.join(hex.read().split())
    bits = bytearray.fromhex(hex)
    paths = filename.rsplit('/', 1)
    new_filename = paths[0] + "/" + paths[1].split('.', 1)[0].upper() + ".bin"
    out = open(new_filename, 'wb')
    out.write(bits)

if __name__ == "__main__":
    main(sys.argv)
