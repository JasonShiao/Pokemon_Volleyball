#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <iostream>
#include "SDL.h"

class TimeController {
public:
    static const int PLAY_STATE, PAUSE_STATE;

    int timeState;
    Uint32 lastUpdate;
    float dT; // delta time (in seconds), Time since the last frame was rendered onto screen

    TimeController();
    void updateTime();
    void pause();
    void resume();
    void reset();

    static TimeController timeController;

};

#endif
