#include "Texture.h"
#include "Core/Files.h"
#include "Core/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG // generate user friendly error messages

#include "Renderer/Renderer.h"
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

	int		   Width, Height, Channels;
	const auto Data = stbi_load(AbsFileName.c_str(), &Width, &Height, &Channels, 4);

	if (!Data)
	{
		LogError("Unable to load image: {}", stbi_failure_reason());
		return nullptr;
	}

	PTexture* NewTexture = Create(AbsFileName, Width, Height, Data);
	LogDebug("Texture {} loaded", FileName.c_str());
	stbi_image_free(Data);
	return NewTexture;
}

void PTextureManager::LoadSDL(PTexture* Texture)
{
	const auto Renderer = GetRenderer()->GetSDLRenderer();
	const auto Width = Texture->GetWidth();
	const auto Height = Texture->GetHeight();
	const auto SDLTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888,
											  SDL_TEXTUREACCESS_STATIC, Width, Height);
	if (!SDLTexture)
	{
		LogError("Unable to create texture: {}", SDL_GetError());
		return;
	}
	SDL_Rect Source(0, 0, Width, Height);
	if (SDL_UpdateTexture(SDLTexture, &Source, Texture->GetData(), Texture->GetPitch()))
	{
		Texture->mSDLTexture = SDLTexture;
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

PTexture* PTextureManager::Get(const std::string& Name)
{
	for (const auto& [K, V] : GetTextures())
	{
		if (V->GetName().ends_with(Name))
		{
			return V.get();
		}
	}
	return nullptr;
}

PTexture* PTextureManager::Create(const std::string& Name, float Width, float Height, const void* Data)
{
	PTexture   Tex;
	const auto DataSize = Tex.mWidth * Tex.mHeight * 4 * sizeof(uint8_t);
	if (Data)
	{
		std::memcpy(Tex.mData, Data, DataSize);
	}
	else
	{
		Tex.mData = static_cast<uint8_t*>(std::malloc(DataSize));
	}
	Tex.mWidth = Width;
	Tex.mHeight = Height;
	Tex.mName = Name;
	LoadSDL(&Tex);

	gTextures[Name] = std::make_shared<PTexture>(Tex);
	return gTextures[Name].get();
}

void PTextureManager::Destroy(const PTexture* Texture)
{
	auto Iter = gTextures.find(Texture->GetName());
	if (Iter != gTextures.end())
	{
		gTextures.erase(Iter);
	}
	else
	{
		LogError("Unable to find texture: {}", Texture->GetName().c_str());
	}
}

std::map<std::string, std::shared_ptr<PTexture>> PTextureManager::GetTextures()
{
	return gTextures;
}