# Breakout Redux
A Breakout clone for the 3DS (Using ctrulib and citro3d/2d)  
A complete rewrite of my previous attempt: [Breakout](https://github.com/Magicrafter13/Breakout)

-Written by Magicrafter13 (Matthew Rease).

### Screenshots

(Pending...)  
![Screenshot1](/BreakoutLeft.png) ![Screenshot2](/BreakoutRight.png)


# Play

(Not in a playable state yet. Check back later!)  
To play this, just download a precompiled binary (3dsx, cia, or elf) from the releases tab.

## Controls

#### Current Testing Controls ####  
* __D-Pad Left/Right__ Subtracts or Adds 5 degrees to the balls angle respectively
* __D-Pad Up/Down__ Adds or Subtracts 1 degree from the balls angle respectively
* __L and R__ Subtacts or Adds 0.5 degrees to the balls angle respectively
* __X and B__ Adds or Subtracts 5 from the balls y coordinate respectively
* __Y and A__ Moves the paddle left or right respectively

#### In - Game ####  
* __D-Pad__ or Joystick to move Left and Right
* __A Button__ to launch ball (when ball attached) or shoot laser if power up has been acquired.
* __Select__ to start game (or return to title)
* __Y__ to open Level editor
* __Start__ to exit

#### In Level Editor ####  
* __D-Pad__ or __Joystick__ to move cursor
* __L__ and __R__ to switch brick type
* __X__ to save level layout
* __B__ or __Start__ to return to title
* __Y__ to randomize brick layout
* __Select__ play your level

## "Nightlies"  
(Not currently available)  
Available on my website. [Click Here!](http://oldforgeinn.ddns.net/Games/?game=Breakout#download)

# Edit / self-compile  
To compile this game, you shouldn't need to install anything extra, as long as you have libctru and citro3d (and citro2d) it should work, if not make sure to tell me.  

Also [bannertool.exe](https://github.com/Steveice10/bannertool/releases) needs to be added to `c:/devkitPro/msys2/opt/bin` on Windows

Just go to the BreakoutRedux subfolder and run `make` in the command line.  
