#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include "ui.h"
#include "sprites.h"
#include "powerups.h"

extern C2D_SpriteSheet uiSheet;
extern C2D_SpriteSheet spriteSheet;
extern C2D_SpriteSheet powerupSheet;
extern void DrawTexture(C2D_Image image, float x, float y, float rotation, C2D_ImageTint* tint, float scaleX, float scaleY);
extern void DrawTexture(C2D_Image image, float x, float y, float scaleX, float scaleY);
extern void DrawTexture(C2D_Image image, float x, float y);

#define ANSI "\x1b["
#define RedF "31"
#define RedB "41"
#define GreenF "32"
#define GreenB "42"
#define YellowF "33"
#define YellowB "43"
#define BlueF "34"
#define BlueB "44"
#define MagentaF "35"
#define MagentaB "45"
#define CyanF "36"
#define CyanB "46"
#define WhiteF "37"
#define WhiteB "47"
#define Bright "2"
#define CReset "\x1b[0m"
#define PReset "\x1b[0;0H"
#define ASep ";"
#define CEnd "m"
#define PEnd "H"

#define GetImage C2D_SpriteSheetGetImage
