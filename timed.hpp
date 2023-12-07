#ifndef TIMED_HPP_
#define TIMED_HPP_

#include "screen.hpp"
#include "SDL2/SDL.h"

template <class Derived>
class Timed
{
  bool running;
  Uint32 elapsed;

public:
  Uint32 duration;

  Timed(Uint32 duration)
    : running(false), duration(duration), elapsed(0) { }

  void start()
  {
    if (!running) {
      running = true;
      elapsed = 0;
    }
  }

  template <class... Args>
  void update(float dt, Args&... args)
  {
    if (!running) return;

    elapsed += dt * 1000;
    if (elapsed >= duration) {
      static_cast<Derived*>(this)->on_timer_finish(args...);
      running = false;
    }
  }
};

#endif
