DATA_FILES := dequeue enqueue enqueue_optimized
DATA_DIR   := data
PDFS_DIR   := pdfs

.PHONY: plot clean

plot: $(DATA_FILES)

%:
	@mkdir -p $(PDFS_DIR)
	gnuplot -e "name='$@'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" plot.gp

clean:
	@rm -rf $(PDFS_DIR)
