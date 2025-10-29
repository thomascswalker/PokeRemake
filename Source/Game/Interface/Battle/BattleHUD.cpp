/**
 * Manages and draws the entire Battle HUD when in Battle Mode.
 */

#include "BattleHUD.h"

#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"

#include "BattleInterfaceConstants.h"

static std::vector<SFrameTile> GPlayerStatusFrameTiles = {
	{  9, 11,	 UI_INDEX_STATUS_FRAME_ARROW_LEFT },
	{ 10, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 11, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 12, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 13, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 14, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 15, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 16, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 17, 11,	  UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 18, 11, UI_INDEX_STATUS_FRAME_CORNER_RIGHT },
	{ 18, 10,	  UI_INDEX_STATUS_FRAME_VERTICAL },
	{ 18,  9,	 UI_INDEX_STATUS_FRAME_VERTICAL },
};

static std::vector<SFrameTile> GPlayerHealthTiles = {
	{ 10, 9, UI_INDEX_HP_LABEL_1 },
	{ 11, 9, UI_INDEX_HP_LABEL_2 },
	{ 12, 9,	 UI_INDEX_HP_FULL },
	{ 13, 9,	 UI_INDEX_HP_FULL },
	{ 14, 9,	 UI_INDEX_HP_FULL },
	{ 15, 9,	 UI_INDEX_HP_FULL },
	{ 16, 9,	 UI_INDEX_HP_FULL },
	{ 17, 9,	 UI_INDEX_HP_FULL },
	{ 18, 9,	 UI_INDEX_HP_END },
};

static std::vector<SFrameTile> GOpponentStatusFrameTiles = {
	{  1, 2,	UI_INDEX_STATUS_FRAME_VERTICAL },
	{  1, 3, UI_INDEX_STATUS_FRAME_CORNER_LEFT },
	{  2, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  3, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  4, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  5, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  6, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  7, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  8, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{  9, 3,	UI_INDEX_STATUS_FRAME_HORIZONTAL },
	{ 10, 3, UI_INDEX_STATUS_FRAME_ARROW_RIGHT },
};

static std::vector<SFrameTile> GOpponentHealthTiles = {
	{  2, 2, UI_INDEX_HP_LABEL_1 },
	{  3, 2, UI_INDEX_HP_LABEL_2 },
	{  4, 2,	UI_INDEX_HP_FULL },
	{  5, 2,	UI_INDEX_HP_FULL },
	{  6, 2,	UI_INDEX_HP_FULL },
	{  7, 2,	UI_INDEX_HP_FULL },
	{  8, 2,	UI_INDEX_HP_FULL },
	{  9, 2,	UI_INDEX_HP_FULL },
	{ 10, 2,	 UI_INDEX_HP_END },
};

PBattleHUD::PBattleHUD()
{
	mInterfaceSprite = std::make_shared<PSprite>();
	mInterfaceSprite->SetTexture(GTextureManager->Get("Interface"));

	mDialogBox = ConstructWidget<PDialogBox>();

	std::string BattleText = "Default text.";
	if (auto BattleMon = GBattleManager->GetBattleMon())
	{
		BattleText = std::format("{} sent out \n{}", GBattleManager->GetCurrentTrainerName().c_str(), BattleMon->GetDisplayName().c_str());
	}

	// Background dialog box
	PWidget::AddChild(ConstructWidget<PDialogBox>());

	mDialogBox->SetText(BattleText);
	mDialogBox->Print();

	mActionBox = ConstructWidget<PBattleActionMenu>();
	mMoveBox = ConstructWidget<PBattleMoveMenu>();
}

PBattleHUD::~PBattleHUD()
{
	mDialogBox->EndPrint();
	mDialogBox->Unparent();
	GWorld->DestroyWidget(mDialogBox);
	mDialogBox = nullptr;

	mActionBox->Unparent();
	GWorld->DestroyWidget(mActionBox);
	mActionBox = nullptr;

	mMoveBox->Unparent();
	GWorld->DestroyWidget(mMoveBox);
	mMoveBox = nullptr;
}

std::array<uint32_t, 6> PBattleHUD::ComputeHealthBarTiles(SPokemon* Mon) const
{
	// Something's off with this math
	std::array<uint32_t, 6> HealthBarTiles;

	uint32_t CurrentHp = Mon->GetCurrentHp();
	uint32_t MaxHp = Mon->GetMaxHp();

	uint32_t FullBarSize = MaxHp / 6;
	uint32_t FullBarCount = CurrentHp / FullBarSize;
	uint32_t PartialBar = (CurrentHp % FullBarSize);
	uint32_t PartialBarIndex = (PartialBar / 12.5f) + 3;

	for (uint32_t I = 0; I < FullBarCount; I++)
	{
		HealthBarTiles[I] = UI_INDEX_HP_FULL;
	}
	for (uint32_t I = FullBarCount; I < 6; I++)
	{
		HealthBarTiles[I] = UI_INDEX_HP_EMPTY;
	}
	HealthBarTiles[FullBarCount] = PartialBarIndex;
	return HealthBarTiles;
}

void PBattleHUD::DrawTiles(const std::vector<SFrameTile>& Tiles) const
{
	for (auto& Tile : Tiles)
	{
		GRenderer->DrawSprite(mInterfaceSprite.get(), Tile.Dest, Tile.Index);
	}
}

void PBattleHUD::DrawPlayerFrame(const PRenderer* Renderer) const
{
	// Draw the player Pokémon
	if (auto Mon = GBattleManager->GetPlayerMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetBackTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), PLAYER_MON_RECT);

		// Draw name/level
		Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_MUL);
		TextRenderer::DrawText(Mon->GetDisplayName(), { PLAYER_ORIGIN_X, PLAYER_ORIGIN_Y });
		Renderer->DrawSprite(mInterfaceSprite.get(), { PLAYER_LEVEL_X, PLAYER_LEVEL_Y, COORD(1), COORD(1) }, UI_INDEX_LEVEL);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { PLAYER_LEVEL_X + COORD(1), PLAYER_LEVEL_Y });

		// Draw HP Bar
		auto HealthBarIndexes = ComputeHealthBarTiles(Mon);
		auto HealthBarTiles = GPlayerHealthTiles;
		for (uint32_t I = 0; I < 6; I++)
		{
			HealthBarTiles[I + 2].Index = HealthBarIndexes[I];
		}
		DrawTiles(HealthBarTiles);

		// Draw HP Text
		auto	HpText = Mon->GetDisplayHp();
		int32_t HpTextLength = HpText.size();
		int32_t HpXOffset = MAX_HP_TEXT_LENGTH - HpTextLength;
		TextRenderer::DrawText(HpText, { PLAYER_ORIGIN_X + COORD(1) + COORD(HpXOffset), PLAYER_ORIGIN_Y + COORD(3) });

		// Draw status frame
		DrawTiles(GPlayerStatusFrameTiles);
		Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);
	}
}

