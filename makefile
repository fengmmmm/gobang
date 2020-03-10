gobang.exe :  gobang.o gobang_algorithm.o Box.o gui.o threadsafe_queue.o game_control.o net.o
	g++  gobang.o gobang_algorithm.o Box.o gui.o threadsafe_queue.o game_control.o net.o `fltk-config --use-images --ldflags` -lws2_32 -o gobang.exe

gobang.o : gobang.cpp
	g++ gobang.cpp `fltk-config --cxxflags`  -c -o gobang.o
	
Box.o : Box.cpp
	g++ Box.cpp `fltk-config --cxxflags`  -c -o Box.o
	
gui.o : gui.cpp 
	g++ gui.cpp `fltk-config --cxxflags`  -c -o gui.o
	
gobang_algorithm.o : gobang_algorithm.cpp
	g++ gobang_algorithm.cpp `fltk-config --cxxflags` -c -o gobang_algorithm.o
	
threadsafe_queue.o : threadsafe_queue.cpp
	g++ threadsafe_queue.cpp -c -o threadsafe_queue.o
	
game_control.o : game_control.cpp
	g++ game_control.cpp `fltk-config --cxxflags` -c -w -o game_control.o
	
net.o :	 net.cpp
	g++ net.cpp `fltk-config --cxxflags` -lws2_32  -c -o net.o

clean:
	rm -rf *.o