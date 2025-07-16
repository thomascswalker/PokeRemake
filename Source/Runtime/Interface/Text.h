#pragma once

#include "Widget.h"

class PText : public PWidget
{
	std::string mText;
	float		mFontSize;

public:
	PText() = default;
	explicit PText(const std::string& Text, float FontSize = 14.0f)
		: mText(Text), mFontSize(FontSize)
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Grow;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(PColor::UIText);
		Renderer->DrawText(mText, FVector2(X + W / 2.0f, Y + H / 2.0f), mFontSize);
	}

	float		GetFontSize() const { return mFontSize; }
	void		SetFontSize(float Size) { mFontSize = Size; }
	std::string GetText() const { return mText; }
	void		SetText(const std::string& Text) { mText = Text; }
};