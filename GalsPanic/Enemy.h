#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_

#include <Windows.h>
#include <vector>
#include "Machine.h"

class User;
class Machine;

class Enemy : public Machine
{
private:
	HWND hWnd_;
	int life_;
	SIZE size_;
public:
	enum state_id { State_Alive, State_Dead, State_Idle, State_Attack};
public:
	explicit Enemy(State* _state, POINT _pose, float _vx, float _vy, SIZE _collision_size, HWND _hWnd, int _life, SIZE _size) 
		: Machine(_state, _pose, _vx, _vy, _collision_size), hWnd_(_hWnd), life_(_life), size_(_size) {}
	~Enemy();

	virtual void Transition(State*);

	int Draw(HDC) const override;
	int Update() override;
	int Input() override;
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void Move();
	void Destroy();

	void AttackUser();
	void EnemyShake();

	// Bounce 함수
	void BounceLine();

	// 충돌 함수
	bool Collision(RECT& _rect);
	bool Collision(std::vector<POINT>& _point_vec);
	bool Collision(User *);
};

#endif



