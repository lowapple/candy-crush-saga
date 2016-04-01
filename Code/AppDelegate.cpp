#include "stdafx.h"

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    
	//!< Auto는 c++ 11부터 지원하는 기능으로 컴파일러가 뒷 타입을 보고 적절하게 선택하는 메소드
	//!< 여기서는 auto director = Director::getInstance(); 이므로 컴파일러가 Director * director = Director::getInstance()로 인식하게 됨
    auto director = Director::getInstance();	//!< cocos2d-x 전체를 관리하는 Director 클래스 생성
    auto glview = director->getOpenGLView();	//!< openGL 그래픽 라이브러리 선언
    if(!glview) {

		//glview = GLView::create("My Game");											//!< 기본적으로 이렇게 되어 있음 (쓰지않는 것을 권장)
																						//!< 기본 해상도 크기는 960 x 640
																						//!< 화면 해상도 대비 크기는 1배수
																						//!< 디폴트 값은 1
																						//!< 960 , 640을 기본으로 만듬
		

		glview = GLView::createWithRect(D_GAME_NAME , cocos2d::Rect(0,0,WIDTH,HEIGHT) , D_WINDOWS_SHOW);	//!< 우리가 사용해야 할 함수
																						//!< 화면 생성 하면서 크기와 해상도 대비 크기 설정
																						//!< 세번째 인자는 화면 해상도 대비 크기
																						//!< 만약 세번째 인자가 2 이면 화면 해상도는 800 , 480 보이는 화면의 크기는 2배인 1600 , 960이 됨

		director->setOpenGLView(glview);												//!< openGLView를 설정 (그래픽 엔진 설정)
    }
        
	//!< 해상도 안드로이드 , 아이폰 , 윈도우 고정 소스
	//!< 왠만하면 노터치 (D_DESIGN_WIDTH , HEIGHT 는 USER_DEFINE에 위치)
	glview->setDesignResolutionSize(WIDTH , HEIGHT , kResolutionShowAll);

    //!< 화면상에 현재 게임 프레임등 정보를 표시 = true
	//!< 표시 안함 = false
    director->setDisplayStats(false);

    //!< 프레임 설정
	//!< 1초에 60번 루핀
    director->setAnimationInterval(1.0 / 60);

    //!< 초기 씬을 생성
	//!< auto 메소드로 씬도 자동으로 선택 하게 됨.
    auto scene = Game::createScene();

    //!< runWithScene을 통해서 성공적인 설정과 함께 어플리케이션 구동
    director->runWithScene(scene);

    return true;
}

//!< 어플리케이션을 실행 도중 홈키나 홀드화면을 눌렀을 때 게임을 멈춰야 하는 경우에 사용하는 함수
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();	//!< 프레임을 멈춤

    // 만약 여기서 오디오 관련된 효과를 사용할시 밑에 구문을 써주면 됨
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//!< 어플리케이션에 다시 들어올 때
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();	//!< 프레임을 재 시작

    // 만약 여기서 오디오 관련된 효과를 사용할시 밑에 구문을 써주면 됨
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
