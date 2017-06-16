#include <Box/EventPublisher.hpp>
#include <Stick/ScopedLock.hpp>

namespace box
{
    using namespace stick;

    EventPublisher::EventPublisher(Allocator & _alloc) :
        m_storage(_alloc),
        m_eventQueue(_alloc)
    {

    }

    EventPublisher::~EventPublisher()
    {

    }

    void EventPublisher::publishEvent(EventPtr _event)
    {
        queueEvent(std::move(_event));
        publish();
    }

    void EventPublisher::publish()
    {
        beginPublishing();

        printf("A\n");

        stick::DynamicArray<Event *> evts(m_eventQueue.allocator());
        {
            ScopedLock<Mutex> lck;
            evts.resize(m_eventQueue.count());
            for (Size i = 0; i < m_eventQueue.count(); ++i)
            {
                evts[i] = m_eventQueue[i].get();
            }
        }

        printf("B\n");

        MappedStorage::RawPtrArray callbacks;
        for (Event * _e : evts)
        {
            printf("C\n");
            callbacks.clear();

            {
                ScopedLock<Mutex> lock(m_callbackMutex);
                auto it = m_storage.callbackMap.find(_e->eventTypeID());
                if (it != m_storage.callbackMap.end())
                {
                    printf("FOUND DA CALLBACKS\n");
                    //we copy the array here so we can savely add new callbacks from within callbacks etc.
                    callbacks = it->value;
                }
            }

            for(auto * cb : callbacks)
            {
                printf("CALL IT BABY\n");
                cb->call(*_e);
            }
        }

        {
            ScopedLock<Mutex> lck;
            m_eventQueue.clear();
        }
        printf("D\n");

        endPublishing();
    }

    void EventPublisher::queueEvent(EventPtr _event)
    {
        ScopedLock<Mutex> lck(m_eventQueueMutex);
        m_eventQueue.append(std::move(_event));
    }

    CallbackID EventPublisher::addEventCallback(const Callback & _cb)
    {
        ScopedLock<Mutex> lck(m_callbackMutex);
        m_storage.addCallback({nextID(), _cb.eventTypeID}, _cb.holder);
    }

    void EventPublisher::removeEventCallback(const CallbackID & _id)
    {
        ScopedLock<Mutex> lck(m_callbackMutex);
        m_storage.removeCallback(_id);
    }

    void EventPublisher::beginPublishing()
    {

    }

    void EventPublisher::endPublishing()
    {

    }
}