#pragma once
#include "Interface/Widget.h"

struct SFrameTile
{
	FRect Source;
	FRect Dest;
};

class PFrame : public PWidget
{
protected:
	PTexture*				mTexture = nullptr;
	std::vector<SFrameTile> mFrameTiles;

	void Initialize(uint32_t SizeX, uint32_t SizeY);

public:
	PFrame(const FRect& InSize);
	void Draw(const PRenderer* Renderer) const override;
};