#include "Sprite.h"

#include "World.h"

void PSprite::Play()
{
	GetWorld()->GetTimerManager()->SetTimer(mTimerHandle, mCurrentAnim, &PAnimation::Next, mAnimationSpeed, true);
}

void PSprite::Stop()
{
	GetWorld()->GetTimerManager()->ClearTimer(mTimerHandle);
}