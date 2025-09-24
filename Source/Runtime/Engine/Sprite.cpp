#include "Sprite.h"

#include "World.h"

void PSprite::Play()
{
	GWorld->GetTimerManager()->SetTimer(mTimerHandle, mCurrentAnim, &PAnimation::Next, mAnimationSpeed, true);
}

void PSprite::Stop()
{
	GWorld->GetTimerManager()->ClearTimer(mTimerHandle);
}