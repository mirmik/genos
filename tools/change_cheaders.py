#!/usr/bin/env python3

import os
import sys

def is_ext(path, ext):
    tpl = os.path.splitext(path)
    return tpl[1] == ext

path = sys.argv[1]
abspath = os.path.abspath(path)
tree = os.walk(abspath)

allfiles = []

if os.path.isdir(abspath):
    for address, dirs, files in tree:
        for f in files:
            allfiles.append(os.path.join(address, f))
else:
    allfiles.append(abspath)

c_files = [ f for f in allfiles if is_ext(f, '.c') ]
cpp_files = [ f for f in allfiles if is_ext(f, '.cpp') ]
h_files = [ f for f in allfiles if is_ext(f, '.h') ]

changes = [
    ("<string.h>", "<cstring>"),
    ("<ctype.h>",  "<cctype>"),
    ("<stdint.h>", "<cstdint>"),
    ("<stddef.h>", "<cstddef>"),
    ("<assert.h>", "<cassert>"),
]

for f in cpp_files + h_files:
    lines = open(f).readlines()
    has_problem = 0 

    outtext = ""
    for a, b in changes:
        for l in lines:
            if a in l:
                #text.replace(a, b)
                has_problem += 1
                outtext = outtext + f"- {l}"
                outtext = outtext + f"+ {l.replace(a,b)}"

                index = lines.index(l)
                lines[index] = l.replace(a,b)

    if has_problem:
        print(f"File: {f} has {has_problem} problem")
        print(outtext)

    text = "".join(lines)
    open(f, "w").write(text)
    
