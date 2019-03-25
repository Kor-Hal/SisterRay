#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <vector>
#include <unordered_map>
#include "non_copyable.h"
#include "event.h"

typedef void(*SrEventCallback)(const void*);

class EventBus : private NonCopyable
{
public:
    EventBus();
    ~EventBus();

    void            dispatch(SrEventType type, void* event);
    SrEventListener listen(SrEventType type, SrEventCallback callback);
    void            clear(SrEventListener listener);

private:
    size_t                              _listenerCount;
    size_t                              _listenerCapacity;
    std::vector<SrEventType>            _types;
    std::vector<SrEventCallback>        _callbacks;
    std::vector<std::vector<size_t>>    _reverse;
    std::vector<size_t>                 _free;
};

#endif
