#pragma once
#include "stdafx.h"

class Candy : public Sprite
{
private:
	Vec2 arrayPosition;
public:
	bool this_is_boom;
	Candy(){ this_is_boom = false; };
	~Candy(){};
	CandyFlavor candyFlavor;

	static Candy* Candy::create(const CandyFlavor& candy_flovor = CandyFlavor::NONE);
	Vec2 getArrayPosition();
	void setArrayPosition(Vec2& position);
};