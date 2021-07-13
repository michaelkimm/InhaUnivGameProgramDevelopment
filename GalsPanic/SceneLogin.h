#pragma once
#ifndef SCENELOGIN_H_
#define SCENELOGIN_H_

#include <windows.h>
#include "SceneState.h"

class SceneManager;
class SceneState;

class SceneLogin : public SceneState
{
private:
	enum strLen { STRLENMAX = 30 };
	TCHAR str_[STRLENMAX];
	int strlen_ = 0;
public:
	SceneLogin();
	SceneLogin(HWND _hWnd);
	~SceneLogin();

	int Draw(HDC) override;
	int Update(SceneManager* s) override;
	int Input() override;
	int Input(SceneManager* s, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};



#endif