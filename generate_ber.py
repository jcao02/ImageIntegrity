#!/usr/bin/env python

from os import walk
from subprocess import Popen

mypath = 'images/'
outfile = open('tmp', 'w')


def main():
    """docstring for main"""

    originals = []  #Name of the originals files
    attacks = []    #Name of the attacks 
    # Getting files and directories
    for(dirpath, dirnames, filenames) in walk(mypath):
        originals.extend(filenames)
        attacks.extend(dirnames)
        break

    # Going throught originals
    #Loop throught attacks
    for attack in attacks:
        modifieds = []
        # Getting modified files from attack directory
        for (dirpath, dirnames, filenames) in walk(mypath + attack):
            modifieds.extend(filenames)
            break

        print attack + "---------------------------------------------"#Printing attack for distinction

        edges = 0.0
        dct   = 0.0
        for orig in originals:
        #Loop throught modified files
            for modified in modifieds:
                if orig == modified:
                    img1 = mypath + orig
                    img2 = mypath + attack + '/' + modified
                    p = Popen(["./watermarking", img1, img2, "1"], stdout=outfile)
                    code = p.wait()
                    outfile.flush()
                    tmp = open("tmp", "r")
                    edges += float(tmp.read())
                    tmp.close()
                    outfile.seek(0)
                    outfile.truncate()
                    p = Popen(["./watermarking", img1, img2, "2"], stdout=outfile)
                    code = p.wait()
                    outfile.flush()
                    tmp = open("tmp", "r")
                    dct += float(tmp.read())
                    tmp.close()
                    outfile.seek(0)
                    outfile.truncate()
        
        l = len(modifieds)
        print "DCT"
        print float(dct/l)
        print "EDGES"
        print float(edges/l)


if __name__ == "__main__":
    main()