void PBattleHUD::DrawBattleFrame(const PRenderer* Renderer) const
{
	// Draw the Pokémon being battled
	if (auto Mon = GBattleManager->GetBattleMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetFrontTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), BATTLE_MON_RECT);

		// Draw name/level
		TextRenderer::DrawText(Mon->GetDisplayName(), { BATTLE_ORIGIN_X, BATTLE_ORIGIN_Y });
		Renderer->DrawSprite(mInterfaceSprite.get(), { BATTLE_LEVEL_X - COORD(1), BATTLE_LEVEL_Y, COORD(1), COORD(1) }, UI_INDEX_LEVEL);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { BATTLE_LEVEL_X, BATTLE_LEVEL_Y });

		// Draw health bar
		auto HealthBarIndexes = ComputeHealthBarTiles(Mon);
		auto HealthBarTiles = GOpponentHealthTiles;
		for (uint32_t I = 0; I < 6; I++)
		{
			HealthBarTiles[I + 2].Index = HealthBarIndexes[I];
		}
		DrawTiles(HealthBarTiles);
		DrawTiles(GOpponentStatusFrameTiles);
	}
}

void PBattleHUD::Draw(const PRenderer* Renderer) const
{
	// White background
	Renderer->SetDrawColor(255, 255, 255, 255);
	Renderer->DrawFillRect({ 0, 0, WINDOW_PXL_WIDTH, WINDOW_PXL_HEIGHT });

	DrawPlayerFrame(Renderer);
	DrawBattleFrame(Renderer);
}

void PBattleHUD::ShowActionBox()
{
	mActionBox->SetVisible(true);
	PWidget::AddChild(mActionBox);
}

void PBattleHUD::HideActionBox()
{
	mActionBox->SetVisible(false);
	PWidget::RemoveChild(mActionBox);
}

void PBattleHUD::ShowDialogBox(const SDialogContext& Context)
{
	mDialogBox->SetVisible(true);
	mDialogBox->SetText(Context.Message);
	mDialogBox->Print();
	PWidget::AddChild(mDialogBox);
}

void PBattleHUD::HideDialogBox()
{
	mDialogBox->SetVisible(false);
	PWidget::RemoveChild(mDialogBox);
	OnDialogComplete.Broadcast();
}

void PBattleHUD::ShowMoveBox()
{
	mMoveBox->SetVisible(true);
	PWidget::AddChild(mMoveBox);
}

void PBattleHUD::HideMoveBox()
{
	mMoveBox->SetVisible(false);
	PWidget::RemoveChild(mMoveBox);
}