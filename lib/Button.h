#pragma once

//This library containing function and structure for buttons

struct Button {
    RECT coords;
    const char* text;
    HDC picture;
};

void drawButton(Button but);

//function for drawing button
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

