#pragma once

#include "Engine/World.h"
#include "Interface/Box.h"
#include "Interface/Text.h"

#define DIALOG_MAX_CHAR 18

class PDialogBox : public PBox
{
	PText*	  mText;
	PTexture* mTexture;

public:
	PDialogBox(const std::string& Text = "")
	{
		SetFixedHeight(100);
		SetResizeModeH(RM_Fixed);

		mText = ConstructWidget<PText>(Text, 20.0f, PColor::Black);
		mText->Padding = { 10 };
		mText->SetAlignment(AL_Left);
		mText->SetLineBreak(true);
		mText->SetLineBreakPos(DIALOG_MAX_CHAR);
		PWidget::AddChild(mText);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		PTexture* Tex = TextureManager::Get("DialogBox");
		FRect	  Geometry = GetGeometry();
		Renderer->DrawTexture(Tex, Tex->GetRect(), Geometry);
	}

	void SetText(const std::string& Text)
	{
		mText->SetText(Text);
	}
};
