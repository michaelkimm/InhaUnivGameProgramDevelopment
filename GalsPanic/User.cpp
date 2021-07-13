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
	// : >> ������Ʈ �ð�
	DWORD cur_time = GetTickCount();
	static DWORD prev_time = cur_time;
	if (prev_time - cur_time < 100)
		return -1;
	prev_time = cur_time;
	// <<

	// : >> ������ ����
	prev_dir_ = cur_dir_;	// ���� ���� �� ��ġ ������Ʈ
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

	int inside_hole = (User::Instance()->CollisionHole(Hole::Instance()));			// ������ ���� ���۾�, ���� �� �Ǻ�
	int on_hole_edge = (User::Instance()->CollisionHoleLine(Hole::Instance()));

	// : >> �̹� ���� ���� �����
	if (ColiisionTail())
	{
		// �� ��ġ ������ �� ù ��ġ�� & ���� ���
		pose_ = polygon_hole_[0];
		polygon_hole_.clear();
		return -1;
	}

	// : >> ���� �����δ� ��������
	if (inside_hole && !on_hole_edge)
	{
		pose_ = prev_pose_;
		return -1;
	}
	// <<

	

	// : >> ���� ���� ���ϰ� �����̱�
	if (on_hole_edge && !(polygon_hole_.size() != 0))
		return 0;
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
			return -1;
		}
		else
		{
			//pose_ = prev_pose_;
			state_ = GOBACK;
		}
	}
	// <<

	// : >> ������ Hole�� ���ʿ��� ������ ��
	if (on_hole_edge && (polygon_hole_.size() != 0))
	{
		this->polygon_hole_.emplace_back(pose_);	// ���� ������ �־�
		InsertHoleToTailNew(Hole::Instance());
	}
	Hole::Instance()->intersect_dir_ = 0;	// ���� & ���� ���� �ʱ�ȭ
	arrange_dir_ = 0;
	// <<

	

	return 0;
}

