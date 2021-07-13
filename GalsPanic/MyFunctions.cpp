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