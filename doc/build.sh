#!/bin/sh
pandoc english.md --pdf-engine=xelatex -V geometry:margin=1in -s -o english.pdf
pandoc francais.md --pdf-engine=xelatex -V geometry:margin=1in -s -o francais.pdf
