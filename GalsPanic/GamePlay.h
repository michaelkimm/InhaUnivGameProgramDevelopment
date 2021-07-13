#pragma once
#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include <Windows.h>
#include "GameState.h"

class GameState;

class GamePlay : public GameState
{
protected:
public:
	GamePlay();
	GamePlay(HWND);
	~GamePlay();

	int Draw(HDC) override;
	int Update() override;
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	int Next() override;

};

#endif

