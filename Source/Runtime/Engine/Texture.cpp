#include "Texture.h"
#include "Core/Files.h"
#include "Core/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG // generate user friendly error messages

#include "stb/stb_image.h"

#include <cstring>
#include <map>

std::map<std::string, std::shared_ptr<PTexture>> gTextures;

uint32_t gNextTextureID = 0;

PTexture::PTexture()
	: mID(gNextTextureID++), mWidth(0), mHeight(0), mChannels(0), mData(nullptr),
	  mSDLTexture(nullptr)
{
}

PTexture::~PTexture() {}

PTexture* PTextureManager::Load(const std::string& FileName)
{
	LogDebug("Finding texture: {}", FileName.c_str());
	auto AbsFileName = Files::FindFile(FileName);
	if (AbsFileName.empty())
	{
		LogError("File not found: {}", FileName.c_str());
		return nullptr;
	}
	LogDebug("Loading texture: {}", AbsFileName.c_str());

	PTexture Tex;
	Tex.mFileName = AbsFileName;
	auto Data = stbi_load(Tex.mFileName.c_str(), &Tex.mWidth, &Tex.mHeight, &Tex.mChannels, 4);
	auto DataSize = Tex.mWidth * Tex.mHeight * 4 * sizeof(int32_t);
	Tex.mData = static_cast<uint8_t*>(malloc(DataSize));
	std::memcpy(Tex.mData, Data, DataSize);
	stbi_image_free(Data);

	if (!Tex.mData)
	{
		LogError("Unable to load image: {}", stbi_failure_reason());
		return nullptr;
	}

	LogDebug("Texture {} loaded", Tex.mFileName.c_str());
	gTextures[FileName] = std::make_shared<PTexture>(Tex);

	return gTextures.at(FileName).get();
}

void PTextureManager::LoadSDL(SDL_Renderer* Renderer)
{
	for (const auto& [K, V] : GetTextures())
	{
		auto Surface = SDL_CreateSurfaceFrom(V->GetWidth(), V->GetHeight(),
											 SDL_PIXELFORMAT_RGBA8888, V->GetData(), V->GetPitch());
		if (!Surface)
		{
			LogError("Unable to create SDL surface: {}", SDL_GetError());
			return;
		}
		auto Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
		SDL_DestroySurface(Surface);
		if (!Texture)
		{
			LogError("Failed to create SDL texture: {}", SDL_GetError());
			return;
		}
		V->mSDLTexture = Texture;
	}
}
void PTextureManager::UnloadSDL()
{
	for (const auto& [K, V] : GetTextures())
	{
		SDL_DestroyTexture(V->GetSDLTexture());
	}
}

PTexture* PTextureManager::Get(const std::string& FileName)
{
	for (const auto& [K, V] : GetTextures())
	{
		if (V->GetFileName().ends_with(FileName))
		{
			return V.get();
		}
	}
	return nullptr;
}

std::map<std::string, std::shared_ptr<PTexture>> PTextureManager::GetTextures()
{
	return gTextures;
}