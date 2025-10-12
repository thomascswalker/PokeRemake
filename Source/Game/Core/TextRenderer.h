#pragma once

#include <bitset>

#include "Engine/Texture.h"

namespace TextRenderer
{
	inline std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> Converter;

	constexpr int GlyphByteSize = 8;
	constexpr int GlyphBitSize = GlyphByteSize * GlyphByteSize;
	constexpr int GlyphCount = 128;
	constexpr int TotalByteSize = GlyphByteSize * GlyphCount;
	constexpr int TotalBitSize = TotalByteSize * 8;

	using TGlyph = std::bitset<GlyphByteSize>;

	void  LoadFontData();
	FRect GetCharRect(char16_t C);
	void  DrawChar(char16_t C, const FVector2& Position, float Scale = 1.0f);
	void  DrawText(const std::string& Text, const FVector2& Position, float Scale = 1.0f);
	void  DrawText(const std::u16string& Text, const FVector2& Position, float Scale = 1.0f);
} // namespace TextRenderer

extern PTexture* GFontTexture;