#pragma once
#ifndef HOLE_H_
#define HOLE_H_

#include <Windows.h>
#include <vector>

#include "User.h"

class User;

class Hole
{
private:
public:
	std::vector<POINT> polygon_hole_;
private:
	static Hole* instance_;
protected:
	Hole() {}
	~Hole() {}
public:
	static Hole* Instance();
public:
	std::vector<int> intersect_pt_idx_vec_;
	enum dir { CW = 1, CCW = 2 };
	int intersect_dir_ = 0;						// CW = 0 / CCW = 1

	int Draw(HDC);
	int Update();

	void Add(std::vector<POINT>);
	void Add(POINT);
	int GetMomentDir();
	friend int User::CollisionHole(Hole *);
	friend int User::CollisionHoleLine(Hole *);
	friend int User::TailIncludeHole(Hole*);
	friend int User::InsertHoleToTail(Hole *);
	friend int User::InsertHoleToTailNew(Hole*);
};

#endif
