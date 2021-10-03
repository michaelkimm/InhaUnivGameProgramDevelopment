#include "User.h"
#include "figureDraw.h"
// #include "Hole.h"
#include "pointLF.h"
#include "MyFunctions.h"
#include "InputManager.h"

#include <iostream>
#include <math.h>

User::User(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
	: Player(_hWnd, _pose, _life, _collision_size, _size) {}
User::User(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size)
	: Player(_hWnd, POINT{ _cx, _cy }, _life, _collision_size, _size) {}

void User::SetData(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
{
	hWnd_ = _hWnd;
	pose_ = _pose;
	life_ = _life;
	collision_size_ = _collision_size;
	size_ = _size;
}

void User::SetData(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size)
{
	SetData(_hWnd, POINT{ _cx, _cy }, _life, _collision_size, _size);
}

int User::Draw(HDC _hdc)
{
	DrawEllipse(_hdc, pose_, size_.cx / 2);
	// Polygon(_hdc, pt_POINT, polygon_hole_.size());
	if (polygon_hole_.size() > 1)
		Polyline(_hdc, &polygon_hole_[0], polygon_hole_.size());
	return 0;
}

int User::Update()
{
	if (state_ == GOBACK)	// 다시 돌아가는 상태
	{
		if (polygon_hole_.size() == 0)
			return 0;
		pose_ = polygon_hole_[polygon_hole_.size() - 1];
		polygon_hole_.pop_back();
	}
	return 0;
}

int User::Input()
{
	return 0;
}

int User::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// static int space_cnt = 0;
	// : >> 구멍 그리기 구현
	switch (message)
	{
		
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			state_ = IDLE;
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_SPACE:
			// 뒤로 가기 구현
			std::cout << "WM_KEYUP VK_SPACE 발동!\n";
			state_ = GOBACK;
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

// 구멍이 꼬리 안에 있거나 겹쳐 있는 경우
int User::TailIncludeHole(std::vector<POINT>& collide_polygon)
{
	int on_hole_edge = 0;
	for (int hole_idx = 0; hole_idx < collide_polygon.size(); hole_idx++)
	{
		on_hole_edge = MyIsPointOnPoly(collide_polygon[hole_idx], this->polygon_hole_);
		if (!IsInside(collide_polygon[hole_idx], this->polygon_hole_))	// 꼬리 안에 없으면 return 0
		{
			if (on_hole_edge)	// 겹친 경우 고려
				continue;
			return 0;
		}
	}
	return 1;
}

int User::InsertHoleToTail(std::vector<POINT>& collide_polygon)
{
	// : >> 점과 선이 만나는 알고리즘 이용하면 해당 점이 2' 3' 선분에 있는 것을 알 수 있다.
	// 첫번째 점
	int first_current = 0;
	int first_next = 0;
	for (first_current = 0; first_current < collide_polygon.size(); first_current++)
	{
		first_next = (first_current + 1) % collide_polygon.size();
		// current-next 선분과 꼬리 첫번째 점이 만나면 current 
		if (LinePointPt(collide_polygon[first_current], collide_polygon[first_next], this->polygon_hole_[0]))
		{
			break;
		}
	}
	// 마지막 점
	int last_current = 0;
	int last_next = 0;
	for (last_current = 0; last_current < collide_polygon.size(); last_current++)
	{
		last_next = (last_current + 1) % collide_polygon.size();
		// current-next 선분과 꼬리 첫번째 점이 만나면 current 
		if (LinePointPt(collide_polygon[last_current], collide_polygon[last_next], this->polygon_hole_.back()))
		{
			break;
		}
	}
	// <<


	std::vector<POINT> tail_polygon_hole_tmp1 = this->polygon_hole_;
	std::vector<POINT> tail_polygon_hole_tmp2 = this->polygon_hole_;

	// : >> 각 벡터 객체의 CC, CCW에 따라 다르게 대입
	std::cout << "둘다 CW이거나 둘다 CCW이면\n";
	// : << 먼저 넣어보고 hole이 hole을 넣은 tale에 속하는지 보자. 안속하면 반대로 실행
	if (TailIncludeHole(collide_polygon))
	{
		std::cout << "포함\n";
		// last 점이 first보다 뒤인 경우
		for (last_next; last_next != first_next; last_next++)
		{
			if (last_next == collide_polygon.size()) last_next = 0;
			tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]);
		}
	}
	else
	{
		// std::cout << "비포함\n";
		//// last 점이 first보다 앞인 경우
		while (1)
		{
			if (last_next == first_current) break;	// 0112 과 같이 한점이 중간에 있을 경우
			tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]);
			last_next++;
			if (last_next == collide_polygon.size()) last_next = 0;
		}
		tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]); // first_current 넣고 끝내기
	}
	std::vector<POINT> combined1 = tail_polygon_hole_tmp1;
	// <<

	// std::cout << "양 방향\n";
	if (TailIncludeHole(collide_polygon))
	{
		// std::cout << "포함\n";
		// last 점이 first보다 뒤인 경우
		for (last_current; last_current != first_current; last_current--)
		{
			if (last_current == -1) last_current = (collide_polygon.size() - 1);
			tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]);
		}
	}
	else
	{
		// std::cout << "비포함\n";
		//// last 점이 first보다 앞인 경우
		while (1)
		{
			if (last_current == first_next) break;	// 0112 과 같이 한점이 중간에 있을 경우
			tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]);
			last_current--;
			if (last_current == -1) last_current = (collide_polygon.size() - 1);
		}
		tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]); // first_current 넣고 끝내기
	}
	std::vector<POINT> combined2 = tail_polygon_hole_tmp2;
	// <<

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);
	std::cout << "area1: " << area1 << ", area2: " << area2 << std::endl;

	collide_polygon.clear();	// 기존 꼬리 및 hole 지우고
	this->polygon_hole_.clear();	//
	
	collide_polygon = abs(area1) > abs(area2) ? combined1 : combined2;	// 너비가 더 큰 것으로 hole 대체
	
	return 0;
}

