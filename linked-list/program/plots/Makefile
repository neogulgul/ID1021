DATA_DIR := data
PDFS_DIR := pdfs

.PHONY: plot clean

plot: fixed_top fixed_bottom fixed_bottom_div_n

%:
	@mkdir -p $(PDFS_DIR)
	gnuplot -e "name='$@'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" plot.gp

clean:
	@rm -rf $(PDFS_DIR)
