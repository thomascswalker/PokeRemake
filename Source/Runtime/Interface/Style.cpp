#include "Style.h"

static std::map<std::string, CSS::Stylesheet> gStylesheets{};
static std::map<std::string, SStyle>		  gWidgetStyles{};

void StyleManager::LoadAllStylesheets()
{
	LoadStylesheet("Styles.css");
}

CSS::Stylesheet* StyleManager::LoadStylesheet(const std::string& FileName)
{
	auto			Name = Files::SplitExt(FileName, false);
	CSS::Stylesheet Sheet;

	if (!CSS::Load(&Sheet, FileName))
	{
		LogError("Failed to load stylesheet: {}", FileName.c_str());
		return nullptr;
	}

	gStylesheets[Name] = Sheet;
	return &gStylesheets[Name];
}

CSS::Stylesheet* StyleManager::GetStylesheet(const std::string& Name)
{
	if (!gStylesheets.contains(Name))
	{
		return nullptr;
	}
	return &gStylesheets[Name];
}
