#pragma once

#include "Core/Rect.h"

#include <map>
#include <memory>
#include <string>

#include "SDL3/SDL.h"

#define TEXTURE_ASH "Ash.png"
#define TEXTURE_GARY "Gary.png"

class PTexture
{
	uint32_t	mID;
	int32_t		mWidth;
	int32_t		mHeight;
	int32_t		mChannels;
	uint8_t*	mData;
	std::string mName;

	SDL_Texture* mSDLTexture;

protected:
	PTexture();

public:
	~PTexture();
	uint32_t	 GetID() const { return mID; }
	int32_t		 GetWidth() const { return mWidth; }
	int32_t		 GetHeight() const { return mHeight; }
	int32_t		 GetChannels() const { return mChannels; }
	int32_t		 GetPitch() const { return mWidth * sizeof(uint32_t); }
	FRect		 GetRect() const { return { 0, 0, (float)mWidth, (float)mHeight }; }
	uint8_t*	 GetData() const { return mData; }
	std::string	 GetName() const { return mName; }
	SDL_Texture* GetSDLTexture() const { return mSDLTexture; }
	void		 SetSDLTexture(SDL_Texture* SDLTex) { mSDLTexture = SDLTex; }

	friend class PTextureManager;
};

using TextureMap = std::map<std::string, std::shared_ptr<PTexture>>;

class PTextureManager
{
	static TextureMap sTextures;

public:
	static TextureMap& GetTextures();

	static PTexture* Load(const std::string& FileName);
	static bool		 LoadSDL(PTexture* Texture);
	static void		 UnloadSDL();
	static PTexture* Get(const std::string& Name);
	static PTexture* Create(const std::string& FileName, float Width, float Height, int Channels, void* Data);
	static void		 Destroy(const PTexture* Texture);
};