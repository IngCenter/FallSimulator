#include <TXLib.h>
#include <iostream>

using namespace std;

const COLORREF MY_LIGHTBLUE = RGB(75, 127, 196);
const COLORREF MY_BISQUE    = RGB(255, 214, 89);

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

HDC block;
HDC quest;
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
    orange_but = txLoadImage("pictures\\orange_but.bmp");
    green_but  = txLoadImage("pictures\\green_but.bmp");
    blue_but   = txLoadImage("pictures\\blue_but.bmp");
    purple_but = txLoadImage("pictures\\purple_but.bmp");

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

                loadingAnimation(2, 3);
                txSleep(50);
                //txMessageBox("Запуск игры пока что не работает!", "Ошибка");
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

    RECT blockBut = {
        extentX - 60, 0,  extentX, 60
    };

    RECT questBut = {
        extentX - 60, 60, extentX, 120
    };

    bool clickedBlock = false;
    bool clickedQuest = false;

    while(!GetAsyncKeyState('Q'))
    {
        background(TX_WHITE);

        txBitBlt(txDC(), extentX - 60, 0,  60, 60, block);
        txBitBlt(txDC(), extentX - 60, 60, 60, 60, quest);

        //block "block"
        if (In(txMousePos(), blockBut) && txMouseButtons() & 1) {
            clickedBlock = true;
        }
        if (txMouseButtons() & 1 && clickedBlock) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, block);

            if (!(txMouseButtons() & 1)) {
                txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, block);
                clickedBlock = false;
            }
        }

        //block "quest"
        if (In(txMousePos(), questBut) && txMouseButtons() & 1) {
            clickedQuest = true;
        }
        if (txMouseButtons() & 1 && clickedQuest) {
            txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, quest);

            if (!(txMouseButtons() & 1)) {
                txBitBlt(txDC(), txMouseX() - 30, txMouseY() - 30, 60, 60, quest);
                clickedQuest = false;
            }
        }

        txSleep(10);
    }
}

