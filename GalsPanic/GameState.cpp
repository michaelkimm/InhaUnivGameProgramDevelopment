#include <windows.h>
#include "GameState.h"
#include "GameLogin.h"
#include "GamePlay.h"
// #include "GameOver.h"


GameState::GameState(HWND _hWnd) : hWnd_(_hWnd)
{
	GetClientRect(_hWnd, &client_);
}

//void GameState::ChangeState(int new_state)
//{
//	delete state_;
//	switch (new_state)
//	{
//	case GAMELOGIN:
//		state_ = new GameLogin(hWnd_);
//		break;
//	case GAMEPLAY:
//		state_ = new GamePlay(hWnd_);
//		break;
//	case GAMEOVER:
//		break;
//	default:
//		break;
//	}
//}
