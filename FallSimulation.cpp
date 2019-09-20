#include <TXLib.h>
#include <iostream>

using namespace std;

int middleX;
int middleY;

int extentX;
int extentY;

void background(COLORREF color);
void drawMenu();

int main()
{
    txCreateWindow(1300, 600);
    middleX = txGetExtentX() / 2;
    middleY = txGetExtentY() / 2;
    extentX = txGetExtentX();
    extentY = txGetExtentY();

    background(TX_WHITE);
    txSleep(50);

    txBegin();

    drawMenu();

    txEnd();

    txDisableAutoPause();
}

void background(COLORREF color)
{
    txSetFillColor(color);
    txClear();
}

void drawMenu()
{
    RECT buttonPlay {
        middleX - 100, extentY / 3 - 50,
        middleX + 100, extentY / 3 + 50
    };

    RECT buttonExit {
        middleX - 100, extentY / 2 - 50,
        middleX + 100, extentY / 2 + 50
    };

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);

    txRectangle(buttonPlay.right, buttonPlay.top, buttonPlay.left, buttonPlay.bottom);
    txRectangle(buttonExit.right, buttonExit.top, buttonExit.left, buttonExit.bottom);
    txSleep(50);

    while (!GetAsyncKeyState('Q')) {
        if (In(txMousePos(), buttonPlay) && txMouseButtons() & 1) {
            while (txMouseButtons() & 1) {
                txSleep(10);
            }

            txMessageBox("Запуск игры пока что не работает!", "Ошибка");
        }
        if (In(txMousePos(), buttonExit) && txMouseButtons() & 1) {
            while (txMouseButtons() & 1) {
                txSleep(10);
            }

            break;
        }
    }
}

