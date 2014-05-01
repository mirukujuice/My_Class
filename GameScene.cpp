#include "GameScene.h"
#include "Animation.h"
#include "SimpleAudioEngine.h"

//#include "AppMacros.h"
#include "Config.h"
#include "PhysicsSprite.h"
//#include "GamePhysicsContactListener.h"

//#define BASKET_LEVEL1 20
//#define BASKET_LEVEL2 40
//#define BASKET_LEVEL3 80

using namespace std;
using namespace CocosDenshion;

//test
GameScene::GameScene()
//:isLeftPoint(true)
:m_timeLeft(60)
,s_dropTime(0.1)
,m_dropTime(1)
//,isShockCat(false)
{
    srand((unsigned int)time(NULL));
}


CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init())
        return false;

    // シングルタッチを有効にする
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    //物理エンジンを適用するため物理空間を生成する
    initPhysics();


    // 背景を追加する
    createBackground();

    // プレイヤーを表示する
    createCat();

    // 弾を表示する
//    createShot();

    // スコアと残り時間を表示する
    createLabel();

    // リンゴの準備
    appleBatchNode = CCSpriteBatchNode::create("apple.png");
    this->addChild(appleBatchNode);

    // 虫の準備
    bugBatchNode = CCSpriteBatchNode::create("bug.png");
    this->addChild(bugBatchNode);

    // 弾の準備
    shotBatchNode = CCSpriteBatchNode::create("shot1.png");
    this->addChild(shotBatchNode);

    // カウントダウン開始
    this->scheduleOnce(schedule_selector(GameScene::showCountdown), 1);

    // あたり判定
    // this->scheduleUpdate();

    return true;
}

void GameScene::createBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 背景を表示する
    CCSprite* pBgUnder = CCSprite::create("mapZ.png");
    pBgUnder->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(pBgUnder, kZOrder_Background, kTag_Background);
/*
    // 地面の生成
    CCNode* pGround = CCNode::create();
    pGround->setTag(kTag_Ground);
    this->addChild(pGround);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBodyDef.userData = pGround;

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    float groundHeight = pBgUnder->getContentSize().height * 0.2;

    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0, groundHeight / PTM_RATIO),
                  b2Vec2(winSize.width / PTM_RATIO, groundHeight / PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
*/
}

void GameScene::createCat()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize bgSize = this->getChildByTag(kTag_Background)->getContentSize();
/*
    // 震えるネコの作成（画像準備のため非表示）
    CCSprite* shockCat = CCSprite::create("cat3.png");
    shockCat->setVisible(false);
    this->addChild(shockCat, kZOrder_Cat, kTag_Cat_Shock);
*/

    // プレイヤーの表示
    PhysicsSprite* pCat = new PhysicsSprite();
    pCat->autorelease();
    pCat->initWithFile("player1.png");
    //CCSprite* pCat = CCSprite::create("player1.png");
    pCat->setPosition(ccp(bgSize.width * 0.5,bgSize.height * 0.2));
    this->addChild(pCat, kZOrder_Cat, kTag_Cat);

    // ネコの物理構造設定
    b2BodyDef catBodyDef;
    catBodyDef.type = b2_staticBody;
    catBodyDef.position.Set(pCat->getPositionX() / PTM_RATIO,
                            pCat->getPositionY() / PTM_RATIO);
    catBodyDef.userData = pCat;
    catBody = world->CreateBody(&catBodyDef);

    b2CircleShape catShape;
    catShape.m_radius = pCat->getContentSize().width * 0.4 / PTM_RATIO;

    b2FixtureDef catFixturedef;
    catFixturedef.shape = &catShape;
    catFixturedef.density = 1;
    catFixturedef.friction = 0.3;

    catBody->CreateFixture(&catFixturedef);
    pCat->setPhysicsBody(catBody);

