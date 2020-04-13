#include "BreakoutRedux.hpp"
#include "Game.hpp"

u32 kDown, kHeld;
touchPosition touch;

C3D_RenderTarget* top, *bot;

C2D_SpriteSheet uiSheet;
C2D_SpriteSheet spriteSheet;
C2D_SpriteSheet powerupSheet;

PrintConsole bottomScreen, infoKillWindow;

std::string VersionText = "  Beta ", VersionNumber = "01.00";
std::string BuildNumber = "20.04.13.0145";// , EngineVersion = "01.00.00";

std::string ConsoleMove(int x, int y);
std::string ConsoleColor(std::string foreground, bool bright);
std::string ConsoleColor(std::string foreground, std::string background, bool bright);

bool touchInBox(touchPosition touch, int x, int y, int w, int h);

void DrawTexture(C2D_Image image, float x, float y, float rotation, C2D_ImageTint* tint, float scaleX, float scaleY);
void DrawTexture(C2D_Image image, float x, float y, float scaleX, float scaleY);
void DrawTexture(C2D_Image image, float x, float y);

//Application
int main(int argc, char **argv) {
	//create/open debug text file

	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	romfsInit();
	csndInit();
	//set background color to RGB 149, 149, 149, with alpha of 255 (if applicable)

	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	uiSheet = C2D_SpriteSheetLoad("romfs:/gfx/ui.t3x");
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	powerupSheet = C2D_SpriteSheetLoad("romfs:/gfx/powerups.t3x");
	if (!uiSheet || !spriteSheet || !powerupSheet)
		svcBreak(USERBREAK_PANIC);

	//initialize audio (if applicable still)

	srand(time(NULL));

	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleInit(GFX_BOTTOM, &infoKillWindow);

	consoleSetWindow(&infoKillWindow, 0, 26, 40, 4);

	//create paddle and ball objects (create classes for them first)

	int GameState = state_title, PreviousGameState = state_title;
	bool OdsMode = false;
	bool UpdateText = true;
	//int CurPlayer = 0;
	Game *CurGame;
	Game Games[1] = { Game(0) };
	int lives = 3;
	int workingLevel[5][10] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	int cursorX, cursorY;

	int frame = 0;

	//read settings file if it exists, otherwise write it, also update it if applicable

	//Main loop
	while (aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&touch);
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		switch (GameState) {
		case state_title:
			if (kDown & KEY_START) {
				Games[0] = Game(0);
				CurGame = &Games[0];
				PreviousGameState = GameState;
				GameState = state_game;
				UpdateText = true;
			}
			if (kDown & KEY_X) {
				PreviousGameState = GameState;
				GameState = state_extra0; //extras 10/13/2017
				UpdateText = true;
			}
			if (kDown & KEY_Y) {
				cursorX = 0;
				cursorY = 0;
				PreviousGameState = GameState;
				GameState = state_editor; //level designer
				UpdateText = true;
			}
			if (kDown & KEY_R) {
				OdsMode = !OdsMode;
				//write, then read settings
				UpdateText = true;
			}

			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			//Write text (only implement the system from before if there is still screen tearing)
			//Clear consoles (make function to clear them and reset text as well, to do the job of CRESET)
			//select main bottom screen console
			DrawTexture(GetImage(uiSheet, ui_title_idx), 80, 20);
			C3D_FrameEnd(0);
			frame++;
			break;
		case state_game:
			// Handle Controls
			if (frame % 20 == 19) {
				lives = CurGame->Lives();
				UpdateText = true;
			}
			if (kDown & KEY_START || lives == 0) {
				GameState = PreviousGameState;
				PreviousGameState = state_title;
				UpdateText = true;
			}
			if (kDown & KEY_SELECT) // change to A upon release
				CurGame->Shoot();
			if (kHeld & KEY_LEFT)
				CurGame->MovePaddle(-3);
			if (kHeld & KEY_RIGHT)
				CurGame->MovePaddle(3);
			if (!(kHeld & (KEY_LEFT | KEY_RIGHT)))
				CurGame->MovePaddle(0);
			// Run Game Logic
			CurGame->Move();
			// Update the Screen
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			CurGame->Draw();
			C3D_FrameEnd(0);
			frame++;
			break;
		case state_betathanks:
			if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) {
				PreviousGameState = GameState;
				GameState = state_title;
			}
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			DrawTexture(GetImage(uiSheet, ui_thanksbeta_idx), 80, 20);
			C3D_FrameEnd(0);
			break;
		case state_extra0:
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			DrawTexture(GetImage(spriteSheet, sprites_brick00_idx), 20, 20, 2.0f, 2.0f);
			DrawTexture(GetImage(spriteSheet, sprites_paddle_idx), 350, 110);
			DrawTexture(GetImage(uiSheet, ui_waveform_idx), 40, 97);
			C3D_FrameEnd(0);
			if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) {
				PreviousGameState = GameState;
				GameState = state_title;
				UpdateText = true;
			}
			break;
		case state_editor:
			// Handle Controls
			if (kDown & KEY_B) {
				PreviousGameState = GameState;
				GameState = state_title;
				UpdateText = true;
			}
			if (kDown & KEY_UP) {
				if (cursorY > 0)
					cursorY--;
				else
					cursorY = 4;
			}
			if (kDown & KEY_DOWN) {
				if (cursorY < 4)
					cursorY++;
				else
					cursorY = 0;
			}
			if (kDown & KEY_LEFT) {
				if (cursorX > 0)
					cursorX--;
				else
					cursorX = 9;
			}
			if (kDown & KEY_RIGHT) {
				if (cursorX < 9)
					cursorX++;
				else
					cursorX = 0;
			}
			if (kDown & KEY_START) {
				std::vector<Brick> newData;
				for (int y = 0; y < 5; y++)
					for (int x = 0; x < 10; x++)
						if (workingLevel[y][x] > 0)
							newData.push_back(Brick(x * 40 + 2, y * 20 + 2, 36, 16, workingLevel[y][x] - 1));
				SetLevel(1, 0, newData);
				Games[0] = Game(1);
				CurGame = &Games[0];
				PreviousGameState = GameState;
				GameState = state_game;
				UpdateText = true;
			}
			if (kDown & (KEY_L | KEY_Y | KEY_ZL)) {
				if (workingLevel[cursorY][cursorX] > 0)
					workingLevel[cursorY][cursorX]--;
				else
					workingLevel[cursorY][cursorX] = 11;
			}
			if (kDown & (KEY_R | KEY_X | KEY_ZR)) {
				if (workingLevel[cursorY][cursorX] < 11)
					workingLevel[cursorY][cursorX]++;
				else
					workingLevel[cursorY][cursorX] = 0;
			}
			// Update the Screen
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			for (int y = 0; y < 5; y++)
				for (int x = 0; x < 10; x++)
					if (workingLevel[y][x] > 0)
						Brick(x * 40 + 2, y * 20 + 2, 36, 16, workingLevel[y][x] - 1).Draw();
			C3D_FrameEnd(0);
			frame++;
			break;
		case state_exit:
			break;
		default:
			GameState = PreviousGameState;
		}

		if (UpdateText) {
			switch (GameState) {
			case state_title:
				consoleSelect(&bottomScreen);
				consoleClear();
				std::cout << ConsoleColor("0", false);
				std::cout << ConsoleMove(0, 2) << "Press Start to begin.\n";
				std::cout << "Press X to see what I'm working on or have planned.\n";
				std::cout << "Press Y to open level editor.\n\n";
				std::cout << "Compatibility Mode: " << (OdsMode ? "On" : "Off") << "\n";
				//std::cout << "(Turn on if not using a 'New' 3DS/2DS)" "\n" "Press R\n";
				//std::cout << "May cause undesireable results on VERY" "\n" "rare occasions.\n";

				//Create a graphic text class, specifically for the press [button] to start game text to flash every 30 frames it toggles
				break;
			case state_game:
				consoleSelect(&bottomScreen);
				consoleClear();
				std::cout << ConsoleColor("0", false);
				std::cout << ConsoleMove(0, 13);
				std::cout << "Score: " << "POINTS GO HERE" << "\nLives: " << lives << "\n";
				//std::cout << "Collision being tested " << (OdsMode ? 100 : 300) << "x/frame.\n";
				std::cout << /*debug_string +*/ "\n";
				break;
			case state_betathanks:
				consoleSelect(&bottomScreen);
				consoleClear();
				std::cout << ConsoleMove(0, 0);
				std::cout << "[Press any key to return to the title.]\n";
				std::cout << "Thanks to:\n\n";
				std::cout << "Jared for helping me " << ConsoleColor(YellowF, true) << "Beta" << ConsoleColor(WhiteF, true) << " test.\n\n";
				std::cout << ConsoleColor(YellowF, false) << "StackOverflow" << ConsoleColor("0", false) << " for helping me with " << ConsoleColor(GreenF, true) << "c++\n\n" << ConsoleColor("0", false);
				std::cout << "The awesome " << ConsoleColor(RedF, false) << "3ds " << ConsoleColor(BlueF, false) << "homebrew" << ConsoleColor("0", false) << " community for\n";
				std::cout << "helping me with many things relating to\n";
				std::cout << ConsoleColor(BlueF, true) << "libctru" << ConsoleColor("0", false) << " and " << ConsoleColor(GreenF, false) << "sf2dlib" << ConsoleColor("0", false) << ".\n\n";
				std::cout << "Bryan for helping me with programming\n\n";
				std::cout << ConsoleColor(MagentaF, true) << "GBAtemp" << ConsoleColor("0", false) << " community for helping me with so";
				std::cout << "much! You guys are so helpful.\n\n\n";
				std::cout << "     And " << ConsoleColor(MagentaF, false) << "YOU" << ConsoleColor("0", false) << " for playing my game!\n";
				std::cout << "    And remember, all feedback and\n";
				std::cout << "       suggestions are welcome!\n";
				break;
			case state_extra0:
				consoleSelect(&bottomScreen);
				consoleClear();
				std::cout << ConsoleMove(0, 0);
				std::cout << "The blue cracked brick was orginally\n";
				std::cout << "going to be the design for the normal\n";
				std::cout << "brick but I decided to instead save that";
				std::cout << "for the future for bricks that take more";
				std::cout << "than one hit.\n\n";
				std::cout << "Future Gamemode: This idea came about\n";
				std::cout << "when I'd thought about all the people I\n";
				std::cout << "saw playing some new game on their\n";
				std::cout << "phones. It's this game where you aim\n";
				std::cout << "your ball, and it shoots out like 100\n";
				std::cout << "balls. And all the bricks in the game\n";
				std::cout << "take many hits, which is determined via\n";
				std::cout << "a number displayed on the brick.\n";
				std::cout << "So I figure I'll add this sometime.\n\n";
				std::cout << "These new graphics and SFX aren't\n";
				std::cout << "necessarily permanent. This update is\n";
				std::cout << "more to show off that actual graphics\n";
				std::cout << "and SFX can and have been added, and\n";
				std::cout << "they may very well be changed and\n";
				std::cout << "improved in the future, so if you don't\n";
				std::cout << "like what's presented to you now, then\n";
				std::cout << "don't worry because it can easily be\n";
				std::cout << "changed and I'm always open to\n";
				std::cout << "suggestions!\n";
				break;
			}

			consoleSelect(&infoKillWindow);
			consoleClear();
			std::cout << ConsoleColor("0", false);
			std::cout << "           Tap red area any time to exit";
			std::cout << "         Breakout Version: " << ConsoleColor(RedF, false) << VersionText << " " << ConsoleColor(YellowF, false) << VersionNumber;
			std::cout << ConsoleColor(GreenF, RedB, false) << "                    Build: " << BuildNumber;
			std::cout << "                                        "; // maybe add a randomized message
			//std::cout << ConsoleColor(GreenF, RedB, false) << "         ISHUPE Engine Version: " << EngineVersion;
			UpdateText = false;
		}

		if (frame == 61)
			frame = 0;

		if (touchInBox(touch, 0, 224, 320, 16)) {
			PreviousGameState = GameState;
			GameState = state_exit;
		}

		if (GameState == state_exit) {
			std::cout << "Exiting...";
			break;
		}
	}
	//close each custom level file

	C2D_SpriteSheetFree(spriteSheet);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	csndExit();

	return 0;
}


