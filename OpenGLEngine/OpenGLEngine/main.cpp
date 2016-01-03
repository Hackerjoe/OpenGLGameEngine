#include "Program.h"

Program MyProgram;

int main(int argc, char** argv)
{
	MyProgram.Init(argc, argv);
	MyProgram.mainLoop();

	return 0;
}