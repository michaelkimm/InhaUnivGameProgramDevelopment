#include "User.h"
#include "figureDraw.h"
#include "Hole.h"
#include "pointLF.h"
#include "MyFunctions.h"

#include <iostream>
#include <math.h>

//User::User(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
//	: Player(_hWnd, _pose, _life, _collision_size, _size), cur_dir_(0), prev_dir_(1) {}
//User::User(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size)
//	: Player(_hWnd, POINT{ _cx, _cy }, _life, _collision_size, _size), cur_dir_(0), prev_dir_(1) {}

User* User::instance_ = 0;

User* User::Instance()
{
	if (instance_ == 0)
		instance_ = new User();
	return instance_;
}

void User::SetData(HWND _hWnd, POINT _pose, int _life, SIZE _collision_size, SIZE _size)
{
	hWnd_ = _hWnd;
	pose_ = _pose;
	life_ = _life;
	collision_size_ = _collision_size;
	size_ = _size;
	cur_dir_ = 0;
	prev_dir_ = 1;
}

void User::SetData(HWND _hWnd, int _cx, int _cy, int _life, SIZE _collision_size, SIZE _size)
{
	SetData(_hWnd, POINT{ _cx, _cy }, _life, _collision_size, _size);
}

int User::Draw(HDC _hdc)
{
	DrawEllipse(_hdc, pose_, size_.cx);
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
	// : >> 업데이트 시간
	DWORD cur_time = GetTickCount();
	static DWORD prev_time = cur_time;
	if (prev_time - cur_time < 100)
		return -1;
	prev_time = cur_time;
	// <<

	// : >> 움직임 구현
	prev_dir_ = cur_dir_;	// 이전 방향 및 위치 업데이트
	prev_pose_ = pose_;

	int user_move = 5;
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		this->SetPose(this->GetPose().x - user_move, this->GetPose().y);
		cur_dir_ = LEFT;
		if (GetKeyState(VK_UP) & 0x8000)
		{
			this->SetPose(this->GetPose().x, this->GetPose().y - user_move);
			cur_dir_ = LEFTUP;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			this->SetPose(this->GetPose().x, this->GetPose().y + user_move);
			cur_dir_ = LEFTDOWN;
		}
	}
	else if (GetKeyState(VK_UP) & 0x8000)
	{
		this->SetPose(this->GetPose().x, this->GetPose().y - user_move);
		cur_dir_ = UP;
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			this->SetPose(this->GetPose().x - user_move, this->GetPose().y);
			cur_dir_ = LEFTUP;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			this->SetPose(this->GetPose().x + user_move, this->GetPose().y);
			cur_dir_ = RIGHTUP;
		}
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		this->SetPose(this->GetPose().x + user_move, this->GetPose().y);
		cur_dir_ = RIGHT;
		if (GetKeyState(VK_UP) & 0x8000)
		{
			this->SetPose(this->GetPose().x, this->GetPose().y - user_move);
			cur_dir_ = RIGHTUP;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			this->SetPose(this->GetPose().x, this->GetPose().y + user_move);
			cur_dir_ = RIGHTDOWN;
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		this->SetPose(this->GetPose().x, this->GetPose().y + user_move);
		cur_dir_ = DOWN;
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			this->SetPose(this->GetPose().x - user_move, this->GetPose().y);
			cur_dir_ = LEFTDOWN;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			this->SetPose(this->GetPose().x + user_move, this->GetPose().y);
			cur_dir_ = RIGHTDOWN;
		}
	}
	// <<

	int inside_hole = (User::Instance()->CollisionHole(Hole::Instance()));			// 움직인 이후 구멍안, 엣지 위 판별
	int on_hole_edge = (User::Instance()->CollisionHoleLine(Hole::Instance()));

	// : >> 이미 만든 꼬리 위라면
	if (ColiisionTail())
	{
		// 내 위치 나가기 전 첫 위치로 & 벡터 비워
		pose_ = polygon_hole_[0];
		polygon_hole_.clear();
		return -1;
	}

	// : >> 구멍 안으로는 못움직임
	if (inside_hole && !on_hole_edge)
	{
		pose_ = prev_pose_;
		return -1;
	}
	// <<

	

	// : >> 엣지 위면 편하게 움직이기
	if (on_hole_edge && !(polygon_hole_.size() != 0))
		return 0;
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
			return -1;
		}
		else
			state_ = GOBACK;
	}
	// <<

	// : >> 꼬리가 Hole과 양쪽에서 만났을 때
	if (on_hole_edge && (polygon_hole_.size() != 0))
	{
		this->polygon_hole_.emplace_back(pose_);	// 닿은 순간도 넣어
		InsertHoleToTailNew(Hole::Instance());
	}
	// <<

	

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

int User::ColiisionTail()
{
	int collision = false;

	for (POINT vec : polygon_hole_)
	{
		if (vec.x == pose_.x && vec.y == pose_.y)
			return !collision;
	}

	return collision;
}

int User::TailIncludeHoleNew(Hole *hole)
{
	int on_hole_edge = 0;
	for (int hole_idx = 0; hole_idx < hole->polygon_hole_.size(); hole_idx++)
	{
		on_hole_edge = CollisionPtLine(pose_, hole->Instance());
		if (!IsInside(hole->polygon_hole_[hole_idx], this->polygon_hole_))
			return 0;
	}
	return 1;
}

int User::CollisionHole(Hole *target_hole)
{
	int collision = false;
	int next = 0;
	for (int current = 0; current < target_hole->polygon_hole_.size(); current++)
	{
		next = current + 1;
		if (next == target_hole->polygon_hole_.size()) next = 0;

		POINT vc = target_hole->polygon_hole_[current];
		POINT vn = target_hole->polygon_hole_[next];

		if (((vc.y >= pose_.y && vn.y < pose_.y) || (vc.y < pose_.y && vn.y >= pose_.y)) &&
			(pose_.x < (vn.x - vc.x)*(pose_.y - vc.y) / (vn.y - vc.y) + vc.x))
		{
			collision = !collision;
		}
	}
	return collision;
}

