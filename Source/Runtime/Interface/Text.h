#pragma once

#include "Widget.h"

class PText : public PWidget
{
	std::string mText     = "";
	PColor mColor         = PColor::UIText;
	float mFontSize       = FONT_RENDER_SCALE;
	EAlignment mAlignment = AL_Center;

public:
	PText() = default;

	explicit PText(const std::string& Text, float FontSize = FONT_RENDER_SCALE, const PColor& Color = PColor::UIText)
		: mText(Text), mColor(Color), mFontSize(FontSize)
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Grow;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(mColor);
		float HalfHeight      = H / 2.0f;
		float HalfWidth       = W / 2.0f;
		float QuarterFontSize = mFontSize / 4.0f;
		FVector2 TextPos      = {X, Y + HalfHeight + QuarterFontSize};
		auto TextWidth        = Renderer->GetTextWidth(mText, mFontSize);

		float HalfTextWidth = TextWidth / 2.0f;
		switch (mAlignment)
		{
		case AL_Left:
			{
				break;
			}
		case AL_Center:
			{
				TextPos.X += HalfWidth - HalfTextWidth;
				break;
			}
		case AL_Right:
			{
				TextPos.X += W;
				TextPos.X -= TextWidth;
				break;
			}
		}
		Renderer->DrawText(mText, TextPos, mFontSize);
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

	void AddChar(char C, size_t Pos = std::string::npos)
	{
		if (Pos == std::string::npos)
		{
			mText.append(1, C);
		}
		else
		{
			mText.insert(Pos, 1, C);
		}
		LogInfo("{}", mText.c_str());
	}

	void RemoveChar(size_t Pos = std::string::npos)
	{
		if (mText.empty())
		{
			return;
		}
		if (Pos == std::string::npos)
		{
			mText.pop_back();
		}
		else
		{
			mText.erase(Pos, 1);
		}
		LogInfo("{}", mText.c_str());
	}

	PColor GetColor() const
	{
		return mColor;
	}

	void SetColor(const PColor& Color)
	{
		mColor = Color;
	}

	EAlignment GetAlignment() const
	{
		return mAlignment;
	}

	void SetAlignment(EAlignment Alignment)
	{
		mAlignment = Alignment;
	}
};
