#ifndef BOX_EVENTPUBLISHER_HPP
#define BOX_EVENTPUBLISHER_HPP

#include <Box/Private/Callback.hpp>
#include <Box/Private/MappedCallbackStorage.hpp>
#include <Stick/Mutex.hpp>
#include <Stick/ScopedLock.hpp>

namespace box
{
    namespace detail
    {
        template<class PublisherType>
        struct STICK_API PublishingPolicyBasic
        {
            using MappedStorage = typename PublisherType::MappedStorage;
            using MutexType = stick::NoMutex;

            inline void publish(const Event & _evt, const MappedStorage & _callbacks)
            {
                auto it = _callbacks.callbackMap.find(_evt.eventTypeID());
                if (it != _callbacks.callbackMap.end())
                {
                    for (auto * cb : it->value)
                    {
                        cb->call(_evt);
                    }
                }
            }

            mutable MutexType mutex;
        };

        template<class PublisherType>
        struct STICK_API PublishingPolicyLocking
        {
            using MappedStorage = typename PublisherType::MappedStorage;
            using MutexType = stick::Mutex;

            inline void publish(const Event & _evt, const MappedStorage & _callbacks)
            {
                typename MappedStorage::RawPtrArray callbacks(_callbacks.storage.allocator());
                {
                    stick::ScopedLock<MutexType> lck(mutex);
                    auto it = _callbacks.callbackMap.find(_evt.eventTypeID());
                    if (it != _callbacks.callbackMap.end())
                    {
                        //we copy the array here so we can savely add new callbacks from within callbacks etc.
                        callbacks = it->value;
                    }
                }

                for (auto * cb : callbacks)
                {
                    cb->call(_evt);
                }
            }

            mutable MutexType mutex;
        };
    }

    template<template<class> class PublishingPolicyT>
    class STICK_API EventPublisherT
    {
    public:

        using PublishingPolicy = PublishingPolicyT<EventPublisherT>;
        using Callback = detail::CallbackT<void, Event>;
        using MappedStorage = detail::MappedCallbackStorageT<typename Callback::CallbackBaseType>;


        EventPublisherT(stick::Allocator & _alloc = stick::defaultAllocator()) :
            m_alloc(&_alloc),
            m_storage(_alloc)
        {

        }

        /**
         * @brief Virtual Destructor, you usually derive from this class.
         */
        virtual ~EventPublisherT()
        {

        }

        /**
         * @brief Publish an event to all registered subscribers.
         *
         * @param _event The event to publish.
         */
        void publish(const Event & _event)
        {
            beginPublishing(_event);
            m_policy.publish(_event, m_storage);
            endPublishing(_event);
        }

        // template<class T, class...Args>
        // void publish(Args..._args)
        // {
        //     publish(stick::makeUnique<T>(*m_alloc, _args...));
        // }

        CallbackID addEventCallback(const Callback & _cb)
        {
            stick::ScopedLock<typename PublishingPolicy::MutexType> lock(m_policy.mutex);
            m_storage.addCallback({nextID(), _cb.eventTypeID}, _cb.holder);
        }

        /**
         * @brief Removes a callback.
         * @param _id The callback id to remove.
         */
        void removeEventCallback(const CallbackID & _id)
        {
            stick::ScopedLock<typename PublishingPolicy::MutexTye> lock(m_policy.mutex);
            m_storage.removeCallback(_id);
        }

        /**
         * @brief Can be overwritten if specific things need to happen right before the publisher emits its events.
         */
        virtual void beginPublishing(const Event & _evt)
        {

        }

        /**
         * @brief Can be overwritten if specific things need to happen right after the publisher emits its events.
         */
        virtual void endPublishing(const Event & _evt)
        {

        }


    protected:

        inline stick::Size nextID() const
        {
            static stick::Size s_id(0);
            return s_id++;
        }

        stick::Allocator * m_alloc;
        MappedStorage m_storage;
        PublishingPolicy m_policy;
    };

    using EventPublisher = EventPublisherT<detail::PublishingPolicyBasic>;
}

#endif //BOX_EVENTPUBLISHER_HPP
