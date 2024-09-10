DATA_DIR := data
PDFS_DIR := pdfs

.PHONY: plot clean

plot: plot_brute_unsorted plot_brute_sorted plot_brute_sorted_simple plot_binary plot_binary_recursive

plot_%:
	@mkdir -p $(PDFS_DIR)
	gnuplot -e "name='$@'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" plot.gp

clean:
	@rm -rf $(PDFS_DIR)
