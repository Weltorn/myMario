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
	T_Engine::pEngine = game;
	game->SetFrame(5);

	game->StartEngine();

	return TRUE;
}
