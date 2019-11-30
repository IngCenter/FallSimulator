#include <TXLib.h>
#include <iostream>
#include <fstream>

using namespace std;

const COLORREF MY_LIGHTBLUE = RGB(75, 127, 196);
const COLORREF MY_BISQUE    = RGB(255, 214, 89);

const int      BLOCK_SIZE   = 120;

const int      BLOCK_TYPE   = 0;
const int      QUEST_TYPE   = 1;
const int      WATER_TYPE   = 2;
const int      FIRE_TYPE    = 3;

const int      MAP_LENGHT   = 15;

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

HDC light_stone;
HDC dark_stone;
HDC vdark_stone;

void background(COLORREF color);
void drawMenu();
void drawButton(Button but);
void loadingAnimation(int delay, int speed);
void mainFunc();
bool addingBlock(bool clicked, RECT blockBut, HDC pic,
                 int blocktype, int* arrElem, MapPart mapParts[]);

void addingStone(int* arrElem, MapPart mapParts[]);
int  findElem(int* arrElem, RECT expCoords);

int main()
{
    txCreateWindow(1300, 600);

    block       = txLoadImage("pictures\\block.bmp");
    quest       = txLoadImage("pictures\\question.bmp");
    water       = txLoadImage("pictures\\water.bmp");
    fire        = txLoadImage("pictures\\fire.bmp");

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
    txDrawText(0, 0, extentX, 50, "����������� ������� ���� FallSimulation");

    drawMenu();

    txEnd();

    txDeleteDC(block);
    txDeleteDC(quest);
    txDeleteDC(water);
    txDeleteDC(fire);

    txDeleteDC(light_stone);
    txDeleteDC(dark_stone);
    txDeleteDC(vdark_stone);

    txDisableAutoPause();
}

void background(COLORREF color)
{
    txSetFillColor(color);
    txClear();
}

void drawButton(Button but)
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
                but.text,
                DT_CENTER | DT_VCENTER);
}