/*
    // カゴ前面の表示
    CCSprite* pBasketFront = CCSprite::create("basket0.png");
    pBasketFront->setPosition(pBasket->getPosition());
    this->addChild(pBasketFront, kZOrder_Basket_Front, kTag_Basket_Front);
*/
/*
    // カゴの物理構造設定
    CCNode* basketInside = CCNode::create();
    basketInside->setTag(kTag_Basket_Inside);
    this->addChild(basketInside);

    b2BodyDef basketBodyDef;
    basketBodyDef.type = b2_staticBody;
    basketBodyDef.position.Set(pBasket->getPositionX() / PTM_RATIO,
                               (pBasket->getPositionY() - pBasket->getContentSize().height * 0.4) / PTM_RATIO);
    basketBodyDef.userData = basketInside;
    basketBody = world->CreateBody(&basketBodyDef);

    b2PolygonShape basketShape;
    basketShape.SetAsBox(pBasket->getContentSize().width * 0.8 / 2 / PTM_RATIO,
                         1 / PTM_RATIO);

    b2FixtureDef basketFixturedef;
    basketFixturedef.shape = &basketShape;

    basketBody->CreateFixture(&basketFixturedef);
*/
}

void GameScene::createLabel()
{

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize viewSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    //float scale = CCEGLView::sharedOpenGLView()->getScaleY();
    //float offset = (winSize.height * scale - viewSize.height) / 2 / scale;

    // 残り時間の表示
    CCSprite* pTimeLeft = CCSprite::create("timeleft.png");
    pTimeLeft->setPosition(ccp(pTimeLeft->getContentSize().width / 2,
                               winSize.height - pTimeLeft->getContentSize().height / 2));
    this->addChild(pTimeLeft, kZOrder_TimeLeft, kTag_TimeLeft);

    CCSize timeLeftSize = pTimeLeft->getContentSize();

    // 残り時間10の位
    m_pTimeLeft_Tenths = CCLabelTTF::create("6", "", 48);
    m_pTimeLeft_Tenths->setPosition(ccp(timeLeftSize.width * 0.45, timeLeftSize.height * 0.4));
    m_pTimeLeft_Tenths->setColor(ccBLACK);
    pTimeLeft->addChild(m_pTimeLeft_Tenths);

    // 残り時間1の位
    m_pTimeLeft_Ones = CCLabelTTF::create("0", "", 48);
    m_pTimeLeft_Ones->setPosition(ccp(timeLeftSize.width * 0.675, timeLeftSize.height * 0.4));
    m_pTimeLeft_Ones->setColor(ccBLACK);
    pTimeLeft->addChild(m_pTimeLeft_Ones);

    // スコアの表示
    CCSprite* pScore = CCSprite::create("game_counter.png");
    pScore->setPosition(ccp(winSize.width - pScore->getContentSize().width / 2,
    		                winSize.height - pScore->getContentSize().height / 2));
    this->addChild(pScore, kZOrder_Score, kTag_Score);

    CCSize scoreSize = pScore->getContentSize();

    // スコア100の位
    m_pScore_Hundreds = CCLabelTTF::create("", "", 48);
    m_pScore_Hundreds->setPosition(ccp(scoreSize.width * 0.4, scoreSize.height * 0.3));
    m_pScore_Hundreds->setColor(ccBLACK);
    pScore->addChild(m_pScore_Hundreds);

    // スコア10の位
    m_pScore_Tenths = CCLabelTTF::create("", "", 48);
    m_pScore_Tenths->setPosition(ccp(scoreSize.width * 0.625, scoreSize.height * 0.3));
    m_pScore_Tenths->setColor(ccBLACK);
    pScore->addChild(m_pScore_Tenths);

    // スコア1の位
    m_pScore_Ones = CCLabelTTF::create("0", "", 48);
    m_pScore_Ones->setPosition(ccp(scoreSize.width * 0.85, scoreSize.height * 0.3));
    m_pScore_Ones->setColor(ccBLACK);
    pScore->addChild(m_pScore_Ones);

}

