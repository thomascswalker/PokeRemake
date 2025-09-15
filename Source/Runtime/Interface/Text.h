#pragma once

#include "Widget.h"

class PText : public PWidget
{
protected:
	std::string mText = "";
	PColor		mColor = PColor::UIText;
	float		mFontSize = FONT_RENDER_SCALE;
	EAlignment	mAlignment = AL_Center;
	bool		mLineBreak = false;
	int32_t		mLineBreakPos = 0;

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
		Renderer->SetClipRect(GetGeometry());
#if _EDITOR
		std::string Text = Param == nullptr ? mText : Param->Get<std::string>();
#else
		std::string Text = mText;
#endif
		Renderer->SetDrawColor(mColor);
		float	 HalfHeight = H / 2.0f;
		float	 HalfWidth = W / 2.0f;
		float	 QuarterFontSize = mFontSize / 4.0f;
		FVector2 TextPos = { X + 2, Y + HalfHeight + QuarterFontSize };
		auto	 TextWidth = Renderer->GetTextWidth(Text, mFontSize);

		float HalfTextWidth = TextWidth / 2.0f;
		switch (mAlignment)
		{
			case AL_Left:
				{
					// TODO: Figure out left alignment
					TextPos.X += 5;
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

		if (!mLineBreak)
		{
			Renderer->DrawText(Text, TextPos, mFontSize);
		}
		else
		{
			int32_t Pos = 0;
			while (Pos < mText.size())
			{
				auto SubText = mText.substr(Pos, mLineBreakPos);
				Renderer->DrawText(SubText, TextPos, mFontSize);

				// Offset text cursor pos
				Pos += mLineBreakPos;
				// Offset text draw Y to the next line
				TextPos.Y += mFontSize;
			}
		}
		Renderer->ReleaseClipRect();
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

	void Add(char C, size_t Pos = std::string::npos)
	{
		if (Pos == std::string::npos)
		{
			mText.append(1, C);
		}
		else
		{
			mText.insert(Pos, 1, C);
		}
	}

	void Remove(size_t Pos = std::string::npos)
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
	}

	bool	GetLineBreak() const { return mLineBreak; }
	void	SetLineBreak(bool LineBreak) { mLineBreak = LineBreak; }
	int32_t GetLineBreakPos() const { return mLineBreakPos; }
	void	SetLineBreakPos(int32_t Pos) { mLineBreakPos = Pos; }

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

	std::string* GetTextAddress()
	{
		return &mText;
	}
};
