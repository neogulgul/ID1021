.PHONY: plot clean

PDFS_DIR = pdfs

plot:
	@mkdir -p $(PDFS_DIR)
	@./plot.sh collisions Modulos Collisions
	@./plot.sh bucket_lookup "Zip Code"
	@./plot.sh bucket_average Buckets Average
	@./plot.sh probing_lookup "Zip Code"
	@./plot.sh probing_average Modulo Average

clean:
	@rm -rf $(PDFS_DIR)
