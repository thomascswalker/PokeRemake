#include "World.h"

#include "Engine/InputManager.h"

/* Grid */

PGrid::PGrid(const uint32_t InX, const uint32_t InY, const uint32_t InCellSize)
	: mSizeX(InX), mSizeY(InY), mCellSize(InCellSize), mOffsetX(0), mOffsetY(0)
{
	if (IInputManager* InputManager = GetInputManager())
	{
		InputManager->KeyDown.AddRaw(this, &PGrid::OnKeyDown);
		InputManager->KeyUp.AddRaw(this, &PGrid::OnKeyUp);
	}
}

void PGrid::Tick(float DeltaTime)
{
	const float Speed = 0.25f * DeltaTime;
	if (mKeysDown[0]) // Right
	{
		AddOffsetX(-Speed);
	}
	if (mKeysDown[1]) // Left
	{
		AddOffsetX(Speed);
	}
	if (mKeysDown[2]) // Down
	{
		AddOffsetY(-Speed);
	}
	if (mKeysDown[3]) // Up
	{
		AddOffsetY(Speed);
	}
}

void PGrid::Draw(IRenderer* Renderer)
{
	const auto HalfScreenWidth = Renderer->GetScreenWidth() / 2;
	const auto HalfScreenHeight = Renderer->GetScreenHeight() / 2;

	const uint32_t Width = mSizeX * mCellSize;
	const uint32_t Height = mSizeY * mCellSize;
	const uint32_t MinX = HalfScreenWidth - Width / 2;
	const uint32_t MinY = HalfScreenHeight - Height / 2;
	const uint32_t MaxX = HalfScreenWidth + Width / 2;
	const uint32_t MaxY = HalfScreenHeight + Height / 2;

	for (uint32_t X = 0; X <= mSizeX; X++)
	{
		Renderer->DrawLine(					 //
			mOffsetX + MinX + X * mCellSize, //
			mOffsetY + MinY,				 //
			mOffsetX + MinX + X * mCellSize, //
			mOffsetY + MaxY					 //
		);
	}

	for (uint32_t Y = 0; Y <= mSizeY; Y++)
	{
		Renderer->DrawLine(					 //
			mOffsetX + MinX,				 //
			mOffsetY + MinY + Y * mCellSize, //
			mOffsetX + MaxX,				 //
			mOffsetY + MinY + Y * mCellSize	 //
		);
	}
}
void PGrid::OnKeyDown(uint32_t ScanCode)
{
	switch (ScanCode)
	{
		case KB_Right: // Right
			mKeysDown[0] = true;
			break;
		case KB_Left: // Left
			mKeysDown[1] = true;
			break;
		case KB_Down: // Down
			mKeysDown[2] = true;
			break;
		case KB_Up: // Up
			mKeysDown[3] = true;
			break;
		default:
			break;
	}
}
void PGrid::OnKeyUp(uint32_t ScanCode)
{
	switch (ScanCode)
	{
		case KB_Right: // Right
			mKeysDown[0] = false;
			break;
		case KB_Left: // Left
			mKeysDown[1] = false;
			break;
		case KB_Down: // Down
			mKeysDown[2] = false;
			break;
		case KB_Up: // Up
			mKeysDown[3] = false;
			break;
		default:
			break;
	}
}

/* World */

PWorld::PWorld() { mGrid = std::make_unique<PGrid>(25, 25, 16); }
