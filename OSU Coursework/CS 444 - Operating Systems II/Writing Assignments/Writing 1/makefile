# Makefile created by Corwin Perren
# Generic makefile for all LaTeX projects downloaded from overleaf
# 
# All this makefile does is call perl against latexmkrc, which is
# the latex equivalent of make

LATEXMK_COMPILE_FLAGS = -cd -pdf output.tex
LATEXMK_CLEAN_FLAGS = -c

.DEFAULT_GOAL := all

all: latexmk_output clean

latexmk_output: 
	perl latexmk.pl $(LATEXMK_COMPILE_FLAGS)
	mv output.pdf CS444_writing1_perrenc.pdf

clean:
	perl latexmk.pl $(LATEXMK_CLEAN_FLAGS)
