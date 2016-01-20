all:
	latexmk -pdf rapport.tex

old:
	pdflatex rapport.tex
	bibtex rapport.aux
	pdflatex rapport.tex
	pdflatex rapport.tex

tex:
	pdflatex rapport.tex

.PHONY:	clean
clean:
	-@\rm rapport.aux rapport.bbl rapport.blg rapport.log rapport.fdb_latexmk