void drawMenu()
{
    //button "Start" (to start level creating)
    Button buttonStart = {
        {
            middleX - 100, extentY / 3 - 50,
            middleX + 100, extentY / 3 + 50
        },  "Start"
    };

    //button "Exit"
    Button buttonExit = {
        {
            middleX - 100, extentY / 2 - 50,
            middleX + 100, extentY / 2 + 50
        },  "Exit"
    };

    //button "Help"
    Button buttonHelp = {
        {
            extentX - 100, 0,
            extentX     ,  50
        }, "? Help"
    };

    //button "Play" (to play on created level)
    Button buttonPlay = {
        {
            extentX - 100, extentY - 60,
            extentX,       extentY
        }, "Play"
    };

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);

    drawButton(buttonStart);
    drawButton(buttonExit);

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_TRANSPARENT);

    drawButton(buttonHelp);

    txSleep(50);
    bool flwindow = false;

    while (!GetAsyncKeyState('Q') || !GetAsyncKeyState(VK_ESCAPE)) {
        if (!gameIsStarted) {
            if (In(txMousePos(), buttonStart.coords) && txMouseButtons() & 1) {
                while (txMouseButtons() & 1) {
                    txSleep(10);
                }

                //loadingAnimation(2, 3);
                txSleep(50);
                mainFunc();
            }
            if (In(txMousePos(), buttonExit.coords)  && txMouseButtons() & 1) {
                while (txMouseButtons() & 1) {
                    txSleep(10);
                }

                break;
            }

            if (In(txMousePos(), buttonHelp.coords)) {

                txSetColor(TX_BLACK);
                txDrawText(buttonHelp.coords.left - 130,
                            buttonHelp.coords.bottom + 10,
                            txGetExtentX() - 10,
                            buttonHelp.coords.bottom + 100,
                            "This hyperlink will be\nopen in browser");
            }
            else {
                txSetColor(TX_WHITE);
                txSetFillColor(TX_WHITE);
                txRectangle(buttonHelp.coords.left - 130,
                            buttonHelp.coords.bottom + 10,
                            txGetExtentX() - 10,
                            buttonHelp.coords.bottom + 100);
            }

            if (In(txMousePos(), buttonHelp.coords)  && txMouseButtons() & 1) {
                while(txMouseButtons() & 1) {
                    txSleep(10);
                }

                system("start help\\index.html");
            }
            /*
            if (In(txMousePos(), buttonPlay.coords) && txMouseButtons() & 1) {

                MapPart gettedMapParts = readFile("level1.fslvl");

                for (int i = 0; i < MAP_LENGHT; i++) {

                    if (gettedMapParts.visible[i]) {

                        switch (gettedMapParts[i].blocktype)
                        {
                            default:
                                 txBitBlt(txDC(),
                                     gettedMapParts[i].coords.left,
                                     gettedMapParts[i].coords.top,
                                     60, 60,
                                     block
                                 );

                            case BLOCK_TYPE:
                                 txBitBlt(txDC(),
                                     gettedMapParts[i].coords.left,
                                     gettedMapParts[i].coords.top,
                                     60, 60,
                                     block
                                 );

                            case QUEST_TYPE:
                                 txBitBlt(txDC(),
                                     gettedMapParts[i].coords.left,
                                     gettedMapParts[i].coords.top,
                                     60, 60,
                                     quest
                                 );

                            case WATER_TYPE:
                                 txBitBlt(txDC(),
                                     gettedMapParts[i].coords.left,
                                     gettedMapParts[i].coords.top,
                                     60, 60,
                                     water
                                 );

                            case FIRE_TYPE:
                                 txBitBlt(txDC(),
                                     gettedMapParts[i].coords.left,
                                     gettedMapParts[i].coords.top,
                                     60, 60,
                                     fire
                                 );
                        }
                    }
                }
            }
            */
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
    int selectedPict = -1;
    const int BLOCK_SIZE = 120;

    RECT blockBut = {
        extentX - BLOCK_SIZE,     0,          extentX,     BLOCK_SIZE
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
        extentX - 120, extentY - 120, extentX, extentY
    };

    MapPart mapParts[MAP_LENGHT + 1];

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);
    Button completeButton = {doneBut, "\n\nSave\n\nFile"};

    for (int elem = 0; elem < MAP_LENGHT; elem++) {
        mapParts[elem].visible = false;
    }

    bool clickedBlock = false;
    bool clickedQuest = false;
    bool clickedWater = false;
    bool clickedFire  = false;

    while (!GetAsyncKeyState('Q')) {
        background(TX_WHITE);

        Win32::TransparentBlt(txDC(), blockBut.left, blockBut.top, 120, 120, block,
                              0, 0, 60, 60, -1);

        Win32::TransparentBlt(txDC(), questBut.left, questBut.top, 120, 120, quest,
                              0, 0, 60, 60, -1);

        Win32::TransparentBlt(txDC(), waterBut.left, waterBut.top, 120, 120, water,
                              0, 0, 60, 60, -1);

        Win32::TransparentBlt(txDC(), fireBut.left,  fireBut.top, 120, 120, fire,
                              0, 0, 60, 60, -1);

        drawButton(completeButton);

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

        //blocks
        clickedBlock = addingBlock(clickedBlock, blockBut, block, BLOCK_TYPE, &arrElem, mapParts);
        clickedQuest = addingBlock(clickedQuest, questBut, quest, QUEST_TYPE, &arrElem, mapParts);
        clickedWater = addingBlock(clickedWater, waterBut, water, WATER_TYPE, &arrElem, mapParts);
        clickedFire  = addingBlock(clickedFire,  fireBut,  fire,  FIRE_TYPE,  &arrElem, mapParts);

        //checking
        if (clickedBlock)
        {
            clickedQuest = false;
            clickedWater = false;
            clickedFire = false;
        }
        else if (clickedQuest)
        {
            clickedWater = false;
            clickedFire = false;
        }
        else if (clickedWater)
        {
            clickedFire = false;
        }

        //selecting block
        for (int i = 0; i < arrElem; i++) {
            if (selectedPict < 0 &&
                In(txMousePos(), mapParts[i].coords) && txMouseButtons() & 1 &&
                !(clickedBlock || clickedQuest || clickedWater || clickedFire)) {

                selectedPict = i;
            }
        }

        //deleting picture
        for (int i = 0; i < arrElem; i++) {
            if (selectedPict < 0 &&
                In(txMousePos(), mapParts[i].coords) && txMouseButtons() & 2 &&
                !(clickedBlock || clickedQuest || clickedWater || clickedFire)) {

                selectedPict = i;
                mapParts[selectedPict] = mapParts[arrElem - 1];
                mapParts[arrElem - 1].visible = false;

                arrElem--;
                selectedPict = -1;
            }
        }

        //moving picture
        if (selectedPict >= 0 && txMouseButtons() & 1) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30,
                     60, 60, mapParts[selectedPict].picture);
        }
        if (selectedPict >= 0 && !(txMouseButtons() & 1)) {
            mapParts[selectedPict].coords = {
                txMouseX() - 30, txMouseY() - 30,
                txMouseX() + 30, txMouseY() + 30
            };
            selectedPict = -1;
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

                        default:
                             lvlfile << "Null,";
                             break;
                    }

                    mapParts[elem].coords.left    =
                    round((mapParts[elem].coords.left + 30) / 60) * 60;

                    mapParts[elem].coords.top     =
                    round((mapParts[elem].coords.top  + 30) / 60) * 60;

                    mapParts[elem].coords.right   =
                    round((mapParts[elem].coords.right  + 30) / 60) * 60;

                    mapParts[elem].coords.bottom  =
                    round((mapParts[elem].coords.bottom  + 30) / 60) * 60;

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

            txMessageBox("Level File created!", "Information");
        }

        txSleep(10);
    }
}

bool addingBlock(bool clicked, RECT blockBut, HDC pic,
                 int blocktype, int* arrElem, MapPart mapParts[])
{
    if (In(txMousePos(), blockBut) && txMouseButtons() & 1) {
        clicked = true;
    }

    if (txMouseButtons() & 1 && clicked) {
        txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, pic);
    }

    if (!(txMouseButtons() & 1) && clicked) {

        if (*arrElem < MAP_LENGHT) {

            if (txMouseX() < txGetExtentX() - BLOCK_SIZE) {

                mapParts[*arrElem] = {
                    {
                        txMouseX() - 30, txMouseY() - 30, txMouseX() + 30, txMouseY() + 30
                    },  true, pic, blocktype
                };

                (*arrElem)++;
            }
        }
        else {

            char maplen_str[50];

            sprintf(maplen_str, "You cannot add more than %d blocks", MAP_LENGHT);
            txMessageBox(maplen_str, "Error");

            (*arrElem)--;
        }

        clicked = false;
    }

    return clicked;
}

