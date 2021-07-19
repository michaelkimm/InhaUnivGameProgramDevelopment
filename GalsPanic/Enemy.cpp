#include "Enemy.h"

#include "State.h"
#include "EnemyState.h"
#include "EnemyEvent.h"
#include "figureDraw.h"

#include <iostream>
using namespace std;

Enemy::~Enemy()
{
	if (state_ != nullptr)
		delete state_;
}

void Enemy::Transition(State* s)
{
	EnemyState* prev_state	= static_cast<EnemyState*> (state_);
	EnemyState* cur_state	= static_cast<EnemyState*> (s);

	cout << "prev_state: " << prev_state->GetName() << endl;
	cout << "cur_state: " << cur_state->GetName() << endl;

	Machine::Transition(s);
}

int Enemy::Draw(HDC hdc) const
{
	DrawEllipse(hdc, pose_, size_.cx);
	return 0;
}

int Enemy::Update()
{
	
	return 0;
}

int Enemy::Input()
{
	return 0;
}

int Enemy::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// static int space_cnt = 0;
	// : >> ���� �׸��� ����
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 0x41:
			// key A�� ���
			cout << "key A down\n";
			Notify(EnemyEvent::Instance(ec_detect_dt_passed));
			break;
		case 0x49:
			// key I�� ���
			cout << "key I down\n";
			Notify(EnemyEvent::Instance(ec_en_Idle_dt));
			break;
		case 0x44:
			// key D�� ���
			cout << "key D down\n";
			Notify(EnemyEvent::Instance(ec_en_attacked));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	// <<
	return 0;
}

void Enemy::Move()
{
	cout << "Enemy Move!\n";
}

void Enemy::Destroy()
{
	cout << "Enemy Destroyed!\n";
}

void Enemy::AttackUser()
{
	cout << "Enemy attack user!\n";
}

void Enemy::EnemyShake()
{
	cout << "Enemy Shaked!\n";
}

void Enemy::BounceLine()
{
}
