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
	if (state_ == GOBACK)	// �ٽ� ���ư��� ����
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
	// : >> ���� �׸��� ����
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
			// �ڷ� ���� ����
			std::cout << "WM_KEYUP VK_SPACE �ߵ�!\n";
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

// ������ ���� �ȿ� �ְų� ���� �ִ� ���
int User::TailIncludeHole(std::vector<POINT>& collide_polygon)
{
	int on_hole_edge = 0;
	for (int hole_idx = 0; hole_idx < collide_polygon.size(); hole_idx++)
	{
		on_hole_edge = MyIsPointOnPoly(collide_polygon[hole_idx], this->polygon_hole_);
		if (!IsInside(collide_polygon[hole_idx], this->polygon_hole_))	// ���� �ȿ� ������ return 0
		{
			if (on_hole_edge)	// ��ģ ��� ���
				continue;
			return 0;
		}
	}
	return 1;
}

int User::InsertHoleToTail(std::vector<POINT>& collide_polygon)
{
	// : >> ���� ���� ������ �˰��� �̿��ϸ� �ش� ���� 2' 3' ���п� �ִ� ���� �� �� �ִ�.
	// ù��° ��
	int first_current = 0;
	int first_next = 0;
	for (first_current = 0; first_current < collide_polygon.size(); first_current++)
	{
		first_next = (first_current + 1) % collide_polygon.size();
		// current-next ���а� ���� ù��° ���� ������ current 
		if (LinePointPt(collide_polygon[first_current], collide_polygon[first_next], this->polygon_hole_[0]))
		{
			break;
		}
	}
	// ������ ��
	int last_current = 0;
	int last_next = 0;
	for (last_current = 0; last_current < collide_polygon.size(); last_current++)
	{
		last_next = (last_current + 1) % collide_polygon.size();
		// current-next ���а� ���� ù��° ���� ������ current 
		if (LinePointPt(collide_polygon[last_current], collide_polygon[last_next], this->polygon_hole_.back()))
		{
			break;
		}
	}
	// <<


	std::vector<POINT> tail_polygon_hole_tmp1 = this->polygon_hole_;
	std::vector<POINT> tail_polygon_hole_tmp2 = this->polygon_hole_;

	// : >> �� ���� ��ü�� CC, CCW�� ���� �ٸ��� ����
	std::cout << "�Ѵ� CW�̰ų� �Ѵ� CCW�̸�\n";
	// : << ���� �־�� hole�� hole�� ���� tale�� ���ϴ��� ����. �ȼ��ϸ� �ݴ�� ����
	if (TailIncludeHole(collide_polygon))
	{
		std::cout << "����\n";
		// last ���� first���� ���� ���
		for (last_next; last_next != first_next; last_next++)
		{
			if (last_next == collide_polygon.size()) last_next = 0;
			tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]);
		}
	}
	else
	{
		// std::cout << "������\n";
		//// last ���� first���� ���� ���
		while (1)
		{
			if (last_next == first_current) break;	// 0112 �� ���� ������ �߰��� ���� ���
			tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]);
			last_next++;
			if (last_next == collide_polygon.size()) last_next = 0;
		}
		tail_polygon_hole_tmp1.emplace_back(collide_polygon[last_next]); // first_current �ְ� ������
	}
	std::vector<POINT> combined1 = tail_polygon_hole_tmp1;
	// <<

	// std::cout << "�� ����\n";
	if (TailIncludeHole(collide_polygon))
	{
		// std::cout << "����\n";
		// last ���� first���� ���� ���
		for (last_current; last_current != first_current; last_current--)
		{
			if (last_current == -1) last_current = (collide_polygon.size() - 1);
			tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]);
		}
	}
	else
	{
		// std::cout << "������\n";
		//// last ���� first���� ���� ���
		while (1)
		{
			if (last_current == first_next) break;	// 0112 �� ���� ������ �߰��� ���� ���
			tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]);
			last_current--;
			if (last_current == -1) last_current = (collide_polygon.size() - 1);
		}
		tail_polygon_hole_tmp2.emplace_back(collide_polygon[last_current]); // first_current �ְ� ������
	}
	std::vector<POINT> combined2 = tail_polygon_hole_tmp2;
	// <<

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);
	std::cout << "area1: " << area1 << ", area2: " << area2 << std::endl;

	collide_polygon.clear();	// ���� ���� �� hole �����
	this->polygon_hole_.clear();	//
	
	collide_polygon = abs(area1) > abs(area2) ? combined1 : combined2;	// �ʺ� �� ū ������ hole ��ü
	
	return 0;
}

void User::UserMove()
{
	// : >> ������ ����
	prev_pose_ = pose_;

	// ������� �������� InputManger�� ������ ���
	int user_move = 5;

	int horizontal_move = 0;
	int vertical_move = 0;
	if (InputManager::Instance()->GetLeftKeyState()) horizontal_move -= 1;
	if (InputManager::Instance()->GetRightKeyState()) horizontal_move += 1;
	if (InputManager::Instance()->GetUpKeyState()) vertical_move -= 1;
	if (InputManager::Instance()->GetDownKeyState()) vertical_move += 1;

	// ��ġ ����
	SetPose(pose_.x + horizontal_move * user_move, pose_.y + vertical_move * user_move);

	return;
}

void User::UserMovefix(std::vector<POINT>& collide_polygon)
{
	int inside_hole = MyIsPointInPoly(pose_, collide_polygon);
	int on_hole_edge = MyIsPointOnPoly(pose_, collide_polygon);

	// : >> �̹� ���� ���� �����
	if (MyIsPointOnPoly(pose_, polygon_hole_, false))
	{
		// �� ��ġ ������ �� ù ��ġ�� & ���� ���
		pose_ = polygon_hole_[0];
		polygon_hole_.clear();
		return;
	}

	// : >> ���� �����δ� ��������
	if (inside_hole && !on_hole_edge)
	{
		pose_ = prev_pose_;
		return;
	}
	// <<



	// : >> ���� ���� ���ϰ� �����̱�
	if (on_hole_edge && !(polygon_hole_.size() != 0))
		return;
	// <<

	// : >> �����̽��� �Է� ����
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (prev_pose_.x != pose_.x || prev_pose_.y != pose_.y)	// ���� ��ġ�� ���� ��ġ�� �ٸ� ���
		{
			this->polygon_hole_.emplace_back(prev_pose_);
		}
		state_ = IDLE;
	}
	else
	{
		if (polygon_hole_.empty())	// �����̽��� �Է� ���ߴµ� �������� �ϸ� ����
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
	int inside_hole = MyIsPointInPoly(pose_, collide_polygon);		// ������ ���� ���۾�, ���� �� �Ǻ�
	int on_hole_edge = MyIsPointOnPoly(pose_, collide_polygon);

	// : >> ������ Hole�� ���ʿ��� ������ ��
	if (on_hole_edge && (polygon_hole_.size() != 0))
	{
		this->polygon_hole_.emplace_back(pose_);	// ���� ������ �־�
		InsertHoleToTail(collide_polygon);
	}
	// <<

	return;
}
