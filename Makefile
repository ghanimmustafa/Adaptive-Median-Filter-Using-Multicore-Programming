SRCDIR=./src
BUILDDIR=./build

# Sequential 

build_seq: dir_seq
	cp $(SRCDIR)/*.png $(BUILDDIR)_seq
	g++ $(SRCDIR)/adaptive_median_filter_seq.cpp -o $(BUILDDIR)_seq/adap_med_seq `pkg-config --cflags --libs opencv4`

dir_seq: # Create file if it does not exists
	@[ -d $(BUILDDIR)_seq ] || mkdir -p $(BUILDDIR)_seq

run_seq: build_seq
	$(BUILDDIR)_seq/adap_med_seq $(BUILDDIR)_seq/lena_noisy.png

# Basic Paralel

build_basic: dir_basic
	cp $(SRCDIR)/*.png $(BUILDDIR)_basic
	g++ $(SRCDIR)/basic_paralel.cpp -o $(BUILDDIR)_seq/bas_par `pkg-config --cflags --libs opencv4` -lpthread -Wall

build_basic_d: dir_basic
	cp $(SRCDIR)/*.png $(BUILDDIR)_basic
	g++ $(SRCDIR)/basic_paralel.cpp -o $(BUILDDIR)_seq/bas_par `pkg-config --cflags --libs opencv4` -lpthread -Wall -DDEBUG

dir_basic: # Create file if it does not exists
	@[ -d $(BUILDDIR)_basic ] || mkdir -p $(BUILDDIR)_basic

run_basic: build_basic
	$(BUILDDIR)_seq/bas_par $(BUILDDIR)_basic/lena_noisy.png

clean:
	rm -r $(BUILDDIR)*/