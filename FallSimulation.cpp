#include <TXLib.h>
#include "lib\\Consts.h"
#include "lib\\ElemFunctions.h"
#include "lib\\ModesFunctions.h"

#include <iostream>

using namespace std;


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
    txDrawText(0, 0, extentX, 50, "Конструктор уровней игры FallSimulation");

    drawMenu();

    if (lvlCreatingIsStarted) {
        mainFunc();
    }
    else if (gameIsStarted) {
        playGame(gettedMapParts);
    }

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
