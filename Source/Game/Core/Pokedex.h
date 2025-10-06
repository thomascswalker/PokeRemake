#pragma once

#include <string>

#include "Core/Array.h"
#include "Core/Json.h"
#include "Core/Singleton.h"
#include "Engine/Serialization.h"
#include "Engine/Texture.h"

#define ID_BULBASAUR  1
#define ID_IVYSAUR	  2
#define ID_VENUSAUR	  3
#define ID_CHARMANDER 4
#define ID_CHARMELEON 5
#define ID_CHARIZARD  6
#define ID_SQUIRTLE	  7
#define ID_WARTORTLE  8
#define ID_BLASTOISE  9
#define ID_CATERPIE	  10
#define ID_METAPOD	  11
#define ID_BUTTERFREE 12
#define ID_WEEDLE	  13
#define ID_KAKUNA	  14
#define ID_BEEDRILL	  15
#define ID_PIDGEY	  16
#define ID_PIDGEOTTO  17
#define ID_PIDGEOT	  18
#define ID_RATTATA	  19
#define ID_RATICATE	  20
#define ID_SPEAROW	  21
#define ID_FEAROW	  22
#define ID_EKANS	  23
#define ID_ARBOK	  24
#define ID_PIKACHU	  25
#define ID_RAICHU	  26
#define ID_SANDSHREW  27
#define ID_SANDSLASH  28
#define ID_NIDORAN_F  29
#define ID_NIDORINA	  30
#define ID_NIDOQUEEN  31
#define ID_NIDORAN_M  32
#define ID_NIDORINO	  33
#define ID_NIDOKING	  34
#define ID_CLEFAIRY	  35
#define ID_CLEFABLE	  36
#define ID_VULPIX	  37
#define ID_NINETALES  38
#define ID_JIGGLYPUFF 39
#define ID_WIGGLYTUFF 40
#define ID_ZUBAT	  41
#define ID_GOLBAT	  42
#define ID_ODDISH	  43
#define ID_GLOOM	  44
#define ID_VILEPLUME  45
#define ID_PARAS	  46
#define ID_PARASECT	  47
#define ID_VENONAT	  48
#define ID_VENOMOTH	  49
#define ID_DIGLETT	  50
#define ID_DUGTRIO	  51
#define ID_MEOWTH	  52
#define ID_PERSIAN	  53
#define ID_PSYDUCK	  54
#define ID_GOLDUCK	  55
#define ID_MANKEY	  56
#define ID_PRIMEAPE	  57
#define ID_GROWLITHE  58
#define ID_ARCANINE	  59
#define ID_POLIWAG	  60
#define ID_POLIWHIRL  61
#define ID_POLIWRATH  62
#define ID_ABRA		  63
#define ID_KADABRA	  64
#define ID_ALAKAZAM	  65
#define ID_MACHOP	  66
#define ID_MACHOKE	  67
#define ID_MACHAMP	  68
#define ID_BELLSPROUT 69
#define ID_WEEPINBELL 70
#define ID_VICTREEBEL 71
#define ID_TENTACOOL  72
#define ID_TENTACRUEL 73
#define ID_GEODUDE	  74
#define ID_GRAVELER	  75
#define ID_GOLEM	  76
#define ID_PONYTA	  77
#define ID_RAPIDASH	  78
#define ID_SLOWPOKE	  79
#define ID_SLOWBRO	  80
#define ID_MAGNEMITE  81
#define ID_MAGNETON	  82
#define ID_FARFETCH	  83
#define ID_DODUO	  84
#define ID_DODRIO	  85
#define ID_SEEL		  86
#define ID_DEWGONG	  87
#define ID_GRIMER	  88
#define ID_MUK		  89
#define ID_SHELLDER	  90
#define ID_CLOYSTER	  91
#define ID_GASTLY	  92
#define ID_HAUNTER	  93
#define ID_GENGAR	  94
#define ID_ONIX		  95
#define ID_DROWZEE	  96
#define ID_HYPNO	  97
#define ID_KRABBY	  98
#define ID_KINGLER	  99
#define ID_VOLTORB	  100
#define ID_ELECTRODE  101
#define ID_EXEGGCUTE  102
#define ID_EXEGGUTOR  103
#define ID_CUBONE	  104
#define ID_MAROWAK	  105
#define ID_HITMONLEE  106
#define ID_HITMONCHAN 107
#define ID_LICKITUNG  108
#define ID_KOFFING	  109
#define ID_WEEZING	  110
#define ID_RHYHORN	  111
#define ID_RHYDON	  112
#define ID_CHANSEY	  113
#define ID_TANGELA	  114
#define ID_KANGASKHAN 115
#define ID_HORSEA	  116
#define ID_SEADRA	  117
#define ID_GOLDEEN	  118
#define ID_SEAKING	  119
#define ID_STARYU	  120
#define ID_STARMIE	  121
#define ID_MR_MIME	  122
#define ID_SCYTHER	  123
#define ID_JYNX		  124
#define ID_ELECTABUZZ 125
#define ID_MAGMAR	  126
#define ID_PINSIR	  127
#define ID_TAUROS	  128
#define ID_MAGIKARP	  129
#define ID_GYARADOS	  130
#define ID_LAPRAS	  131
#define ID_DITTO	  132
#define ID_EEVEE	  133
#define ID_VAPOREON	  134
#define ID_JOLTEON	  135
#define ID_FLAREON	  136
#define ID_PORYGON	  137
#define ID_OMANYTE	  138
#define ID_OMASTAR	  139
#define ID_KABUTO	  140
#define ID_KABUTOPS	  141
#define ID_AERODACTYL 142
#define ID_SNORLAX	  143
#define ID_ARTICUNO	  144
#define ID_ZAPDOS	  145
#define ID_MOLTRES	  146
#define ID_DRATINI	  147
#define ID_DRAGONAIR  148
#define ID_DRAGONITE  149
#define ID_MEWTWO	  150
#define ID_MEW		  151

struct SPokemonDef;
class PPokedexManager;

struct SPokemonDef : ISerializable
{
	int32_t					 Id = 0;
	std::string				 Name;
	std::vector<std::string> Types;

	uint32_t MaxHp = 0;
	uint32_t Attack = 0;
	uint32_t Defense = 0;
	uint32_t SpAttack = 0;
	uint32_t SpDefense = 0;
	uint32_t Speed = 0;

	PTexture* Front = nullptr;
	PTexture* Back = nullptr;

	SPokemonDef() = default;
	SPokemonDef(const JSON& Json)
	{
		Deserialize(Json);
	}

	JSON Serialize() const override
	{
		JSON Json;
		Json["Id"] = Id;
		Json["Name"] = Name;
		Json["Type"] = JSON::array();

		Json["Stats"] = JSON::object();
		Json["Stats"]["HP"] = MaxHp;
		Json["Stats"]["Attack"] = Attack;
		Json["Stats"]["Defense"] = Defense;
		Json["Stats"]["Sp. Attack"] = SpAttack;
		Json["Stats"]["Sp. Defense"] = SpDefense;
		Json["Stats"]["Speed"] = Speed;
		return Json;
	}

	void Deserialize(const JSON& Json) override
	{
		Id = Json["Id"];
		Name = Json["Name"];
		Types = Json["Type"].get<std::vector<std::string>>();

		const auto Stats = Json["Stats"];
		MaxHp = Stats["HP"].get<uint32_t>();
		Attack = Stats["Attack"].get<uint32_t>();
		Defense = Stats["Defense"].get<uint32_t>();
		SpAttack = Stats["Sp. Attack"].get<uint32_t>();
		SpDefense = Stats["Sp. Defense"].get<uint32_t>();
		Speed = Stats["Speed"].get<uint32_t>();

		std::string FmtName = Json.contains("AlternateName") ? Json["AlternateName"].get<std::string>() : Name;
		std::string FrontFileName = std::format("{}Front", FmtName.c_str());
		Front = GTextureManager->Get(FrontFileName);
		std::string BackFileName = std::format("{}Back", FmtName.c_str());
		Back = GTextureManager->Get(BackFileName);
	}

	std::string ToString() const
	{

		return Serialize().dump();
	}
};

class PPokedexManager : public ISingleton<PPokedexManager>
{
	TArray<SPokemonDef> Defs;

	bool LoadDefs();
	bool LoadSprites();

public:
	bool Init();

	SPokemonDef* Get(int32_t Index);
	SPokemonDef* GetById(int32_t Id);
	SPokemonDef* GetByName(const std::string& Name);
};
