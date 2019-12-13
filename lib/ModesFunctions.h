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

//function for drawing menu and handling clicks in main menu
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

//function for clear background
void background(COLORREF color)
{
    txSetFillColor(color);
    txClear();
}

//loading animation
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

//main editor function
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

//main play mode function
void playGame(MapPart gettedMapParts[])
{
    int minX = gettedMapParts[0].coords.left;
    int iMin = 0;

    bool isGameOver = false;

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

        int old_y = player_y;
        for (int i = 0; i < MAP_LENGHT; i++) {

            if (gettedMapParts[i].visible) {

                if  (player_y                <= gettedMapParts[i].coords.top   &&
                    (player_y + PLAYER_SIZE) >= gettedMapParts[i].coords.top   &&
                     player_x                <  gettedMapParts[i].coords.right &&
                     player_x + PLAYER_SIZE  >  gettedMapParts[i].coords.left) {

                    switch (gettedMapParts[i].blocktype)
                    {
                        default:
                             player_y -= player_speed;

                        case BLOCK_TYPE:
                             player_y -= player_speed;
                             break;

                        case QUEST_TYPE:
                             player_y -= player_speed;
                             break;

                        case WATER_TYPE:
                             player_y -= player_speed;

                             txMessageBox("You lose...", "Game over");

                             isGameOver = true;
                             break;

                        case FIRE_TYPE:
                             player_y -= player_speed;

                             txMessageBox("You lose...", "Game over");

                             isGameOver = true;
                             break;
                    }
                }
            }
        }

        if (old_y == player_y) {
            player_y += player_speed;
        }

        if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) {
            player_y -= 2 * player_speed;
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
        if (GetAsyncKeyState(VK_SPACE)) {
            player_y -= 5 * player_speed;
        }

        if (GetAsyncKeyState(VK_OEM_PLUS)) {
            player_speed += BLOCK_SIZE / 10;
        }
        if (GetAsyncKeyState(VK_OEM_MINUS)) {
            player_speed -= BLOCK_SIZE / 10;
        }

        if (player_y > txGetExtentY()) {
            player_y -= player_speed;
        }

        if (player_x > txGetExtentX()) {
            txMessageBox("You win!", "Winner");
            break;
        }

        if (isGameOver) {
            break;
        }

        txSleep(50);
    }
}

