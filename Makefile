all: main.cpp
	g++ main.cpp -o ceye_test -g -O0  -std=c++11 -L/home/semargl/root/lib/ -I/home/semargl/root/include \
	-Wl,-rpath /home/semargl/root/lib \
	-lcybereye  -lopencv_world -lboost_system -lboost_serialization -lpthread
