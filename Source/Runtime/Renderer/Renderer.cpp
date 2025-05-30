// https://github.com/TheSpydog/SDL_gpu_examples/tree/main

#include "Renderer.h"

#include "Core/Logging.h"
#include "Engine/World.h"
#include "Shader.h"

bool PRenderer::Initialize()
{
	return mContext->IsGPU() ? Initialize3D() // Initialize SDL_GPU based on the cmd arg '-r'
							 : true; // Always true (2D is already initialized in SDLContext)
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
	SDL_ReleaseWindowFromGPUDevice(mContext->Device, mContext->Window);
}

void PRenderer::Render() const
{
	mContext->IsGPU() ? Render3D() : Render2D();
}

// Main render function
void PRenderer::Render3D() const
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

	const auto			  Width = static_cast<float>(GetScreenWidth());
	const auto			  Height = static_cast<float>(GetScreenHeight());
	const SDL_GPUViewport Viewport = {
		0,		// Left
		0,		// Top
		Width,	// Right
		Height, // Bottom
		0.1f,	// Min depth
		1.0f,	// Max depth
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

int32_t PRenderer::GetScreenWidth() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Width;
}

int32_t PRenderer::GetScreenHeight() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Height;
}
