#pragma once

enum class PlatformType
{
	Unknown,
	SDL,
	GBA
};

class IPlatform
{
public:
	virtual ~IPlatform() = default;

	virtual bool OnStart(int argc, char** argv) = 0;
	virtual void OnStop() = 0;
	virtual void OnLoop() = 0;
	virtual bool OnEvent(void* Event) = 0;
	virtual void OnDraw() = 0;

	virtual bool		 IsRunning() = 0;
	virtual PlatformType GetPlatformType() = 0;
	virtual PEngine*	 GetEngine() = 0;
};

#define CONSTRUCT_AND_START_GAME(PlatformType, GameType)    \
	const auto Platform = std::make_unique<PlatformType>(); \
	const auto Game = std::make_unique<GameType>();         \
	if (Platform->OnStart(argc, argv))                      \
	{                                                       \
		Platform->GetEngine()->SetGame(Game.get());         \
		while (Platform->IsRunning())                       \
		{                                                   \
			Platform->OnLoop();                             \
		}                                                   \
	}
