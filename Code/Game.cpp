#include "stdafx.h"
#include "GameManager.h"

Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	if (!Layer::init())
		return false;
	// ---------------------------- [ 터치 리스너 ]
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	LayerColor* board = LayerColor::create(Color4B(255, 255, 255, 50));
	board->setPosition(Vec2::ZERO);
	board->setContentSize(Size(650, 650));
	board->setName("BOARD");
	this->addChild(board, Board_tag);

	score = Label::create("dd", "Arial", 60, Size::ZERO, TextHAlignment::CENTER);
	score->setPosition(Vec2((WIDTH / 6) * 5, HEIGHT - 100));
	score->setName("SCORE");
	this->addChild(score);

	game_time = Label::create("30", "Arial", 100, Size::ZERO, TextHAlignment::CENTER);
	game_time->setPosition(Vec2((WIDTH / 6) * 5, HEIGHT - 350));
	game_time->setName("GAME_TIME");
	this->addChild(game_time);

	// ---------------------------- [ 게임 매니저 ]
	gameManager = new GameManager();
	gameManager->Initialize(board);

	execute = true;

	action_time = false;
	cur_score = 0;

	curTime = 0;
	time = 30;

	schedule(schedule_selector(Game::Loop), 0.0f);
	return true;
}

void Game::Loop(float dt)
{

	curTime += dt;
	if (curTime >= 1.0f)
	{
		curTime -= curTime;
		time -= 1;

		if (time <= 0)
		{
			time = 0;
			//execute = false;
			game_time->setString("GAME\nOVER");
		}
		else
		{
			game_time->setString(StringUtils::format("%d", time));
		}
	}

	if (cur_score <= gameManager->getScore())
	{
		if (!action_time)
		{
			action_time = true;
			score->runAction(
				Sequence::create(
				ScaleTo::create(0.3f, 2.5f),
				ScaleTo::create(0.3f, 1.5f),
				CallFunc::create([=](){action_time = false; }), nullptr));
		}

		score->setString(StringUtils::format("%d", cur_score));
		cur_score += 1;
	}

}

bool Game::onTouchBegan(Touch *touch, Event *unused_event){
	touchPosition = touch->getLocation();

	if (!execute)
	{
		if (game_time->getBoundingBox().containsPoint(touchPosition))
		{
			//action_time = false;
			//cur_score = 0;
			//execute = true;
			//curTime = 0;
			//time = 30;
			//score->stopAllActions();
			Director::getInstance()->replaceScene(Game::createScene());
			this->unschedule(schedule_selector(Game::Loop));
		}
	}

	if (execute)
	{
		gameManager->TouchBegan(touch);
	}
	return true;
}

void Game::onTouchMoved(Touch *touch, Event *unused_event){
	if (execute)
		gameManager->TouchMoved(touch);
}

void Game::onTouchEnded(Touch *touch, Event *unused_event){
	if (execute)
		gameManager->TouchEnded(touch);
}