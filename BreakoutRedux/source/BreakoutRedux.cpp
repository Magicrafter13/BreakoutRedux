#include "BreakoutRedux.hpp"
#include "Game.hpp"

u32 kDown, kHeld;
touchPosition touch;

C3D_RenderTarget* top, *bot;

C2D_SpriteSheet uiSheet;
C2D_SpriteSheet spriteSheet;
C2D_SpriteSheet powerupSheet;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

std::string VersionText = "  Beta ", VersionNumber = "01.07.02";
std::string BuildNumber = "18.04.22.1228", EngineVersion = "01.00.00";

std::string ConsoleMove(int x, int y) {
	return std::string(ANSI) + std::string(y + ";" + x) + std::string(PEnd);
}

std::string ConsoleColor(std::string foreground, bool bright) {
	return std::string (ANSI) + foreground + (bright ? ";2" : "") + std::string(CEnd);
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
	if (!uiSheet || !spriteSheet || !powerupSheet) svcBreak(USERBREAK_PANIC);

	//check if custom level files exist, if not, create them
	//load custom level files

	//initialize audio (if applicable still)

	srand(time(NULL));

	consoleInit(GFX_BOTTOM, &bottomScreen); consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox); consoleInit(GFX_BOTTOM, &debugBox);

	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	//create paddle and ball objects (create classes for them first)

	std::string GameState = "title";
	std::string PreviousGameState = "title";
	bool OdsMode = false;
	bool UpdateText = true, UpdateTextThanks = false;
	int CurPlayer = 0;
	Game *CurGame;
	Game Games[1] = { Game() };

	//read settings file if it exists, otherwise write it, also update it if applicable

	//Main loop
	while (aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&touch);
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		if (GameState == "title") {
			if (kDown & KEY_START) {
				Games[0] = Game();
				CurGame = &Games[0];
				//resetBalls(the_ball);
				//the_paddle.reset();
				//setNewBallAngle(the_ball[0].angle);
				/*for (int i = 0; i < def_level_count; i++)
					for (int j = 0; j < 50; j++)
						brick_array[i][j].reset();
				level = 1; points = 0; last_power = 0;
				times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
				the_ball[0].is_attached = true;*/
				PreviousGameState = GameState;
				GameState = "game";
				UpdateText = true;
			}
			if (kDown & KEY_X) {
				PreviousGameState = GameState;
				GameState = "extra0"; //extras 10/13/2017
				UpdateText = true;
			}
			if (kDown & KEY_Y) {
				PreviousGameState = GameState;
				GameState = "editor"; //level designer
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
		} else if (GameState == "game") {
			if (kDown & KEY_START /* || lives equal 0 */) /*set return state to 2*/ //create a player class/object
			{
				GameState = PreviousGameState;
			 	PreviousGameState = "game";
			}
			for (Ball *ball : CurGame->GetBalls()) {
				if (kDown & KEY_LEFT) ball->AddDegree(-50);
				if (kDown & KEY_RIGHT) ball->AddDegree(50);
				if (kDown & KEY_UP) ball->AddDegree(10);
				if (kDown & KEY_DOWN) ball->AddDegree(-10);
				if (kHeld & KEY_L) ball->AddDegree(-5);
				if (kHeld & KEY_R) ball->AddDegree(5);
				if (kDown & KEY_X) ball->Up(5); //these should be temporary
				if (kDown & KEY_B) ball->Up(-5);
			}
			if (kHeld & KEY_Y) CurGame->GetPaddle()->Move(-3);
			if (kHeld & KEY_A) CurGame->GetPaddle()->Move(3);
			//rest of game code happens

			for (Ball* ball : CurGame->GetBalls()) {
				ball->Update(CurGame->GetBricks(), *(CurGame->GetPaddle()));
			}
			//set ball trails, unless this is implemented inside the ball class

			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			//for each powerup, move add one to its y position
			//if powerup is off screen, remove it from the arraylist of onscreen powerups
			for (Brick brick : *(CurGame->GetBricks())) {
				if (brick.exists()) brick.Draw();
			}
			CurGame->GetPaddle()->Draw();
			//for each ball, for (int i = 7; i > 0; i--) draw-scaled extra ball[i], end loop, draw ball
			for (Ball *ball : CurGame->GetBalls()) {
				ball->Draw();
			}
			//[original code] pp2d_draw_texture_scale(extraBallID[i], (tBall.trail_new_frame_circle[i].x - tBall.trail_new_frame_circle[i].rad) + 1.0, (tBall.trail_new_frame_circle[i].y - tBall.trail_new_frame_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0); //RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i))
			C3D_FrameEnd(0);
		} else if (GameState == "betathanks") {
			if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) { //set return state to 2
				PreviousGameState = GameState;
				GameState = "title";
			}
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			DrawTexture(GetImage(uiSheet, ui_thanksbeta_idx), 80, 20);
			C3D_FrameEnd(0);
		} else if (GameState == "extra0") {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.585f, 0.585f, 0.585f, 1.0f));
			C2D_SceneBegin(top);
			DrawTexture(GetImage(spriteSheet, sprites_brick00_idx), 20, 20, 2.0f, 2.0f);
			DrawTexture(GetImage(spriteSheet, sprites_paddle_idx), 350, 110);
			DrawTexture(GetImage(uiSheet, ui_waveform_idx), 40, 97);
			C3D_FrameEnd(0);
			if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) {
				PreviousGameState = GameState;
				GameState = "title";
			}
		} else if (GameState == "editor") {
			//stuff
			PreviousGameState = GameState;
			GameState = "title";
		}

		if (UpdateText) {
			if (GameState == "title" || (GameState == "exit" && PreviousGameState == "title")) {
				consoleSelect(&bottomScreen); consoleClear();
				std::cout << ConsoleColor("0", false);
				std::cout << ConsoleMove(0, 2) << "Press Select to begin.\n";
				std::cout << "Press X to see what I'm working on or have planned.\n";
				std::cout << "Press Y to open level editor.\n\n";
				std::cout << "Compatibility Mode: " << (OdsMode ? "On" : "Off") << "\n";
				std::cout << "(Turn on if not using a 'New' 3DS/2DS)" "\n" "Press R\n";
				std::cout << "May cause undesireable results on VERY" "\n" "rare occasions.\n";

				//Create a graphic text class, specifically for the press [button] to start game text to flash every 30 frames it toggles
			} else if (GameState == "game" || (GameState == "exit" && PreviousGameState == "game")) {
				consoleSelect(&bottomScreen); consoleClear();
				std::cout << ConsoleColor("0", false);
				std::cout << ConsoleMove(0, 13);
				std::cout << "Score: " << "POINTS GO HERE" << "\nLives: " << "LIVES GO HERE" << "\n";
				std::cout << "Collision being tested " << (OdsMode ? 100 : 300) << "x/frame.\n";
				std::cout << /*debug_string +*/ "\n";
			} else if (GameState == "betathanks" || (GameState == "exit" && PreviousGameState == "betathanks")) {
				consoleSelect(&bottomScreen); consoleClear();
				std::cout << ConsoleMove(0,0);
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
			} else if (GameState == "extra0" || (GameState == "exit" && PreviousGameState == "extra0")) {
				consoleSelect(&bottomScreen); consoleClear();
				std::cout << ConsoleMove(0,0);
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
			}

			consoleSelect(&killBox); consoleClear();
			std::cout << ConsoleColor(RedF, RedB, false);
			for (int i = 0; i < 80; i++) std::cout << " ";

			consoleSelect(&versionWin); consoleClear();
			std::cout << ConsoleColor("0", false);
			std::cout << "     Tap red area any time to exit";
			std::cout << "Breakout Version: " << ConsoleColor(RedF, false) << VersionText << " " << ConsoleColor(YellowF, false) << VersionNumber;
			std::cout << ConsoleColor(GreenF, RedB, false) << "              Build: " << BuildNumber;
			std::cout << ConsoleColor(GreenF, RedB, false) << "   ISHUPE Engine Version: " << EngineVersion;
			UpdateText = false;
		}

		if (touchInBox(touch, 0, 224, 320, 16)) {
			PreviousGameState = GameState;
			GameState = "exit";
		}

		if (GameState == "exit") {
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
