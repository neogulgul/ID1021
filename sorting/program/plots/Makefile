DATA_DIR := data
PDFS_DIR := pdfs

.PHONY: plot clean

plot: selection_sort no_check_selection_sort insertion_sort merge_sort merge_sort_division_by_n merge_sort_division_by_log_n
	# This is very stupid
	gnuplot -e "A='selection_sort'" -e "B='no_check_selection_sort'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" comparison_2.gp
	gnuplot -e "A='selection_sort'" -e "B='insertion_sort'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" comparison_2.gp
	gnuplot -e "A='insertion_sort'" -e "B='insertion_sort_back_to_front'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" comparison_2.gp
	gnuplot -e "A='insertion_sort'" -e "B='insertion_sort_front_to_back'" -e "C='insertion_sort_back_to_front'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" comparison_3.gp
	gnuplot -e "A='selection_sort'" -e "B='insertion_sort'" -e "C='merge_sort'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" comparison_3.gp

%:
	@mkdir -p $(PDFS_DIR)
	gnuplot -e "name='$@'" -e "dataDir='$(DATA_DIR)'" -e "pdfsDir='$(PDFS_DIR)'" plot.gp

clean:
	@rm -rf $(PDFS_DIR)
