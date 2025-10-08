#pragma once

#include "Engine/TextPrinter.h"
#include "Engine/World.h"
#include "Interface/Box.h"

#include "Frame.h"

class PDialogBox : public PFrame
{
	PTextPrinter mPrinter;
	PTexture*	 mTexture = nullptr;

public:
	PDialogBox();

	void Draw(const PRenderer* Renderer) const override;
	void Tick(float DeltaTime) override;

	void SetText(const std::string& Text);
	void Print();
	void EndPrint();
};
