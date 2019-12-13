#pragma once

//This library containing structure and array for map parts

struct MapPart {
    RECT coords;
    bool visible;
    HDC picture;
    int blocktype;
};

MapPart gettedMapParts[MAP_LENGHT + 1];
