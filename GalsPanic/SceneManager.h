#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <windows.h>
#include "SceneState.h"
//#include "SceneMain.h"
//#include "SceneScore.h"

class SceneState;

class SceneManager
{
private:
	friend class SceneState;
	SceneState* state_;
	HWND hWnd_manager_;
public:
	SceneManager();
	SceneManager(HWND);
	~SceneManager();

	enum SCENE { LOGIN, MAIN, SCORE };
	void SetSceneState(int _scene_type);
	void Update();
	void Input();
	void Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Draw(HDC hdc);
};

#endif

