#include "Json.h"

#include "Files.h"

void ExpandRef(JSON* Json, const std::string& Ref)
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
}

void Expand(JSON* Json)
{
    if (Json->is_object())
    {
        for (auto& [K, V] : Json->items())
        {
            if (V.is_object() || V.is_array())
            {
                Expand(&V);
            }
            else if (K == "$Ref" && V.is_string())
            {
                ExpandRef(Json, V);
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
