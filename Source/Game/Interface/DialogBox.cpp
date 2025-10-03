#include "DialogBox.h"

#include "Core/Font.h"

PDialogBox::PDialogBox()
{
	mTexture = PTextureManager::Get("DialogBox");
}

void PDialogBox::Draw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(255, 255, 255, 255);
	FRect Geometry = { 0, DIALOG_Y, DIALOG_WIDTH, DIALOG_HEIGHT };

	Renderer->DrawFillRect(Geometry);
	Renderer->DrawTexture(mTexture, mTexture->GetRect(), Geometry);

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