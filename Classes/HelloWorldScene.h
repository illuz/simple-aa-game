#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);



	// Added

	void replayCallback(cocos2d::Ref* pSender);

	void initGame(int _tingNum = 0, float _rotateSpeed = 1, int _targetNum = 5);

	// touch function
	bool onTouchBegan(Touch* touches, Event  *event);

	// draw the tings
	void drawTings(void);

	// schedule callback function
	void scheCallback(float dt);

private:
	// touch position
	Vec2 _beginPos;

	cocos2d::Label *restLabel;
	cocos2d::Label *totalLabel;
	cocos2d::Label *failLabel;
	cocos2d::Label *replayLabel;
	
	DrawNode *drawNode;

	int rotateAngle;
	float rotateSpeed;
	int tingNum;
	int targetNum;

	std::vector<float> tingArc;
	float minArc;

	bool isRun;

	const float PPI = 3.1415 / 180;
	const float LENGTH = 100;
	const float SMALL_CIRCLE_R = 8;
};

#endif // __HELLOWORLD_SCENE_H__
