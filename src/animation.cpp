#include "animation.h"
#include "platform.h"

Animation::Animation(string name) {
    this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber) {
    if(frameNumber + 1 < frames.size())
        return frameNumber + 1;
    else
        return 0;
}

Frame* Animation::getNextFrame(Frame *frame) {
    return getFrame(getNextFrameNumber(frame->frameNumber));
}

int Animation::getEndFrameNumber() {
    return frames.size() - 1;
}

Frame* Animation::getFrame(int frameNumber) {
    if (frames.size() == 0)
        return NULL;

    list<Frame>::iterator i = frames.begin();

    int counter = 0;
    for(counter = 0; counter < frames.size() && counter < frameNumber ; counter++) {
        i++; // Follow the counter
    }

    Frame *frame = &(*i); // dereference the iterator to get the Frame and get its address
                          // Directly assign with the iterator will not work!
    return frame;
}

void Animation::loadAnimation(ifstream &file, list<DataGroupType> &groupTypes) {

    // animation name
#   ifdef PLATFORM_MAC
    getline(file, name, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, name, '\n'); // '\n' for Windows/Unix
#   endif

    string buffer;
    stringstream ss;

    // frame number
#   ifdef PLATFORM_MAC
    getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, buffer, '\n'); // '\n' for Windows/Unix
#   endif

    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    int numberOfFrames;
    ss >> numberOfFrames;
    cout << "=============================================" << endl;
    cout << "animation: " << name << " has " << numberOfFrames << " frames" << endl;

    for(int i = 0; i < numberOfFrames; i++) {
        Frame newFrame;
        newFrame.loadFrame(file, groupTypes);
        frames.push_back(newFrame);
    }



}
