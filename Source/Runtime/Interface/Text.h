#pragma once

#include "Widget.h"

class PText : public PWidget
{
	std::string mText;
	PColor mColor;
	float mFontSize;

public:
	PText() = default;

	explicit PText(const std::string& Text, float FontSize = FONT_RENDER_SCALE, const PColor& Color = PColor::White)
		: mText(Text), mColor(Color), mFontSize(FontSize)
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Grow;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(mColor);
		Renderer->DrawText(mText, FVector2(X + W / 2.0f, Y + H / 2.0f), mFontSize);
	}

	float GetFontSize() const
	{
		return mFontSize;
	}

	void SetFontSize(float Size)
	{
		mFontSize = Size;
	}

	std::string GetText() const
	{
		return mText;
	}

	void SetText(const std::string& Text)
	{
		mText = Text;
	}

	PColor GetColor() const
	{
		return mColor;
	}

	void SetColor(const PColor& Color)
	{
		mColor = Color;
	}
};
