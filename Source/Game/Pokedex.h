#pragma once

#include <string>

#include "Core/Array.h"
#include "Core/Json.h"

struct SPokeStats
{
	uint32_t Hp;
	uint32_t Attack;
	uint32_t Defense;
	uint32_t SpAttack;
	uint32_t SpDefense;
	uint32_t Speed;

	SPokeStats() = default;
	SPokeStats(const JSON& Json)
	{
		Hp = Json["HP"].get<uint32_t>();
		Attack = Json["Attack"].get<uint32_t>();
		Defense = Json["Defense"].get<uint32_t>();
		SpAttack = Json["Sp. Attack"].get<uint32_t>();
		SpDefense = Json["Sp. Defense"].get<uint32_t>();
		Speed = Json["Speed"].get<uint32_t>();
	}

	JSON Serialize() const
	{
		JSON Json;
		Json["HP"] = Hp;
		Json["Attack"] = Attack;
		Json["Defense"] = Defense;
		Json["Sp. Attack"] = SpAttack;
		Json["Sp. Defense"] = SpDefense;
		Json["Speed"] = Speed;
		return Json;
	}

	std::string ToString() const
	{
		return Serialize().dump();
	}
};

struct SPokeDef
{
	int32_t					 Id;
	std::string				 Name;
	std::vector<std::string> Types;
	SPokeStats				 Stats;

	SPokeDef() = default;
	SPokeDef(const JSON& Json)
	{
		Id = Json["Id"];
		Name = Json["Name"];
		Types = Json["Type"].get<std::vector<std::string>>();
		Stats = SPokeStats(Json["Stats"]);
	}

	JSON Serialize() const
	{
		JSON Json;
		Json["Id"] = Id;
		Json["Name"] = Name;
		Json["Type"] = JSON::array();
		for (const auto& Type : Types)
		{
			Json["Type"].push_back(Type);
		}
		Json["Stats"] = Stats.Serialize();
		return Json;
	}

	std::string ToString() const
	{

		return Serialize().dump();
	}
};

class PPokedexManager
{
	TArray<SPokeDef> Defs;

public:
	PPokedexManager();

	SPokeDef* Get(int32_t Index) { return &Defs[Index]; }
};