CCPoint oldLoc;

bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	oldLoc = pTouch->getLocation();

    CCPoint loc = pTouch->getLocation();
    CCNode* pCat = this->getChildByTag(kTag_Cat);

    // プレイヤーがタップされた時のみ有効にする
    //return pCat->boundingBox().containsPoint(loc);
}

void GameScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
   // if (isShockCat)
   //     return;

    // static CCPoint loc = pTouch->getLocation();

    CCSize bgSize = this->getChildByTag(kTag_Background)->getContentSize();

    CCSprite* pCat = (CCSprite*)this->getChildByTag(kTag_Cat);
    //CCSprite* pShot = (CCSprite*)this->getChildByTag(kTag_Shot);
    CCPoint catLoc = pCat->getPosition();

    CCPoint newLoc = pTouch->getLocation();
    catLoc.x += newLoc.x - oldLoc.x;
    catLoc.y += newLoc.y - oldLoc.y;

    // 横移動制限
    catLoc.x = (catLoc.x < pCat->getContentSize().width / 2) ? pCat->getContentSize().width / 2 : catLoc.x;
    catLoc.x = (catLoc.x > bgSize.width - pCat->getContentSize().width / 2) ? bgSize.width - pCat->getContentSize().width / 2 : catLoc.x;
    // 縦移動制限
    catLoc.y = (catLoc.y < pCat->getContentSize().height / 2) ? pCat->getContentSize().height / 2 : catLoc.y;
    catLoc.y = (catLoc.y > bgSize.height - pCat->getContentSize().height / 2) ? bgSize.height - pCat->getContentSize().height / 2 : catLoc.y;

    /*
    if (catLoc.x < pCat->getContentSize().width / 2) {
    	catLoc.x = pCat->getContentSize().width / 2;
    } else if (catLoc.x > bgSize.width - pCat->getContentSize().width / 2) {
    	catLoc.x = bgSize.width - pCat->getContentSize().width / 2;
    }
    */

    pCat->setPosition(catLoc);
    //pShot->setPosition(catLoc);
    catBody->SetTransform(b2Vec2(catLoc.x / PTM_RATIO, catLoc.y / PTM_RATIO), 0);
	//catBody->SetTransform(b2Vec2(loc.x / PTM_RATIO, catBody->GetPosition().y), 0);
    oldLoc = newLoc;

    // loc = newLoc;

/*
    // ネコを反転するしきい値を計算する
    float threshold = CCDirector::sharedDirector()->getWinSize().width * 0.005f;

    // しきい値を超えた場合はプレイヤーを反転させる
    CCSprite* pCat = (CCSprite*)this->getChildByTag(kTag_Cat);
    if (loc.x - pTouch->getPreviousLocation().x > threshold ||
        pTouch->getPreviousLocation().x - loc.x > threshold) {
        if (loc.x > pTouch->getPreviousLocation().x)
        {
            if (isLeftPoint)
            {
                isLeftPoint = false;
                pCat->setFlipX(!isLeftPoint);
            }
        }
        else
        {
            if (!isLeftPoint)
            {
                isLeftPoint = true;
                pCat->setFlipX(!isLeftPoint);
            }
        }
    }
*/

    // ネコを移動する


    // プレイヤーのアニメーション
    CCAction* catAction = pCat->getActionByTag(kTag_Action_Cat_Move);
    if (!catAction)
    {
        CCFiniteTimeAction* action = Animation::catAction();
        action->setTag(kTag_Action_Cat_Move);
        pCat->runAction(action);
    }

