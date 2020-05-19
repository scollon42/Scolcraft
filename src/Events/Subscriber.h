#pragma once

#include <functional>

namespace events {
class SubscriberBase
{
};

template<typename T>
class Subscriber final : public SubscriberBase
{
public:
  explicit Subscriber(const std::function<void(T)> &callback)
    : _callback(callback)
  {
  }

  void notify(T element) const
  {
    _callback(element);
  }


private:
  std::function<void(T)> _callback;
};
}// namespace events
