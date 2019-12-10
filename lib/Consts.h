#pragma once

//This library containing main constants and global variables

const COLORREF MY_LIGHTBLUE = RGB(75,  127, 196);
const COLORREF MY_BISQUE    = RGB(255, 214, 89);
const COLORREF MY_RED       = RGB(237, 28,  36);

const int      BLOCK_SIZE   = 120;
const int      PLAYER_SIZE  = BLOCK_SIZE / 5;

const int      BLOCK_TYPE   = 0;
const int      QUEST_TYPE   = 1;
const int      WATER_TYPE   = 2;
const int      FIRE_TYPE    = 3;

const int      MAP_LENGHT   = 15;

int middleX;
int middleY;

int extentX;
int extentY;

int arrElem = 0;

bool lvlCreatingIsStarted = false;
bool gameIsStarted = false;

HDC block;
HDC quest;
HDC water;
HDC fire;

HDC light_stone;
HDC dark_stone;
HDC vdark_stone;