/*
    // カゴを移動する
    float basketOffset = bgSize.width * 0.05;
    if (isLeftPoint)
        basketOffset *= -1;

    CCSprite* pBasket = (CCSprite*)this->getChildByTag(kTag_Basket);
    pBasket->setPosition(ccp(loc.x - basketOffset, pBasket->getPositionY()));

    CCSprite* pBasketFront = (CCSprite*)this->getChildByTag(kTag_Basket_Front);
    pBasketFront->setPosition(pBasket->getPosition());

    // カゴ内部を移動する
    basketBody->SetTransform(b2Vec2(pBasket->getPositionX() / PTM_RATIO,
                                    basketBody->GetPosition().y),
                             0);
*/
}

void GameScene::showCountdown()
{
    this->scheduleOnce(schedule_selector(GameScene::playCountdownSound), 0.5);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 3の表示
    CCSprite* pNum3 = CCSprite::create("number3.png");
    pNum3->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    pNum3->setScale(0);
    pNum3->runAction(Animation::num3Animation());
    this->addChild(pNum3, kZOrder_Countdown);

    // 2の表示
    CCSprite* pNum2 = CCSprite::create("number2.png");
    pNum2->setPosition(pNum3->getPosition());
    pNum2->setScale(0);
    pNum2->runAction(Animation::num2Animation());
    this->addChild(pNum2, kZOrder_Countdown);

    // 1の表示
    CCSprite* pNum1 = CCSprite::create("number1.png");
    pNum1->setPosition(pNum3->getPosition());
    pNum1->setScale(0);
    pNum1->runAction(Animation::num1Animation());
    this->addChild(pNum1, kZOrder_Countdown);

    // GOの表示
    CCSprite* pGo = CCSprite::create("go.png");
    pGo->setPosition(pNum3->getPosition());
    pGo->setScale(0);
    pGo->runAction(Animation::num0Animation(this, callfunc_selector(GameScene::startGame)));
    this->addChild(pGo, kZOrder_Countdown);
}

void GameScene::playCountdownSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("countdown.mp3");
}

void GameScene::startGame()
{
	// CCLog("startGame");

    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3", true);

    // 毎フレームupdate( )関数を呼び出すように設定する
    scheduleUpdate();

    // ゲームの開始
    this->schedule(schedule_selector(GameScene::playingGame), m_dropTime);
    this->schedule(schedule_selector(GameScene::playingGame2), s_dropTime);
    this->schedule(schedule_selector(GameScene::setTimeLeft));
}

// スプライト削除
void GameScene::spriteMoveFinished(CCNode* sender)
{
	// 終了したアクションのスプライトの取得
	CCSprite * sprite = (CCSprite *)sender;
	bool isCleanUp = true;
	// スプライトをレイヤーから削除して破棄する
	this->removeChild(sprite, isCleanUp);
}

void GameScene::playingGame()
{
    this->unschedule(schedule_selector(GameScene::playingGame));

    // ゲーム終了時は処理を中断
    if (m_timeLeft <= 0)
        return;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int random = rand();

    // 位置をランダムで設定

    int locY = 100 * 1.1;
    int locX = random % 85 + 10;

 //   if (random % 5 == 0)
        // 虫を落とす
        createBug(ccp(winSize.width * locX / 100, winSize.height * locY / 100));
        //createShot(ccp(winSize.width * locX / 100, winSize.height * locY / 100));
 //   else
        // リンゴを落とす
        createApple(ccp(winSize.width * locX / 100, winSize.height * locY / 100));

    // 呼び出し時間変更
    m_dropTime = m_dropTime * 0.990;
    this->schedule(schedule_selector(GameScene::playingGame), m_dropTime);
}

void GameScene::playingGame2()
{
	//this->schedule(schedule_selector(GameScene::playingGame2));

	    // ゲーム終了時は処理を中断
	if (m_dropTime <= 0)
	    return;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize catSize = this->getChildByTag(kTag_Cat)->getContentSize();

	CCSprite* pShot = (CCSprite*)this->getChildByTag(kTag_Cat);
	CCPoint shotLoc = pShot->getPosition();

	createShot(ccp(shotLoc.x, shotLoc.y + catSize.height * 0.5));

	// 呼び出し時間変更
	//s_dropTime = s_dropTime * 1.0;
	//this->schedule(schedule_selector(GameScene::playingGame2), s_dropTime);
}

