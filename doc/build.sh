#!/bin/sh
pandoc manual-en.md --pdf-engine=xelatex -V geometry:margin=1in -s -o manual-en.pdf
pandoc manual-fr.md --pdf-engine=xelatex -V geometry:margin=1in -s -o manual-fr.pdf
pandoc challenges-en.md --pdf-engine=xelatex -V geometry:margin=1in -s -o challenges-en.pdf
pandoc challenges-fr.md --pdf-engine=xelatex -V geometry:margin=1in -s -o challenges-fr.pdf
