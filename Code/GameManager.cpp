#include "GameManager.h"
#include "Candy.h"

void GameManager::Initialize(Node* _ownerLayer)
{
	srand((unsigned int)time(NULL));
	ownerLayer = _ownerLayer;
	vertical_boom = false;
	horizontal_boom = false;
	pluse_boom = false;
	no_three = false;
	vAllCandy.clear();
	vCandy.clear();
	dropCandy = false;
	first_execute = false;
	three_drop = false;
	is_swap = false;
	boomCandy = false;
	is_swap_return = false;
	score = 0;
	touch_begine = Vec2::ZERO;
	touch_end = Vec2::ZERO;

	this->ClearBoard();
}

Candy* GameManager::CreateCandy(const Vec2& arr_position)
{
	Candy* candy = Candy::create(RandomCandy(rand() % 6 + 1));
	candy->setContentSize(candy->getContentSize() * 0.925);
	candy_size = candy->getContentSize().width;
	candy->setAnchorPoint(Vec2::ZERO);
	candy->setPosition(ArrayToMap(arr_position));
	candy->setArrayPosition(Vec2(arr_position));
	candy->setTag(0);
	ownerLayer->addChild(candy, Candy_tag);

	vAllCandy.push_back(candy);

	return candy;
}

CandyFlavor GameManager::RandomCandy(int n)
{
	switch (n) {
	case 1:
		return CandyFlavor::A;
		break;
	case 2:
		return CandyFlavor::B;
		break;
	case 3:
		return CandyFlavor::C;
		break;
	case 4:
		return CandyFlavor::D;
		break;
	case 5:
		return CandyFlavor::E;
		break;
	case 6:
		return CandyFlavor::F;
		break;
	}
}

Vec2 GameManager::MapToArray(const Vec2& position)
{
	int xpos = position.x / candy_size;
	int ypos = position.y / candy_size;
	return Vec2(xpos, ypos);
}

Vec2 GameManager::ArrayToMap(const Vec2& position)
{
	int xpos = position.x * candy_size;
	int ypos = position.y * candy_size;
	return Vec2(xpos, ypos);
}

void GameManager::ClearBoard()
{
	first_execute = true;
	// Draw.
	for (int i = 0; i < CANDY_HEIGHT; i++){
		for (int j = 0; j < CANDY_WIDTH; j++)
		{
			Candy* newCandy = CreateCandy(Vec2(j, i));
			candyBoard[i][j] = newCandy;
		}
	}

	// --- [ 검사 ]
	for (int i = 0; i < CANDY_HEIGHT; i++){
		for (int j = 0; j < CANDY_WIDTH; j++){
			std::vector<Vec2> dir = { Vec2(1, 0), Vec2(0, 1) };

			int x = candyBoard[i][j]->getArrayPosition().x;
			int y = candyBoard[i][j]->getArrayPosition().y;

			Candy* curCandy = candyBoard[y][x];
			for (auto it : dir){
				while ((CheckCandy(x, y, it) + CheckCandy(x, y, it * -1) + 1) >= 3)
				{
					if (curCandy == candyBoard[y][x]){
						vAllCandy.erase(std::find(vAllCandy.begin(), vAllCandy.end(), candyBoard[y][x]));
						ownerLayer->removeChild(candyBoard[y][x], true);

						candyBoard[y][x] = nullptr;

						Candy* newCandy = CreateCandy(Vec2(x, y));
						candyBoard[y][x] = newCandy;
					}
				}
			}
		}
	}
	first_execute = false;
}

// --------------------------------------- [ 터치 함수 ]