void GameScene::setTimeLeft(float time)
{
    // 時間の更新
    m_timeLeft -= time;

    if (m_timeLeft <= 0)
    {
        // ゲーム終了
        this->unschedule(schedule_selector(GameScene::setTimeLeft));

        SimpleAudioEngine::sharedEngine()->playEffect("whistle.mp3");
        this->scheduleOnce(schedule_selector(GameScene::finishGame), 4);
        return;
    }

    // 残り時間の表示
    int tenthsPlace = (int)m_timeLeft / 10;
    int onesPlace = m_timeLeft - tenthsPlace * 10;

    CCString* tenthsPlaceString;
    if (tenthsPlace == 0)
        tenthsPlaceString = CCString::create(" ");
    else
        tenthsPlaceString = CCString::createWithFormat("%d", tenthsPlace);

    m_pTimeLeft_Tenths->setString(tenthsPlaceString->getCString());

    CCString* onesPlaceString = CCString::createWithFormat("%d", onesPlace);
    m_pTimeLeft_Ones->setString(onesPlaceString->getCString());
}

void GameScene::createShot(CCPoint point)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pShot = CCSprite::createWithTexture(shotBatchNode->getTexture());
    pShot->setPosition(point);
   /* CCRect pShotRect = CCRectMake(
    		pShot->getPosition().x - (pShot->getContentSize().width/2),
    		pShot->getPosition().y - (pShot->getContentSize().height/2),
    		pShot->getContentSize().width,
    		pShot->getContentSize().height);
    */
    pShot->boundingBox();
    //CCMoveTo* actionMove = CCMoveTo::create(7.0f, ccp(pApple->getPositionX(), pApple->getContentSize().height* -2));
    CCActionInterval* bAction;
    bAction = CCMoveBy::create(1.0f, ccp(0, winSize.height));
    pShot->runAction(bAction);
    this->addChild(pShot, kZOrder_Shot, kTag_Shot);

    // スプライト削除
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    //アクションの実行
    pShot->runAction(CCSequence::create(bAction, actionMoveDone, NULL));
}

void GameScene::createApple(CCPoint point)
{

    // リンゴを表示
//  PhysicsSprite* pApple = new PhysicsSprite();
//  pApple->autorelease();
//  pApple->initWithTexture(appleBatchNode->getTexture());
	CCSprite* pApple = CCSprite::createWithTexture(appleBatchNode->getTexture());
    pApple->setPosition(point);
//    CCRect pAppleRect = CCRectMake(
//    		pApple->getPosition().x - (pApple->getContentSize().width/2),
//    		pApple->getPosition().y - (pApple->getContentSize().height/2),
//    		pApple->getContentSize().width,
//    		pApple->getContentSize().height);

    pApple->boundingBox();
    CCMoveTo* actionMove = CCMoveTo::create(7.0f, ccp(pApple->getPositionX(), pApple->getContentSize().height* -2));
    pApple->runAction(actionMove);
    this->addChild(pApple, kZOrder_Apple, kTag_Apple);

    // スプライト削除
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    //アクションの実行
    pApple->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));


/* 保留
    // リンゴの物理構造設定
    b2BodyDef appleBodyDef;
    appleBodyDef.type = b2_dynamicBody;
    appleBodyDef.position.Set(pApple->getPositionX() / PTM_RATIO,
                              pApple->getPositionY() / PTM_RATIO);
    appleBodyDef.userData = pApple;
    b2Body* appleBody = world->CreateBody(&appleBodyDef);

    b2CircleShape appleShape;
    appleShape.m_radius = pApple->getContentSize().width * 0.3 / PTM_RATIO;

    b2FixtureDef appleFixturedef;
    appleFixturedef.shape = &appleShape;
    appleFixturedef.density = 1;
    appleFixturedef.friction = 0.9;

    appleBody->CreateFixture(&appleFixturedef);
    pApple->setPhysicsBody(appleBody);
*/
}

