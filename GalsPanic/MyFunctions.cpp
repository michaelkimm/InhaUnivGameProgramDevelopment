#include "MyFunctions.h"


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

// hole_->polygon_
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

int MyIsPointInPoly(POINT& pose_, std::vector<POINT>& polygon_hole_)
{
	
	int collision = false;
	int next = 0;
	for (int current = 0; current < polygon_hole_.size(); current++)
	{
		next = current + 1;
		if (next == polygon_hole_.size()) next = 0;

		POINT vc = polygon_hole_[current];
		POINT vn = polygon_hole_[next];

		if (((vc.y >= pose_.y && vn.y < pose_.y) || (vc.y < pose_.y && vn.y >= pose_.y)) &&
			(pose_.x < (vn.x - vc.x)*(pose_.y - vc.y) / (vn.y - vc.y) + vc.x))
		{
			collision = !collision;
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

int LinePointPt(POINT p1, POINT p2, POINT pt) {

	return LinePoint(p1.x, p1.y, p2.x, p2.y, pt.x, pt.y);
}

int LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	// calculate the distance to intersection point
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		return true;
	}
	return false;
}

int LineLinePt(POINT p1, POINT p2, POINT p3, POINT p4)
{
	return LineLine(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
}

int PtInPoly(POINT &pt, std::vector<POINT> &target_vec)
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


// user_->pose_ / hole_->polygon_만 필요
int MyIsPointOnPoly(POINT& pose_, std::vector<POINT> &polygon_hole_, bool think_as_poly)
{
	int collision = false;
	int next = 0;
	for (int current = 0; current < polygon_hole_.size(); current++)
	{
		next = current + 1;
		if (next == polygon_hole_.size())
		{
			if (think_as_poly)
				next = 0;
			else
				continue;
		}

		if (LinePoint(polygon_hole_[current].x, polygon_hole_[current].y,
			polygon_hole_[next].x, polygon_hole_[next].y, pose_.x, pose_.y))
		{
			collision = !collision;
			return collision;
		}
	}
	return collision;
}

POINT operator+(POINT & p1, POINT & p2)
{
	return POINT{ p1.x + p2.x, p1.y + p2.y };
}

POINT operator-(POINT & p1, POINT & p2)
{
	return POINT{ p1.x - p2.x, p1.y - p2.y };
}

bool operator==(POINT & p1, POINT & p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

bool operator!=(POINT & p1, POINT & p2)
{
	return (p1.x != p2.x) || (p1.y != p2.y);
}

void operator+=(POINT & p1, POINT & p2)
{
	p1.x = p1.x + p2.x;
	p1.y = p1.y + p2.y;
}

void operator-=(POINT & p1, POINT & p2)
{
	p1.x = p1.x - p2.x;
	p1.y = p1.y - p2.y;
}

POINT operator*(int m, POINT & p1)
{
	return POINT{ p1.x * m, p1.y * m };
}

POINT operator*(POINT & p1, int m)
{
	return POINT{ p1.x * m, p1.y * m };
}
