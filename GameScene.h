#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
//#include "Config.h"

USING_NS_CC;

//class GamePhysicsContactListener;

class GameScene : public CCLayer
{
protected:
    enum kTag
    {
        kTag_Background = 1,
        kTag_Score,
        kTag_TimeLeft,
        kTag_Cat,
        kTag_Shot,
        kTag_Action_Cat_Move,
        kTag_Apple,
        kTag_Bug,
    };

    enum kZOrder
    {
        kZOrder_Background,
        kZOrder_Apple,
        kZOrder_Bug,
        kZOrder_Cat,
        kZOrder_Shot,
        kZOrder_Score,
        kZOrder_TimeLeft,
        kZOrder_Countdown,
    };

    // Chapter 7.1で追記する内容
    CCLabelTTF* m_pScore_Ones;
    CCLabelTTF* m_pScore_Tenths;
    CCLabelTTF* m_pScore_Hundreds;
    CCLabelTTF* m_pTimeLeft_Ones;
    CCLabelTTF* m_pTimeLeft_Tenths;
    void createBackground();
    void createCat();
    void createLabel();
    //void boundingBox();


    // Chapter 7.2で追記する内容
    //bool isLeftPoint;

  // Chapter 7.4で追記する内容
    CCSpriteBatchNode* appleBatchNode;
    CCSpriteBatchNode* bugBatchNode;
    CCSpriteBatchNode* shotBatchNode;
    float m_timeLeft;
    float m_dropTime;
    float s_dropTime;
    void playCountdownSound();
    void showCountdown();
    void startGame();
    void playingGame();
    void playingGame2();
    void setTimeLeft(float time);
    void createShot(CCPoint point);
    void createApple(CCPoint point);
    void createBug(CCPoint point);
    void finishGame();


    // Chapter 7.5で追記する内容
    b2World* world;
    void initPhysics();


    // Chapter 7.7で追記する内容
    b2Body* catBody;
  //  b2Body* basketBody;

  /*
    // Chapter 7.8で追記する内容
    GamePhysicsContactListener* mGamePhysicsContactListener;
    int m_appleCounter;
    bool isShockCat;
    void shockCat();
    void getOverCat();
    void setScoreNumber();
    void changeBasket();
    void removeObject(CCNode* pObject, void* body);
  */
  /*
    // Chapter 7.9で追記する内容
    void moveToNextScene();
  */

public:
    // Chapter 7.1で追記する内容
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameScene);

    // Chapter 7.2で追記する内容
    GameScene();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);


    // スプライト削除
    void spriteMoveFinished(CCNode* sender);


    // Chapter 7.5で追記する内容
    void update(float dt);
  //  void direct(float dt);

};

#endif // __GAME_SCENE_H__
