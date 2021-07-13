#pragma once

#include "framework.h"
#include "Hole.h"
#include <vector>

int L2N(POINT& p1, POINT& p2);
int PointInnerProduct(POINT& p1, POINT& p2);
bool IsInside(POINT pt, std::vector<POINT> &polygon);
int CollisionPtLine(POINT pose_, Hole *target_hole);
int LinePoint(float x1, float y1, float x2, float y2, float px, float py);
int ccw(const POINT & pa, const POINT & pb, const POINT & pc);
double GetArea(const std::vector<POINT>& polygon);