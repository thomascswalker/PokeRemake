#include "Application/Application.h"
#include "Application/ArgParser.h"
#include "EditorGame.h"
#include "EditorHUD.h"

int main(int argc, char** argv)
{
	CREATE_APP(PEditorGame, PEditorHUD);
}
