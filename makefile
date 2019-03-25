all: raytracer test

raytracer: raytracer.cpp
	g++ -o raytracer.exe raytracer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
	
test: test.cpp
	g++ -o test.exe test.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
	
	