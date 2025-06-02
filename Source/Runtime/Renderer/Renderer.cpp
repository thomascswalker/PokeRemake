// https://github.com/TheSpydog/SDL_gpu_examples/tree/main

#include "Renderer.h"

#include "Core/Logging.h"
#include "Engine/Game.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "SDL3/SDL_surface.h"
#include "Shader.h"

bool PRenderer::Initialize()
{
	if (mContext->IsGPU())
	{
		Initialize3D();
	}
	return true; // Always true (2D is already initialized in SDLContext)
}

void PRenderer::PostInitialize()
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

void PRenderer::Render2D()
{
	SDL_SetRenderDrawColor(mContext->Renderer, 38, 38, 38, 255);
	SDL_RenderClear(mContext->Renderer);

	auto View = GetActiveCameraView();
	if (!View)
	{
		return;
	}

	auto	ViewMatrix = View->GetViewMatrix();
	FMatrix ProjMatrix;
	switch (View->GetViewMode())
	{
		case VM_Orthographic:
			{
				ProjMatrix = MakeOrthographicMatrix(0.0f, GetScreenWidth(), 0.0f, GetScreenHeight(),
													0.0f, 1.0f);
				break;
			}
		case VM_Perspective:
			{
				ProjMatrix = MakePerspectiveMatrix(
					View->GetFOV(), GetScreenWidth() / GetScreenHeight(), 0.1f, 100.0f);
				break;
			}
		default:
			LogError("Unsupported view mode.");
			return;
	}

	mMVP = ViewMatrix * ProjMatrix;

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (IDrawable* Drawable : World->GetDrawables())
		{
			Drawable->Draw(this);
		}
	}

	SDL_RenderPresent(mContext->Renderer);
}

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const
{
	SDL_SetRenderDrawColor(mContext->Renderer, R, G, B, A);
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

void PRenderer::DrawTextureAt(PTexture* Texture, const FRect& Rect, const FVector2& Position) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	auto ViewPosition = GetActiveCameraView()->GetPosition();
	auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
	auto Offset = Position - ViewPosition2D;
	auto ScreenSize = GetScreenSize();

	auto Min = Rect.Min() + Offset;
	auto Max = Rect.Max() + Offset;

	// Camera position
	Min = (Min + ScreenSize) * 0.5f;
	Max = (Max + ScreenSize) * 0.5f;

	SDL_FRect Source = { 0, 0, 16, 16 };
	SDL_FRect Dest = { Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y };

	SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_BLEND);
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

	auto ViewPosition = GetActiveCameraView()->GetPosition();
	auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
	auto Offset = Position - ViewPosition2D;
	auto ScreenSize = GetScreenSize();

	auto Min = Rect.Min() + Offset;
	auto Max = Rect.Max() + Offset;

	// Camera position
	Min = (Min + ScreenSize) * 0.5f;
	Max = (Max + ScreenSize) * 0.5f;

	float	  SourceOffset = Index * 16; // Assuming each sprite is 16x16 pixels
	SDL_FRect Source = { SourceOffset, 0, 16, 16 };
	SDL_FRect Dest = { Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y };

	SDL_SetRenderDrawColor(mContext->Renderer, 255, 255, 255, 255);
	SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_NONE);
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
