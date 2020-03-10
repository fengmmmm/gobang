#include <unordered_map>
#include <string>
#define	_for(a, b) for(unsigned i=a;i<b;++i)
#include "game_control.h"

int main(int argc, char* argv[]){
	std::unordered_map<std::string, bool> mp{
		{"-net",false},
		{"-white",false},
	};
	_for(0, argc) mp[argv[i]] = true;
	
	game_control game;
	if(mp["-net"]&&mp["-white"]){
		game.set_white();
	}
	if(mp["-net"]){
		game.set_net();
	}
	game.game_start();
	return 0;
}




