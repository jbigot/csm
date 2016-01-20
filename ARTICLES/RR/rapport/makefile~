all:
	latexmk -pdf article.tex

old:
	pdflatex article.tex
	bibtex article.aux
	pdflatex article.tex
	pdflatex article.tex

tex:
	pdflatex article.tex

.PHONY:	clean
clean:
	-@\rm article.aux article.bbl article.blg article.log article.fdb_latexmk

