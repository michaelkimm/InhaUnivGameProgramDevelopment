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
	POINT pose_;
	POINT prev_pose_;
	// 방향
	int cur_dir_;
	int prev_dir_;
	enum dir{ LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN };
	// 상태
	int state_ = 0;
	enum state_{ IDLE, GOBACK };
public:
	static User* Instance();
protected:
	User() {}
	~User() {}
private:
	static User* instance_;
public:
	/*User(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size);
	User(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size);*/

	POINT GetPose() const { return pose_; }
	void SetPose(POINT _pose) { pose_ = _pose; }
	void SetPose(int x, int y) { pose_ = POINT{ x, y }; }
	void SetData(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size);
	void SetData(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size);

	int Draw(HDC) override;
	int Update() override;
	int Input();
	int Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	int CollisionHole(Hole*);
	int CollisionHoleLine(Hole*);
	int ColiisionTail();
	int TailIncludeHoleNew(Hole*);
	int LinePoint(float x1, float y1, float x2, float y2, float px, float py);
	int LinePointPt(POINT p1, POINT p2, POINT pt);
	int LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	int LineLinePt(POINT p1, POINT p2, POINT p3, POINT p4);
	int PtInPoly(POINT &pt, std::vector<POINT> &target_vec);
	int InsertHoleToTailNew(Hole*);
};

#endif
