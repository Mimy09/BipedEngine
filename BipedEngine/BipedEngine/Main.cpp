//#define BPD_NO_D3D
#include "win32/bpd-include.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	bpd::Window wnd;
	wnd.initalize("Test", 0, 0, 800, 600);
	wnd.RunWindow();
	
	return 0;
}