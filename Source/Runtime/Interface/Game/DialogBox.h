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
	void Start() override
	{
		mTexture = TextureManager::Get("DialogBox");
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(255, 255, 255, 255);
		FRect Geometry = GetGeometry();

		/**
		 * TODO: Currently hardcoded because layout is not working as expected.
		 */
		Geometry.H = 100;
		Geometry.W = WINDOW_DEFAULT_WIDTH;
		Geometry.X = 0;
		Geometry.Y = WINDOW_DEFAULT_HEIGHT - Geometry.H;
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