int User::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// static int space_cnt = 0;
	// : >> ���� �׸��� ����
	switch (message)
	{
		//case WM_KEYDOWN:
		//	switch (wParam)
		//	{
		//	case VK_SPACE:
		//		//std::cout << "���� ��ġ: " << "(" << prev_pose_.x << ", " << prev_pose_.y
		//		//	<< "\t���� ��ġ: " << "(" << pose_.x << ", " << pose_.y << ")\n";

		//		if (prev_pose_.x == pose_.x && prev_pose_.y == pose_.y)	// ���� ��ġ�� ���� ��ġ�� �ٸ� ���
		//		{
		//			this->polygon_hole_.emplace_back(pose_);
		//		//	std::cout << "�����̽� " << ++space_cnt << "�� ����\n";
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

	// ������ ù���� ���� �ٰ��� �ȿ� �ִ��� Ȯ��
	if (PtInPoly((target_hole->polygon_hole_)[0], this->polygon_hole_) && this->polygon_hole_.size() > 2)
		include = 1;
	else
		include = 0;

	// ��� ������ �������� ���� Ȯ��
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
			if (LineLinePt(vc1, vn1, vc2, vn2)) // ����
			{
				if (next2 == 0)	// ù���� ������ �� ������ �̷�� ���
				{
					if (LinePoint(vc1.x, vc1.y, vn1.x, vn1.y, vn2.x, vn2.y))	// ����
					{

					}
					continue;
				}
				if (current2 == 0 || next2 == (this->polygon_hole_.size() - 1))
				{
					if (current2 == 0 && target_hole->intersect_dir_ == 0)			// ���� ���忡�� ������ �ð� �������� ���ĵ��ֳ� �ݽð� �������� ���ĵ��ֳ�?
					{
						target_hole->intersect_dir_ = target_hole->CW;
						std::cout << "���� ���忡�� ������ �ð� �������� ����\n";
					}
					if (next2 == (this->polygon_hole_.size() - 1) && target_hole->intersect_dir_ == 0)
					{
						target_hole->intersect_dir_ = target_hole->CCW;
						std::cout << "���� ���忡�� ������ �ݽð� �������� ����\n";
					}
					target_hole->intersect_pt_idx_vec_.emplace_back(current1);
					target_hole->intersect_pt_idx_vec_.emplace_back(next1);
					continue;
				}
				include = 0;	// �ȸ�����!
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

	// ������ ù���� ���� �ٰ��� �ȿ� �ִ��� Ȯ��
	if (PtInPoly((target_hole->polygon_hole_)[0], this->polygon_hole_) && this->polygon_hole_.size() > 2)
		include = 1;
	else
		include = 0;

	// ��� ������ �������� ���� Ȯ��
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
			if (next2 == 0)	// ù���� ������ �� ������ �̷�� ���
				continue;
			if (LineLinePt(vc1, vn1, vc2, vn2)) // ����
			{
				if (current2 == 0 || next2 == (this->polygon_hole_.size() - 1))
				{
					if (current2 == 0 && target_hole->intersect_dir_ == 0)			// ���� ���忡�� ������ �ð� �������� ���ĵ��ֳ� �ݽð� �������� ���ĵ��ֳ�?
					{
						target_hole->intersect_dir_ = target_hole->CW;
						std::cout << "���� ���忡�� ������ �ð� �������� ����\n";
					}
					if (next2 == (this->polygon_hole_.size() - 1) && target_hole->intersect_dir_ == 0)
					{
						target_hole->intersect_dir_ = target_hole->CCW;
						std::cout << "���� ���忡�� ������ �ݽð� �������� ����\n";
					}
					target_hole->intersect_pt_idx_vec_.emplace_back(current1);
					target_hole->intersect_pt_idx_vec_.emplace_back(next1);
					continue;
				}
				include = 0;	// �ȸ�����!
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
	//std::cout << "���� �� intersect_pt_idx_vec_ ������: " << hole->intersect_pt_idx_vec_.size() << std::endl;
	//// hole->intersect_pt_idx_vec_ 01�� 23�� ���� ���(���۰� ���� ���� ���п��� ���� ���)
	//if (hole->intersect_pt_idx_vec_[0] == hole->intersect_pt_idx_vec_[2] && hole->intersect_pt_idx_vec_[1] == hole->intersect_pt_idx_vec_[3]) {}
	//// hole->intersect_pt_idx_vec_ 12�� ���� ���(���۰� �� ���̿� �� ���� ���� ���)
	//else if (hole->intersect_pt_idx_vec_[1] == hole->intersect_pt_idx_vec_[2])
	//{
	//	std::cout << "hole ���� ������ ����, idx: " << 1 << std::endl;
	//	polygon_hole_.emplace_back(hole->polygon_hole_[hole->intersect_pt_idx_vec_[1]]);
	//}
	//// hole->intersect_pt_idx_vec_ 0123�� ��� �ٸ� ���(���۰� �� ���̿� 2�� �̻� �� ���� ���)
	//	// �߰��� �ε��� 0�� ���� ���
	//	// �߰��� �ε��� 0�� ���� ���
	if (hole->intersect_dir_ == Hole::CW)	// ���� ù������ ���� ���� ��� / ������ -> ó��
	{
		std::cout << "CW ���� 0: " << hole->intersect_pt_idx_vec_[0] << ", 1: " << hole->intersect_pt_idx_vec_[1] <<
			", 2: " << hole->intersect_pt_idx_vec_[2] << ", 3: " << hole->intersect_pt_idx_vec_[3] << std::endl;
		if (hole->intersect_pt_idx_vec_[0] > hole->intersect_pt_idx_vec_[3])
		{
			std::cout << "1\n";
			int idx = hole->intersect_pt_idx_vec_[2];
			while (1)
			{
				std::cout << "���� �ں��� Ŀ! hole ���� ������ ����, idx: " << idx << std::endl;
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
				std::cout << "hole ���� ������ ����, idx: " << idx << std::endl;
				polygon_hole_.emplace_back(hole->polygon_hole_[idx]);
			}
		}
	}
	else if (hole->intersect_dir_ == Hole::CCW)	// ���� �������� ���� ���� ��� / ó�� -> ������
	{
		std::cout << "CCW ���� 0: " << hole->intersect_pt_idx_vec_[0] << ", 1: " << hole->intersect_pt_idx_vec_[1] <<
			", 2: " << hole->intersect_pt_idx_vec_[2] << ", 3: " << hole->intersect_pt_idx_vec_[3] << std::endl;
		for (int idx = hole->intersect_pt_idx_vec_[1]; idx <= hole->intersect_pt_idx_vec_[2]; idx++)
		{
			std::cout << "hole ���� ������ ����, idx: " << idx << std::endl;
			polygon_hole_.emplace_back(hole->polygon_hole_[idx]);
		}
	}

	// ���� ���θ� hole���� ����
	hole->polygon_hole_.clear();
	hole->polygon_hole_ = polygon_hole_;
	// ���� ����
	polygon_hole_.clear();
	std::cout << "���� �� hole ������: " << hole->polygon_hole_.size() << std::endl;
	return 0;
}

int User::InsertHoleToTailNew(Hole *hole)
{
	static int cnt;
	std::cout << "try cnt: " << ++cnt << std::endl;
	std::cout << "����\n";
	if (GetMomentDir())
		std::cout << "���� �� ����\n";
	else
		std::cout << "���� �� ����\n";

	if (Hole::Instance()->GetMomentDir())
		std::cout << "���� �� ����\n";
	else
		std::cout << "���� �� ����\n";

	// : >> ���� ���� ������ �˰����� �̿��ϸ� �ش� ���� 2' 3' ���п� �ִ� ���� �� �� �ִ�.
	// ù��° ��
	int first_current = 0;
	int first_next = 0;
	for (first_current = 0; first_current < hole->polygon_hole_.size(); first_current++)
	{
		first_next = (first_current + 1) % hole->polygon_hole_.size();
		// current-next ���а� ���� ù��° ���� ������ current 
		if (LinePointPt(hole->polygon_hole_[first_current], hole->polygon_hole_[first_next], this->polygon_hole_[0]))
		{
			break;
		}
	}
	// ������ ��
	int last_current = 0;
	int last_next = 0;
	for (last_current = 0; last_current < hole->polygon_hole_.size(); last_current++)
	{
		last_next = (last_current + 1) % hole->polygon_hole_.size();
		// current-next ���а� ���� ù��° ���� ������ current 
		if (LinePointPt(hole->polygon_hole_[last_current], hole->polygon_hole_[last_next], this->polygon_hole_.back()))
		{
			break;
		}
	}
	// <<


	vector<POINT> tail_polygon_hole_tmp1 = this->polygon_hole_;
	vector<POINT> tail_polygon_hole_tmp2 = this->polygon_hole_;

	// : >> �� ���� ��ü�� CC, CCW�� ���� �ٸ��� ����
	std::cout << "�Ѵ� CW�̰ų� �Ѵ� CCW�̸�\n";
	// : << ���� �־�� hole�� hole�� ���� tale�� ���ϴ��� ����. �ȼ��ϸ� �ݴ�� ����
	if (TailIncludeHoleNew(Hole::Instance()))
	{
		std::cout << "����\n";
		// last ���� first���� ���� ���
		for (last_next; last_next != first_next; last_next++)
		{
			if (last_next == hole->polygon_hole_.size()) last_next = 0;
			tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]);
		}
	}
	else
	{
		// std::cout << "������\n";
		//// last ���� first���� ���� ���
		while (1)
		{
			if (last_next == first_current) break;	// 0112 �� ���� ������ �߰��� ���� ���
			tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]);
			last_next++;
			if (last_next == hole->polygon_hole_.size()) last_next = 0;
		}
		tail_polygon_hole_tmp1.emplace_back(hole->polygon_hole_[last_next]); // first_current �ְ� ������
	}
	vector<POINT> combined1 = tail_polygon_hole_tmp1;
	// <<

	// std::cout << "�� ����\n";
	if (TailIncludeHoleNew(Hole::Instance()))
	{
		// std::cout << "����\n";
		// last ���� first���� ���� ���
		for (last_current; last_current != first_current; last_current--)
		{
			if (last_current == -1) last_current = (hole->polygon_hole_.size() - 1);
			tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]);
		}
	}
	else
	{
		// std::cout << "������\n";
		//// last ���� first���� ���� ���
		while (1)
		{
			if (last_current == first_next) break;	// 0112 �� ���� ������ �߰��� ���� ���
			tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]);
			last_current--;
			if (last_current == -1) last_current = (hole->polygon_hole_.size() - 1);
		}
		tail_polygon_hole_tmp2.emplace_back(hole->polygon_hole_[last_current]); // first_current �ְ� ������
	}
	vector<POINT> combined2 = tail_polygon_hole_tmp2;
	// <<

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);
	std::cout << "area1: " << area1 << ", area2: " << area2 << std::endl;

	hole->polygon_hole_.clear();	// ���� ���� �� hole �����
	this->polygon_hole_.clear();	//
	
	hole->polygon_hole_ = abs(area1) > abs(area2) ? combined1 : combined2;	// �ʺ� �� ū ������ hole ��ü
	
	return 0;
}

