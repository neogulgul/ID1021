# This can probably be done better

if (!exists("A")) {
	print "No A provided."
	exit
}
if (!exists("B")) {
	print "No B provided."
	exit
}
if (!exists("C")) {
	print "No C provided."
	exit
}
if (!exists("dataDir")) {
	print "No data directory provided."
	exit
}
if (!exists("pdfsDir")) {
	print "No pdfs directory provided."
	exit
}

set xlabel "n, elements in array"
set ylabel "time (ns)"

set grid
set term pdf

set key noenhanced

inputPathA = dataDir . "/" . A . ".dat"
inputPathB = dataDir . "/" . B . ".dat"
inputPathC = dataDir . "/" . C . ".dat"

set output pdfsDir . "/" . A . "_vs_" . B . "_vs_" . C . ".pdf"
plot inputPathA using 1:2 with linespoints title A, \
     inputPathB using 1:2 with linespoints title B, \
     inputPathC using 1:2 with linespoints title C