void GameScene::createBug(cocos2d::CCPoint point)
{
    // 虫を表示
//    PhysicsSprite* pBug = new PhysicsSprite();
//    pBug->autorelease();
//    pBug->initWithTexture(bugBatchNode->getTexture());
	CCSprite* pBug = CCSprite::createWithTexture(bugBatchNode->getTexture());
    pBug->setPosition(point);
    CCMoveTo* actionMove = CCMoveTo::create(4.0f, ccp(pBug->getPositionX(), pBug->getContentSize().height* -2));
    pBug->runAction(actionMove);
    this->addChild(pBug, kZOrder_Bug, kTag_Bug);

    // スプライト削除
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
    //アクションの実行
    pBug->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
/*
    // 虫の物理構造設定
    b2BodyDef bugBodyDef;
    bugBodyDef.type = b2_dynamicBody;
    bugBodyDef.position.Set(pBug->getPositionX() / PTM_RATIO,
                            pBug->getPositionY() / PTM_RATIO);
    bugBodyDef.userData = pBug;
    b2Body* bugBody = world->CreateBody(&bugBodyDef);

    b2CircleShape bugShape;
    bugShape.m_radius = pBug->getContentSize().width * 0.3 / PTM_RATIO;

    b2FixtureDef bugFixturedef;
    bugFixturedef.shape = &bugShape;
    bugFixturedef.density = 1;
    bugFixturedef.friction = 0.3;

    bugBody->CreateFixture(&bugFixturedef);
    pBug->setPhysicsBody(bugBody);
*/
}

void GameScene::finishGame()
{
/*
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 「Game Over」を表示する
    CCSprite* gameOver = CCSprite::create("game_gameover.png");
    gameOver->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    gameOver->setScale(0);
    gameOver->runAction(Animation::gameOverAnimation(this, callfunc_selector(GameScene::moveToNextScene)));
    this->addChild(gameOver, kZOrder_Countdown, kTag_Countdown);
*/
}

void GameScene::initPhysics()
{
    // 重力の設定
    b2Vec2 gravity;
    gravity.Set(0.0, -1.0);

    // worldを生成
    world = new b2World(gravity);
}
/*
    // 衝突判定処理を追加する
    mGamePhysicsContactListener = new GamePhysicsContactListener(this, callfunc_selector(GameScene::shockCat));
    world->SetContactListener(mGamePhysicsContactListener);

*/