void GameManager::TouchBegan(Touch* _touch)
{
	if (!boomCandy){
		if (!dropCandy){
			if (!pluse_boom){
				if (!is_swap){
					//if (!three_drop){

						no_three = false;

						touch_begine = _touch->getLocation();
						touch_end = _touch->getLocation();

						// 터치를 활성화 한다.
						is_begine_touch_enable = true;

						Vec2 temp = MapToArray(touch_begine);
						if (candyBoard[(int)temp.y][(int)temp.x]->candyFlavor == CandyFlavor::BOOM)
						{
							vBoomCandy.clear();

							// log("BOOM");
							PluseBoom(temp.x, temp.y, Vec2(1, 0));
							PluseBoom(temp.x, temp.y, Vec2(1, 0)* -1);
							PluseBoom(temp.x, temp.y, Vec2(0, 1));
							PluseBoom(temp.x, temp.y, Vec2(0, 1)*-1);

							boomCandy = true;
							dropCandy = true;
							pluse_boom = true;
							is_swap = true;

							vBoomCandy.push_back(candyBoard[(int)temp.y][(int)temp.x]);
							// --------------------------------------

							int ARR_Y[CANDY_WIDTH] = { 7, 7, 7, 7, 7, 7, 7 }; //기본적으로 생성될 위치
							for (auto it : vBoomCandy)
							{
								three_drop = true;

								// 해당 블럭은 지운다.
								int arr_x = (int)it->getArrayPosition().x;

								ARR_Y[arr_x] += 1;
								Candy* newCandy = CreateCandy(Vec2(arr_x, ARR_Y[arr_x] - 1));
							}

							for (Candy* bCandy : vBoomCandy) {
								for (Candy* aCandy : vAllCandy) {
									if ((aCandy->getArrayPosition().x == bCandy->getArrayPosition().x) &&
										(aCandy->getArrayPosition().y > bCandy->getArrayPosition().y)){
										// --------------------------------
										aCandy->setTag(aCandy->getTag() + 1);
									}
								}
							}

							for (Candy* bCandy : vBoomCandy){
								vAllCandy.erase(std::find(vAllCandy.begin(), vAllCandy.end(), bCandy));
								ownerLayer->removeChild(bCandy, true);
								score += 5;
							}

							int max_num = 1;

							// -- [ 실행 ]
							for (Candy* candy : vAllCandy)
							{
								// ------------------------- [ 내려오는 값중 가장 큰 값을 체크한다. ]
								if (candy->getTag() >= max_num)
									max_num = candy->getTag();

								if (candy)
								{
									candy->runAction(MoveBy::create(0.2f * candy->getTag(), Vec2(0, -1 * (candy->getTag()) * candy_size)));
									candy->setArrayPosition(Vec2(candy->getArrayPosition() - Vec2(0, candy->getTag())));
									candyBoard[(int)candy->getArrayPosition().y][(int)candy->getArrayPosition().x] = candy;
									candy->setTag(0);
								}
							}

							vertical_boom = false;
							horizontal_boom = false;

							ownerLayer->runAction(
								Sequence::create(
								DelayTime::create(0.2f * max_num),
								CallFuncN::create(
								[=](Node* candy)
							{
								boomCandy = false;
								dropCandy = false;
								pluse_boom = false;
								is_swap = false;

								DropAndCheck();
							}), nullptr));
						}
					//}
				}
			}
		}
	}
}

void GameManager::TouchMoved(Touch* _touch){
	if (!boomCandy){
		if (is_begine_touch_enable){
			if (!dropCandy){
				if (!pluse_boom){
					if (!is_swap)
					{
						touch_end = _touch->getLocation();
					}
				}
			}
		}
	}
}

