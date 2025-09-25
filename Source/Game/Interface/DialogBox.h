#pragma once

#include "Engine/TextPrinter.h"
#include "Engine/World.h"
#include "Interface/Box.h"

class PDialogBox : public PBox
{
	PTextPrinter mPrinter;
	PTexture*	 mTexture = nullptr;

public:
	PDialogBox()
	{
		mTexture = PTextureManager::Get("DialogBox");
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(255, 255, 255, 255);
		FRect Geometry = { 0, WINDOW_DEFAULT_HEIGHT - 100, WINDOW_DEFAULT_WIDTH, 100 };

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
