#pragma once

#include "framework.h"
#include "Hole.h"
#include <vector>

POINT operator+(POINT& p1, POINT& p2);
POINT operator-(POINT& p1, POINT& p2);
bool operator==(POINT& p1, POINT& p2);
bool operator!=(POINT& p1, POINT& p2);
void operator+=(POINT& p1, POINT& p2);
void operator-=(POINT& p1, POINT& p2);

POINT operator*(int m, POINT& p1);
POINT operator*(POINT& p1, int m);

int L2N(POINT& p1, POINT& p2);
int PointInnerProduct(POINT& p1, POINT& p2);

bool IsInside(POINT pt, std::vector<POINT> &polygon);
int MyIsPointInPoly(POINT& pose_, std::vector<POINT>& polygon_hole_);

int CollisionPtLine(POINT pose_, Hole *target_hole);
int LinePoint(float x1, float y1, float x2, float y2, float px, float py);
int ccw(const POINT & pa, const POINT & pb, const POINT & pc);
double GetArea(const std::vector<POINT>& polygon);

int LinePoint(float x1, float y1, float x2, float y2, float px, float py);
int LinePointPt(POINT p1, POINT p2, POINT pt);
int LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
int LineLinePt(POINT p1, POINT p2, POINT p3, POINT p4);
int PtInPoly(POINT &pt, std::vector<POINT> &target_vec);

int MyIsPointOnPoly(POINT& pose_, std::vector<POINT> &polygon_hole_, bool think_as_poly = true);