#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "timer.h"

class Animation
{
  struct Frame
  {
    Frame(SDL_Rect& rect, unsigned int timeToNextFrame)
      : rect(rect)
      , timeToNextFrame(timeToNextFrame)
    { }

    SDL_Rect rect;
    unsigned int timeToNextFrame;
  };

  public:
    Animation() {
      timer = new Timer();
    }

    ~Animation() {
    }

    void addFrame(SDL_Rect rect, float timeToNextFrame) {
      frames.emplace_back(rect, timeToNextFrame);
    }

    void stop() {
      active = false;
    }

    void play() {
      active = true;
    }

    void reset() {
      currentFrame = 0;
    }

    SDL_Rect getFrame() {
      ellapsedMS  = timer->elapsed();

      if (ellapsedMS >= frames.at(currentFrame).timeToNextFrame)
      {
        if (!active) {
          return frames.at(0).rect;
        }

        currentFrame++;
        timer->reset();

        if (currentFrame == frames.size()) {
          currentFrame = 0;
        }
      }
      return frames.at(currentFrame).rect;
    }
  private:
    bool active = true;
    std::vector<Frame> frames;
    unsigned int ellapsedMS = 0;
    int currentFrame = 0;
    Timer* timer;

};
