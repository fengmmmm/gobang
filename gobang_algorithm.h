#ifndef GOBANG_ALGORITHM_H
#define GOBANG_ALGORITHM_H

#include <vector>
#include <cstdlib>
#include <mutex>
#include <FL/Fl_ask.H>
#define _fori(a,b) for(int i=a;i<b;++i)
#define _forj(a,b) for(int j=a;j<b;++j)

class gobang_algorithm{
public:
	gobang_algorithm(int n, int m);
	~gobang_algorithm();
	int get_status(int role, int x, int y);
	bool insert(int role, int x, int y);
private:
	//判断某个方向上的冲四数量
	int num_go_four(int direction, int x, int y);
	//判断某个方向是否有活四
	int num_live_four(int direction, int x, int y);
	//判断某个方向是否有活三
	int num_live_three(int direction, int x, int y);
	//0:继续 1:黑胜 2:长连禁手 3:四四禁手 4:三三禁手
	int check_black(int x, int y);
	//0:继续 1:白胜 
	int check_white(int x, int y);	
	
	
	std::vector<std::vector<char> > arr;
	int N, M;
	int(*pos)[10][2];
};

#endif