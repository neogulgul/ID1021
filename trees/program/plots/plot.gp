if (!exists("name")) {
	print "No name provided."
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

inputPath = dataDir . "/" . name . ".dat"

set output pdfsDir . "/" . name . ".pdf"
plot inputPath using 1:2 with linespoints title name
