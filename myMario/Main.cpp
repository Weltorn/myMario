#include "MarioGame.h"
#include "resource.h"

// WinMainº¯Êý
//
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, 
				   PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"Mario";
	MarioGame* game = new MarioGame(h_instance,WIN_CLASS,WinTitle, 
		                            IDI_TANKNEW, IDI_TANKNEW, WIN_WIDTH, WIN_HEIGHT);
	game->SetFrame(10);
	T_Engine::pEngine = game;
	game->StartEngine();
	return TRUE;
}
