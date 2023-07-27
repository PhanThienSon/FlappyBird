#include "HelloWorldScene.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);

	// return the scene
	return scene;
 
}

bool HelloWorld::init()
{

	if (!Layer::init())
	{
		return false;
	}
		
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 5);
	edgeBody->setCollisionBitmask(2);
	edgeBody->setContactTestBitmask(true);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);

	bird = Sprite::create("bird.png");
	bird->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	auto birdPhysic = PhysicsBody::createBox(bird->getContentSize());
	birdPhysic->setContactTestBitmask(true);
	birdPhysic->setCollisionBitmask(4);
	bird->setPhysicsBody(birdPhysic);
	addChild(bird);
	
	__String *tempScore = __String::createWithFormat("%i", score);
	label = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height*0.1);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.8));
	label->setColor(Color3B::RED);
	addChild(label);

	this->scheduleUpdate();
	this->schedule(schedule_selector(HelloWorld::CreatePipe), 0.005*visibleSize.width);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::OnTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::OnContactBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool HelloWorld::OnContactBegan(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 3 ||
		a->getCollisionBitmask() == 3 && b->getCollisionBitmask() == 4)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Point.mp3");
		++score;
		__String *tempScore = __String::createWithFormat("%i", score);
		label->setString(tempScore->getCString());
	}
	else
	{
		if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 2 ||
			a->getCollisionBitmask() == 2 && b->getCollisionBitmask() == 4)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Hit.mp3");
			auto sqe = GameOver::createScene();
			Director::getInstance()->replaceScene(sqe);
		}
	}
	return true;
}

bool HelloWorld::OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	isFalling = false;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Wing.mp3");
	this->scheduleOnce(schedule_selector(HelloWorld::stopFly), 0.25);
	return true;
}

void HelloWorld::stopFly(float dt)
{
	isFalling = true;
}

void HelloWorld::update(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (isFalling == true)
	{
		bird->setPositionX(visibleSize.width / 2);
		bird->setPositionY(bird->getPositionY() - visibleSize.height * 0.0025);
	}
	else
	{
		bird->setPositionX(visibleSize.width / 2);
		bird->setPositionY(bird->getPositionY() + visibleSize.height * 0.0025);
	}
}
void HelloWorld::CreatePipe(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto topPipe = Sprite::create("bottom_bar.png");
	auto bottomPipe = Sprite::create("up_bar.png");

	auto topPhysic = PhysicsBody::createBox(topPipe->getContentSize());
	topPhysic->setCollisionBitmask(2);
	topPhysic->setContactTestBitmask(true);
	topPipe->setPhysicsBody(topPhysic);

	auto bottomPhysic = PhysicsBody::createBox(bottomPipe->getContentSize());
	bottomPhysic->setCollisionBitmask(2);
	bottomPhysic->setContactTestBitmask(true);
	bottomPipe->setPhysicsBody(bottomPhysic);

	topPhysic->setDynamic(false);
	bottomPhysic->setDynamic(false);

	auto random = CCRANDOM_0_1();
	if (random < 0.35)
	{
		random = 0.35;
	}
	else
	{
		if (random > 0.65)
		{
			random = 0.65;
		}
	}

	auto topPipePosition = visibleSize.height * random + topPipe->getContentSize().height / 2;
	topPipe->setPosition(Vec2(visibleSize.width + topPipe->getContentSize().width, topPipePosition));// set vi tri topPipe

	bottomPipe->setPosition(Vec2(topPipe->getPositionX(), topPipePosition -
		((Sprite::create("bird.png")->getContentSize().height * 2)) - topPipe->getContentSize().height));

	addChild(topPipe);
	addChild(bottomPipe);

	auto topMove = MoveBy::create(0.01*visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
	auto bottomMove = MoveBy::create(0.01*visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));

	topPipe->runAction(topMove);
	bottomPipe->runAction(bottomMove);

	auto pointNode = Node::create();
	pointNode->setPosition(Vec2(topPipe->getPositionX(), topPipe->getPositionY() - topPipe->getContentSize().height / 2
		- (Sprite::create("bird.png")->getContentSize().height) * 2 / 2));
	auto nodePhysic = PhysicsBody::createBox(Size(1,(Sprite::create("bird.png")->getContentSize().height * 2 / 2)));
	nodePhysic->setCollisionBitmask(3);
	nodePhysic->setContactTestBitmask(true);
	nodePhysic->setDynamic(false);
	pointNode->setPhysicsBody(nodePhysic);

	auto nodeMove = MoveBy::create(0.01*visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
	pointNode->runAction(nodeMove);
	addChild(pointNode);
}