void GameScene::update(float dt)
{

	// CCLog("update");

    // 物理シミュレーションの正確さを決定するパラメーター
    int velocityIterations = 8;
    int positionIterations = 1;

    // worldを更新する
    world->Step(dt, velocityIterations, positionIterations);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //ここで当たり判定などの設定をする

    // 全リンゴの配列を作成
	CCArray* appleArr = CCArray::create();
	// 全ショットの配列を作成
	CCArray* shotArr = CCArray::create();
	// 全バグの配列を作成
	CCArray* bugArr = CCArray::create();
	// 全オブジェクトを取得
	CCArray* objArr = this->m_pChildren;

	// 全リンゴ、全ショット、全バグを取得
	for (int i = 0; i < objArr->count(); i++) {
		// 要素を一つ取得
		CCObject* obj = objArr->objectAtIndex(i);
		CCSprite* spriteObj = (CCSprite *)obj;

		if (spriteObj->getTag() == kTag_Apple) {	// リンゴの場合
			appleArr->addObject(obj);
		} else if (spriteObj->getTag() == kTag_Shot) {    // ショットの場合
			shotArr->addObject(obj);
		} else if (spriteObj->getTag() == kTag_Bug) {    // バグの場合
			bugArr->addObject(obj);
		}
	}

	// リンゴを順に取得
	for (int i = 0; i < appleArr->count(); i++) {
		CCSprite* apple = (CCSprite*)appleArr->objectAtIndex(i);
		// ショットを順に取得
		for (int j = 0; j < shotArr->count(); j++) {
			CCSprite* shot = (CCSprite*)shotArr->objectAtIndex(j);
			// バグを順に取得
			for (int k = 0; k < bugArr->count(); k++) {
						CCSprite* bug = (CCSprite*)bugArr->objectAtIndex(k);

			// リンゴとショットとのあたり判定
			if(apple->boundingBox().intersectsRect(shot->boundingBox()))
			{
			// リンゴとショットが当たった時の処理
				CCLog("APPLEHIT!");
				// エフェクト作成
				CCPoint appleSize = apple->getPosition();
				CCParticleSystemQuad* particle1 = CCParticleSystemQuad::create("star1.plist");
				particle1->setPosition(ccp(appleSize.x, appleSize.y));
				particle1->setAutoRemoveOnFinish(true);
				this->addChild(particle1);
				//apple->removeFromParent();
				//shot->removeFromParent();
				apple->removeFromParentAndCleanup(true);
				shot->removeFromParentAndCleanup(true);
			}

			if(bug->boundingBox().intersectsRect(shot->boundingBox()))
			{
			// バグとショットが当たった時の処理
				CCLog("BUGHIT!");
				// エフェクト作成
				CCPoint bugSize = bug->getPosition();
				CCParticleSystemQuad* particle2 = CCParticleSystemQuad::create("star2.plist");
				particle2->setPosition(ccp(bugSize.x, bugSize.y));
				particle2->setAutoRemoveOnFinish(true);
				this->addChild(particle2);
				//bug->removeFromParent();
				//shot->removeFromParent();
				bug->removeFromParentAndCleanup(true);
				shot->removeFromParentAndCleanup(true);
			}

		  }
		}
	}
}

