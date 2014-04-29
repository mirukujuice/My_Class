#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "cocos2d.h"

USING_NS_CC;

class Animation
{
public:
    // Chapter 7.3で説明する内容
    static CCFiniteTimeAction* catAction();

    // Chapter 7.4で説明する内容
    static CCFiniteTimeAction* num3Animation();
    static CCFiniteTimeAction* num2Animation();
    static CCFiniteTimeAction* num1Animation();
    static CCFiniteTimeAction* num0Animation(CCObject* target, SEL_CallFunc selector);
/*
    // Chapter 7.8���������������������
    static CCFiniteTimeAction* shockCatAnimation(CCObject* target, SEL_CallFunc selector);

    // Chapter 7.9���������������������
    static CCFiniteTimeAction* gameOverAnimation(CCObject* target, SEL_CallFunc selector);
*/
};

#endif // __ANIMATION_H__
