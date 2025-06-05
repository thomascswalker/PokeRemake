// https://github.com/TheSpydog/SDL_gpu_examples/tree/main

#include "Renderer.h"

#include "Core/Logging.h"
#include "Engine/Game.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_test_common.h"
#include "Shader.h"

bool PRenderer::Initialize()
{
	if (mContext->IsGPU())
	{
		Initialize3D();
	}
	return true; // Always true (2D is already initialized in SDLContext)
}

void PRenderer::PostInitialize() const
{
	PTextureManager::LoadSDL(mContext->Renderer);
}

bool PRenderer::Initialize3D()
{
	const auto FormatFlags =
		SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;

	LogInfo("Creating {} GPU Device", mContext->GPUMode.c_str());
	mContext->Device = SDL_CreateGPUDevice(FormatFlags, true, mContext->GPUMode.c_str());
	if (!mContext->Device)
	{
		LogError("Failed to create GPU Device: {}", SDL_GetError());
		return false;
	}

	if (!SDL_ClaimWindowForGPUDevice(mContext->Device, mContext->Window))
	{
		LogError("Failed to claim window for GPU Device: {}", SDL_GetError());
		return false;
	}

	LogInfo("Created GPU device.");

	// Compile shaders
	if (!CompileShaders(mContext->Device))
	{
		LogError("Failed to compile shaders.");
		return false;
	}
	// Load shaders
	std::vector<SDL_GPUShader*> Shaders;
	if (!LoadShaders(mContext->Device, &Shaders))
	{
		LogError("Failed to compile shaders.");
		return false;
	}
	LogDebug("Loaded {} shaders.", Shaders.size());

	const SDL_GPUTextureFormat TextureFormat =
		SDL_GetGPUSwapchainTextureFormat(mContext->Device, mContext->Window);
	SDL_GPUColorTargetDescription TextureFormats[] = { { .format = TextureFormat } };
	SDL_GPUGraphicsPipelineCreateInfo PipelineInfo = {
		.vertex_shader = Shaders[1],
		.fragment_shader = Shaders[0],
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.target_info = {
			.color_target_descriptions = TextureFormats,
			.num_color_targets = 1,
		},
	};

	PipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	mPipeline = SDL_CreateGPUGraphicsPipeline(mContext->Device, &PipelineInfo);
	ReleaseShaders(mContext->Device, &Shaders);
	if (!mPipeline)
	{
		LogError("Failed to create GPU GraphicsPipeline: {}", SDL_GetError());
		return false;
	}

	// Release shaders once they've been loaded into the pipeline
	return true;
}

void PRenderer::Uninitialize() const
{
	PTextureManager::UnloadSDL();
	SDL_ReleaseWindowFromGPUDevice(mContext->Device, mContext->Window);
}

void PRenderer::Render()
{
	mContext->IsGPU() ? Render3D() : Render2D();
}

// Main render function
void PRenderer::Render3D()
{
	const auto CommandBuffer = SDL_AcquireGPUCommandBuffer(mContext->Device);
	if (CommandBuffer == nullptr)
	{
		LogError("AcquireGPUCommandBuffer: {}", SDL_GetError());
		return;
	}

	SDL_GPUTexture* SwapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(CommandBuffer, mContext->Window, &SwapchainTexture,
											   nullptr, nullptr))
	{
		LogError("WaitAndAcquireGPUSwapchainTexture: {}", SDL_GetError());
		return;
	}

	if (!SwapchainTexture)
	{
		LogError("SwapchainTexture is null.");
		return;
	}
	const SDL_GPUColorTargetInfo ColorTargetInfo = {
		.texture = SwapchainTexture,
		.clear_color = { 0.1f, 0.1f, 0.1f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};

	const auto RenderPass = SDL_BeginGPURenderPass(CommandBuffer, &ColorTargetInfo, 1, nullptr);
	SDL_BindGPUGraphicsPipeline(RenderPass, mPipeline);

	const auto			  Width = GetScreenWidth();
	const auto			  Height = GetScreenHeight();
	const SDL_GPUViewport Viewport = {
		0,		// Left
		0,		// Top
		Width,	// Right
		Height, // Bottom
		0.1f,	// Min depth
		1.0f,	// Max depthw
	};
	SDL_SetGPUViewport(RenderPass, &Viewport);

	SDL_DrawGPUPrimitives(RenderPass, 3, 1, 0, 0);
	SDL_EndGPURenderPass(RenderPass);

	SDL_SubmitGPUCommandBuffer(CommandBuffer);
}

void PRenderer::Render2D() const
{
	SDL_SetRenderDrawColor(mContext->Renderer, 38, 38, 38, 255);
	SDL_RenderClear(mContext->Renderer);

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (const IDrawable* Drawable : World->GetDrawables(DP_BACKGROUND))
		{
			Drawable->Draw(this);
		}
		for (const IDrawable* Drawable : World->GetDrawables(DP_FOREGROUND))
		{
			Drawable->Draw(this);
		}
	}

	if (GetSettings()->bDebugDraw)
	{
		SetDrawColor(0, 128, 255, 255);
		DrawPointAt({ 0, 0 }, 4);
	}
	SDL_RenderPresent(mContext->Renderer);
}

FVector2 PRenderer::WorldToScreen(const FVector2& Position) const
{
	const auto ScreenSize = GetScreenSize();
	const auto ViewPosition = GetCameraView()->GetPosition();
	const auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
	const auto Offset = Position - ViewPosition2D;

	return (Offset + ScreenSize) * 0.5f;
}

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const
{
	SDL_SetRenderDrawColor(mContext->Renderer, R, G, B, A);
	if (A < 255) // Is there transparency?
	{
		SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_BLEND);
	}
	else
	{
		SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_NONE);
	}
}