std::string ConsoleMove(int x, int y) {
	return std::string(ANSI) + std::string(y + ";" + x) + std::string(PEnd);
}

std::string ConsoleColor(std::string foreground, bool bright) {
	return std::string(ANSI) + foreground + (bright ? ";2" : "") + std::string(CEnd);
}

std::string ConsoleColor(std::string foreground, std::string background, bool bright) {
	return std::string(ANSI) + foreground + ";" + background + (bright ? ";2" : "") + std::string(CEnd);
}

bool touchInBox(touchPosition touch, int x, int y, int w, int h) {
	return (kDown & KEY_TOUCH && touch.px > x && touch.px < x + w && touch.py > y && touch.py < y + h);
}

void DrawTexture(C2D_Image image, float x, float y, float rotation, C2D_ImageTint* tint, float scaleX, float scaleY) {
	C2D_DrawImageAtRotated(image, x, y, 1.0f, rotation, tint, scaleX, scaleY);
}

void DrawTexture(C2D_Image image, float x, float y, float scaleX, float scaleY) {
	C2D_DrawImageAt(image, x, y, 1.0f, NULL, scaleX, scaleY);
}

void DrawTexture(C2D_Image image, float x, float y) {
	DrawTexture(image, x, y, 1.0f, 1.0f);
}
