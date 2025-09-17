#pragma once
#include "Object.h"
#include "Timer.h"
#include "World.h"

#define DIALOG_MAX_CHAR 6
#define DIALOG_SPEED	5

static TMap<std::string, std::string> gTextVars = {
	{ "${PLAYER}",  "ASH" },
	{  "${RIVAL}", "GARY" }
};

class PTextPrinter : public PObject
{
	std::string mText{};
	size_t		mCursor = 0;
	FVector2	mOffset;
	bool		mPrinting = false;
	float		mAnimationDelta = 0.0f;
	float		mAnimationSpeed = 0.05f;

public:
	void StartTimer()
	{
		mAnimationDelta = 0;
		mPrinting = true;
	}

	void EndTimer()
	{
		mAnimationDelta = 0;
		mCursor = 0;
		mPrinting = false;
	}

	void Tick(float DeltaTime) override
	{
		if (!mPrinting)
		{
			return;
		}
		mAnimationDelta += DeltaTime;
		if (mAnimationDelta > mAnimationSpeed)
		{
			NextChar();
			mAnimationDelta = 0;
		}
	}

	void Reset()
	{
		mCursor = 0;
		mText.clear();
		mOffset = { 0, 0 };
	}

	void NextChar()
	{
		if (mCursor >= mText.size())
		{
			return;
		}

		mCursor++;
	}

	char GetChar() const { return mText[mCursor]; }

	void SetText(const std::string& Text)
	{
		auto FormattedText = Text;
		for (auto& [K, V] : gTextVars)
		{
			FormattedText = String::Replace(FormattedText, K, V);
		}
		mText = FormattedText;
	}
	std::string GetText() const { return mText; }
	std::string GetDisplayText() const { return mText.substr(0, mCursor); }

	FVector2 GetOffset() const { return mOffset; }
};