#include <Game.hpp>

int main(int argc, const char** argv)
{
	if(argc >= 2){
		if(!Util::setResourceDirectory(argv[1])){
			printf("Invalid resource directory: %s\n", argv[1]);
		}
		else{
			printf("Resource directory has been set to: %s\n", argv[1]);
		}
	}
	else printf("Resource directory has not been set!\n");
	
	Game();

	return 0;
}