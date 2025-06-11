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
	const auto AbsFileName = Files::FindFile(FileName);
	if (AbsFileName.empty())
	{
		LogError("File not found: {}", FileName.c_str());
		return nullptr;
	}
	LogDebug("Loading texture: {}", AbsFileName.c_str());

	PTexture Tex;
	Tex.mFileName = AbsFileName;
	const auto Data =
		stbi_load(Tex.mFileName.c_str(), &Tex.mWidth, &Tex.mHeight, &Tex.mChannels, 4);
	const auto DataSize = Tex.mWidth * Tex.mHeight * 4 * sizeof(uint8_t);
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
		const auto Width = V->GetWidth();
		const auto Height = V->GetHeight();
		const auto Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888,
											   SDL_TEXTUREACCESS_STATIC, Width, Height);
		if (!Texture)
		{
			LogError("Unable to create texture: {}", SDL_GetError());
			continue;
		}
		SDL_Rect Source(0, 0, Width, Height);
		if (SDL_UpdateTexture(Texture, &Source, V->GetData(), V->GetPitch()))
		{
			V->mSDLTexture = Texture;
		}
	}
}
void PTextureManager::UnloadSDL()
{
	LogDebug("Destroying all SDL textures");
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