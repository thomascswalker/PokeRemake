#pragma once

#include "Application/Context.h"

#include "Core/Rect.h"

#include <vector>

class PRenderer
{
	SDLContext*					mContext;
	SDL_GPUGraphicsPipeline*	mGeometryPipeline;
	SDL_GPUGraphicsPipeline*	mLinePipeline;
	std::vector<SDL_GPUShader*> mShaders;

	float mZoomFactor = 10.0f;

public:
	explicit PRenderer(SDLContext* InContext)
		: mContext(InContext), mGeometryPipeline(nullptr), mLinePipeline(nullptr)
	{
	}

	bool Initialize();
	void Uninitialize() const;

	/* SDL_GPU */

	bool					 Initialize3D();
	void					 Render3D() const;
	SDL_GPUGraphicsPipeline* CreatePipeline(SDL_GPUPrimitiveType PrimitiveType, SDL_GPUFillMode FillMode,
				   const SDL_GPUGraphicsPipelineTargetInfo& TargetInfo) const;

	/* Standard SDL */

	void Render2D() const;

	/* Drawing */

	void Render() const;
	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;

	/* Misc */

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mContext->Renderer); }
	int32_t		GetScreenWidth() const;
	int32_t		GetScreenHeight() const;
};