void GameManager::TouchEnded(Touch* _touch)
{
	if (is_begine_touch_enable){
		if (!boomCandy){
			if (!dropCandy){
				if (!pluse_boom){
					if (!is_swap){
						vBoomCandy.clear();
						Vec2 temp = touch_end - touch_begine;
						//if (!is_touch_swap){
						// ---------------- [ 방향값을 저장한다 ]
						Vec2 arr_dir;
						if (temp.y >= 70.0f)
							arr_dir = Vec2(0, 1);
						else if (temp.y <= -70.0f)
							arr_dir = Vec2(0, -1);
						else if (temp.x >= 70.0f)
							arr_dir = Vec2(1, 0);
						else if (temp.x <= -70.0f)
							arr_dir = Vec2(-1, 0);

						// ---------------- [ 바꿀놈과 Swap ]
						Vec2 sel = MapToArray(touch_begine);
						Candy* selected = candyBoard[(int)sel.y][(int)sel.x]; // -- 선택 캔디

						Vec2 tar = sel + arr_dir;
						Candy* target = candyBoard[(int)tar.y][(int)tar.x];	  // -- 타겟 캔디

						if (sel.x < 0 || sel.x >= CANDY_WIDTH || sel.y < 0 || sel.y >= CANDY_HEIGHT)
							return;
						if (tar.x < 0 || tar.x >= CANDY_WIDTH || tar.y < 0 || tar.y >= CANDY_HEIGHT)
							return;

						if (!selected || !target)
							return;

						// 체인지
						SwapCandy(selected, target);
					}
				}
			}
		}
	}
}

// -------------------------------------- [ Fuction ]

void GameManager::SwapCandy(Candy* target1, Candy* target2)
{
	if (!boomCandy){
		if (!pluse_boom){
			if (!dropCandy){
				if (!is_swap){

					if (!target1 || !target2)
						return;

					if (target1)
						target1->stopAllActions();
					if (target2)
						target2->stopAllActions();

					Vec2 target1_position = ArrayToMap(target1->getArrayPosition());
					Vec2 target2_position = ArrayToMap(target2->getArrayPosition());

					Vec2 arr_target1_position = target1->getArrayPosition();
					Vec2 arr_target2_position = target2->getArrayPosition();

					auto action1 = MoveTo::create(0.2f, target2_position);
					auto action2 = MoveTo::create(0.2f, target1_position);

					//if (is_touch_swap){
					// Candy의 포지션을 바꿈
					target1->runAction(Sequence::create(action1->clone(), CallFunc::create([=](){}), nullptr));
					target2->runAction(Sequence::create(action2->clone(), CallFunc::create([=](){}), nullptr));

					// Candy의 배열 값을 바꿈
					target1->setArrayPosition(arr_target2_position);
					target2->setArrayPosition(arr_target1_position);

					// 배열의 Candy값을 바꿈
					candyBoard[(int)arr_target1_position.y][(int)arr_target1_position.x] = target2;
					candyBoard[(int)arr_target2_position.y][(int)arr_target2_position.x] = target1;

					// ------------ [ 상하좌우로 같은게 있지 않으면 ]
					if (!is_swap){
						is_swap = true;
						ownerLayer->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([=]()
						{
							if (is_swap)
							{
								is_swap = false;
								if (!boomCandy)
								{
									if (!dropCandy)
									{
										if (is_swap_return)
										{
											is_swap = false;
											vBoomCandy.clear();
											is_swap_return = false;
											is_begine_touch_enable = false;
										}
										else
										{
											vCandy.clear();
											vBoomCandy.clear();
											if (SwapCandyCheck(target1) || SwapCandyCheck(target2))
											{
												is_swap = false;
												is_begine_touch_enable = true;
												three_drop = false;

												touch_begine = Vec2::ZERO - Vec2(100, 100);
												touch_end = Vec2::ZERO - Vec2(100, 100);

												BoomBoomBoom(target1, target2);
											}
											else if (!no_three)
											{
												if (!boomCandy)
												{
													if (!dropCandy)
													{
														if (!is_swap_return)
															is_swap_return = true;
														is_swap = false;
														no_three = true;
														target1->stopAllActions();
														target2->stopAllActions();
														SwapCandy(target1, target2);
													}
												}
											}
										}
									}
								}
							}
						}), nullptr));
					}
				}
			}
		}
	}
}

