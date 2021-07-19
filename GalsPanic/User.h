#pragma once
#ifndef USER_H_
#define USER_H_

#include <Windows.h>
#include <vector>
#include "Player.h"
#include "pointLF.h"

class Hole;
class Player;

class User : public Player
{
private:
	std::vector<POINT> polygon_hole_;
	// 위치
	POINT prev_pose_;
	// 상태
	int state_ = 0;
	enum state_{ IDLE, GOBACK };
public:
	User() {}
	~User() {}
	User(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size);
	User(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size);

	POINT GetPose() const { return pose_; }
	void SetPose(POINT _pose) { pose_ = _pose; }
	void SetPose(int x, int y) { pose_ = POINT{ x, y }; }
	void SetData(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size);
	void SetData(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size);

	int Draw(HDC) override;
	int Update() override;
	int Input();
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	int TailIncludeHole(std::vector<POINT>& collide_polygon);
	int InsertHoleToTail(std::vector<POINT>& collide_polygon);

	// update 0713
	void UserMove();
	void UserMovefix(std::vector<POINT>& collide_polygon);
	void UserMeetHole(std::vector<POINT>& collide_polygon);
};

#endif
