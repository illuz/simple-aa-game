#include "HelloWorldScene.h"
#include "math/CCMath.h"
#include <vector>
#include <string>

using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    // add a menu item with "X" image, which is clicked to quit the program
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	// total label
	totalLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	totalLabel->setColor(Color3B(255, 0, 0));
	totalLabel->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.9);
	this->addChild(totalLabel);

	// rest label
	restLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	restLabel->setColor(Color3B(255, 0, 0));
	restLabel->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.8);
	this->addChild(restLabel);

	// add fail label
	failLabel = Label::createWithTTF("Game Over!!", "fonts/Marker Felt.ttf", 30);
	failLabel->setColor(Color3B(255, 0, 0));
	failLabel->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.8);
	this->addChild(failLabel);

	// add replay label
    replayLabel = Label::createWithTTF("Replay", "fonts/Marker Felt.ttf", 30);
	replayLabel->setColor(Color3B(255, 0, 0));
	auto replayItem = MenuItemLabel::create(replayLabel, CC_CALLBACK_1(HelloWorld::replayCallback, this));
	replayItem->setPosition(visibleSize.width * 0.9, visibleSize.height * 0.9);

	auto menu = Menu::create(replayItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// schedule
	schedule(schedule_selector(HelloWorld::scheCallback));

	// create DrawNode and add it to scene
	drawNode = DrawNode::create();
	this->addChild(drawNode);

	// init Game
	initGame();

    return true;
}

void HelloWorld::initGame(int _tingNum, float _rotateSpeed, int _targetNum) {
	rotateSpeed = _rotateSpeed;
	tingNum = _tingNum;
	targetNum = _targetNum;
	rotateAngle = 0;
	isRun = true;
	minArc = 4 * asin(SMALL_CIRCLE_R / 200.0);

	// init tingArc
	tingArc.clear();
	for (int i = 0; i < tingNum; ++i)
		tingArc.push_back(360.0 / tingNum * i * PPI);

	stringstream ss, ss1;
	ss << "Total: " << targetNum;
	totalLabel->setString(ss.str());
	ss1 << "Rest: " << (int)tingArc.size() - tingNum;
	restLabel->setString(ss1.str());
	failLabel->setVisible(false);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::replayCallback(Ref* pSender)
{
	if (isRun) return;
	initGame(tingNum, rotateSpeed, targetNum);
	isRun = true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event  *event)
{
	_beginPos = touch->getLocation();
	tingArc.push_back((180 + 360 - rotateAngle) % 360 * PPI);
	stringstream ss, ss1;
	ss << "Total: " << targetNum;
	totalLabel->setString(ss.str());
	ss1 << "Rest: " << (int)tingArc.size() - tingNum;
	restLabel->setString(ss1.str());
	return true;
}

void HelloWorld::drawTings(void)
{
	if (!isRun) return;

	auto s = Director::getInstance()->getWinSize();
	Vec2 center = Vec2(s.width * 0.5, s.height * 0.6);

	drawNode->clear();

	drawNode->drawSegment(center, center + Vec2(sin(3.14) * 200, cos(3.14) * 200), 1, Color4F(0.5, 1.0, 0.3, 1));

	for (auto &arc : tingArc) {
		// draw a line
		drawNode->drawSegment(center, center + Vec2(sin(rotateAngle * PPI + arc) * LENGTH, cos(rotateAngle * PPI + arc) * LENGTH), 1, Color4F(1.0, 1.0, 1.0, 1));
		drawNode->drawDot(center + Vec2(sin(rotateAngle * PPI + arc) * LENGTH, cos(rotateAngle * PPI + arc) * LENGTH), SMALL_CIRCLE_R, Color4F(1, 1, 1, 1));
	}

	// draw center circle
	drawNode->drawDot(center, 40, Color4F(1, 1, 1, 1));

	// sort and check conflict
	if (tingArc.size() <= 1) return;

	sort(tingArc.begin(), tingArc.end());

	for (int i = 0; i < tingArc.size(); ++i) {
		int next = (i + 1) % tingArc.size();

		if (abs(tingArc[i] - tingArc[next]) < minArc) {
			// exist conflict

			isRun = false;

			failLabel->setVisible(true);

			return;
		}
	}

	// check if win
	if (tingArc.size() == targetNum + tingNum ) {
		initGame(tingNum + 1, rotateSpeed + 0.5, targetNum + 1);
	}
}

void HelloWorld::scheCallback(float dt)
{
	if (!isRun) return;
	rotateAngle += rotateSpeed;
	if (rotateAngle > 360)
		rotateAngle %= 360;
	drawTings();
}
