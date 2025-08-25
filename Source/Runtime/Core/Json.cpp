#include "Json.h"

#include "Array.h"
#include "Files.h"
#include "String.h"

void ExpandRef(JSON* Json, const std::string& Ref, const JSON& Override)
{
    std::string Buffer;
    auto FileName = Files::FindFile(Ref + ".JSON");
    if (FileName.empty())
    {
        LogError("Cannot find Ref: {}", Ref.c_str());
        return;
    }
    Files::ReadFile(FileName, Buffer);

    JSON RefJson = JSON::parse(Buffer);
    Json->merge_patch(RefJson);
    //
    // if (!(Override.is_object() || Override.is_array()))
    // {
    //     return;
    // }

    for (const auto& [K, V] : Override.items())
    {
        TArray Keys = String::Split(K, "::");
        if (Keys.Size() == 0)
        {
            continue;
        }
        JSON* Ptr = Json;
        for (auto Key : Keys)
        {
            if (Ptr->is_object())
            {
                Ptr = &Ptr->at(Key);
            }
            else if (Ptr->is_array())
            {
                Ptr = &Ptr->operator[](std::stoi(Key));
            }
        }
        *Ptr = V;
    }
}

void Expand(JSON* Json)
{
    if (Json->is_object())
    {
        for (auto& [K, V] : Json->items())
        {
            if (!K.starts_with("$"))
            {
                Expand(&V);
            }
            else if (K.starts_with("$"))
            {
                ExpandRef(Json, K.substr(1), V);
            }
        }
    }
    else if (Json->is_array())
    {
        for (auto& V : *Json)
        {
            Expand(&V);
        }
    }
}
