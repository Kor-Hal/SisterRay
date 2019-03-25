#include <algorithm>
#include "event_bus.h"

EventBus::EventBus()
: _listenerCount(0)
, _listenerCapacity(1)
{
    _types.push_back(0);
    _callbacks.push_back(nullptr);
}

EventBus::~EventBus()
{

}

void EventBus::dispatch(SrEventType type, void* event)
{
    if (!type)
        return;

    if (_reverse.size() <= type)
        return;

    const std::vector<size_t>& rev = _reverse[type];

    for (size_t i : rev)
    {
        SrEventCallback cb = _callbacks[i];
        cb(event);
    }
}

SrEventListener EventBus::listen(SrEventType type, SrEventCallback callback)
{
    size_t id;

    if (!type)
        return 0;

    if (_reverse.size() <= type)
        _reverse.resize((size_t)type);

    std::vector<size_t>& rev = _reverse[type];

    if (!_free.empty())
    {
        id = _free.back();
        _free.pop_back();
    }
    else
    {
        id = _listenerCapacity++;
        _callbacks.resize(id);
        _types.resize(id);
    }
    _callbacks[id] = callback;
    _types[id] = type;
    _listenerCount++;
    rev.push_back(id);

    return SrEventListener(id);
}

void EventBus::clear(SrEventListener listener)
{
    if (!listener)
        return;

    if (listener >= _listenerCapacity)
        return;

    SrEventType type = _types[listener];
    if (!type)
        return;

    std::vector<size_t>& rev = _reverse[type];
    std::remove(rev.begin(), rev.end(), listener);
    _types[listener] = 0;
    _callbacks[listener] = nullptr;
    _free.push_back(listener);
    _listenerCount--;
}