int User::GetMomentDir()
{
	pointLF Center{ 0, 0 };
	// : >> ������ �߾� �� ���
	for (int i = 0; i < polygon_hole_.size(); i++)
	{
		Center = pointLF{ Center.x + polygon_hole_[i].x, Center.y + polygon_hole_[i].y };
	}
	Center = pointLF{ Center.x / polygon_hole_.size(), Center.y / polygon_hole_.size() };
	// <<

	// : >> �� ���п� ���� ���� �� ������ ���ϱ�
	double outer_product_sum = 0;
	int current = 0, next = 0;
	for (current; current < polygon_hole_.size(); current++)
	{
		next = (current + 1) % polygon_hole_.size();
		pointLF centrifugal_force = pointLF{ polygon_hole_[current].x - Center.x, polygon_hole_[current].y - Center.y };
		pointLF vt_tmp = pointLF{ (double)(polygon_hole_[next].x - polygon_hole_[current].x), (double)(polygon_hole_[next].y - polygon_hole_[current].y) };	// �ܰ��� ����
		outer_product_sum = outer_product_sum + centrifugal_force.x * vt_tmp.y - centrifugal_force.y * vt_tmp.x; // ������
	}
	outer_product_sum = outer_product_sum / polygon_hole_.size();

	// : <<
	// ���� ���� return
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
		// pt�� polygon�� �ش� ������ y��ǥ ���̿� ���� ��
		if (!((polygon[i].y >= pt.y) != (polygon[j].y >= pt.y)))
			continue;
		// pt.x�� �ش� ���а� �����ϴ� ���� x��ǥ�� cross_pt�� ����
		cross_pt_x = (pt.y - polygon[i].y) * (polygon[j].x - polygon[i].x) / 1.0f / (polygon[j].y - polygon[i].y) + polygon[i].x;
		// cross_pt.x > pt.x�� ����
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