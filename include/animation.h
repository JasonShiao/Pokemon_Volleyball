#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
#include <string>
#include <list>

using namespace std;

class Animation {
public:
    string name; // identify the Animation
    list<Frame> frames; // an animation is composed of a list of frames
    Animation(string name = "");

    int getNextFrameNumber(int frameNumber);
    Frame* getNextFrame(Frame *frame);

    int getEndFrameNumber();
    Frame* getFrame(int frameNumber);

    // TODO:
    void loadAnimation(ifstream &file, list<DataGroupType> &groupTypes);

};


#endif
