#pragma once

#include "stdafx.h"
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
    Animation(SDL_Texture* texture, bool looping = true) {
      this->texture = texture;
      this->looping = looping;
    }

    ~Animation() {
    }

    void addFrame(SDL_Rect rect, float timeToNextFrame) {
      frames.emplace_back(rect, timeToNextFrame);
    }

    void stop() {
      active = false;
    }

    void start() {
      active = true;
    }

    void reset() {
      currentFrame = 0;
    }

    SDL_Rect getFrame(Timer* timer) {
      ellapsedMS  = timer->elapsed();

      if (ellapsedMS >= frames.at(currentFrame).timeToNextFrame)
      {
        if (!active) {
          return frames.at(0).rect;
        }

        if (currentFrame != frames.size()) {
          currentFrame++;
        }

        timer->reset();

        if (currentFrame == frames.size()) {
          if (looping) {
            currentFrame = 0;
          } else {
            currentFrame = frames.size() - 1;
          }
        }
      }
      return frames.at(currentFrame).rect;
    }

    SDL_Texture* getTexture() {
      return texture;
    }

  private:
    bool active = true;
    bool looping = true;
    std::vector<Frame> frames;
    unsigned int ellapsedMS = 0;
    int currentFrame = 0;
    SDL_Texture* texture;

};
