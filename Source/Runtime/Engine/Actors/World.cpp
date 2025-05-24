#include "World.h"

/* Grid */

AGrid::AGrid(const uint32_t InX, const uint32_t InY, const uint32_t InCellSize)
	: mSizeX(InX), mSizeY(InY), mCellSize(InCellSize)
{
}

void AGrid::Draw(IRenderer* Renderer)
{
	const auto ScreenWidth = Renderer->GetScreenWidth();
	const auto ScreenHeight = Renderer->GetScreenHeight();

	for (uint32_t X = 0; X < ScreenWidth; X += mCellSize)
	{
		Renderer->DrawLine(X, 0, X, ScreenHeight);
	}

	for (uint32_t Y = 0; Y < ScreenHeight; Y += mCellSize)
	{
		Renderer->DrawLine(0, Y, ScreenWidth, Y);
	}
}

/* World */

AWorld::AWorld() { mGrid = std::make_unique<AGrid>(10, 10, 12); }
void AWorld::Draw(IRenderer* Renderer)
{
	if (mGrid)
	{
		mGrid->Draw(Renderer);
	}
}