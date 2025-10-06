#include "Texture.h"

#include "Core/Files.h"
#include "Core/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG // generate user friendly error messages

#include <cstring>
#include <map>

#include "Renderer/Renderer.h"
#include "stb/stb_image.h"

TextureMap PTextureManager::sTextures = {};

uint32_t gNextTextureID = 0;

PTexture::PTexture()
	: mID(gNextTextureID++), mWidth(0), mHeight(0), mChannels(0), mData(nullptr),
	  mSDLTexture(nullptr) {}

PTexture::~PTexture() {}

/**
 * @brief Load the specified texture file into memory.
 * @param FileName The absolute filepath to the texture.
 *
 * @return A pointer to the texture object, or null if the file was not found or is unreadable.
 */
PTexture* PTextureManager::Load(const std::string& FileName)
{
	int	  Width, Height, Channels;
	int	  DesiredChannelCount = 4;
	void* Data = stbi_load(FileName.c_str(), &Width, &Height, &Channels, DesiredChannelCount);

	if (!Data)
	{
		LogError("Unable to load texture '{}': {}", FileName.c_str(), stbi_failure_reason());
		return nullptr;
	}

	PTexture* NewTexture = Create(FileName, Width, Height, DesiredChannelCount, Data);
	if (!NewTexture)
	{
		return nullptr;
	}

	return NewTexture;
}

PTexture* PTextureManager::LoadMemory(const std::string& Name, void* Data, int32_t Width, int32_t Height, int32_t Channels)
{
	PTexture* NewTexture = Create(Name, Width, Height, Channels, Data);
	if (!NewTexture)
	{
		return nullptr;
	}
	return NewTexture;
}

void PTextureManager::LoadAllTextures()
{
	const auto Textures = Files::GetFilesInDirectory("Data/Sprites", true);
	for (const auto& Texture : Textures)
	{
		Load(Texture);
	}
}

bool PTextureManager::LoadSDL(PTexture* Texture)
{
	const auto Renderer = GRenderer->GetSDLRenderer();
	const auto Width = Texture->GetWidth();
	const auto Height = Texture->GetHeight();
	const auto SDLTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888,
											  SDL_TEXTUREACCESS_STATIC, Width, Height);

	if (!SDLTexture)
	{
		LogError("Unable to create texture: {}", SDL_GetError());
		return false;
	}

	SDL_Rect Source(0, 0, Width, Height);
	if (SDL_UpdateTexture(SDLTexture, &Source, Texture->GetData(), Texture->GetPitch()))
	{
		Texture->mSDLTexture = SDLTexture;
		return true;
	}
	LogError("Unable to update SDL texture: {}", SDL_GetError());
	return false;
}

void PTextureManager::UnloadSDL()
{
	for (const auto& V : GetTextures() | std::views::values)
	{
		SDL_DestroyTexture(V->GetSDLTexture());
	}
}

PTexture* PTextureManager::Get(const std::string& Name)
{
	for (auto [Key, Value] : GetTextures())
	{
		if (Key == Name)
		{
			return sTextures.at(Name).get();
		}
		if (Value->GetName() == Name)
		{
			return Value.get();
		}
	}

	LogWarning("Texture {} not found.", Name.c_str());
	return nullptr;
}

PTexture* PTextureManager::Create(const std::string& FileName, float Width, float Height, int Channels, void* Data)
{
	PTexture   Tex;
	const auto DataSize = Width * Height * Channels;
	Tex.mData = static_cast<uint8_t*>(malloc(DataSize));
	memcpy(Tex.mData, Data, DataSize);

	if (!Tex.mData)
	{
		LogError("Unable to allocate memory for texture: {}", FileName.c_str());
		return nullptr;
	}

	auto BaseName = Files::SplitExt(FileName);
	Tex.mName = BaseName;
	Tex.mPath = Files::DirName(FileName);
	Tex.mWidth = Width;
	Tex.mHeight = Height;
	Tex.mChannels = Channels;

	if (!LoadSDL(&Tex))
	{
		return nullptr;
	}
	free(Data);
	sTextures[BaseName] = std::make_shared<PTexture>(Tex);
	return sTextures[BaseName].get();
}

void PTextureManager::Destroy(const PTexture* Texture)
{
	auto Iter = sTextures.find(Texture->GetName());
	if (Iter != sTextures.end())
	{
		sTextures.erase(Iter);
	}
	else
	{
		LogError("Unable to find texture: {}", Texture->GetName().c_str());
	}
}

TextureMap& PTextureManager::GetTextures()
{
	return sTextures;
}
