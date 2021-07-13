#pragma once
#ifndef SCENEMAIN_H_
#define SCENEMAIN_H_

#include <windows.h>
#include "SceneState.h"

class SceneManager;
class SceneState;
class User;
class Enemy;
class Hole;

class SceneMain : public SceneState
{
private:
	// 더블 버퍼
	HBITMAP hDoubleBufferImage_ = 0;
	// 뒤 배경
	HBITMAP hBackImage_;
	BITMAP bitBack_;

	HBITMAP hBackImage2_;
	BITMAP bitBack2_;
	// 브러시
	HBRUSH hBrush_;
	HBRUSH hOldBrush_;
private:
	// User* user_;
	Enemy* enemy_;
public:
	SceneMain();
	SceneMain(HWND _hWnd);
	~SceneMain();

	int Draw(HDC) override;
	int Update(SceneManager* s) override;
	int Input() override;
	int Input(SceneManager* s, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif