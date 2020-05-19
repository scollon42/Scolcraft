#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <Events/Subscriber.h>
#include <spdlog/spdlog.h>

namespace events {

using SubscriberList = std::vector<SubscriberBase *>;
using SubscribersMap = std::unordered_map<std::string, SubscriberList>;

class Dispatcher
{
public:
  [[nodiscard]] static Dispatcher &get() noexcept
  {
    static auto dispatcher = std::make_unique<Dispatcher>();

    return *dispatcher;
  }

  ~Dispatcher()
  {
    spdlog::info("Destroying dispatcher");
    for (const auto &sub_list : _subscribers_map) {
      for (auto sub : sub_list.second) {
        delete sub;
      }
    }
  }

  template<typename T>
  void subscribe(const std::function<void(T)> &callback) noexcept
  {
    auto subscriber = new Subscriber<T>(callback);

    auto list = _subscribers_map.find(T::get_name());
    if (list != _subscribers_map.end()) {
      list->second.push_back(subscriber);
    } else {
      SubscriberList new_list{ subscriber };
      _subscribers_map.emplace(std::make_pair(T::get_name(), new_list));
    }
  }

  template<typename T>
  void dispatch(T event)
  {
    auto subs_list = _subscribers_map.at(T::get_name());

    for (const auto &sub : subs_list) {
      auto subscriber = static_cast<Subscriber<T> *>(sub);
      subscriber->notify(event);
    }
  }

private:
  SubscribersMap _subscribers_map;
};

}// namespace events
