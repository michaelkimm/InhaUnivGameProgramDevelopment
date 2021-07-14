#pragma once

#include "framework.h"
#include "Hole.h"
#include <vector>


// POINT pt�� polygon ���ο� �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
bool IsInside(POINT pt, std::vector<POINT> &polygon);					// �Ϻ� �� ���� ���� ���ζ�� ����
int MyIsPointInPoly(POINT& pose_, std::vector<POINT>& polygon_hole_);	// �� ���� ���� ���ζ�� ����x

// POINT pt�� polygon ���� ���� �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
int MyIsPointOnPoly(POINT& pose_, std::vector<POINT> &polygon_hole_, bool think_as_poly = true);

// pt���� ����p1-p2�� �浹�ϴ��� Ȯ���ϴ� �Լ��Դϴ�.
int LinePointPt(POINT p1, POINT p2, POINT pt);
int LinePoint(float x1, float y1, float x2, float y2, float px, float py);

// ����p1-p2�� ����p3-p4�� �浹�ϴ��� Ȯ���ϴ� �Լ��Դϴ�.
int LineLinePt(POINT p1, POINT p2, POINT p3, POINT p4);
int LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

// : >> ���� ���ϱ� ���� �Լ����Դϴ�
// 2���� ������ ���� ���Դϴ�
int ccw(const POINT & pa, const POINT & pb, const POINT & pc);
// ���� ���ϱ� �Լ��Դϴ�.
double GetArea(const std::vector<POINT>& polygon);



