SRCDIR=./src
BUILDDIR=./build
IMAGE_IN_USE=salt_and_pepper_noisy_image.png

# Usage for parallel versions
# make run_basic n=($number of threads)
# make run_smt n=($number of threads)

#Sequential Local Median Filter
build_seq_l: dir_seq_l
	cp $(SRCDIR)/*.png $(BUILDDIR)_seq
	g++ $(SRCDIR)/local_median_filter_seq.cpp -o $(BUILDDIR)_seq/local_med_seq `pkg-config --cflags --libs opencv4`

dir_seq_l: # Create file if it does not exists
	@[ -d $(BUILDDIR)_seq ] || mkdir -p $(BUILDDIR)_seq

run_seq_l: build_seq_l
	$(BUILDDIR)_seq/local_med_seq $(BUILDDIR)_seq/$(IMAGE_IN_USE)

# Sequential Adaptive Median Filter

build_seq: dir_seq
	cp $(SRCDIR)/*.png $(BUILDDIR)_seq
	g++ $(SRCDIR)/adaptive_median_filter_seq.cpp -o $(BUILDDIR)_seq/adap_med_seq `pkg-config --cflags --libs opencv4`

build_seq_d: dir_seq
	cp $(SRCDIR)/*.png $(BUILDDIR)_seq
	g++ $(SRCDIR)/adaptive_median_filter_seq.cpp -o $(BUILDDIR)_seq/adap_med_seq `pkg-config --cflags --libs opencv4` -DDEBUG
dir_seq: # Create file if it does not exists
	@[ -d $(BUILDDIR)_seq ] || mkdir -p $(BUILDDIR)_seq

run_seq: build_seq
	$(BUILDDIR)_seq/adap_med_seq $(BUILDDIR)_seq/$(IMAGE_IN_USE)

# Basic Paralel Adaptive Median Filter

build_basic: dir_basic
	cp $(SRCDIR)/*.png $(BUILDDIR)_basic
	g++ $(SRCDIR)/basic_paralel.cpp -o $(BUILDDIR)_basic/bas_par `pkg-config --cflags --libs opencv4` -lpthread -Wall

build_basic_d: dir_basic
	cp $(SRCDIR)/*.png $(BUILDDIR)_basic
	g++ $(SRCDIR)/basic_paralel.cpp -o $(BUILDDIR)_basic/bas_par `pkg-config --cflags --libs opencv4` -lpthread -Wall -DDEBUG

dir_basic: # Create file if it does not exists
	@[ -d $(BUILDDIR)_basic ] || mkdir -p $(BUILDDIR)_basic

run_basic: $(BUILDDIR)_basic/bas_par
	$(BUILDDIR)_basic/bas_par $(BUILDDIR)_basic/$(IMAGE_IN_USE) $(n)

# SMT Paralel Adaptive Median Filter

build_smt: dir_smt
	cp $(SRCDIR)/*.png $(BUILDDIR)_smt
	g++ $(SRCDIR)/smt_paralel.cpp -o $(BUILDDIR)_smt/smt_par `pkg-config --cflags --libs opencv4` -lpthread -Wall

build_smt_d: dir_smt
	cp $(SRCDIR)/*.png $(BUILDDIR)_smt
	g++ $(SRCDIR)/smt_paralel.cpp -o $(BUILDDIR)_smt/smt_par `pkg-config --cflags --libs opencv4` -lpthread -Wall -DDEBUG

dir_smt: # Create file if it does not exists
	@[ -d $(BUILDDIR)_smt ] || mkdir -p $(BUILDDIR)_smt

run_smt: $(BUILDDIR)_smt/smt_par 
	$(BUILDDIR)_smt/smt_par $(BUILDDIR)_smt/$(IMAGE_IN_USE) $(n)

clean:
	rm -r $(BUILDDIR)*/
