#include <TXLib.h>
#include "Consts.h"
#include "ModesFunctions.h"

#include <iostream>
#include <fstream>

#pragma once

//This library containing functions for work with the elements

void drawButton(Button but);
bool addingBlock(bool clicked, RECT blockBut, HDC pic,
                 int blocktype, int* arrElem, MapPart mapParts[]);

int readFile(string file, MapPart gettedMapParts[]);
void checkElem(MapPart mapParts[]);

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

            RECT elRectCoords = {

                (round((txMouseX() - 30)  / 60) * 60),
                (round((txMouseY() - 30)  / 60) * 60),
                (round((txMouseX() + 30)  / 60) * 60),
                (round((txMouseY() + 30)  / 60) * 60)
            };

            if (txMouseX() < txGetExtentX() - BLOCK_SIZE)
                {

                mapParts[*arrElem] = {
                    elRectCoords, true, pic, blocktype
                };

                checkElem(mapParts);

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

int readFile(string file, MapPart gettedMapParts[])
{
    for (int i = 0; i < MAP_LENGHT; i++) {

        gettedMapParts[i].visible = false;
    }

    int arrElem = 0;

    ifstream lvlfile(file);
    string buff;

    while (lvlfile.good()) {

        getline(lvlfile, buff);

        if (buff.size() > 5) {
            int posComma  = buff.find(",");
            int posComma2 = buff.find(",", posComma  + 1);
            int posComma3 = buff.find(",", posComma2 + 1);
            int posComma4 = buff.find(",", posComma3 + 1);

            string buff1 = buff.substr(0,             posComma);
            string buff2 = buff.substr(posComma  + 1, posComma2   - posComma  - 1);
            string buff3 = buff.substr(posComma2 + 1, posComma3   - posComma2 - 1);
            string buff4 = buff.substr(posComma3 + 1, posComma4   - posComma3 - 1);
            string buff5 = buff.substr(posComma4 + 1, buff.size() - posComma4 - 1);

            gettedMapParts[arrElem].coords = {
                atoi(buff2.c_str()), atoi(buff3.c_str()),
                atoi(buff4.c_str()), atoi(buff5.c_str())
            };

            gettedMapParts[arrElem].visible = true;

            if (buff1 == "Block") {
                gettedMapParts[arrElem].blocktype = BLOCK_TYPE;
                gettedMapParts[arrElem].picture = block;
            }
            else if (buff1 == "Quest") {
                gettedMapParts[arrElem].blocktype = QUEST_TYPE;
                gettedMapParts[arrElem].picture = quest;
            }
            else if (buff1 == "Water") {
                gettedMapParts[arrElem].blocktype = WATER_TYPE;
                gettedMapParts[arrElem].picture = water;
            }
            else if (buff1 == "Fire") {
                gettedMapParts[arrElem].blocktype = FIRE_TYPE;
                gettedMapParts[arrElem].picture = fire;
            }
            else {
                gettedMapParts[arrElem].blocktype = 0;
                gettedMapParts[arrElem].visible = false;
            }

            arrElem++;
        }
    }

    lvlfile.close();

    return arrElem;
}

void checkElem(MapPart mapParts[])
{
    for (int elem = 0; elem < MAP_LENGHT; elem++) {
        for (int elem2 = 0; elem2 < MAP_LENGHT; elem2++) {

            if (elem2 != elem &&
                mapParts[elem].coords.left == mapParts[elem2].coords.left &&
                mapParts[elem].coords.top == mapParts[elem2].coords.top) {

                mapParts[elem].coords.left = mapParts[elem].coords.left + 60;
                mapParts[elem].coords.right = mapParts[elem].coords.right + 60;
            }
        }
    }
}

