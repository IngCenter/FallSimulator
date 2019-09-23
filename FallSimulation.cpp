#include <TXLib.h>
#include <iostream>

using namespace std;

int middleX;
int middleY;

int extentX;
int extentY;

struct Button {
    RECT coords;
    const char* text;
    HDC picture;
};

void background(COLORREF color);
void drawMenu();
void drawButton(Button but);
void loadingAnimation(int delay);
void mainFunc();

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

    txSleep(50);
    loadingAnimation(10);
    txSleep(1000);
    drawMenu();

    txEnd();

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

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);

    drawButton(buttonPlay);
    drawButton(buttonExit);

    txSleep(50);

    while (!GetAsyncKeyState('Q')) {
        if (In(txMousePos(), buttonPlay.coords) && txMouseButtons() & 1) {
            while (txMouseButtons() & 1) {
                txSleep(10);
            }

            loadingAnimation(18);
            txSleep(50);
            mainFunc();
            txMessageBox("Запуск игры пока что не работает!", "Ошибка");
        }
        if (In(txMousePos(), buttonExit.coords) && txMouseButtons() & 1) {
            while (txMouseButtons() & 1) {
                txSleep(10);
            }

            break;
        }
    }
}

void loadingAnimation(int delay)
{
    background(TX_WHITE);

    for (int circle_radius = 0;
        circle_radius * circle_radius < extentX * extentX + extentY * extentY;
        circle_radius += 3) {

        txSetColor(TX_BLACK, 2);
        txSetFillColor(TX_BLACK);

        txCircle(0, 0, circle_radius);

        txSleep(delay);
    }

    background(TX_WHITE);
}

void mainFunc()
{
    HDC block = txLoadImage("pictures\\block.bmp");
    HDC quest = txLoadImage("pictures\\question.bmp");

    txBitBlt(txDC(), extentX - 60, 0,  60, 60, block);
    txBitBlt(txDC(), extentX - 60, 60, 60, 60, quest);
}

