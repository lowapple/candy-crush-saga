#pragma once

#include "stdafx.h"
#include "Game.h"

/**
@brief    Cocos2d-x 메인이라고 볼 수 있는 AppDelegate
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    어플리케이션을 초기화 할 때 실행되는 구문
    @return true    성공적인 초기화
    @return false   초기화가 정상적으로 안되었을 시 반환
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  어플리케이션 실행 중 나갈 때 사용 구문
    @param  없음
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  어플리케이션 나가고 나서 들어올 때 사용 구문
    @param  없음
    */
    virtual void applicationWillEnterForeground();
};
