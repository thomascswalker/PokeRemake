#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "SDL3/SDL.h"

class PTexture
{
	uint32_t	mID;
	int32_t		mWidth;
	int32_t		mHeight;
	int32_t		mChannels;
	uint8_t*	mData;
	std::string mFileName;

	SDL_Texture* mSDLTexture;

protected:
	PTexture();

public:
	~PTexture();
	uint32_t	 GetID() const { return mID; }
	int32_t		 GetWidth() const { return mWidth; }
	int32_t		 GetHeight() const { return mHeight; }
	int32_t		 GetChannels() const { return mChannels; }
	int32_t		 GetPitch() const { return mWidth * 4; }
	uint8_t*	 GetData() const { return mData; }
	std::string	 GetFileName() const { return mFileName; }
	SDL_Texture* GetSDLTexture() const { return mSDLTexture; }

	friend class PTextureManager;
};

class PTextureManager
{
public:
	static std::map<std::string, std::shared_ptr<PTexture>> GetTextures();

	static PTexture* Load(const std::string& FileName);
	static void		 LoadSDL(SDL_Renderer* Renderer);
	static void		 UnloadSDL();
	static PTexture* Get(const std::string& FileName);
};