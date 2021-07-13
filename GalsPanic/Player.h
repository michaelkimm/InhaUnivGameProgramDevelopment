#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include <Windows.h>

class Player
{
protected:
	HWND hWnd_;
protected:
	POINT pose_;
	int life_;
	SIZE collision_size_;
	SIZE size_;
public:
	Player() {}
	Player(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
		: hWnd_(_hWnd), pose_(_pose), life_(_life), collision_size_(_collision_size), size_(_size) {}
	virtual ~Player() {}

	virtual int Draw(HDC) = 0;
	virtual int Update() = 0;
	virtual int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

#endif

