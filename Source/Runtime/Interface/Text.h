#pragma once

#include "Widget.h"

class PText : public PWidget
{
	std::string mText;
	float		mFontSize;

public:
	explicit PText(const std::string& Text, float FontSize = 14.0f)
		: mText(Text), mFontSize(FontSize)
	{
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->DrawText(mText, FVector2(X + W / 2.0f, Y + H / 2.0f), mFontSize);
	}

	float		GetFontSize() const { return mFontSize; }
	void		SetFontSize(float Size) { mFontSize = Size; }
	std::string GetText() const { return mText; }
	void		SetText(const std::string& Text) { mText = Text; }
};