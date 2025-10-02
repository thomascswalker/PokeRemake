#pragma once

#include "../Core/GameConstants.h"
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
		FRect Geometry = { 0, DIALOG_Y, DIALOG_WIDTH, DIALOG_HEIGHT };

		Renderer->DrawFillRect(Geometry);
		Renderer->DrawTexture(mTexture, mTexture->GetRect(), Geometry);

		Renderer->SetDrawColor(0, 0, 0, 255);
		auto TextPosition = FVector2(DIALOG_LINE_X, DIALOG_LINE1_Y);
		Renderer->DrawText(mPrinter.GetDisplayText(), TextPosition, DIALOG_FONT_SIZE, false, DIALOG_FONT_GAP);
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
