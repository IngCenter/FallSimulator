#pragma once

struct MapPart {
    RECT coords;
    bool visible;
    HDC picture;
    int blocktype;
};

MapPart gettedMapParts[MAP_LENGHT + 1];
