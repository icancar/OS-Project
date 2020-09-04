#include <iostream.h>
#include <dos.h>
#include "System.h"
#include "thread.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	System::init();
	int a=userMain(argc, argv);
	System::restore();
	return a;
}



