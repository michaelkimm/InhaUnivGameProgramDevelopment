#pragma once
#ifndef SCENESCORE_H_
#define SCENESCORE_H_

#include <windows.h>
#include "SceneState.h"


class SceneManager;
class SceneState;

class SceneScore : public SceneState
{
private:
public:
	SceneScore() : SceneState() {}
	SceneScore(HWND _hWnd) : SceneState(_hWnd) {}
	~SceneScore() {}

	int Draw(HDC) override;
	int Update(SceneManager* s) override;
	int Input(SceneManager* s, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif