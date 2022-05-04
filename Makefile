SRCDIR=./src
BUILDDIR=./build

build_seq: dir_seq
	cp $(SRCDIR)/*.png $(BUILDDIR)_seq
	g++ $(SRCDIR)/adaptive_median_filter_seq.cpp -o $(BUILDDIR)_seq/adap_med_seq `pkg-config --cflags --libs opencv4`

dir_seq: # Create file if it does not exists
	[ -d $(BUILDDIR)_seq ] || mkdir -p $(BUILDDIR)_seq

run_seq: build_seq
	$(BUILDDIR)_seq/adap_med_seq $(BUILDDIR)_seq/lena_noisy.png

clean:
	rm -r $(BUILDDIR)*/