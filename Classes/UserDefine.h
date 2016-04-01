#pragma once

#define WIDTH 1000
#define HEIGHT 650

#define D_WINDOWS_SHOW 0.65

#define D_GAME_NAME "My Game"

enum
{
	Board_tag,
	Candy_tag,
	UI_tag
};

enum class CandyFlavor
{
	NONE,
	A,
	B,
	C,
	D,
	E,
	F,
	BOOM
};