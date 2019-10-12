#include <TXLib.h>
#include <iostream>
#include <fstream>

using namespace std;

const COLORREF MY_LIGHTBLUE = RGB(75, 127, 196);
const COLORREF MY_BISQUE    = RGB(255, 214, 89);

const int      BLOCK_TYPE   = 0;
const int      QUEST_TYPE   = 1;
const int      WATER_TYPE   = 2;
const int      FIRE_TYPE    = 3;

const int      MAP_LENGHT   = 5;

int middleX;
int middleY;

int extentX;
int extentY;

bool gameIsStarted = false;

struct Button {
    RECT coords;
    const char* text;
    HDC picture;
};

struct MapPart {
    RECT coords;
    bool visible;
    HDC picture;
    int blocktype;
};

HDC block;
HDC quest;
HDC water;
HDC fire;

HDC orange_but;
HDC green_but;
HDC blue_but;
HDC purple_but;

void background(COLORREF color);
void drawMenu();
void drawButton(Button but, bool picture);
void loadingAnimation(int delay, int speed);
void mainFunc();

int main()
{
    txCreateWindow(1300, 600);

    block      = txLoadImage("pictures\\block.bmp");
    quest      = txLoadImage("pictures\\question.bmp");
    water      = txLoadImage("pictures\\water.bmp");
    fire       = txLoadImage("pictures\\fire.bmp");
    //orange_but = txLoadImage("pictures\\orange-but.bmp");
    //green_but  = txLoadImage("pictures\\green-but.bmp");
    //blue_but   = txLoadImage("pictures\\blue-but.bmp");
    //purple_but = txLoadImage("pictures\\purple-but.bmp");

    middleX = txGetExtentX() / 2;
    middleY = txGetExtentY() / 2;
    extentX = txGetExtentX();
    extentY = txGetExtentY();

    background(TX_WHITE);
    txSleep(50);

    txBegin();

    txSleep(1000);

    txSetColor(MY_LIGHTBLUE, 2);
    txSetFillColor(TX_WHITE);
    txRectangle(0, 0, extentX, extentY);

    txSetColor(MY_LIGHTBLUE, 2);
    txSetFillColor(MY_LIGHTBLUE);
    txRectangle(0, 0, extentX, 50);

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);
    txDrawText(0, 0, extentX, 50, "Конструктор уровней игры FallSimulation");

    drawMenu();

    txEnd();

    txDeleteDC(block);
    txDeleteDC(quest);
    txDeleteDC(water);
    txDeleteDC(fire);

    txDisableAutoPause();
}

void background(COLORREF color)
{
    txSetFillColor(color);
    txClear();
}

void drawButton(Button but, bool picture)
{
    //drawing button
    txRectangle(but.coords.left,
                but.coords.top,
                but.coords.right,
                but.coords.bottom);

    //drawing text
    txDrawText (but.coords.left,
                but.coords.top,
                but.coords.right,
                but.coords.bottom,
                but.text);
}

void drawMenu()
{
    //button "Play"
    Button buttonPlay = {
        {
            middleX - 100, extentY / 3 - 50,
            middleX + 100, extentY / 3 + 50
        },  "Play"
    };

    //button "Exit"
    Button buttonExit = {
        {
            middleX - 100, extentY / 2 - 50,
            middleX + 100, extentY / 2 + 50
        },  "Exit"
    };

    //button "Settings"
    Button buttonSets = {
        {
            extentX - 100, 0,
            extentX     , 50
        }, "Settings"
    };

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);

    drawButton(buttonPlay, true);
    drawButton(buttonExit, true);

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_TRANSPARENT);

    drawButton(buttonSets, false);

    txSleep(50);

    while (!GetAsyncKeyState('Q')) {
        if (!gameIsStarted) {
            if (In(txMousePos(), buttonPlay.coords) && txMouseButtons() & 1) {
                while (txMouseButtons() & 1) {
                    txSleep(10);
                }

                //loadingAnimation(2, 3);
                txSleep(50);
                mainFunc();
            }
            if (In(txMousePos(), buttonExit.coords) && txMouseButtons() & 1) {
                while (txMouseButtons() & 1) {
                    txSleep(10);
                }

                break;
            }
            if (In(txMousePos(), buttonSets.coords) && txMouseButtons() & 1) {
                while(txMouseButtons() & 1) {
                    txSleep(10);
                }

                txSetColor(TX_BLACK, 3);
                txSetFillColor(MY_BISQUE);

                txRectangle(middleX - 200, middleY - 100, middleX + 200, middleY + 100);
                //code
            }
        }

        txSleep(10);
    }
}

