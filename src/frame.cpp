#include "frame.h"
#include "platform.h"

void Frame::Draw(SDL_Texture *spriteSheet, float x, float y){
    SDL_Rect dest; //
    dest.x = x - offset.x; // Align the pivot point of the frame
    dest.y = y - offset.y; //  with the destination location
    dest.w = clip.w;
    dest.h = clip.h;

    renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}

void Frame::loadFrame(ifstream &file, list<DataGroupType> &groupTypes) {
    GroupBuilder::buildGroups(groupTypes, frameData);

    string buffer;
    stringstream ss;
    // 1st line of a frame: frame header
#   ifdef PLATFORM_MAC
    getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, buffer, '\n'); // '\n' for Windows/Unix
#   endif
    cout << "======= A " << buffer << " Start ========" << endl;

     // clip: clip x, y, w and h
 #   ifdef PLATFORM_MAC
     getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
 #   else
     getline(file, buffer, '\n'); // '\n' for Windows/Unix
 #   endif
    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    ss >> clip.x >> clip.y >> clip.w >> clip.h;
    cout << "clip.x = " << clip.x << " clip.y = " << clip.y << " clip.w = " << clip.w << " clip.h = " << clip.h << endl;

    // offset pivot
#   ifdef PLATFORM_MAC
    getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, buffer, '\n'); // '\n' for Windows/Unix
#   endif
    ss.clear();
    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    ss >> offset.x >> offset.y;
    cout << "offset.x = " << offset.x << " offset.y = " << offset.y << endl;

    // duration
#   ifdef PLATFORM_MAC
    getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, buffer, '\n'); // '\n' for Windows/Unix
#   endif
    ss.clear();
    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    ss >> duration;
    cout << "duration = " << duration << endl;

    // index
#   ifdef PLATFORM_MAC
    getline(file, buffer, '\n'); // '\r' for Mac, '\n' for Windows/Unix
#   else
    getline(file, buffer, '\n'); // '\n' for Windows/Unix
#   endif
    ss.clear();
    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    ss >> frameNumber; // index
    cout << "frame index = " << frameNumber << endl;

    //GroupBuilder::loadGroups(file, frameData); // additional data, e.g. collision box

    cout << "======= End of A frame ========" << endl;

}
