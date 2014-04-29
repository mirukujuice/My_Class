#include "Animation.h"

CCFiniteTimeAction* Animation::catAction()
{
    CCArray *animFrames = CCArray::create();
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    for(int i = 0; i < 3; i++)
    {
        CCString* fileName = CCString::createWithFormat("player%d.png", i);
        CCSpriteFrame *frame = cache->spriteFrameByName(fileName->getCString());
        if (!frame)
        {
            CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(fileName->getCString());
            CCRect bounds = CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height);
            frame = CCSpriteFrame::createWithTexture(texture, bounds);
        }

        animFrames->addObject(frame);
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);

    return CCAnimate::create(animation);
}

CCFiniteTimeAction* Animation::num3Animation()
{
    CCDelayTime* delay1 = CCDelayTime::create(0.5);
    CCScaleTo* scaleUp = CCScaleTo::create(0.1, 1);
    CCDelayTime* delay2 = CCDelayTime::create(0.3);
    CCScaleTo* scaleDown = CCScaleTo::create(0.1, 0);

    return CCSequence::create(delay1, scaleUp, delay2, scaleDown, NULL);
}

CCFiniteTimeAction* Animation::num2Animation()
{
    CCDelayTime* delay = CCDelayTime::create(1);

    return CCSequence::create(delay, Animation::num3Animation(), NULL);
}

CCFiniteTimeAction* Animation::num1Animation()
{
    CCDelayTime* delay = CCDelayTime::create(2);

    return CCSequence::create(delay, Animation::num3Animation(), NULL);
}

CCFiniteTimeAction* Animation::num0Animation(CCObject* target, SEL_CallFunc selector)
{
    CCDelayTime* delay = CCDelayTime::create(3);
    CCCallFunc* func = CCCallFunc::create(target, selector);

    return CCSequence::create(delay, Animation::num3Animation(), func, NULL);
}

/*
CCFiniteTimeAction* Animation::shockCatAnimation(CCObject* target, SEL_CallFunc selector)
{
    CCMoveBy* move1 = CCMoveBy::create(0.025, ccp(  5, 0));
    CCMoveBy* move2 = CCMoveBy::create(0.05,  ccp(-10, 0));
    CCMoveBy* move3 = CCMoveBy::create(0.025, ccp(  5, 0));
    CCFiniteTimeAction* seq = CCSequence::create(move1, move2, move3, NULL);
    CCRepeat* repeat = CCRepeat::create(seq, 20);
    CCCallFunc* func = CCCallFunc::create(target, selector);

    return CCSequence::create(repeat, func, NULL);
}

CCFiniteTimeAction* Animation::gameOverAnimation(CCObject* target, SEL_CallFunc selector)
{
    CCScaleTo* scaleUp = CCScaleTo::create(0.5, 1);
    CCDelayTime* delay = CCDelayTime::create(4);
    CCCallFunc* func = CCCallFunc::create(target, selector);

    return CCSequence::create(scaleUp, delay, func, NULL);
}
*/
