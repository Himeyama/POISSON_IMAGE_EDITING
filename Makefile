# Makefile for poisson_image_editing.cpp

PROGRAM = poisson

$(PROGRAM): poisson_image_editing.cpp
	g++ poisson_image_editing.cpp -luuid -std=c++11 -O2 `pkg-config --cflags --libs opencv4` -o poisson

install: $(PROGRAM)
	install -s $(PROGRAM) /usr/local/bin/$(PROGRAM)
