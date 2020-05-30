
SVG="/Users/LoicJouans/Documents/ENS/DS/project/report/svg"

all: report

report:
	xelatex -output-directory=meta report
	BIBINPUTS=".:" TEXMFOUTPUT="meta:" bibtex meta/report
	xelatex -output-directory=meta report
	ln -sf meta/report.pdf

pic:
	inkscape -D -z --file=$(SVG)/nnetStruct.svg --export-pdf=$(SVG)/nnetStruct.pdf --export-latex
	inkscape -D -z --file=$(SVG)/neurons.svg --export-pdf=$(SVG)/neurons.pdf --export-latex
