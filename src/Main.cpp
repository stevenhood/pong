#include "header.h"
#include "templates.h"

#include "Pong.h"

#define BASE_SCREEN_WIDTH 640
#define BASE_SCREEN_HEIGHT 480

int main(int argc, char *argv[])
{
	int iResult;
	Pong oMain;
	char buf[64];
	sprintf(buf, "PONG (%d x %d)", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);

	iResult = oMain.Initialise(buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "fonts/jackinput/JAi_____.TTF", 44);
	iResult = oMain.MainLoop();
	oMain.Deinitialise();

	return iResult;
}
