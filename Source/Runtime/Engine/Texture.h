#pragma once

#include <map>
#include <memory>
#include <string>

#include "Core/Rect.h"
#include "SDL3/SDL.h"

class PTexture
{
	uint32_t	mID;
	int32_t		mWidth;
	int32_t		mHeight;
	int32_t		mChannels;
	uint8_t*	mData;
	std::string mName;
	std::string mPath;

	SDL_Texture* mSDLTexture;

protected:
	PTexture();

public:
	~PTexture();

	uint32_t GetID() const
	{
		return mID;
	}

	int32_t GetWidth() const
	{
		return mWidth;
	}

	int32_t GetHeight() const
	{
		return mHeight;
	}

	int32_t GetChannels() const
	{
		return mChannels;
	}

	int32_t GetPitch() const
	{
		return mWidth * sizeof(uint32_t);
	}

	FRect GetRect() const
	{
		return { 0, 0, (float)mWidth, (float)mHeight };
	}

	uint8_t* GetData() const
	{
		return mData;
	}

	std::string GetName() const
	{
		return mName;
	}

	std::string GetPath() const
	{
		return mPath;
	}

	SDL_Texture* GetSDLTexture() const
	{
		return mSDLTexture;
	}

	void SetSDLTexture(SDL_Texture* SDLTex)
	{
		mSDLTexture = SDLTex;
	}

	friend class PTextureManager;
};

class PTextureManager
{
	using TextureMap = std::map<std::string, std::shared_ptr<PTexture>>;
	TextureMap mTextures;

public:
	PTextureManager();
	TextureMap& GetTextures();

	PTexture* Load(const std::string& FileName);
	PTexture* LoadMemory(const std::string& Name, void* Data, int32_t Width, int32_t Height, int32_t Channels);
	void	  LoadAllTextures();
	bool	  LoadSDL(PTexture* Texture);
	void	  UnloadSDL();
	PTexture* Get(const std::string& Name);
	PTexture* Create(const std::string& FileName, float Width, float Height, int Channels, void* Data);
	void	  Destroy(const PTexture* Texture);
};

extern PTextureManager* GTextureManager;
