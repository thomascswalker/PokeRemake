#include "TextRenderer.h"

#include <wchar.h>

#include "Engine/Texture.h"
#include "Renderer/Renderer.h"

#include "GameConstants.h"

PTexture*		GFontTexture = nullptr;
static FVector2 GGlyphRenderSize(GLYPH_WIDTH, GLYPH_HEIGHT);

std::map<std::u16string, int> GCharIndexes = {
	{					  u"A",	0 },
	{					  u"B",	1 },
	{					  u"C",	2 },
	{					  u"D",	3 },
	{					  u"E",	4 },
	{					  u"F",	5 },
	{					  u"G",	6 },
	{					  u"H",	7 },
	{					  u"I",	8 },
	{					  u"J",	9 },
	{					  u"K", 10 },
	{					  u"L", 11 },
	{					  u"M", 12 },
	{					  u"N", 13 },
	{					  u"O", 14 },
	{					  u"P", 15 },
	{					  u"Q", 16 },
	{					  u"R", 17 },
	{					  u"S", 18 },
	{					  u"T", 19 },
	{					  u"U", 20 },
	{					  u"V", 21 },
	{					  u"W", 22 },
	{					  u"X", 23 },
	{					  u"Y", 24 },
	{					  u"Z", 25 },
	{					  u"(", 26 },
	{					  u")", 27 },
	{					  u":", 28 },
	{					  u";", 29 },
	{					  u"[", 30 },
	{					  u"]", 31 },
	{					  u"a", 32 },
	{					  u"b", 33 },
	{					  u"c", 34 },
	{					  u"d", 35 },
	{					  u"e", 36 },
	{					  u"f", 37 },
	{					  u"g", 38 },
	{					  u"h", 39 },
	{					  u"i", 40 },
	{					  u"j", 41 },
	{					  u"k", 42 },
	{					  u"l", 43 },
	{					  u"m", 44 },
	{					  u"n", 45 },
	{					  u"o", 46 },
	{					  u"p", 47 },
	{					  u"q", 48 },
	{					  u"r", 49 },
	{					  u"s", 50 },
	{					  u"t", 51 },
	{					  u"u", 52 },
	{					  u"v", 53 },
	{					  u"w", 54 },
	{					  u"x", 55 },
	{					  u"y", 56 },
	{					  u"z", 57 },
	{					 u"\'", 64 },
	{				   CHAR_PK, 65 }, // PK
	{				   CHAR_MN, 66 }, // MN
	{					  u"-", 67 },
	{					  u"?", 70 },
	{					  u"!", 71 },
	{					  u".", 72 },
	{ CHAR_RIGHT_ARROW_SELECTED, 76 }, // Right Arrow Selected
	{		  CHAR_RIGHT_ARROW, 77 }, // Right Arrow Selection
	{		   CHAR_DOWN_ARROW, 78 }, // Down Arrow
	{					  u"0", 86 },
	{					  u"1", 87 },
	{					  u"2", 88 },
	{					  u"3", 89 },
	{					  u"4", 90 },
	{					  u"5", 91 },
	{					  u"6", 92 },
	{					  u"7", 93 },
	{					  u"8", 94 },
	{					  u"9", 95 },
};

void TextRenderer::LoadFontData()
{
	std::string FileName = "Font.png";
	GFontTexture = GTextureManager->Get("Font");
	if (!GFontTexture)
	{
		LogError("Failed to load font texture");
	}
}

FRect TextRenderer::GetCharRect(char16_t C)
{

	int Index = 0;
	for (auto& [Key, Value] : GCharIndexes)
	{
		if (*Key.data() == C)
		{
			Index = Value;
			break;
		}
	}
	float X = Index % FONT_COL_COUNT;
	float Y = Index / FONT_COL_COUNT;
	return { X * 8, Y * 8, 8, 8 };
}

bool TextRenderer::DrawChar(char16_t C, const FVector2& Position)
{
	auto  Source = GetCharRect(C);
	FRect Dest{ Position, GGlyphRenderSize };
	GRenderer->DrawTexture(GFontTexture, Source, Dest);
	return true;
}
bool TextRenderer::DrawText(const std::string& Text, const FVector2& Position)
{
	std::u16string Text16 = Converter.from_bytes(Text);
	return DrawText(Text16, Position);
}

bool TextRenderer::DrawText(const std::u16string& Text, const FVector2& Position)
{
	int XOffset = 0;
	int YOffset = 0;

	for (auto C : Text)
	{
		if (C == '\n')
		{
			XOffset = 0;
			YOffset = 2; // Skip a row
			continue;
		}
		if (C == ' ')
		{
			XOffset++;
			continue;
		}
		FVector2 Offset = GGlyphRenderSize * FVector2(XOffset, YOffset);
		DrawChar(C, Position + Offset);
		XOffset++;
	}

	return true;
}