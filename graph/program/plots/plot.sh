#!/bin/bash

data="$1"

if [ "$data" == "" ]; then
	echo "No data specified."
else
	data_dir="data"
	pdfs_dir="pdfs"

	x_axis="$2"
	if [ "$x_axis" == "" ]; then
		x_axis="n, elements in array"
	fi
	y_axis="$3"
	if [ "$y_axis" == "" ]; then
		y_axis="time (ns)"
	fi

	data_path="$data_dir/$data.dat"
	echo "Path to data : \"$data_path\""
	echo "x-axis       : \"$x_axis\""
	echo "y-axis       : \"$y_axis\""

	gnuplot \
	-e "data='$data'" \
	-e "dataDir='$data_dir'" \
	-e "pdfsDir='$pdfs_dir'" \
	-e "x_axis='$x_axis'" \
	-e "y_axis='$y_axis'" \
	plot.gp
fi
