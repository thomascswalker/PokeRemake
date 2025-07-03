#pragma once
#include "Widget.h"

class PClipArea : public PWidget
{
	PTexture* mClipTexture;

public:
	PClipArea()
		: mClipTexture()
	{
		mClipTexture = PTextureManager::Create(mInternalName, 100, 100, nullptr);
	}
	~PClipArea() override
	{
		SDL_DestroyTexture(mClipTexture->GetSDLTexture());
		PTextureManager::Destroy(mClipTexture);
	}

	// Override because we are going to handle drawing this widget's
	// children a bit differently.
	void DrawChildren(const PRenderer* Renderer) const override {}
	void PreDraw(const PRenderer* Renderer) override
	{
		const auto SDLTex = Renderer->CreateRenderTarget(W, H);
		mClipTexture->SetSDLTexture(SDLTex);
		Renderer->SetRenderTarget(mClipTexture->GetSDLTexture());
	}
	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(PColor::Red);
		Renderer->DrawFillRect(GetGeometry());
	}
	void PostDraw(const PRenderer* Renderer) override
	{
		Renderer->ReleaseRenderTarget();
		Renderer->DrawTexture(mClipTexture, GetGeometry(), GetGeometry());
		SDL_DestroyTexture(mClipTexture->GetSDLTexture());
	}
};