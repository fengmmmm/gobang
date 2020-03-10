#include "gobang_algorithm.h"


gobang_algorithm::gobang_algorithm(int n, int m)
	:N(n), M(m)
{
	arr.resize(N, std::vector<char>(M, '\0'));
	static int pos_init[4][10][2] = {
		{ { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, -1 }, { 0, -2 }, { 0, -3 }, { 0, -4 }, { 0, -5 } },
		{ { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 }, { -1, 0 }, { -2, 0 }, { -3, 0 }, { -4, 0 }, { -5, 0 } },
		{ { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { -1, -1 }, { -2, -2 }, { -3, -3 }, { -4, -4 }, { -5, -5 } },
		{ { 1, -1 }, { 2, -2 }, { 3, -3 }, { 4, -4 }, { 5, -5 }, { -1, 1 }, { -2, 2 }, { -3, 3 }, { -4, 4 }, { -5, 5 } }
	};
	pos = pos_init;
}

gobang_algorithm::~gobang_algorithm(){

}

int gobang_algorithm::get_status(int role, int x, int y){
	if (role == 1) return check_black(x, y);
	if (role == 2) return check_white(x, y);
	return 0;
}

bool gobang_algorithm::insert(int role, int x, int y){
	if (role != 1 && role != 2 || x < 0 || x >= N || y<0 || y >= M)
		return false;
	if ('\0' == arr[x][y]){
		arr[x][y] = '0' + role;
		return true;
	}
	return 	false;
}

int gobang_algorithm::num_go_four(int direction, int x, int y){
	//统计A、B方向上的直接相连数和跳跃相连数
	int countA = 0, jumpA = 0, countB = 0, jumpB = 0, count_space = 0;
	//跳跃标记
	bool jump_flag = false;
	_forj(0, 5){	//A方向
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M){
			if (arr[mi][mj] == '2') break;
			if (!jump_flag && arr[mi][mj] == '1') ++countA;
			else if (jump_flag && arr[mi][mj] == '1') ++jumpA;
			else if (!jump_flag && arr[mi][mj] == '\0') {
				jump_flag = true;	++count_space;
			}
		}
	}
	jump_flag = false;
	_forj(5, 10){	//B方向
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M){
			if (arr[mi][mj] == '2') break;
			if (!jump_flag && arr[mi][mj] == '1') ++countB;
			else if (jump_flag && arr[mi][mj] == '1') ++jumpB;
			else if (!jump_flag && arr[mi][mj] == '\0'){
				jump_flag = true; ++count_space;
			}
		}
	}
	//先判断一条直线上两个个冲四的特例
	if (countA + countB == 0 && jumpA == 3 && jumpB == 3
		|| countA == 1 && countB == 1 && jumpA == 1 && jumpB == 1
		|| countA + countB == 1 && jumpA == 2 && jumpB == 2)
		return 2;
	if (count_space == 1 && (countA + countB + jumpA == 3 || countA + countB + jumpB == 3)){
		//std::cout << "num_space" << count_space << std::endl;
		//fl_alert("num_space :%d, countA: %d, countB: %d",count_space, countA, countB);
		return 1;
	}
	return 0;
}

int gobang_algorithm::num_live_four(int direction, int x, int y){
	int count = 0;
	bool checkA = false, checkB = false, spaceA = false, spaceB = false;
	//判断是否为四个直接相连且两端都有空格，空格之后是否为黑棋
	_forj(0, 5){
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M){
			//如果是空格，进一步判断空格之后是否为黑棋
			if (spaceA){
				if (arr[mi][mj] != '1') checkA = true;
				break;
			}
			if (arr[mi][mj] == '1')	++count;
			else if (arr[mi][mj] == '\0'){
				spaceA = true;
			}

		}
		//超过边界
		else if (spaceA){
			checkA = true; break;
		}
	}
	_forj(5, 10){
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M){
			//如果是空格，进一步判断空格之后是否为黑棋
			if (spaceB){
				if (arr[mi][mj] != '1') checkB = true;
				break;
			}
			if (arr[mi][mj] == '1')	++count;
			else if (arr[mi][mj] == '\0'){
				spaceB = true;
			}

		}
		//超过边界
		else if (spaceB){
			checkB = true; break;
		}
	}
	if (count == 3 && checkA && checkB)
		return 1;
	return 0;
}

int gobang_algorithm::num_live_three(int direction, int x, int y){
	_forj(0, 4){
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '\0'){
			arr[mi][mj] = '1';		//新加黑棋
			if (num_live_four(direction, x, y)){//判断是否为活四
				arr[mi][mj] = '\0';	//还原为空格
				return 1;
			}
			arr[mi][mj] = '\0';	//还原为空格
			break;
		}
	}
	_forj(5, 9){
		int mi = x + pos[direction][j][0];
		int mj = y + pos[direction][j][1];
		if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '\0'){
			arr[mi][mj] = '1';		//新加黑棋
			if (num_live_four(direction, x, y)){//判断是否为活四
				arr[mi][mj] = '\0';	//还原为空格
				return 1;
			}
			arr[mi][mj] = '\0';	//还原为空格
			break;
		}
	}
	return 0;
}

//0:继续 1:黑胜 2:长连禁手 3:四四禁手 4:三三禁手
int gobang_algorithm::check_black(int x, int y){
	//优先判五连
	_fori(0, 4){
		int count = 0;
		_forj(0, 4){
			int mi = x + pos[i][j][0];
			int mj = y + pos[i][j][1];
			if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '1'){
				++count;
			}
			else break;
		}
		_forj(5, 9){
			int mi = x + pos[i][j][0];
			int mj = y + pos[i][j][1];
			if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '1'){
				++count;
			}
			else break;
		}
		//黑胜
		if (count == 4)	return 1;
		//长连禁手
		if (count > 4)	return 2;
	}
	int live_three = 0, line_four = 0, live_four = 0;
	_fori(0, 4){
		line_four += num_go_four(i, x, y);
		live_four += num_live_four(i, x, y);
		live_three += num_live_three(i, x, y);
	}
	if (line_four + live_four >= 2){
		//fl_alert("return 3, line_four: %d, live_four:%d ",line_four, live_four);
		return 3;
	}
	if (live_three >= 2){
		return 4;
	}
	return 0;
}

//0:继续 1:白胜 
int gobang_algorithm::check_white(int x, int y){
	_fori(0, 4){
		int count = 0;
		_forj(0, 4){
			int mi = x + pos[i][j][0];
			int mj = y + pos[i][j][1];
			if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '2'){
				++count;
			}
			else break;
		}
		_forj(5, 9){
			int mi = x + pos[i][j][0];
			int mj = y + pos[i][j][1];
			if (mi >= 0 && mi < N && mj >= 0 && mj < M && arr[mi][mj] == '2'){
				++count;
			}
			else break;
		}
		if (count >= 4) return 1;
	}
	return 0;
}