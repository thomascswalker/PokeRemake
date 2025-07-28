#include "Texture.h"
#include "Core/Files.h"
#include "Core/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG // generate user friendly error messages

#include "Core/Containers.h"
#include "Renderer/Renderer.h"
#include "stb/stb_image.h"

#include <cstring>
#include <map>

TextureMap PTextureManager::sTextures = {};

uint32_t gNextTextureID = 0;

PTexture::PTexture()
	: mID(gNextTextureID++), mWidth(0), mHeight(0), mChannels(0), mData(nullptr),
	  mSDLTexture(nullptr) {}

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

	int Width, Height, Channels;
	int DesiredChannelCount = 4;
	void* Data              = stbi_load(AbsFileName.c_str(), &Width, &Height, &Channels, DesiredChannelCount);

	if (!Data)
	{
		LogError("Unable to load image: {}", stbi_failure_reason());
		return nullptr;
	}

	PTexture* NewTexture = Create(AbsFileName, Width, Height, DesiredChannelCount, Data);
	if (!NewTexture)
	{
		return nullptr;
	}

	LogDebug("Texture {} loaded", FileName.c_str());
	return NewTexture;
}

void PTextureManager::LoadAllTextures()
{
	const auto Textures = Files::GetFilesInDirectory("Resources/Textures");
	for (const auto& Texture : Textures)
	{
		Load(Texture);
	}
}

bool PTextureManager::LoadSDL(PTexture* Texture)
{
	const auto Renderer   = GetRenderer()->GetSDLRenderer();
	const auto Width      = Texture->GetWidth();
	const auto Height     = Texture->GetHeight();
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
		LogDebug("Updated SDL Texture.");
		return true;
	}
	LogError("Unable to update SDL texture: {}", SDL_GetError());
	return false;
}

void PTextureManager::UnloadSDL()
{
	LogDebug("Destroying all SDL textures");
	for (const auto& V : GetTextures() | std::views::values)
	{
		SDL_DestroyTexture(V->GetSDLTexture());
	}
}

PTexture* PTextureManager::Get(const std::string& Name)
{
	for (auto Key : GetTextures() | std::views::keys)
	{
		if (Key == Name)
		{
			return sTextures.at(Name).get();
		}
	}
	for (auto& Value : GetTextures() | std::views::values)
	{
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
	PTexture Tex;
	const auto DataSize = Width * Height * Channels;
	Tex.mData           = static_cast<uint8_t*>(malloc(DataSize));
	LogDebug("Copying texture data into new texture {}", FileName.c_str());
	memcpy(Tex.mData, Data, DataSize);

	if (!Tex.mData)
	{
		LogError("Unable to allocate memory for texture: {}", FileName.c_str());
		return nullptr;
	}

	auto BaseName = Files::SplitExt(FileName);
	Tex.mName     = BaseName;
	Tex.mPath     = Files::DirName(FileName);
	Tex.mWidth    = Width;
	Tex.mHeight   = Height;
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