/*
    // world内の全オブジェクトをループする
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (!b->GetUserData())
            continue;

        // オブジェクトを取得する
        CCNode* object = (CCNode*)b->GetUserData();
        int objectTag = object->getTag();

        if (objectTag == kTag_Apple_In_Basket)
        {
            // カゴに入ったリンゴ
            m_appleCounter++;
            if (m_appleCounter == BASKET_LEVEL1 ||
                m_appleCounter == BASKET_LEVEL2 ||
                m_appleCounter == BASKET_LEVEL3)
            {
                changeBasket();
            }

            setScoreNumber();
            removeObject(object, (void*)b);
        }
        else if (objectTag == kTag_Bug_In_Basket)
        {
            // カゴに入った虫
            m_appleCounter = m_appleCounter - 10;

            if (m_appleCounter < 0)
                m_appleCounter = 0;

            changeBasket();
            setScoreNumber();
            removeObject(object, (void*)b);
        }
        else if (objectTag == kTag_Remove_Apple ||
                 objectTag == kTag_Remove_Bug)
        {
            // 地面に落ちたリンゴと虫
            if (!object->getActionByTag(kTag_Action_Remove_Object))
            {
                // 1病後に消えるアクションをセットする
                CCDelayTime* delay = CCDelayTime::create(1);
                CCCallFuncND* func = CCCallFuncND::create(this, callfuncND_selector(GameScene::removeObject), (void*)b);
                CCSequence* action = CCSequence::createWithTwoActions(delay, func);
                action->setTag(kTag_Action_Remove_Object);
                object->runAction(action);
            }
        }
    }
}



void GameScene::shockCat()
{
    CCSprite* shockCat = (CCSprite*)this->getChildByTag(kTag_Cat_Shock);
    CCAction* action = shockCat->getActionByTag(kTag_Action_Cat_Shock);

    if (action && !action->isDone())
        return;

    // ショックネコを表示する
    CCSprite* cat = (CCSprite*)this->getChildByTag(kTag_Cat);
    shockCat->setPosition(ccp(catBody->GetPosition().x * PTM_RATIO,
                              catBody->GetPosition().y * PTM_RATIO));
    shockCat->setVisible(true);
    shockCat->setFlipX(cat->isFlipX());

    // 通常ネコを非表示する
    cat->setVisible(false);
    isShockCat = true;

    // ショックネコに震えるアニメーションを与える
    CCFiniteTimeAction* shockAction;
    shockAction = Animation::shockCatAnimation(this,
                                               callfunc_selector(GameScene::getOverCat));
    shockAction->setTag(kTag_Action_Cat_Shock);
    shockCat->runAction(shockAction);
}

void GameScene::getOverCat()
{
    // ショックネコを非表示する
    CCSprite* shockCat = (CCSprite*)this->getChildByTag(kTag_Cat_Shock);
    shockCat->setVisible(false);

    // 通常ネコを表示する
    CCSprite* cat = (CCSprite*)this->getChildByTag(kTag_Cat);
    cat->setVisible(true);
    isShockCat = false;
}

void GameScene::setScoreNumber()
{
    int showScore;

    if (m_appleCounter > 999)
        showScore = 999;
    else if (m_appleCounter < 0)
        showScore = 0;
    else
        showScore = m_appleCounter;

    // 残り時間を表示する
    int hundredsPlace = showScore / 100;
    int tenthsPlace = (showScore - hundredsPlace * 100) / 10;
    int onesPlace = showScore - hundredsPlace * 100 - tenthsPlace * 10;

    // 残り時間の100の位
    CCString* hundredsPlaceString;
    if (hundredsPlace == 0)
        hundredsPlaceString = CCString::create(" ");
    else
        hundredsPlaceString = CCString::createWithFormat("%d", hundredsPlace);

    m_pScore_Hundreds->setString(hundredsPlaceString->getCString());

    // 残り時間の10の位
    CCString* tenthsPlaceString;
    if (hundredsPlace == 0 && tenthsPlace == 0)
        tenthsPlaceString = CCString::create(" ");
    else
        tenthsPlaceString = CCString::createWithFormat("%d", tenthsPlace);

    m_pScore_Tenths->setString(tenthsPlaceString->getCString());

    // 残り時間の1の位
    CCString* onesPlaceString = CCString::createWithFormat("%d", onesPlace);
    m_pScore_Ones->setString(onesPlaceString->getCString());
}

void GameScene::removeObject(CCNode* pObject, void* body)
{
    // オブジェクトを削除する
    pObject->removeFromParentAndCleanup(true);
    world->DestroyBody((b2Body*)body);
}

void GameScene::changeBasket()
{
    string basket;
    if (m_appleCounter >= BASKET_LEVEL3)
        basket = "basket4.png";
    else if (m_appleCounter >= BASKET_LEVEL2)
        basket = "basket3.png";
    else if (m_appleCounter >= BASKET_LEVEL1)
        basket = "basket2.png";
    else
        basket = "basket1.png";

    CCSprite* pBasket = (CCSprite*)this->getChildByTag(kTag_Basket);

    // 新しいカゴを用意する
    CCSprite* pNewBasket = CCSprite::create(basket.c_str());
    pNewBasket->setPosition(pBasket->getPosition());
    this->addChild(pNewBasket, kZOrder_Basket, kTag_Basket);

    // 古いカゴを消去する
    pBasket->removeFromParentAndCleanup(true);
}

void GameScene::moveToNextScene()
{
    // update( )関数の呼び出しを停止する
    unscheduleUpdate();

    // 次の画面へ遷移する
    CCTransitionProgressRadialCCW* tran;
    tran = CCTransitionProgressRadialCCW::create(1, GameScene::scene());
    CCDirector::sharedDirector()->replaceScene(tran);
}
*/