int GameManager::CheckCandy(int arr_x, int arr_y, const Vec2& dir)
{
	int arr_next_x = arr_x + dir.x;
	int arr_next_y = arr_y + dir.y;

	if (arr_next_x < 0 || arr_next_x >= CANDY_WIDTH || arr_next_y < 0 || arr_next_y >= CANDY_WIDTH)
		return 0;
	if (candyBoard[arr_y][arr_x] == nullptr || candyBoard[arr_next_y][arr_next_x] == nullptr)
		return 0;
	if (candyBoard[arr_y][arr_x]->candyFlavor != candyBoard[arr_next_y][arr_next_x]->candyFlavor)
		return 0;

	if (!first_execute){
		vCandy.push_back(candyBoard[arr_next_y][arr_next_x]);
	}
	return 1 + CheckCandy(arr_next_x, arr_next_y, dir);
}

int GameManager::BoomCheck(int arr_x, int arr_y, const Vec2& dir)
{
	int arr_next_x = arr_x + dir.x;
	int arr_next_y = arr_y + dir.y;

	if (arr_next_x < 0 || arr_next_x >= CANDY_WIDTH || arr_next_y < 0 || arr_next_y >= CANDY_WIDTH)
		return 0;
	if (candyBoard[arr_y][arr_x] == nullptr || candyBoard[arr_next_y][arr_next_x] == nullptr)
		return 0;
	if (candyBoard[arr_y][arr_x]->candyFlavor != candyBoard[arr_next_y][arr_next_x]->candyFlavor)
		return 0;

	//if (!first_execute){
	//	vCandy.push_back(candyBoard[arr_next_y][arr_next_x]);
	//}

	return 1 + CheckCandy(arr_next_x, arr_next_y, dir);
}

int GameManager::PluseBoom(int arr_x, int arr_y, const Vec2& dir)
{
	int arr_next_x = arr_x + dir.x;
	int arr_next_y = arr_y + dir.y;

	if (arr_next_x < 0 || arr_next_x >= CANDY_WIDTH || arr_next_y < 0 || arr_next_y >= CANDY_WIDTH)
		return 0;
	if (candyBoard[arr_y][arr_x] == nullptr || candyBoard[arr_next_y][arr_next_x] == nullptr)
		return 0;

	auto boomIter = std::find_if(vBoomCandy.begin(), vBoomCandy.end(), [=](Candy* candy)->bool
	{
		if (candyBoard[arr_next_y][arr_next_x]->getArrayPosition() == candy->getArrayPosition())
			return true;
		else
			return false;
	});

	if (boomIter != vBoomCandy.end())
		log("overlap");
	else
		vBoomCandy.push_back(candyBoard[arr_next_y][arr_next_x]);


	return 1 + PluseBoom(arr_next_x, arr_next_y, dir);
}

bool GameManager::SwapCandyCheck(Candy* candy)
{
	int arr_x = candy->getArrayPosition().x;
	int arr_y = candy->getArrayPosition().y;

	int vertical = 1;
	int horizontal = 1;
	bool debug = false; // 중복 값 방지

	// -------------------------------------------- [ Vertical ]
	vCandy.clear();
	vertical += CheckCandy(arr_x, arr_y, Vec2(0, 1));
	vertical += CheckCandy(arr_x, arr_y, Vec2(0, 1)*-1);
	//log("V : %d", vertical);

	if (vertical > 3){
		for (auto it : vCandy){
			auto boomIter = std::find_if(vBoomCandy.begin(), vBoomCandy.end(), [=](Candy* candy)->bool{
				if (it->getArrayPosition() == candy->getArrayPosition())return true; else return false; });
				if (boomIter != vBoomCandy.end())
					debug = true;
				else{
					no_three = true;
					vBoomCandy.push_back(it);
				}
		}
		vertical_boom = true;
		vCandy.clear();
	}
	else if (vertical == 3){
		for (auto it : vCandy){
			auto boomIter = std::find_if(vBoomCandy.begin(), vBoomCandy.end(), [=](Candy* candy)->bool{
				if (it->getArrayPosition() == candy->getArrayPosition())return true; else return false; });
				if (boomIter != vBoomCandy.end())
					debug = true;
				else{
					no_three = true;
					vBoomCandy.push_back(it);
				}
		}
		vCandy.clear();
	}

	// -------------------------------------------- [ Horizontal ]
	vCandy.clear();
	horizontal += CheckCandy(arr_x, arr_y, Vec2(1, 0));
	horizontal += CheckCandy(arr_x, arr_y, Vec2(1, 0)*-1);
	//log("H : %d", horizontal);

	if (horizontal > 3){
		for (auto it : vCandy){
			auto boomIter = std::find_if(vBoomCandy.begin(), vBoomCandy.end(), [=](Candy* candy)->bool{
				if (it->getArrayPosition() == candy->getArrayPosition())return true; else return false; });
				if (boomIter != vBoomCandy.end())
					debug = true;
				else{
					no_three = true;
					vBoomCandy.push_back(it);
				}
		}
		vCandy.clear();
		horizontal_boom = true;
	}
	else if (horizontal == 3){
		for (auto it : vCandy){
			auto boomIter = std::find_if(vBoomCandy.begin(), vBoomCandy.end(), [=](Candy* candy)->bool{
				if (it->getArrayPosition() == candy->getArrayPosition())return true; else return false; });
				if (boomIter != vBoomCandy.end())
					debug = true;
				else{
					no_three = true;
					vBoomCandy.push_back(it);
				}
		}
		vCandy.clear();
	}

	// ----------------


	if (vertical >= 3 || horizontal >= 3)
	{
		if (!debug)
		{
			vBoomCandy.push_back(candy);
		}
		return true;
	}
	return false;
}

