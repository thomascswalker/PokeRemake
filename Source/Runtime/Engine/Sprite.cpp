#include "Sprite.h"

#include "World.h"

void PSprite::Play()
{
	GTimerManager->SetTimer(mTimerHandle, mCurrentAnim, &PAnimation::Next, mAnimationSpeed, true);
}

void PSprite::Stop()
{
	GTimerManager->ClearTimer(mTimerHandle);
}