#ifndef CLICKABLE_HPP_
#define CLICKABLE_HPP_

#include "screen.hpp"

template <class Derived>
class Clickable
{
  Derived* impl()
  {
    return static_cast<Derived*>(this);
  }

public:
  template <class... Args>
  void handle_region(SDL_Rect r, Args&... args)
  {
    if (screen.mouse.in(r)) {
      if constexpr (requires { impl()->on_hover(args...); }) {
	impl()->on_hover(args...);
      }

      if (screen.clicked) {
	impl()->on_click(args...);
      }

      return;
    }

    if constexpr (requires { impl()->on_hover_end(); }) {
      impl()->on_hover_end();
    }
  }
};

#endif
