#include "DialogBox.h"

#include "Core/Font.h"
#include "Core/GameConstants.h"

#include "Frame.h"

PDialogBox::PDialogBox()
	: PFrame(FRect(DIALOG_X, DIALOG_Y, DIALOG_W, DIALOG_H))
{
}

void PDialogBox::Draw(const PRenderer* Renderer) const
{
	PFrame::Draw(Renderer);

	Renderer->SetDrawColor(0, 0, 0, 255);
	auto TextPosition = FVector2(DIALOG_LINE_X, DIALOG_LINE1_Y);
	PokeFont::DrawText(mPrinter.GetDisplayText(), TextPosition);
}

void PDialogBox::Tick(float DeltaTime)
{
	PWidget::Tick(DeltaTime);
	mPrinter.Tick(DeltaTime);
}

void PDialogBox::SetText(const std::string& Text)
{
	mPrinter.SetText(Text);
}

void PDialogBox::Print()
{
	mPrinter.Play();
}

void PDialogBox::EndPrint()
{
	mPrinter.Stop();
}