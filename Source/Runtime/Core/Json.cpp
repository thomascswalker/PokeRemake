#include "Json.h"

#include "Files.h"
#include "String.h"

void ExpandAnchor(JSON* Out, const std::string& Ref)
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
}

void Expand(JSON* Out)
{
	if (Out->is_object())
	{
		for (auto& [K, V] : Out->items())
		{
			K == ANCHOR ? ExpandAnchor(Out, V) : Expand(&V);
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