int User::CollisionHoleLine(Hole *target_hole)
{
	int collision = false;
	int next = 0;
	for (int current = 0; current < target_hole->polygon_hole_.size(); current++)
	{
		next = current + 1;
		if (next == target_hole->polygon_hole_.size()) next = 0;

		if (LinePoint(target_hole->polygon_hole_[current].x, target_hole->polygon_hole_[current].y,
			target_hole->polygon_hole_[next].x, target_hole->polygon_hole_[next].y, pose_.x, pose_.y))
		{
			collision = !collision;
			return collision;
		}
	}
	return collision;
}

int User::LinePoint(float x1, float y1, float x2, float y2, float px, float py) {

	// get distance from the point to the two ends of the line
	float d1 = sqrtf((px - x1)*(px - x1) + (py - y1)*(py - y1));
	float d2 = sqrtf((px - x2)*(px - x2) + (py - y2)*(py - y2));

	// get the length of the line
	float lineLen = sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	float buffer = 0.1f;    // higher # = less accurate

	// if the two distances are equal to the line's 
	// length, the point is on the line!
	// note we use the buffer here to give a range, 
	// rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		return true;
	}
	return false;
}

int User::LinePointPt(POINT p1, POINT p2, POINT pt) {

	return LinePoint(p1.x, p1.y, p2.x, p2.y, pt.x, pt.y);
}

int User::LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	// calculate the distance to intersection point
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		return true;
	}
	return false;
}

int User::LineLinePt(POINT p1, POINT p2, POINT p3, POINT p4)
{
	return LineLine(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
}

int User::PtInPoly(POINT &pt, std::vector<POINT> &target_vec)
{
	int collision = false;
	int next = 0;
	for (int current = 0; current < target_vec.size(); current++)
	{
		next = current + 1;
		if (next == target_vec.size()) next = 0;

		POINT vc = target_vec[current];
		POINT vn = target_vec[next];

		if (((vc.y >= pt.y && vn.y < pt.y) || (vc.y < pt.y && vn.y >= pt.y)) &&
			(pt.x < (vn.x - vc.x)*(pt.y - vc.y) / (vn.y - vc.y) + vc.x))
		{
			collision = !collision;
		}
	}
	return collision;
}

int User::InsertHoleToTailNew(Hole *hole)
{
	// : >> 점과 선이 만나는 알고리즘 이용하면 해당 점이 2' 3' 선분에 있는 것을 알 수 있다.
	// 첫번째 점
	int first_current = 0;
	int first_next = 0;
	for (first_current = 0; first_current < hole->polygon_hole_.size(); first_current++)
	{
		first_next = (first_current + 1) % hole->polygon_hole_.size();
		// current-next 선분과 꼬리 첫번째 점이 만나면 current 
		if (LinePointPt(hole->polygon_hole_[first_current], hole->polygon_hole_[first_next], this->polygon_hole_[0]))
		{
			break;
		}
	}
	// 마지막 점
	int last_current = 0;
	int last_next = 0;
	for (last_current = 0; last_current < hole->polygon_hole_.size(); last_current++)
	{
		last_next = (last_current + 1) % hole->polygon_hole_.size();
		// current-next 선분과 꼬리 첫번째 점이 만나면 current 
		if (LinePointPt(hole->polygon_hole_[last_current], hole->polygon_hole_[last_next], this->polygon_hole_.back()))
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
	if (TailIncludeHoleNew(Hole::Instance()))
	{
		std::cout << "포함\n";
		// last 점이 first보다 뒤인 경우
		for (last_next; last_next != first_next; last_next++)
		{
			if (last_next == hole->polygon_hole_.size()) last_next = 0;
			tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]);
		}
	}
	else
	{
		// std::cout << "비포함\n";
		//// last 점이 first보다 앞인 경우
		while (1)
		{
			if (last_next == first_current) break;	// 0112 과 같이 한점이 중간에 있을 경우
			tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]);
			last_next++;
			if (last_next == hole->polygon_hole_.size()) last_next = 0;
		}
		tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]); // first_current 넣고 끝내기
	}
	std::vector<POINT> combined1 = tail_polygon_hole_tmp1;
	// <<

	// std::cout << "양 방향\n";
	if (TailIncludeHoleNew(Hole::Instance()))
	{
		// std::cout << "포함\n";
		// last 점이 first보다 뒤인 경우
		for (last_current; last_current != first_current; last_current--)
		{
			if (last_current == -1) last_current = (hole->polygon_hole_.size() - 1);
			tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]);
		}
	}
	else
	{
		// std::cout << "비포함\n";
		//// last 점이 first보다 앞인 경우
		while (1)
		{
			if (last_current == first_next) break;	// 0112 과 같이 한점이 중간에 있을 경우
			tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]);
			last_current--;
			if (last_current == -1) last_current = (hole->polygon_hole_.size() - 1);
		}
		tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]); // first_current 넣고 끝내기
	}
	std::vector<POINT> combined2 = tail_polygon_hole_tmp2;
	// <<

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);
	std::cout << "area1: " << area1 << ", area2: " << area2 << std::endl;

	hole->polygon_hole_.clear();	// 기존 꼬리 및 hole 지우고
	this->polygon_hole_.clear();	//
	
	hole->polygon_hole_ = abs(area1) > abs(area2) ? combined1 : combined2;	// 너비가 더 큰 것으로 hole 대체
	
	return 0;
}