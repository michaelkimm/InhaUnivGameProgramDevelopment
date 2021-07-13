#include <Windows.h>
#include "Hole.h"
#include "pointLF.h"

Hole* Hole::instance_ = 0;

Hole * Hole::Instance()
{
	if (instance_ == NULL)
		instance_ = new Hole;
	return instance_;
}

int Hole::Draw(HDC _hdc)
{
	Polygon(_hdc, &polygon_hole_[0], polygon_hole_.size());
	return 0;
}

int Hole::Update()
{
	return 0;
}

void Hole::Add(std::vector<POINT> _v)
{
	for (POINT vec : _v)
	{
		polygon_hole_.emplace_back(vec);
	}
}

void Hole::Add(POINT _p)
{
	polygon_hole_.emplace_back(_p);
}


int Hole::GetMomentDir()
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