#pragma once

#include "Core/CoreFwd.h"

struct PColor
{
	union
	{
		struct
		{
			uint8_t R;
			uint8_t G;
			uint8_t B;
			uint8_t A;
		};

		uint8_t RGBA[4];
	};

	PColor()
		: R(0), G(0), B(0), A(0) {}

	explicit PColor(uint8_t InR, uint8_t InG, uint8_t InB)
		: R(InR), G(InG), B(InB), A(255) {}

	explicit PColor(uint8_t InR, uint8_t InG, uint8_t InB, uint8_t InA)
		: R(InR), G(InG), B(InB), A(InA) {}

	explicit PColor(uint8_t Value)
		: R(Value), G(Value), B(Value), A(Value) {}

	explicit PColor(const std::string& Hex)
	{
		std::string HexString = Hex;
		if (Hex.starts_with("#"))
		{
			HexString = Hex.substr(1);
		}

		const auto HexValue = std::strtol(HexString.c_str(), nullptr, 16);
		R                   = HexValue >> 16 & 0xFF;
		G                   = HexValue >> 8 & 0xFF;
		B                   = HexValue & 0xFF;
		A                   = 255;
	}

	PColor WithAlpha(uint8_t Alpha)
	{
		return PColor(R, G, B, Alpha);
	}

	// Standard colors

	static PColor Red;
	static PColor Green;
	static PColor Blue;
	static PColor White;
	static PColor Black;
	static PColor OffWhite;

	// Interface colors

	static PColor UIPrimary;
	static PColor UIPrimaryHover;
	static PColor UIPrimaryClicked;
	static PColor UISecondary;
	static PColor UISecondaryHover;
	static PColor UISecondaryClicked;
	static PColor UIBackground;
	static PColor UIText;
	static PColor UIPanel;
	static PColor UIBorder;
	static PColor UIDebug1;
	static PColor UIDebug2;
	static PColor UIDebug3;

	// Editor colors

	static PColor Selection;
	static PColor SelectionOpaque;
};
