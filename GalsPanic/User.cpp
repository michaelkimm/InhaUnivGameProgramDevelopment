#include "User.h"
#include "figureDraw.h"
#include "Hole.h"
#include "pointLF.h"

#include <iostream>
#include <math.h>



using std::vector;

int L2N(POINT& p1, POINT& p2);
int PointInnerProduct(POINT& p1, POINT& p2);
bool IsInside(POINT pt, std::vector<POINT> &polygon);
int CollisionPtLine(POINT pose_, Hole *target_hole);
int LinePoint(float x1, float y1, float x2, float y2, float px, float py);

int ccw(const POINT & pa, const POINT & pb, const POINT & pc)
{
	return (pa.x * pb.y + pb.x * pc.y + pc.x * pa.y) - (pb.x * pa.y + pa.x * pc.y + pc.x * pb.y);
}

double GetArea(const std::vector<POINT>& polygon)
{
	double sum = 0;

	int size = polygon.size();

	if (size < 3) return sum;

	for (int i = 1; i < size - 1; ++i)
		sum += (double)ccw(polygon[0], polygon[i], polygon[i + 1]);

	return sum / 2;
}

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
		{
			//pose_ = prev_pose_;
			state_ = GOBACK;
		}
	}
	// <<

	// : >> 꼬리가 Hole과 양쪽에서 만났을 때
	if (on_hole_edge && (polygon_hole_.size() != 0))
	{
		this->polygon_hole_.emplace_back(pose_);	// 닿은 순간도 넣어
		InsertHoleToTailNew(Hole::Instance());
	}
	Hole::Instance()->intersect_dir_ = 0;	// 꼬리 & 구멍 방향 초기화
	arrange_dir_ = 0;
	// <<

	

	return 0;
}

