#pragma once
#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <windows.h>

class GameState
{
protected:
	//int state_;
	GameState* next_;
	HWND hWnd_;
	RECT client_;
	int b_quit;
public:
	// enum stateList{ GAMELOGIN, GAMEPLAY, GAMEOVER };

	GameState() {}
	GameState(HWND _hWnd);
	virtual ~GameState() {}

	virtual int Draw(HDC) = 0;
	virtual int Update() = 0;
	virtual int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual int Next() = 0;
	inline GameState* NextState() { return next_; }
	//void ChangeState(int);
};

#endif



