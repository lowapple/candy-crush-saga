#pragma once
#include "stdafx.h"

#define CANDY_WIDTH 7
#define CANDY_HEIGHT 7

class Candy;
class GameManager
{
private:
	Node* ownerLayer;
private:
	// -- [���� ����]
	Candy* candyBoard[CANDY_HEIGHT][CANDY_WIDTH];
	// -- [Candy�� Width�� �����]
	float candy_size;
	// -- [��ġ�� �����Ѱ���, ������]
	Vec2 touch_begine, touch_end;
	bool vertical_boom, horizontal_boom;
	bool pluse_boom;
	bool no_three;
	bool first_execute;
	bool dropCandy;
	bool three_drop;

	bool boomCandy;

	bool is_swap_return;
	bool is_swap;

	bool is_begine_touch_enable;

	int score;

	Candy* boomberCandy;
private:// ------------------------------------------ [ Function ]
	// -- [ ĵ�� ���� ]
	Candy* CreateCandy(const Vec2& arr_position);
	// -- [ ĵ�� ���� ���� ]
	CandyFlavor RandomCandy(int n);
	// -- [ ĵ�� �ʱ�ȭ ]
	void ClearBoard();
	Vec2 MapToArray(const Vec2& position);
	Vec2 ArrayToMap(const Vec2& position);
	// -- [ Ŭ���� ĵ�� ã�´� ]
	void SwapCandy(Candy* target1, Candy* target2);
	bool SwapCandyCheck(Candy* candy);
	int  BoomCheck(int arr_x, int arr_y, const Vec2& dir);
	// -- [ ��Ʈ�����ִ� ĵ���� ������ ���� ]
	int  CheckCandy(int arr_x, int arr_y, const Vec2& dir);
	// -- [ Y������ ���� ĵ���� �� ]
	int  PluseBoom(int arr_x, int arr_y,const Vec2& dir);
	// -- [ ĵ����� ������ �� ĵ����� �˻��Ѵ� ]
	void DropAndCheck();
	void BoomBoomBoom(Candy* target1, Candy* target2);
	// ��Ʈ�� ĵ�� ����
	std::vector<Candy*> vAllCandy;
	std::vector<Candy*> vCandy;		// �˻� ĵ��
	std::vector<Candy*> vBoomCandy; // ���� ĵ��
public:
	// --------------------------[ �ʱ�ȭ �Ѵ� ]
	void Initialize(Node* _ownerLayer);
	// --------------------------[ ��ġ�� �޾ƿ´� ]
	void TouchBegan(Touch* _touch);
	void TouchMoved(Touch* _touch);
	void TouchEnded(Touch* _touch);

	int getScore(){ return score; };
};