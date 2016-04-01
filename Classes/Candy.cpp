#include "Candy.h"

Candy* Candy::create(const CandyFlavor& candy_flovor)
{
	Candy *candy = new Candy();
	if (candy && candy->init())
	{
		// 캔디 이미지
		switch (candy_flovor) {
		case CandyFlavor::A:
			candy->setTexture("A.png");
			break;
		case CandyFlavor::B:
			candy->setTexture("B.png");
			break;
		case CandyFlavor::C:
			candy->setTexture("C.png");
			break;
		case CandyFlavor::D:
			candy->setTexture("D.png");
			break;
		case CandyFlavor::E:
			candy->setTexture("E.png");
			break;
		case CandyFlavor::F:
			candy->setTexture("F.png");
			break;
		}
		// 현재 캔디의 종류
		candy->candyFlavor = candy_flovor;
		candy->autorelease();
		candy->setTag(1);
		return candy;
	}
	CC_SAFE_DELETE(candy);
}

Vec2 Candy::getArrayPosition()
{
	return this->arrayPosition;
}

void Candy::setArrayPosition(Vec2& position)
{
	this->arrayPosition = position;
}