#include <boardUtility.h>

#include <drivers/gpiotbl.h>

int ledon(int argc, char* argv[]) {
	pinnum_set_level(RED_LED,1);
	return 0;
}

int ledoff(int argc, char* argv[]) {
	pinnum_set_level(RED_LED,0);
	return 0;
}

void Genos::utility::addBoardUtility(SimpleShell& shell) {
	shell.addCommand("ledon", ledon);
	shell.addCommand("ledoff", ledoff);
}
