#include "Json.h"

#include "Files.h"
#include "String.h"

/**
 * @brief Expands the value of a key which contains the arrow `->` operator.
 * This will recursively iterate through the key which contains the arrows and
 * drill down the JSON object finding the corresponding keys until the last one
 * is reached. At this point, the value of that final key is set.
 * @param Out The root JSON object which has an anchor.
 * @param Key The key containing arrows.
 * @param Value The final value to be set on the last property of the key.
 */
void ExpandArrow(JSON* Out, const std::string& Key, const JSON& Value)
{
	TArray Keys = Strings::Split(Key, ARROW);
	for (auto K : Keys)
	{
		if (Out->is_object())
		{
			Out = &Out->at(K);
		}
		else if (Out->is_array())
		{
			Out = &Out->operator[](std::stoi(K));
		}
	}
	*Out = Value;
}

void ExpandAnchor(JSON* Out, const std::string& Ref, const JSON& Override)
{
	std::string Buffer;
	auto		FileName = Files::FindFile(Ref + ".JSON");
	if (FileName.empty())
	{
		LogError("Cannot find Ref: {}", Ref.c_str());
		return;
	}
	Files::ReadFile(FileName, Buffer);

	JSON RefJson = JSON::parse(Buffer);
	Out->merge_patch(RefJson);

	for (const auto& [K, V] : Override.items())
	{
		ExpandArrow(Out, K, V);
	}
}

void Expand(JSON* Out)
{
	if (Out->is_object())
	{
		for (auto& [K, V] : Out->items())
		{
			K.starts_with(ANCHOR) ? ExpandAnchor(Out, K.substr(1), V) : Expand(&V);
		}
	}
	else if (Out->is_array())
	{
		for (auto& V : *Out)
		{
			Expand(&V);
		}
	}
}
void OnPropertyMissing(const JSON& Json, const char* Property)
{
	std::vector<std::string> Keys;
	for (auto Item : Json.items())
	{
		Keys.push_back(Item.key());
	}
	std::string KeysString;
	for (auto Item : Keys)
	{
		KeysString += Item + ", ";
	}
	LogWarning("Property {} is missing: [{}]", Property, KeysString.c_str());
}
