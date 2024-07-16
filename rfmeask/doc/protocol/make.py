#!/usr/bin/env python3

import os
import licant

def latex(tgt, tgtname, deps, src):
	licant.makefile(tgt, tgtname=tgtname, deps=deps, src=src, do="pdflatex -jobname={tgtname} {src}")

licant.source("ru/protocol.tex")
licant.source("ru/scpi-section.tex")
licant.source("ru/ncpi-section.tex")

licant.source("en/protocol.tex")
licant.source("en/scpi-section.tex")
licant.source("en/ncpi-section.tex")

latex(src="ru/protocol.tex", tgt="protocol-ru.pdf", tgtname="protocol-ru",
	deps = [
		"ru/protocol.tex",
		"ru/scpi-section.tex",
		"ru/ncpi-section.tex",
])

latex(src="en/protocol.tex", tgt="protocol-en.pdf", tgtname="protocol-en",
	deps = [
		"en/protocol.tex",
		"en/scpi-section.tex",
		"en/ncpi-section.tex",
])

@licant.routine(deps = ["protocol-ru.pdf"])
def show_ru():
	os.system("evince protocol-ru.pdf")

@licant.routine(deps = ["protocol-en.pdf"])
def show_en():
	os.system("evince protocol-en.pdf")

@licant.routine
def clean():
	os.system("rm *.log")
	os.system("rm *.pdf")
	os.system("rm *.aux")
	os.system("rm *.toc")

licant.fileset("all", targets=["protocol-ru.pdf", "protocol-en.pdf"])

licant.ex("all")