void User::UserMove()
{
	// : >> 움직임 구현
	prev_pose_ = pose_;

	// 어느정도 움직일지 InputManger로 데이터 얻기
	int user_move = 5;

	int horizontal_move = 0;
	int vertical_move = 0;
	if (InputManager::Instance()->GetLeftKeyState()) horizontal_move -= 1;
	if (InputManager::Instance()->GetRightKeyState()) horizontal_move += 1;
	if (InputManager::Instance()->GetUpKeyState()) vertical_move -= 1;
	if (InputManager::Instance()->GetDownKeyState()) vertical_move += 1;

	// 위치 변경
	SetPose(pose_.x + horizontal_move * user_move, pose_.y + vertical_move * user_move);

	return;
}

void User::UserMovefix(std::vector<POINT>& collide_polygon)
{
	int inside_hole = MyIsPointInPoly(pose_, collide_polygon);
	int on_hole_edge = MyIsPointOnPoly(pose_, collide_polygon);

	// : >> 이미 만든 꼬리 위라면
	if (MyIsPointOnPoly(pose_, polygon_hole_, false))
	{
		// 내 위치 나가기 전 첫 위치로 & 벡터 비워
		pose_ = polygon_hole_[0];
		polygon_hole_.clear();
		return;
	}

	// : >> 구멍 안으로는 못움직임
	if (inside_hole && !on_hole_edge)
	{
		pose_ = prev_pose_;
		return;
	}
	// <<



	// : >> 엣지 위면 편하게 움직이기
	if (on_hole_edge && !(polygon_hole_.size() != 0))
		return;
	// <<

	// : >> 스페이스바 입력 구현
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (prev_pose_.x != pose_.x || prev_pose_.y != pose_.y)	// 이전 위치와 현재 위치가 다를 경우
		{
			this->polygon_hole_.emplace_back(prev_pose_);
		}
		state_ = IDLE;
	}
	else
	{
		if (polygon_hole_.empty())	// 스페이스바 입력 안했는데 나가려고 하면 막기
		{
			pose_ = prev_pose_;
			return;
		}
		else
			state_ = GOBACK;
	}
	// <<
}

void User::UserMeetHole(std::vector<POINT>& collide_polygon)
{
	int inside_hole = MyIsPointInPoly(pose_, collide_polygon);		// 움직인 이후 구멍안, 엣지 위 판별
	int on_hole_edge = MyIsPointOnPoly(pose_, collide_polygon);

	// : >> 꼬리가 Hole과 양쪽에서 만났을 때
	if (on_hole_edge && (polygon_hole_.size() != 0))
	{
		this->polygon_hole_.emplace_back(pose_);	// 닿은 순간도 넣어
		InsertHoleToTail(collide_polygon);
	}
	// <<

	return;
}