void PRenderer::DrawPoint(const FVector2& V, float Thickness) const
{
	if (Thickness > 0.0f)
	{
		SDL_FRect R = { V.X - (Thickness / 2.0f), V.Y - (Thickness / 2.0f), Thickness, Thickness };
		SDL_RenderFillRect(mContext->Renderer, &R);
	}
	else
	{
		SDL_RenderPoint(mContext->Renderer, V.X, V.Y);
	}
}

void PRenderer::DrawLine(float X1, float Y1, float X2, float Y2) const
{
	SDL_RenderLine(mContext->Renderer, X1, Y1, X2, Y2);
}

void PRenderer::DrawRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderRect(mContext->Renderer, &SRect);
}

void PRenderer::DrawFillRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderFillRect(mContext->Renderer, &SRect);
}

void PRenderer::DrawPolygon(const std::vector<FVector2>& Vertices,
							const std::vector<int32_t>&	 Indexes) const
{
	float R, G, B, A;
	SDL_GetRenderDrawColorFloat(mContext->Renderer, &R, &G, &B, &A);
	std::vector<SDL_Vertex> SDLVertices;
	for (size_t i = 0; i < Vertices.size(); ++i)
	{
		SDL_Vertex V;
		V.position.x = Vertices[i].X;
		V.position.y = Vertices[i].Y;
		V.color = { R, G, B, A };
		SDLVertices.emplace_back(V);
	}
	SDL_RenderGeometry(mContext->Renderer, nullptr, SDLVertices.data(),
					   static_cast<int>(SDLVertices.size()), Indexes.data(),
					   static_cast<int>(Indexes.size()));
}
void PRenderer::DrawGrid() const
{
	SetDrawColor(100, 100, 100, 255);

	const float ScreenWidth = GetScreenWidth();
	const float ScreenHeight = GetScreenHeight();

	float X0, X1, Y0, Y1;

	// Draw vertical lines
	for (int32_t Index = 0; Index <= TILE_COLUMNS; Index++)
	{
		X0 = Index * HALF_TILE_SIZE;
		X1 = X0;
		Y0 = 0;
		Y1 = ScreenHeight;
		DrawLine(X0, Y0, X1, Y1);
	}

	// Draw horizontal lines
	for (int32_t Index = 0; Index <= TILE_COLUMNS; Index++)
	{
		X0 = 0;
		X1 = ScreenWidth;
		Y0 = Index * HALF_TILE_SIZE;
		Y1 = Y0;
		DrawLine(X0, Y0, X1, Y1);
	}
}

void PRenderer::DrawPointAt(const FVector2& Position, float Thickness) const
{
	auto ScreenPosition = WorldToScreen(Position);
	DrawPoint(ScreenPosition, Thickness);
}

void PRenderer::DrawLineAt(const FVector2& Start, const FVector2& End) const
{
	auto ScreenStart = WorldToScreen(Start);
	auto ScreenEnd = WorldToScreen(End);
	SDL_RenderLine(mContext->Renderer, ScreenStart.X, ScreenStart.Y, ScreenEnd.X, ScreenEnd.Y);
}

void PRenderer::DrawRectAt(const FRect& Rect, const FVector2& Position) const
{
	auto ScreenPosition = WorldToScreen(Position);
	DrawRect({ ScreenPosition.X, ScreenPosition.Y, Rect.W, Rect.H });
}

void PRenderer::DrawFillRectAt(const FRect& Rect, const FVector2& Position) const
{
	auto ScreenPosition = WorldToScreen(Position);
	DrawFillRect({ ScreenPosition.X, ScreenPosition.Y, Rect.W, Rect.H });
}

void PRenderer::DrawTextureAt(PTexture* Texture, const FRect& Rect, const FVector2& Position) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	auto ScreenPosition = WorldToScreen(Position);
	auto Min = Rect.Min() + ScreenPosition;
	auto Max = Rect.Max() + ScreenPosition;

	SDL_FRect Source = { 0, 0, 16, 16 };
	SDL_FRect Dest = { Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y };

	SDL_RenderTexture(mContext->Renderer, Tex, &Source, &Dest);
}
void PRenderer::DrawSpriteAt(PTexture* Texture, const FRect& Rect, const FVector2& Position,
							 int32_t Index) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	auto ViewPosition = GetCameraView()->GetPosition();
	auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
	auto Offset = Position - ViewPosition2D;
	auto ScreenSize = GetScreenSize();

	auto Min = Rect.Min() + Offset;
	auto Max = Rect.Max() + Offset;

	// Camera position
	Min = (Min + ScreenSize) * 0.5f;
	Max = (Max + ScreenSize) * 0.5f;

	float	  SourceOffset = Index * SPRITE_WIDTH; // Assuming each sprite is 16x16 pixels
	SDL_FRect Source = { SourceOffset, 0, SPRITE_WIDTH, SPRITE_WIDTH };
	SDL_FRect Dest = { Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y };

	SDL_RenderTexture(mContext->Renderer, Tex, &Source, &Dest);
}

float PRenderer::GetScreenWidth() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Width;
}

float PRenderer::GetScreenHeight() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Height;
}

FVector2 PRenderer::GetScreenSize() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return { static_cast<float>(Width), static_cast<float>(Height) };
}

FRect PRenderer::GetViewport() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return { 0, 0, static_cast<float>(Width), static_cast<float>(Height) };
}
