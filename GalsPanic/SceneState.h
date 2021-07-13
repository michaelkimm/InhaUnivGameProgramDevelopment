#pragma once
#ifndef SCENESTATE_H_
#define SCENESTATE_H_

#include <windows.h>
#include "SceneManager.h"

class SceneManager;

class SceneState
{
protected:
	void ChangeState(SceneManager*, int);
protected:
	HWND hWnd_;
	RECT client_;
	int b_quit;

public:
	SceneState() {}
	SceneState(HWND _hWnd);
	virtual ~SceneState() {}

	/*virtual void Setup(SceneManager*, HWND _hWnd) {}
	virtual void Update(SceneManager*) {}
	virtual void InPut(SceneManager*, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void Draw(HDC hdc) {}*/

	virtual int Draw(HDC) = 0;
	virtual int Update(SceneManager*) = 0;
	virtual int Input() = 0;
	virtual int Input(SceneManager*, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

#endif

