#include "KeyboardEvent.h"

events::KeyboardEvent::KeyboardEvent(int key, int action)
  : _key(key), _action(action)
{
}

int events::KeyboardEvent::get_key() const noexcept
{
  return _key;
}

int events::KeyboardEvent::get_action() const noexcept
{
  return _action;
}
