#include "ActorManager.h"

#include "Core/Files.h"

static JSON ActorDefs{};

void ActorManager::LoadActorDefs()
{
    auto DefFiles = Files::GetFilesInDirectory("Data/Actor");
    for (const auto& FileName : DefFiles)
    {
        std::string Buffer;
        if (!Files::ReadFile(FileName, Buffer))
        {
            LogError("Failed to read file: {}", FileName.c_str());
            continue;
        }
        auto Base       = Files::SplitExt(FileName, false);
        JSON ActorDef   = JSON::parse(Buffer.data());
        ActorDefs[Base] = ActorDef;
        LogDebug("Loaded actor def: {}", Base.c_str());
    }
}

JSON& ActorManager::GetActorDefs()
{
    return ActorDefs;
}

JSON* ActorManager::GetActorDef(const std::string& Name)
{
    if (ActorDefs.contains(Name))
    {
        return &ActorDefs[Name];
    }
    LogWarning("Actor definition {} not found.", Name.c_str());
    return nullptr;
}