void GameManager::DropAndCheck()
{
	if (!is_swap){
		if (!boomCandy){
			if (!dropCandy){
				if (!pluse_boom){
					//if (!three_drop){
					// ------------------- [ 전체 검사 후 vBoom에 넣는다 ]
					vBoomCandy.clear();
					std::vector<Vec2> dir = { Vec2(1, 0), Vec2(0, 1) };
					for (auto aV : vAllCandy)
					{
						SwapCandyCheck(aV);
					}

					if (vBoomCandy.size() >= 3){
						// ------------------- [ 검색후 삭제 ]
						auto vIter = vBoomCandy.begin();
						for (; vIter != vBoomCandy.end();)
						{
							int x = (*vIter)->getArrayPosition().x;
							int y = (*vIter)->getArrayPosition().y;

							if (vertical_boom || horizontal_boom)
							{
								if (vertical_boom){
									if (BoomCheck((*vIter)->getArrayPosition().x, (*vIter)->getArrayPosition().x, Vec2(0, 1)) +
										BoomCheck((*vIter)->getArrayPosition().x, (*vIter)->getArrayPosition().y, Vec2(0, 1) * -1) + 1 >= 3)
									{
										(*vIter)->setTexture("BOB.png");
										(*vIter)->candyFlavor = CandyFlavor::BOOM;
										vIter = vBoomCandy.erase(vIter);
										break;
									}
								}
								else if (horizontal_boom)
								{
									if (BoomCheck((*vIter)->getArrayPosition().x, (*vIter)->getArrayPosition().x, Vec2(1, 0)) +
										BoomCheck((*vIter)->getArrayPosition().x, (*vIter)->getArrayPosition().y, Vec2(1, 0) * -1) + 1 >= 3)
									{
										(*vIter)->setTexture("BOB.png");
										(*vIter)->candyFlavor = CandyFlavor::BOOM;
										vIter = vBoomCandy.erase(vIter);
										break;
									}
								}

							}
							vIter++;
						}
					}

					int ARR_Y[CANDY_WIDTH] = { 7, 7, 7, 7, 7, 7, 7 }; //기본적으로 생성될 위치
					for (auto it : vBoomCandy)
					{
						three_drop = true;

						// 해당 블럭은 지운다.
						int arr_x = (int)it->getArrayPosition().x;
						int arr_y = (int)it->getArrayPosition().y;


						ARR_Y[arr_x] += 1;
						Candy* newCandy = CreateCandy(Vec2(arr_x, ARR_Y[arr_x] - 1));
					}

					for (Candy* bCandy : vBoomCandy) {
						for (Candy* aCandy : vAllCandy) {
							if ((aCandy->getArrayPosition().x == bCandy->getArrayPosition().x) &&
								(aCandy->getArrayPosition().y > bCandy->getArrayPosition().y)){
								// --------------------------------
								aCandy->setTag(aCandy->getTag() + 1);
							}
						}
					}

					for (Candy* bCandy : vBoomCandy){
						vAllCandy.erase(std::find(vAllCandy.begin(), vAllCandy.end(), bCandy));
						ownerLayer->removeChild(bCandy, true);
						score += 5;
					}

					int max_num = 0;
					// -- [ 실행 ]
					for (Candy* candy : vAllCandy){
						// ------------------------- [ 내려오는 값중 가장 큰 값을 체크한다. ]
						if (candy->getTag() >= max_num)
							max_num = candy->getTag();

						if (candy)
						{
							candy->runAction(MoveBy::create(0.2f * candy->getTag(), Vec2(0, -1 * (candy->getTag()) * candy_size)));
							candy->setArrayPosition(Vec2(candy->getArrayPosition() - Vec2(0, candy->getTag())));
							candyBoard[(int)candy->getArrayPosition().y][(int)candy->getArrayPosition().x] = candy;
							candy->setTag(0);
						}
					}
					if (max_num == 0)
						max_num += 1;

					vertical_boom = false;
					horizontal_boom = false;

					ownerLayer->runAction(Sequence::create(DelayTime::create(0.2f * max_num),
						CallFuncN::create([=](Node* candy)
					{
						dropCandy = false;
						three_drop = false;

						DropAndCheck();

					}), nullptr));

					//auto vBoomIter = vBoomCandy.begin();
					//	for (auto v : dir)
					//	{
					//		vertical_boom = false;
					//		horizontal_boom = false;
					//
					//		vBoomCandy.clear();
					//		if ((CheckCandy(x, y, v) + CheckCandy(x, y, v*-1) + 1) >= 3)
					//		{
					//			dropCandy = true;
					//			SwapCandyCheck((*aIter));
					//
					//			bool first_erase = false;
					//			// -- [ 생성 ]
					//			int ARR_Y[CANDY_WIDTH] = { 7, 7, 7, 7, 7, 7, 7 }; //기본적으로 생성될 위치
					//			auto vBoomIter = vBoomCandy.begin();
					//
					//			for (auto it : vBoomCandy)
					//			{
					//				if (vertical_boom || horizontal_boom)
					//				{
					//					if (vertical_boom){
					//						if (BoomCheck(it->getArrayPosition().x, it->getArrayPosition().x, Vec2(0, 1)) +
					//							BoomCheck(it->getArrayPosition().x, it->getArrayPosition().y, Vec2(0, 1) * -1) + 1 >= 3)
					//						{
					//							it->setTexture("BOB.png");
					//							it->candyFlavor = CandyFlavor::BOOM;
					//							vBoomIter = vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), it));
					//							first_erase = true;
					//							break;
					//						}
					//					}
					//					else if (horizontal_boom)
					//					{
					//						if (BoomCheck(it->getArrayPosition().x, it->getArrayPosition().x, Vec2(1, 0)) +
					//							BoomCheck(it->getArrayPosition().x, it->getArrayPosition().y, Vec2(1, 0) * -1) + 1 >= 3)
					//						{
					//							it->setTexture("BOB.png");
					//							it->candyFlavor = CandyFlavor::BOOM;
					//							vBoomIter = vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), it));
					//							first_erase = true;
					//							break;
					//						}
					//					}
					//
					//				}
					//			}
					//
					//
					//			vBoomIter = vBoomCandy.begin();
					//			for (; vBoomIter != vBoomCandy.end();)
					//			{
					//				// 해당 블럭은 지운다.
					//				int arr_x = (int)(*vBoomIter)->getArrayPosition().x;
					//				int arr_y = (int)(*vBoomIter)->getArrayPosition().y;
					//
					//
					//				ARR_Y[arr_x] += 1;
					//				Candy* newCandy = CreateCandy(Vec2(arr_x, ARR_Y[arr_x] - 1));
					//				vBoomIter++;
					//			}
					//
					//			for (Candy* bCandy : vBoomCandy) {
					//				for (Candy* aCandy : vAllCandy) {
					//					if ((aCandy->getArrayPosition().x == bCandy->getArrayPosition().x) &&
					//						(aCandy->getArrayPosition().y > bCandy->getArrayPosition().y)){
					//						// --------------------------------
					//						aCandy->setTag(aCandy->getTag() + 1);
					//					}
					//				}
					//			}
					//
					//			for (Candy* bCandy : vBoomCandy){
					//				if ((*aIter))
					//				{
					//					aIter = vAllCandy.erase(std::find(vAllCandy.begin(), vAllCandy.end(), bCandy));
					//					ownerLayer->removeChild(bCandy, true);
					//					score += 5;
					//				}
					//			}
					//
					//			int max_num = 0;
					//			// -- [ 실행 ]
					//			for (Candy* candy : vAllCandy){
					//				// ------------------------- [ 내려오는 값중 가장 큰 값을 체크한다. ]
					//				if (candy->getTag() >= max_num)
					//					max_num = candy->getTag();
					//
					//				if (candy)
					//				{
					//					candy->runAction(MoveBy::create(0.2f * candy->getTag(), Vec2(0, -1 * (candy->getTag()) * candy_size)));
					//					candy->setArrayPosition(Vec2(candy->getArrayPosition() - Vec2(0, candy->getTag())));
					//					candyBoard[(int)candy->getArrayPosition().y][(int)candy->getArrayPosition().x] = candy;
					//					candy->setTag(0);
					//				}
					//			}
					//			if (max_num != 0)
					//				max_num += 1;
					//
					//			ownerLayer->runAction(Sequence::create(DelayTime::create(0.2f * max_num),
					//				CallFuncN::create([=](Node* candy)
					//			{
					//				dropCandy = false;
					//				if (!no_three)
					//				{
					//					if (!pluse_boom)
					//					{
					//
					//						vertical_boom = false;
					//						horizontal_boom = false;
					//
					//						DropAndCheck();
					//					}
					//				}
					//
					//			}), nullptr));
					//		}
					//	}
					//	aIter++;
					//}
				}
				//}
			}
		}
	}
}

