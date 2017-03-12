all: main.cpp
	g++ main.cpp -o ceye_test -g -O0 -L/home/semargl/root/lib/ -I/home/semargl/root/include \
	-Wl,-rpath /home/semargl/root/lib \
	-lopencv_core -lopencv_videoio -lopencv_highgui -lcybereye -lpthread 
