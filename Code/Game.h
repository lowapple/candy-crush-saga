#pragma once
#include "stdafx.h"

class GameManager;
class Game : public Layer
{
private:
	bool execute;
	Vec2 touchPosition;
	GameManager* gameManager;
	Label* game_time;
	Label* score;

	bool action_time;
	int cur_score;

	float curTime;
	int time;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;

    static Scene* createScene();
    bool init() override;  
	void Loop(float dt);
    CREATE_FUNC(Game);
};