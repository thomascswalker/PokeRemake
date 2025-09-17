#pragma once

#include "Engine/TextPrinter.h"
#include "Engine/World.h"
#include "Interface/Box.h"
#include "SDL3/SDL_test_common.h"

class PDialogBox : public PBox
{
	PTextPrinter mPrinter;
	PTexture*	 mTexture = nullptr;

public:
	PDialogBox()
	{
		SetResizeMode(RM_Fixed, RM_Fixed);
		mFixedSize = { WINDOW_DEFAULT_WIDTH, 100 };
		SetPositionMode(PM_Fixed);
		mFixedPosition = { 0, WINDOW_DEFAULT_HEIGHT - mFixedSize.Y };
	}

	void Start() override
	{
		mTexture = TextureManager::Get("DialogBox");
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(255, 255, 255, 255);
		FRect Geometry = GetGeometry();

		Renderer->DrawFillRect(Geometry);
		Renderer->DrawTexture(mTexture, mTexture->GetRect(), Geometry);

		Renderer->SetDrawColor(0, 0, 0, 255);
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
		mPrinter.Play();
	}

	void EndPrint()
	{
		mPrinter.Stop();
	}
};
