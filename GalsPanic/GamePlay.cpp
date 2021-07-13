#include <windows.h>
#include "GamePlay.h"
#include "figureDraw.h"

GamePlay::GamePlay()
{
}

GamePlay::GamePlay(HWND _hWnd) : GameState(_hWnd)
{
}

GamePlay::~GamePlay()
{
}

int GamePlay::Draw(HDC hdc)
{
	DrawEllipse(hdc, POINT{ 100, 100 }, 100);
	return 0;
}

int GamePlay::Update()
{
	return 0;
}

int GamePlay::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int GamePlay::Next()
{
	return 0;
}
