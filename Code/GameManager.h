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
	// -- [게임 보드]
	Candy* candyBoard[CANDY_HEIGHT][CANDY_WIDTH];
	// -- [Candy의 Width가 저장됨]
	float candy_size;
	// -- [터치가 시작한곳과, 끝난곳]
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
	// -- [ 캔디 생성 ]
	Candy* CreateCandy(const Vec2& arr_position);
	// -- [ 캔디 랜덤 생성 ]
	CandyFlavor RandomCandy(int n);
	// -- [ 캔디 초기화 ]
	void ClearBoard();
	Vec2 MapToArray(const Vec2& position);
	Vec2 ArrayToMap(const Vec2& position);
	// -- [ 클릭한 캔디를 찾는다 ]
	void SwapCandy(Candy* target1, Candy* target2);
	bool SwapCandyCheck(Candy* candy);
	int  BoomCheck(int arr_x, int arr_y, const Vec2& dir);
	// -- [ 터트릴수있는 캔디의 개수를 구함 ]
	int  CheckCandy(int arr_x, int arr_y, const Vec2& dir);
	// -- [ Y축으로 남은 캔디의 값 ]
	int  PluseBoom(int arr_x, int arr_y,const Vec2& dir);
	// -- [ 캔디들이 떨어진 후 캔디들을 검사한다 ]
	void DropAndCheck();
	void BoomBoomBoom(Candy* target1, Candy* target2);
	// 터트릴 캔디를 담음
	std::vector<Candy*> vAllCandy;
	std::vector<Candy*> vCandy;		// 검사 캔디
	std::vector<Candy*> vBoomCandy; // 삭제 캔디
public:
	// --------------------------[ 초기화 한다 ]
	void Initialize(Node* _ownerLayer);
	// --------------------------[ 터치를 받아온다 ]
	void TouchBegan(Touch* _touch);
	void TouchMoved(Touch* _touch);
	void TouchEnded(Touch* _touch);

	int getScore(){ return score; };
};