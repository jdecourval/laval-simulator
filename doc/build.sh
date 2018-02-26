#!/bin/sh
pandoc manual-en.md -f markdown_github+tex_math_dollars --mathjax --pdf-engine=xelatex -V geometry:margin=1in -s -o manual-en.pdf
pandoc manual-fr.md -f markdown_github+tex_math_dollars --mathjax --pdf-engine=xelatex -V geometry:margin=1in -s -o manual-fr.pdf
pandoc challenges-en.md -f markdown_github+tex_math_dollars --mathjax --pdf-engine=xelatex -V geometry:margin=1in -s -o challenges-en.pdf
pandoc challenges-fr.md -f markdown_github+tex_math_dollars --mathjax --pdf-engine=xelatex -V geometry:margin=1in -s -o challenges-fr.pdf
