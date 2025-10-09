#pragma once

#include <bitset>

#include "Engine/Texture.h"

namespace TextRenderer
{
	constexpr int GlyphByteSize = 8;
	constexpr int GlyphBitSize = GlyphByteSize * GlyphByteSize;
	constexpr int GlyphCount = 128;
	constexpr int TotalByteSize = GlyphByteSize * GlyphCount;
	constexpr int TotalBitSize = TotalByteSize * 8;

	using TGlyph = std::bitset<GlyphByteSize>;

	void  LoadFontData();
	FRect GetCharRect(uint8_t C);
	bool  DrawChar(uint8_t C, const FVector2& Position);
	bool  DrawText(const std::string& Text, const FVector2& Position);
} // namespace TextRenderer

extern PTexture* GFontTexture;