void loadingAnimation(int delay, int speed)
{
    background(TX_WHITE);

    for (int circle_radius = 0;
        circle_radius * circle_radius < extentX * extentX + extentY * extentY;
        circle_radius += speed) {

        background(TX_WHITE);

        txSetColor(TX_BLACK, 2);
        txSetFillColor(TX_BLACK);

        txCircle(0, 0, circle_radius);

        txSleep(delay);
    }

    background(TX_WHITE);
}

void mainFunc()
{
    gameIsStarted = true;

    int arrElem = 0;
    const int BLOCK_SIZE = 120;

    RECT blockBut = {
        extentX - BLOCK_SIZE,     0,          extentX, BLOCK_SIZE
    };

    RECT questBut = {
        extentX - BLOCK_SIZE,     BLOCK_SIZE, extentX, 2 * BLOCK_SIZE
    };

    RECT waterBut = {
        extentX - BLOCK_SIZE, 2 * BLOCK_SIZE, extentX, 3 * BLOCK_SIZE
    };
    RECT fireBut = {
        extentX - BLOCK_SIZE, 3*  BLOCK_SIZE, extentX, 4 * BLOCK_SIZE
    };



    RECT doneBut = {
        extentX - 60, extentY - 60, extentX, extentY
    };


    MapPart mapParts[MAP_LENGHT + 1];

    Button completeButton = {doneBut, "Done"};

    for (int elem = 0; elem < MAP_LENGHT; elem++) {
        mapParts[elem].visible = false;
    }

    bool clickedBlock = false;
    bool clickedQuest = false;
    bool clickedWater = false;
    bool clickedfire  = false;

    while (!GetAsyncKeyState('Q')) {
        background(TX_WHITE);

        Win32::TransparentBlt(txDC(), blockBut.left, blockBut.top, 120, 120, block,
                              0, 0, 60, 60, -1);

        Win32::TransparentBlt(txDC(), questBut.left, questBut.top, 120, 120, quest,
                              0, 0, 60, 60, -1);

        Win32::TransparentBlt(txDC(), waterBut.left, waterBut.top, 120, 120, water,
                              0, 0, 60, 60, -1);
        Win32::TransparentBlt(txDC(), fireBut.left, fireBut.top, 120, 120, fire,
                              0, 0, 60, 60, -1);

        drawButton(completeButton, false);

        for (int elem = 0; elem < MAP_LENGHT; elem++) {

            mapParts[elem].coords.left = round((mapParts[elem].coords.left + 30) / 60) * 60;
            mapParts[elem].coords.top  = round((mapParts[elem].coords.top  + 30) / 60) * 60;

            if (mapParts[elem].visible) {

                txBitBlt(txDC(),
                        mapParts[elem].coords.left,
                        mapParts[elem].coords.top,
                        60, 60,
                        mapParts[elem].picture
                );
            }
        }

        //block "block"
        if (In(txMousePos(), blockBut) && txMouseButtons() & 1) {
            clickedBlock = true;
        }
        if (txMouseButtons() & 1 && clickedBlock) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, block);

        }
        if (!(txMouseButtons() & 1) && clickedBlock) {

            if (arrElem < MAP_LENGHT) {
                mapParts[arrElem] = {
                    {
                        txMouseX() - 30, txMouseY() - 30, txMouseX() + 30, txMouseY() + 30
                    },  true, block, BLOCK_TYPE
                };
                arrElem++;
            }
            else {
                char maplen_str[50];
                sprintf(maplen_str, "You cannot add more than %d blocks", MAP_LENGHT);
                txMessageBox(maplen_str, "Error");
                arrElem--;
            }

            //arrElem++;
            clickedBlock = false;
        }

        //block "quest"
        if (In(txMousePos(), questBut) && txMouseButtons() & 1) {
            clickedQuest = true;
        }
        if (txMouseButtons() & 1 && clickedQuest) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, quest);
        }
        if (!(txMouseButtons() & 1) && clickedQuest) {

            if (arrElem < MAP_LENGHT) {
                mapParts[arrElem] = {
                    {
                        txMouseX() - 30, txMouseY() - 30, txMouseX() + 30, txMouseY() + 30
                    },  true, quest, QUEST_TYPE
                };
                arrElem++;
            }
            else {
                char maplen_str[50];
                sprintf(maplen_str, "You cannot add more than %d blocks", MAP_LENGHT);
                txMessageBox(maplen_str, "Error");
                arrElem--;
            }

            clickedQuest = false;
        }

        //block "water"
        if (In(txMousePos(), waterBut) && txMouseButtons() & 1) {
            clickedWater = true;
        }
        if (txMouseButtons() & 1 && clickedWater) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, water);
        }
        if (!(txMouseButtons() & 1) && clickedWater) {

            if (arrElem < MAP_LENGHT) {
                mapParts[arrElem] = {
                    {
                        txMouseX() - 30, txMouseY() - 30, txMouseX() + 30, txMouseY() + 30
                    },  true, water, WATER_TYPE
                };
                arrElem++;
            }
            else {
                char maplen_str[50];
                sprintf(maplen_str, "You cannot add more than %d blocks", MAP_LENGHT);
                txMessageBox(maplen_str, "Error");
                arrElem--;
            }

            clickedWater = false;
        }

        //block "fire"
           if (In(txMousePos(), fireBut) && txMouseButtons() & 1) {
            clickedfire = true;
        }
        if (txMouseButtons() & 1 && clickedfire) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, fire);
        }
        if (!(txMouseButtons() & 1) && clickedfire) {

            if (arrElem < MAP_LENGHT) {
                mapParts[arrElem] = {
                    {
                        txMouseX() - 30, txMouseY() - 30, txMouseX() + 30, txMouseY() + 30
                    },  true, fire, FIRE_TYPE
                };
                arrElem++;
            }
            else {
                char maplen_str[50];
                sprintf(maplen_str, "You cannot add more than %d blocks", MAP_LENGHT);
                txMessageBox(maplen_str, "Error");
                arrElem--;
            }

            clickedfire = false;
        }

        //button to complete LevelCreating
        if (In(txMousePos(), doneBut) && txMouseButtons() & 1) {
            while (txMouseButtons() & 1) {
                txSleep(10);
            }

            ofstream lvlfile;
            lvlfile.open("level1.fslvl");
            for (int elem = 0; elem < arrElem; elem++) {
                if (mapParts[elem].visible) {
                    switch(mapParts[elem].blocktype)
                    {
                        case BLOCK_TYPE:
                             lvlfile << "Block,";
                             break;
                        case QUEST_TYPE:
                             lvlfile << "Quest,";
                             break;
                        case WATER_TYPE:
                             lvlfile << "Water,";
                             break;
                        case FIRE_TYPE:
                             lvlfile << "Fire,";
                             break;
                    }

                    lvlfile << mapParts[elem].coords.left;
                    lvlfile << ",";
                    lvlfile << mapParts[elem].coords.top;
                    lvlfile << ",";
                    lvlfile << mapParts[elem].coords.right;
                    lvlfile << ",";
                    lvlfile << mapParts[elem].coords.bottom;
                    lvlfile << "\n";

                    lvlfile << "";
                }
            }
        }

        txSleep(10);
    }
}


