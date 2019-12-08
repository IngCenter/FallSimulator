#include <TXLib.h>
#include "Consts.h"
#include "Button.h"
#include "MapPart.h"
#include "ElemFunctions.h"

#include <iostream>
#include <fstream>

#pragma once

//This library containing game mode functions

void drawMenu();
void background(COLORREF color);
void loadingAnimation(int delay, int speed);
void mainFunc();
void playGame(MapPart gettedMapParts[]);

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
    drawButton(buttonPlay);

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_TRANSPARENT);

    drawButton(buttonHelp);

    txSleep(50);

    while (!GetAsyncKeyState('Q') && !GetAsyncKeyState(VK_ESCAPE)) {
        if (!lvlCreatingIsStarted && !gameIsStarted) {
            if (In(txMousePos(), buttonStart.coords) && txMouseButtons() & 1) {
                while (txMouseButtons() & 1) {
                    txSleep(10);
                }

                loadingAnimation(2, 7);
                txSleep(50);

                lvlCreatingIsStarted = true;
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

            if (In(txMousePos(), buttonPlay.coords) && txMouseButtons() & 1) {

                int arrElem = readFile("level1.fslvl", gettedMapParts);

                gameIsStarted = true;
            }
        }

        if (lvlCreatingIsStarted || gameIsStarted) {
            break;
        }

        txSleep(10);
    }
}

void background(COLORREF color)
{
    txSetFillColor(color);
    txClear();
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

    while (!GetAsyncKeyState('Q') || !GetAsyncKeyState(VK_ESCAPE)) {
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

            txBitBlt(txDC(),
                     txMouseX() - 30, txMouseY() - 30,
                     60, 60, mapParts[selectedPict].picture);
        }
        if (selectedPict >= 0 && !(txMouseButtons() & 1)) {

            RECT elRectCoords = {
                (round((txMouseX() - 30)  / 60) * 60),
                (round((txMouseY() - 30)  / 60) * 60),
                (round((txMouseX() + 30)  / 60) * 60),
                (round((txMouseY() + 30)  / 60) * 60)
            };

            mapParts[selectedPict].coords = elRectCoords;
            checkElem(mapParts);

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

        if (GetAsyncKeyState('Q') || GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }
    }
}

void playGame(MapPart gettedMapParts[])
{
    int minX = gettedMapParts[0].coords.left;
    int iMin = 0;

    for (int i = 0; i < arrElem; i++) {

        if (gettedMapParts[i].coords.left < minX)
        {
            minX = gettedMapParts[i].coords.left;
            iMin = i;
        }
    }

    int player_x = gettedMapParts[iMin].coords.left;
    int player_y = gettedMapParts[iMin].coords.top - (PLAYER_SIZE + 5);

    int player_speed = PLAYER_SIZE;

    while (!GetAsyncKeyState('Q') && !GetAsyncKeyState(VK_ESCAPE)) {

        background(TX_WHITE);

        txSetColor(MY_RED, 3);
        txSetFillColor(MY_RED);

        txRectangle(player_x,
                    player_y,
                    player_x + PLAYER_SIZE,
                    player_y + PLAYER_SIZE);

        for (int i = 0; i < MAP_LENGHT; i++) {

            if (gettedMapParts[i].visible) {

                txBitBlt(txDC(),
                    gettedMapParts[i].coords.left,
                    gettedMapParts[i].coords.top,
                    60, 60,
                    gettedMapParts[i].picture
                );
            }
        }

        if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) {
            player_y -= player_speed;
        }
        if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)) {
            player_x -= player_speed;
        }
        if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) {
            player_y += player_speed;
        }
        if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)) {
            player_x += player_speed;
        }

        if (GetAsyncKeyState(VK_OEM_PLUS)) {
            player_speed += BLOCK_SIZE / 10;
        }
        if (GetAsyncKeyState(VK_OEM_MINUS)) {
            player_speed -= BLOCK_SIZE / 10;
        }

        txSleep(50);
    }
}

