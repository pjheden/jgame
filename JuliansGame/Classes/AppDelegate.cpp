#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	Size visibleSize = director->getVisibleSize();
    glview = GLViewImpl::create("My Game");
    director->setOpenGLView(glview);
   
	// turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	auto fileUtils = FileUtils::getInstance( );
    auto screenSize = glview->getFrameSize( );
    std::vector<std::string> resDirOrders;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // check which assets the devices requires
    if ( 2048 <= screenSize.width || 2048 <= screenSize.height ) // retina iPad
    {
        resDirOrders.push_back( "ipadhd" );
        resDirOrders.push_back( "ipad" );
        resDirOrders.push_back( "iphonehd5" );
        resDirOrders.push_back( "iphonehd" );
        resDirOrders.push_back( "iphone" );
        
        glview->setDesignResolutionSize( 2048, 1536, ResolutionPolicy::NO_BORDER );

    }
    else if ( 1024 <= screenSize.width || 1024 <= screenSize.height ) // non retina iPad
    {
        resDirOrders.push_back( "ipad" );
        resDirOrders.push_back( "iphonehd5" );
        resDirOrders.push_back( "iphonehd" );
        resDirOrders.push_back( "iphone" );
        
        glview->setDesignResolutionSize( 1024, 768, ResolutionPolicy::NO_BORDER );
    }
    else if ( 1136 <= screenSize.width || 1136 <= screenSize.height ) // retina iPhone (5 and 5S)
    {
        resDirOrders.push_back("iphonehd5");
        resDirOrders.push_back("iphonehd");
        resDirOrders.push_back("iphone");
        
        glview->setDesignResolutionSize( 1136, 640, ResolutionPolicy::NO_BORDER );
    }
    else if ( 960 <= screenSize.width || 960 <= screenSize.height ) // retina iPhone (4 and 4S)
    {
        resDirOrders.push_back( "iphonehd" );
        resDirOrders.push_back( "iphone" );
        
        glview->setDesignResolutionSize( 960, 640, ResolutionPolicy::NO_BORDER );
    }

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    resDirOrders.push_back( "ipad" );
    resDirOrders.push_back( "iphonehd5" );
    resDirOrders.push_back( "iphonehd" );
    resDirOrders.push_back( "iphone" );
    
    glview->setFrameSize( 1024, 768 );
    glview->setDesignResolutionSize( 1024, 768, ResolutionPolicy::NO_BORDER );

#endif
    
    fileUtils->setSearchPaths(resDirOrders);

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = MainMenu::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
