#pragma once

#include "Engine/World.h"
#include "Interface/Widget.h"

template <int N>
class PMultiSpinner : public PWidget
{
	static_assert(N == 2 || N == 3 || N == 4, "N must either 2, 3, or 4.");
	DECLARE_MULTICAST_DELEGATE(DValueChanged, FVector3);

	FVector3  mValue;
	PSpinner* mSpinners[4];

	void OnValueChangedInternal(float NewValue)
	{
		ValueChanged.Broadcast(mValue);
	}

public:
	DValueChanged ValueChanged;

	PMultiSpinner()
	{
		mLayoutMode = LM_Horizontal;
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;

		for (int32_t Index = 0; Index < N; Index++)
		{
			auto Spinner = ConstructWidget<PSpinner>();
			mSpinners[Index] = Spinner;
			Spinner->Padding.Left = 5;
			Spinner->Padding.Right = 5;
			Spinner->ValueChanged.AddRaw(this, &PMultiSpinner::OnValueChangedInternal);
			PWidget::AddChild(Spinner);
		}
	}

	void Bind(PParameter* NewParam) override
	{
		PWidget::Bind(NewParam);

		switch (N)
		{
			case 3:
				mValue = NewParam->Get<FVector3>();

				mSpinners[0]->SetValue(mValue.X);
				mSpinners[1]->SetValue(mValue.Y);
				mSpinners[2]->SetValue(mValue.Z);

				ValueChanged.AddRaw(Param, &PParameter::Set<FVector3>);
				break;
			case 2:
			case 4:
			default:
				LogError("Invalid N value.");
				break;
		}
	}
};