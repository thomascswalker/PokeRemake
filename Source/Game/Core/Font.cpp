#include "Font.h"

#include "Engine/Texture.h"
#include "Renderer/Renderer.h"

#include "GameConstants.h"

PTexture*		GFontTexture = nullptr;
static FVector2 GGlyphRenderSize(GLYPH_WIDTH, GLYPH_HEIGHT);

std::map<char, int> GCharIndexes = {
	{	  'A',  0 },
	{	  'B',  1 },
	{	  'C',  2 },
	{	  'D',  3 },
	{	  'E',  4 },
	{	  'F',  5 },
	{	  'G',  6 },
	{	  'H',  7 },
	{	  'I',  8 },
	{	  'J',  9 },
	{	  'K', 10 },
	{	  'L', 11 },
	{	  'M', 12 },
	{	  'N', 13 },
	{	  'O', 14 },
	{	  'P', 15 },
	{	  'Q', 16 },
	{	  'R', 17 },
	{	  'S', 18 },
	{	  'T', 19 },
	{	  'U', 20 },
	{	  'V', 21 },
	{	  'W', 22 },
	{	  'X', 23 },
	{	  'Y', 24 },
	{	  'Z', 25 },
	{	  '(', 26 },
	{	  ')', 27 },
	{	  ':', 28 },
	{	  ';', 29 },
	{	  '[', 30 },
	{	  ']', 31 },
	{	  'a', 32 },
	{	  'b', 33 },
	{	  'c', 34 },
	{	  'd', 35 },
	{	  'e', 36 },
	{	  'f', 37 },
	{	  'g', 38 },
	{	  'h', 39 },
	{	  'i', 40 },
	{	  'j', 41 },
	{	  'k', 42 },
	{	  'l', 43 },
	{	  'm', 44 },
	{	  'n', 45 },
	{	  'o', 46 },
	{	  'p', 47 },
	{	  'q', 48 },
	{	  'r', 49 },
	{	  's', 50 },
	{	  't', 51 },
	{	  'u', 52 },
	{	  'v', 53 },
	{	  'w', 54 },
	{	  'x', 55 },
	{	  'y', 56 },
	{	  'z', 57 },
	{	  '\'', 64 },
	{	  '0', 86 },
	{	  '1', 87 },
	{	  '2', 88 },
	{	  '3', 89 },
	{	  '4', 90 },
	{	  '5', 91 },
	{	  '6', 92 },
	{	  '7', 93 },
	{	  '8', 94 },
	{	  '9', 95 },
	{ CHAR_PK, 65 }, // PK
	{ CHAR_MN, 66 }, // MN
	{	  '>', 77 }, // Right Arrow
};

void PokeFont::LoadFontData()
{
	std::string FileName = "Font.png";
	GFontTexture = PTextureManager::Get("Font");
	if (!GFontTexture)
	{
		LogError("Failed to load font texture");
	}
}

FRect PokeFont::GetCharRect(uint8_t C)
{
	int	  Index = GCharIndexes[C];
	float X = Index % FONT_COL_COUNT;
	float Y = Index / FONT_COL_COUNT;
	return { X * 8, Y * 8, 8, 8 };
}

bool PokeFont::DrawChar(uint8_t C, const FVector2& Position)
{
	auto  Source = GetCharRect(C);
	FRect Dest{ Position, GGlyphRenderSize };
	GRenderer->DrawTexture(GFontTexture, Source, Dest);
	return true;
}

bool PokeFont::DrawText(const std::string& Text, const FVector2& Position)
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