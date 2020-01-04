//
// Created by erico on 03/01/2020.
//

#include "Screen.h"

Screen::Screen() {
    width = 0;
    height = 0;
    colorDepth = 0;
    gameSpeed = 40;
    frameDelay = 1.f/40.f;
    backBufferWidth = 0;
    backBufferHeight = 0;
	initialized = false;
}

Screen::~Screen() {

}

Screen screen;