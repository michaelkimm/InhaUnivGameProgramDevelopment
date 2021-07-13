#pragma once
#ifndef GAMELOGIN_H_
#define GAMELOGIN_H_

#include <windows.h>
#include "GameState.h"

class GameState;

class GameLogin : public GameState
{
private:
	enum strLen{ STRLENMAX = 30 };
	TCHAR str_[STRLENMAX];
	int strlen_ = 0;
public:
	GameLogin() {}
	GameLogin(HWND);
	~GameLogin();

	int Draw(HDC) override;
	int Update() override;
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	int Next() override;
};

#endif


