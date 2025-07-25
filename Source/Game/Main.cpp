#include "TestGame.h"

#include "Application/Application.h"
#include "Application/ArgParser.h"
#include "../Runtime/Interface/Game/GameHUD.h"

int main(int argc, char** argv)
{
	CREATE_APP(TestGame, PGameHUD);
}