int User::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// static int space_cnt = 0;
	// : >> 구멍 그리기 구현
	switch (message)
	{
		//case WM_KEYDOWN:
		//	switch (wParam)
		//	{
		//	case VK_SPACE:
		//		//std::cout << "이전 위치: " << "(" << prev_pose_.x << ", " << prev_pose_.y
		//		//	<< "\t현재 위치: " << "(" << pose_.x << ", " << pose_.y << ")\n";

		//		if (prev_pose_.x == pose_.x && prev_pose_.y == pose_.y)	// 이전 위치와 현재 위치가 다를 경우
		//		{
		//			this->polygon_hole_.emplace_back(pose_);
		//		//	std::cout << "스페이스 " << ++space_cnt << "번 누름\n";
		//		}
		//		break;
		//	default:
		//		break;
		//	}
		//	break;
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

int User::TailIncludeHole(Hole *target_hole)
{
	int include = 0;

	// 구멍의 첫점이 꼬리 다각형 안에 있는지 확인
	if (PtInPoly((target_hole->polygon_hole_)[0], this->polygon_hole_) && this->polygon_hole_.size() > 2)
		include = 1;
	else
		include = 0;

	// 모든 선분이 만나는지 여부 확인
	int next1 = 0;
	int next2 = 0;
	for (int current2 = 0; current2 < this->polygon_hole_.size(); current2++)
	{
		next2 = current2 + 1;
		if (next2 == this->polygon_hole_.size()) next2 = 0;

		POINT vc2 = this->polygon_hole_[current2];
		POINT vn2 = this->polygon_hole_[next2];
		
		for (int current1 = 0; current1 < target_hole->polygon_hole_.size(); current1++)
		{
			next1 = current1 + 1;
			if (next1 == target_hole->polygon_hole_.size()) next1 = 0;

			POINT vc1 = target_hole->polygon_hole_[current1];
			POINT vn1 = target_hole->polygon_hole_[next1];
			if (LineLinePt(vc1, vn1, vc2, vn2)) // 조건
			{
				if (next2 == 0)	// 첫점과 끝점이 한 선분을 이루는 경우
				{
					if (LinePoint(vc1.x, vc1.y, vn1.x, vn1.y, vn2.x, vn2.y))	// 만약
					{

					}
					continue;
				}
				if (current2 == 0 || next2 == (this->polygon_hole_.size() - 1))
				{
					if (current2 == 0 && target_hole->intersect_dir_ == 0)			// 꼬리 입장에서 구멍은 시계 방향으로 정렬되있나 반시계 방향으로 정렬돼있나?
					{
						target_hole->intersect_dir_ = target_hole->CW;
						std::cout << "꼬리 입장에서 구멍은 시계 방향으로 정렬\n";
					}
					if (next2 == (this->polygon_hole_.size() - 1) && target_hole->intersect_dir_ == 0)
					{
						target_hole->intersect_dir_ = target_hole->CCW;
						std::cout << "꼬리 입장에서 구멍은 반시계 방향으로 정렬\n";
					}
					target_hole->intersect_pt_idx_vec_.emplace_back(current1);
					target_hole->intersect_pt_idx_vec_.emplace_back(next1);
					continue;
				}
				include = 0;	// 안만난다!
			}
		}
	}
	
	return include;
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

/*
int User::TailIncludeHole(Hole *target_hole)
{
	int include = 0;

	// 구멍의 첫점이 꼬리 다각형 안에 있는지 확인
	if (PtInPoly((target_hole->polygon_hole_)[0], this->polygon_hole_) && this->polygon_hole_.size() > 2)
		include = 1;
	else
		include = 0;

	// 모든 선분이 만나는지 여부 확인
	int next1 = 0;
	int next2 = 0;
	for (int current1 = 0; current1 < target_hole->polygon_hole_.size(); current1++)
	{
		next1 = current1 + 1;
		if (next1 == target_hole->polygon_hole_.size()) next1 = 0;

		POINT vc1 = target_hole->polygon_hole_[current1];
		POINT vn1 = target_hole->polygon_hole_[next1];

		for (int current2 = 0; current2 < this->polygon_hole_.size(); current2++)
		{
			next2 = current2 + 1;
			if (next2 == this->polygon_hole_.size()) next2 = 0;

			POINT vc2 = this->polygon_hole_[current2];
			POINT vn2 = this->polygon_hole_[next2];
			if (next2 == 0)	// 첫점과 끝점이 한 선분을 이루는 경우
				continue;
			if (LineLinePt(vc1, vn1, vc2, vn2)) // 조건
			{
				if (current2 == 0 || next2 == (this->polygon_hole_.size() - 1))
				{
					if (current2 == 0 && target_hole->intersect_dir_ == 0)			// 꼬리 입장에서 구멍은 시계 방향으로 정렬되있나 반시계 방향으로 정렬돼있나?
					{
						target_hole->intersect_dir_ = target_hole->CW;
						std::cout << "꼬리 입장에서 구멍은 시계 방향으로 정렬\n";
					}
					if (next2 == (this->polygon_hole_.size() - 1) && target_hole->intersect_dir_ == 0)
					{
						target_hole->intersect_dir_ = target_hole->CCW;
						std::cout << "꼬리 입장에서 구멍은 반시계 방향으로 정렬\n";
					}
					target_hole->intersect_pt_idx_vec_.emplace_back(current1);
					target_hole->intersect_pt_idx_vec_.emplace_back(next1);
					continue;
				}
				include = 0;	// 안만난다!
			}
		}
	}
	return include;
}
*/

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

int User::InsertHoleToTail(Hole *hole)
{
	//std::cout << "변경 전 intersect_pt_idx_vec_ 사이즈: " << hole->intersect_pt_idx_vec_.size() << std::endl;
	//// hole->intersect_pt_idx_vec_ 01과 23이 같을 경우(시작과 끝이 같은 선분에서 끝날 경우)
	//if (hole->intersect_pt_idx_vec_[0] == hole->intersect_pt_idx_vec_[2] && hole->intersect_pt_idx_vec_[1] == hole->intersect_pt_idx_vec_[3]) {}
	//// hole->intersect_pt_idx_vec_ 12가 같을 경우(시작과 끝 사이에 한 점만 있을 경우)
	//else if (hole->intersect_pt_idx_vec_[1] == hole->intersect_pt_idx_vec_[2])
	//{
	//	std::cout << "hole 점을 꼬리에 넣자, idx: " << 1 << std::endl;
	//	polygon_hole_.emplace_back(hole->polygon_hole_[hole->intersect_pt_idx_vec_[1]]);
	//}
	//// hole->intersect_pt_idx_vec_ 0123가 모두 다를 경우(시작과 끝 사이에 2개 이상 점 있을 경우)
	//	// 중간에 인덱스 0이 있을 경우
	//	// 중간에 인덱스 0이 없을 경우
	if (hole->intersect_dir_ == Hole::CW)	// 꼬리 첫번쨰를 먼저 만날 경우 / 마지막 -> 처음
	{
		std::cout << "CW 방향 0: " << hole->intersect_pt_idx_vec_[0] << ", 1: " << hole->intersect_pt_idx_vec_[1] <<
			", 2: " << hole->intersect_pt_idx_vec_[2] << ", 3: " << hole->intersect_pt_idx_vec_[3] << std::endl;
		if (hole->intersect_pt_idx_vec_[0] > hole->intersect_pt_idx_vec_[3])
		{
			std::cout << "1\n";
			int idx = hole->intersect_pt_idx_vec_[2];
			while (1)
			{
				std::cout << "앞이 뒤보다 커! hole 점을 꼬리에 넣자, idx: " << idx << std::endl;
				polygon_hole_.emplace_back(hole->polygon_hole_[idx]);
				idx--;
				if (idx < 0)
					idx = hole->polygon_hole_.size() - 1;
				if (idx == hole->intersect_pt_idx_vec_[0])
					break;
			}
		}
		else
		{
			std::cout << "2\n";
			for (int idx = hole->intersect_pt_idx_vec_[2]; idx >= hole->intersect_pt_idx_vec_[1]; idx--)
			{
				std::cout << "hole 점을 꼬리에 넣자, idx: " << idx << std::endl;
				polygon_hole_.emplace_back(hole->polygon_hole_[idx]);
			}
		}
	}
	else if (hole->intersect_dir_ == Hole::CCW)	// 꼬리 마지막을 먼저 만날 경우 / 처음 -> 마지막
	{
		std::cout << "CCW 방향 0: " << hole->intersect_pt_idx_vec_[0] << ", 1: " << hole->intersect_pt_idx_vec_[1] <<
			", 2: " << hole->intersect_pt_idx_vec_[2] << ", 3: " << hole->intersect_pt_idx_vec_[3] << std::endl;
		for (int idx = hole->intersect_pt_idx_vec_[1]; idx <= hole->intersect_pt_idx_vec_[2]; idx++)
		{
			std::cout << "hole 점을 꼬리에 넣자, idx: " << idx << std::endl;
			polygon_hole_.emplace_back(hole->polygon_hole_[idx]);
		}
	}

	// 꼬리 전부를 hole에게 복사
	hole->polygon_hole_.clear();
	hole->polygon_hole_ = polygon_hole_;
	// 꼬리 비우기
	polygon_hole_.clear();
	std::cout << "변경 후 hole 사이즈: " << hole->polygon_hole_.size() << std::endl;
	return 0;
}

int User::InsertHoleToTailNew(Hole *hole)
{
	static int cnt;
	std::cout << "try cnt: " << ++cnt << std::endl;
	std::cout << "시작\n";
	if (GetMomentDir())
		std::cout << "꼬리 양 방향\n";
	else
		std::cout << "꼬리 음 방향\n";

	if (Hole::Instance()->GetMomentDir())
		std::cout << "구멍 양 방향\n";
	else
		std::cout << "구멍 음 방향\n";

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


	vector<POINT> tail_polygon_hole_tmp1 = this->polygon_hole_;
	vector<POINT> tail_polygon_hole_tmp2 = this->polygon_hole_;

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
	vector<POINT> combined1 = tail_polygon_hole_tmp1;
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
	vector<POINT> combined2 = tail_polygon_hole_tmp2;
	// <<

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);
	std::cout << "area1: " << area1 << ", area2: " << area2 << std::endl;

	hole->polygon_hole_.clear();	// 기존 꼬리 및 hole 지우고
	this->polygon_hole_.clear();	//
	
	hole->polygon_hole_ = abs(area1) > abs(area2) ? combined1 : combined2;	// 너비가 더 큰 것으로 hole 대체
	
	return 0;
}

int User::GetMomentDir()
{
	pointLF Center{ 0, 0 };
	// : >> 벡터의 중앙 점 얻기
	for (int i = 0; i < polygon_hole_.size(); i++)
	{
		Center = pointLF{ Center.x + polygon_hole_[i].x, Center.y + polygon_hole_[i].y };
	}
	Center = pointLF{ Center.x / polygon_hole_.size(), Center.y / polygon_hole_.size() };
	// <<

	// : >> 각 선분에 대해 외적 후 누적합 구하기
	double outer_product_sum = 0;
	int current = 0, next = 0;
	for (current; current < polygon_hole_.size(); current++)
	{
		next = (current + 1) % polygon_hole_.size();
		pointLF centrifugal_force = pointLF{ polygon_hole_[current].x - Center.x, polygon_hole_[current].y - Center.y };
		pointLF vt_tmp = pointLF{ (double)(polygon_hole_[next].x - polygon_hole_[current].x), (double)(polygon_hole_[next].y - polygon_hole_[current].y) };	// 외곽선 벡터
		outer_product_sum = outer_product_sum + centrifugal_force.x * vt_tmp.y - centrifugal_force.y * vt_tmp.x; // 외적합
	}
	outer_product_sum = outer_product_sum / polygon_hole_.size();

	// : <<
	// 누적 합이 return
	return outer_product_sum > 0;
}


int L2N(POINT& p1, POINT& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int PointInnerProduct(POINT& p1, POINT& p2)
{
	return (p1.x * p2.x) + (p1.y * p2.y);
}

bool IsInside(POINT pt, std::vector<POINT> &polygon)
{
	int crosses = 0;
	int j;
	float cross_pt_x;
	for (int i = 0; i < polygon.size(); i++)
	{
		j = (i + 1) % polygon.size();
		// pt가 polygon의 해당 선분의 y좌표 사이에 있을 때
		if (!((polygon[i].y >= pt.y) != (polygon[j].y >= pt.y)))
			continue;
		// pt.x가 해당 선분과 교차하는 점의 x좌표를 cross_pt라 하자
		cross_pt_x = (pt.y - polygon[i].y) * (polygon[j].x - polygon[i].x) / 1.0f / (polygon[j].y - polygon[i].y) + polygon[i].x;
		// cross_pt.x > pt.x면 교차
		//std::cout << "cross_pt_x: " << cross_pt_x << "\tpt.x: " << pt.x <<  std::endl;
		if (cross_pt_x > pt.x)
			crosses++;
	}
	return crosses % 2 > 0;
}

int CollisionPtLine(POINT pose_, Hole *target_hole)
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

int LinePoint(float x1, float y1, float x2, float y2, float px, float py) {

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