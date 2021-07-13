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