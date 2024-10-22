if (!exists("data")) {
	print "No data provided."
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
if (!exists("x_axis")) {
	print "No x-axis provided."
	exit
}
if (!exists("y_axis")) {
	print "No y-axis provided."
	exit
}

set grid
set term pdf
set key noenhanced

set xlabel x_axis
set ylabel y_axis

inputPath  = dataDir . "/" . data . ".dat"
outputPath = pdfsDir . "/" . data . ".pdf"

set output outputPath
plot inputPath using 1:2 with linespoints title data
