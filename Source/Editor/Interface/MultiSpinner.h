#pragma once
#include "Engine/World.h"
#include "Interface/Widget.h"

template <int N>
class PMultiSpinner : public PWidget
{
	std::array<PSpinner*, N> mSpinners;

public:
	PMultiSpinner()
	{
		mLayoutMode = LM_Horizontal;
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;
		mPadding = { 0 };

		for (int32_t Index = 0; Index < N; Index++)
		{
			mSpinners[Index] = ConstructWidget<PSpinner>();
			PWidget::AddChild(mSpinners[Index]);
		}
	}

	void Bind(PParameter* NewParam) override
	{
		PWidget::Bind(NewParam);

		FVector2 V2;
		FVector3 V3;
		FVector4 V4;
		switch (N)
		{
			case 2:
				V2 = NewParam->Get<FVector2>();
				mSpinners[0]->SetValue(V2.X);
				mSpinners[1]->SetValue(V2.Y);
				break;
			case 3:
				V3 = NewParam->Get<FVector3>();
				mSpinners[0]->SetValue(V3.X);
				mSpinners[1]->SetValue(V3.Y);
				mSpinners[2]->SetValue(V3.Z);
				break;
			case 4:
				V4 = NewParam->Get<FVector4>();
				mSpinners[0]->SetValue(V4.X);
				mSpinners[1]->SetValue(V4.Y);
				mSpinners[2]->SetValue(V4.Z);
				mSpinners[3]->SetValue(V4.W);
				break;
			default:
				LogError("Invalid N value.");
				return;
		}
	}
};