void GameManager::BoomBoomBoom(Candy* target1, Candy* target2)
{
	if (!is_swap){
		if (!boomCandy){
			// ---------------------------------------------- [ 4방향 검색 ]
			if (!dropCandy){
				if (!three_drop){
					if (!pluse_boom){
						vBoomCandy.clear();
						//SwapCandyCheck(selected); // 선택 캔디 검사
						//SwapCandyCheck(target);   // 타겟 캔디 검사

						vertical_boom = false;
						horizontal_boom = false;

						if (SwapCandyCheck(target1) || SwapCandyCheck(target2))
						{
							three_drop = true;
							boomCandy = true;
						}

						//if (vertical_boom || horizontal_boom)
						//{
						if (horizontal_boom)
						{
							int target1_num = BoomCheck(target1->getArrayPosition().x, target1->getArrayPosition().y, Vec2(1, 0)) +
								BoomCheck(target1->getArrayPosition().x, target1->getArrayPosition().y, Vec2(1, 0) * -1) + 1;

							int target2_num = BoomCheck(target2->getArrayPosition().x, target2->getArrayPosition().y, Vec2(1, 0)) +
								BoomCheck(target2->getArrayPosition().x, target2->getArrayPosition().y, Vec2(1, 0) * -1) + 1;

							if (target1_num > 3)
							{
								target1->setTexture("BOB.png");
								target1->candyFlavor = CandyFlavor::BOOM;
								vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), target1));
							}
							else if (target2_num > 3)
							{
								target2->setTexture("BOB.png");
								target2->candyFlavor = CandyFlavor::BOOM;
								vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), target2));
							}
						}
						else if (vertical_boom)
						{
							int target1_num = BoomCheck(target1->getArrayPosition().x, target1->getArrayPosition().y, Vec2(0, 1)) +
								BoomCheck(target1->getArrayPosition().x, target1->getArrayPosition().y, Vec2(0, 1) * -1) + 1;

							int target2_num = BoomCheck(target2->getArrayPosition().x, target2->getArrayPosition().y, Vec2(0, 1)) +
								BoomCheck(target2->getArrayPosition().x, target2->getArrayPosition().y, Vec2(0, 1) * -1) + 1;

							if (target1_num > 3)
							{
								target1->setTexture("BOB.png");
								target1->candyFlavor = CandyFlavor::BOOM;
								vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), target1));
							}

							if (target2_num > 3)
							{
								target2->setTexture("BOB.png");
								target2->candyFlavor = CandyFlavor::BOOM;
								vBoomCandy.erase(std::find(vBoomCandy.begin(), vBoomCandy.end(), target2));
							}
						}

						//}
						// ---------------------------------------------------[ 터치 ]
						if (vBoomCandy.size()){
							// -- [ 생성 ]
							int ARR_Y[CANDY_WIDTH] = { 7, 7, 7, 7, 7, 7, 7 }; //기본적으로 생성될 위치
							auto vBoomIter = vBoomCandy.begin();

							vBoomIter = vBoomCandy.begin();
							for (; vBoomIter != vBoomCandy.end();)
							{
								// 해당 블럭은 지운다.
								int arr_x = (int)(*vBoomIter)->getArrayPosition().x;
								int arr_y = (int)(*vBoomIter)->getArrayPosition().y;

								ARR_Y[arr_x] += 1;
								Candy* newCandy = CreateCandy(Vec2(arr_x, ARR_Y[arr_x] - 1));
								vBoomIter++;
							}

							// -- [ 계산 ]
							// Y의 값을 계산한다. -> 얼마나 내려오는가
							// 각각의 오브젝트의 Tag에 저장한다.
							// 기본 Tag의 값은 0
							for (Candy* bCandy : vBoomCandy) {
								for (Candy* aCandy : vAllCandy) {
									if ((aCandy->getArrayPosition().x == bCandy->getArrayPosition().x) &&
										(aCandy->getArrayPosition().y > bCandy->getArrayPosition().y)){
										// --------------------------------
										aCandy->setTag(aCandy->getTag() + 1);
										if (vertical_boom || horizontal_boom) {
											//if (aCandy != boomberCandy)
											aCandy->setTag(aCandy->getTag());
										}
									}
								}
							}

							ownerLayer->runAction(Sequence::create(DelayTime::create(0.2),
								CallFunc::create([=]() {

								for (Candy* bCandy : vBoomCandy){
									if (bCandy){
										vAllCandy.erase(std::find(vAllCandy.begin(), vAllCandy.end(), bCandy));
										ownerLayer->removeChild(bCandy, true);
										score += 8;
									}
								}

								int max_num = 0;
								// -- [ 실행 ]
								for (Candy* candy : vAllCandy) {
									// ------------------------- [ 내려오는 값중 가장 큰 값을 체크한다. ]
									if (candy->getTag() >= max_num)
										max_num = candy->getTag();

									candy->runAction(MoveBy::create(0.2f*candy->getTag(), Vec2(0, -1 * (candy->getTag()) * candy_size)));
									candy->setArrayPosition(Vec2(candy->getArrayPosition() - Vec2(0, candy->getTag())));
									candyBoard[(int)candy->getArrayPosition().y][(int)candy->getArrayPosition().x] = candy;
									candy->setTag(0);
								}

								if (max_num == 0)
									max_num += 1;

								vertical_boom = false;
								horizontal_boom = false;

								ownerLayer->runAction(Sequence::create(DelayTime::create(0.2f * max_num),
									CallFuncN::create([=](Node* candy)
								{
									three_drop = false;
									boomCandy = false;
									DropAndCheck();
								}), nullptr));

							}), nullptr));
						}
					}
				}
			}
		}
	}
}