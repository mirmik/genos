#!/usr/bin/env python3

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("files", nargs="+")
parser.add_argument("-i", "--inplace", action="store_true")
parser.add_argument("-d", "--deleted", action="store_true")

args = parser.parse_args()

for file in args.files:
    text = open(file).read()
    lines = text.splitlines()

    lines_to_remove = []
    start_section = 0
    section_to_remove = False
    for i in range(len(lines)):
        line = lines[i]
        if "volatile" in line and "{" in line and "}":
            #oneline volatile function
            lines_to_remove.append(i)
            start_section = i
            section_to_remove = False
            continue

        #if line has only whitespace
        if section_to_remove and ("}" in line or line.strip() == ""):
            if "}" in line:
                lines_to_remove.append(i)
            if section_to_remove:
                for j in range(start_section, i):
                    lines_to_remove.append(j)
            start_section = i
            section_to_remove = False
            continue

        if line.strip() == "":
            start_section = i
            continue

        if "volatile" in line:
            section_to_remove = True

    print(f"file: {file} : {lines_to_remove}")

    if args.deleted:
        for i in sorted(lines_to_remove):
            print(f"{i}: {lines[i]}")
        continue
    else:
        text_without_volatile = ""
        if args.inplace:
            for i in range(len(lines)):
                if i not in lines_to_remove:
                    text_without_volatile += lines[i] + "\n"

            open(file, "w").write(text_without_volatile)
        else:
            for i in range(len(lines)):
                if i not in lines_to_remove:
                    text_without_volatile += f"{i}: " +  lines[i] + "\n"

            print(text_without_volatile)
