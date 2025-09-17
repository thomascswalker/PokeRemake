#pragma once

#include "Engine/TextPrinter.h"
#include "Engine/World.h"
#include "Interface/Box.h"

class PDialogBox : public PBox
{
	PTextPrinter mPrinter;
	PTexture*	 mTexture;

public:
	PDialogBox()
	{
		SetFixedHeight(100);
		SetResizeModeH(RM_Fixed);
	}

	void Start() override
	{
		mTexture = TextureManager::Get("DialogBox");
	}

	void Draw(const PRenderer* Renderer) const override
	{
		FRect Geometry = GetGeometry();
		Renderer->DrawTexture(mTexture, mTexture->GetRect(), Geometry);

		auto Position = Geometry.GetPosition() + FVector2(24, 48);
		Renderer->DrawText(mPrinter.GetDisplayText(), Position, 32.0f);
	}

	void Tick(float DeltaTime) override
	{
		PWidget::Tick(DeltaTime);
		mPrinter.Tick(DeltaTime);
	}

	void SetText(const std::string& Text)
	{
		mPrinter.SetText(Text);
	}

	void Print()
	{
		mPrinter.StartTimer();
	}

	void EndPrint()
	{
		mPrinter.EndTimer();
	}
};
