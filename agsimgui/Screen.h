//
// Created by erico on 03/01/2020.
//

#ifndef AGSIMGUI_SCREEN_H
#define AGSIMGUI_SCREEN_H

class Screen
{

    public:
    enum Driver {
        eSoftware = 2,
        eDirectx9 = 1,
        eOpenGL = 0,
    };

	bool initialized;
    int width;
    int height;
    int colorDepth;
    int gameSpeed;
    float frameDelay;
    long backBufferWidth;
    long backBufferHeight;
    Driver driver;

    Screen();
    ~Screen();
};

extern Screen screen;

#endif //AGSIMGUI_SCREEN_H
