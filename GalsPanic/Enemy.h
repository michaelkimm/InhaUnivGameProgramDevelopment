#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_

#include <Windows.h>
#include "Player.h"

class Player;

class Enemy : public Player
{
private:
public:
	Enemy() {}
	Enemy(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
		: Player(_hWnd, _pose, _life, _collision_size, _size) {}
	Enemy(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size)
		: Player(_hWnd, POINT{ _cx, _cy }, _life, _collision_size, _size) {}
	~Enemy() {}

	int Draw(HDC) override;
	int Update() override;
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif



