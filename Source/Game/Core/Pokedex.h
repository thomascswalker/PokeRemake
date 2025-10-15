#pragma once

#include <string>

#include "Core/Array.h"
#include "Core/Json.h"
#include "Core/Singleton.h"
#include "Engine/Serialization.h"
#include "Engine/Texture.h"

struct SPokemonDef;
class PPokedexManager;

enum class EPokeType
{
	Normal = 0x00,
	Fighting = 0x01,
	Flying = 0x02,
	Poison = 0x03,
	Ground = 0x04,
	Rock = 0x05,
	Bird = 0x06,
	Bug = 0x07,
	Ghost = 0x08,
	Fire = 0x14,
	Water = 0x15,
	Grass = 0x16,
	Electric = 0x17,
	Psychic = 0x18,
	Ice = 0x19,
	Dragon = 0x1A,
};

static std::map<EPokeType, std::string> GTypes{
	{	  EPokeType::Normal,	 "Normal" },
	{ EPokeType::Fighting, "Fighting" },
	{	  EPokeType::Flying,	 "Flying" },
	{	  EPokeType::Poison,	 "Poison" },
	{	  EPokeType::Ground,	 "Ground" },
	{	  EPokeType::Rock,	   "Rock" },
	{	  EPokeType::Bird,	   "Bird" },
	{	  EPokeType::Bug,	  "Bug" },
	{	  EPokeType::Ghost,	"Ghost" },
	{	  EPokeType::Fire,	   "Fire" },
	{	  EPokeType::Water,	"Water" },
	{	  EPokeType::Grass,	"Grass" },
	{ EPokeType::Electric, "Electric" },
	{  EPokeType::Psychic,  "Psychic" },
	{	  EPokeType::Ice,	  "Ice" },
	{	  EPokeType::Dragon,	 "Dragon" }
};

static std::string GetPokeTypeString(EPokeType Type)
{
	return GTypes[Type];
}

enum class EMoveCategory
{
	Physical = 0x00,
	Special = 0x01,
	Status = 0x02,
};

static std::map<EMoveCategory, std::string> GMoveCategories{
	{ EMoveCategory::Physical, "Physical" },
	{  EMoveCategory::Special,  "Special" },
	{	  EMoveCategory::Status,	 "Status" }
};

static std::string GetMoveCategoryString(EMoveCategory Category)
{
	return GMoveCategories[Category];
}

struct SMoveDef : ISerializable
{
	int32_t		  Id;
	std::string	  Name;
	EPokeType	  Type;
	uint32_t	  Power;
	EMoveCategory Category;
	uint32_t	  Accuracy;
	uint32_t	  PP;

	SMoveDef(const JSON& Json)
	{
		SMoveDef::Deserialize(Json);
	}

	JSON Serialize() const override
	{
		JSON Result;
		Result["Id"] = Id;
		Result["Name"] = Name;
		Result["Type"] = Type;
		Result["Power"] = Power;
		Result["Accuracy"] = Accuracy;
		Result["PP"] = PP;
		Result["Category"] = Category;
		return Result;
	}

	void Deserialize(const JSON& Json) override
	{
		Id = Json["Id"];
		Name = Strings::ToUpper(Json["Name"]);
		Type = Json["Type"];
		Power = !Json.at("Power").is_null() ? Json["Power"].get<uint32_t>() : 0;
		Accuracy = !Json.at("Accuracy").is_null() ? Json["Accuracy"].get<uint32_t>() : 0;
		PP = !Json.at("PP").is_null() ? Json["PP"].get<uint32_t>() : 0;
		Category = Json["Category"];
	}
};

struct SPokemonDef : ISerializable
{
	int32_t					 Id = 0;
	std::string				 Name;
	std::vector<std::string> Types;
	std::vector<uint32_t>	 DefaultMoves;
	std::vector<uint32_t>	 LearnedMoves;
	std::vector<uint32_t>	 TMMoves;
	std::vector<uint32_t>	 HMMoves;

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
		SPokemonDef::Deserialize(Json);
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
		Name = Strings::ToUpper(Json["Name"]);
		Types = Json["Type"].get<std::vector<std::string>>();

		const auto Stats = Json["Stats"];
		MaxHp = Stats["HP"].get<uint32_t>();
		Attack = Stats["Attack"].get<uint32_t>();
		Defense = Stats["Defense"].get<uint32_t>();
		SpAttack = Stats["Sp. Attack"].get<uint32_t>();
		SpDefense = Stats["Sp. Defense"].get<uint32_t>();
		Speed = Stats["Speed"].get<uint32_t>();

		const auto Moves = Json["Moves"];
		DefaultMoves = Moves["Defaults"].get<std::vector<uint32_t>>();

		std::string FmtName = Json.contains("AlternateName") ? Json["AlternateName"] : Json["Name"];
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
	TArray<SPokemonDef> Mons;
	TArray<SMoveDef>	Moves;

	bool LoadPokemonDefs();
	bool LoadMoveDefs();
	bool LoadSprites();

public:
	bool Init();

	SPokemonDef* GetMonByIndex(int32_t Index);
	SPokemonDef* GetMonById(int32_t Id);
	SPokemonDef* GetMonByName(const std::string& Name);

	SMoveDef* GetMoveByIndex(int32_t Index);
	SMoveDef* GetMoveById(int32_t Id);
};
