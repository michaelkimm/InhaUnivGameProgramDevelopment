#include "Enemy.h"

#include "State.h"
#include "EnemyState.h"
#include "EnemyEvent.h"
#include "figureDraw.h"
#include "MyFunctions.h"

#include <math.h>
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
	DrawEllipse(hdc, pose_, size_.cx / 2);
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
	// : >> 구멍 그리기 구현
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 0x41:
			// key A인 경우
			cout << "key A down\n";
			Notify(EnemyEvent::Instance(ec_detect_dt_passed));
			break;
		case 0x49:
			// key I인 경우
			cout << "key I down\n";
			Notify(EnemyEvent::Instance(ec_en_Idle_dt));
			break;
		case 0x44:
			// key D인 경우
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
	// : >> 위치 움직이기
	float milsec = 0.01;
	pose_.x += vx_ * milsec;
	pose_.y += vy_ * milsec;
	// <<

	// : >> 충돌 감지 및 상태 값 조정
	// 벽 부딛히면 통과
	// << 

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

bool Enemy::Collision(RECT& _rect)
{
	return (pose_.x + size_.cx / 2 >= _rect.right) || (pose_.x - size_.cx / 2 < 0) || (pose_.y + size_.cy > _rect.bottom) || (pose_.y - size_.cy < 0);
}

bool Enemy::Collision(std::vector<POINT>& _point_vec)
{
	int rad = size_.cx / 2;
	// 중점과 선분 사이 거리가 
	
	return CirclePoly(pose_.x, pose_.y, rad, _point_vec);
}

bool Enemy::Collision(User *_user)
{
	int enemy_rad = size_.cx / 2;
	return (pow(pose_.x - _user->GetPose().x, 2) + pow(pose_.y - _user->GetPose().y, 2)) <= pow(enemy_rad + (_user->GetSize().cx / 2), 2);
}
