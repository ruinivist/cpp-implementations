#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "subscriber.hpp"
#include "thread_pool.hpp"

template <typename TEventId, typename TEventInfo>
class Subscribable {
    using TSubscriber = Subscriber<TEventId, TEventInfo>;

   private:
    std::unordered_map<TEventId, std::vector<std::shared_ptr<TSubscriber>>>
        subscribersMap_;

   public:
    void subscribe(const TEventId& eventId,
                   std::shared_ptr<TSubscriber> subscriber) {
        subscribersMap_[eventId].push_back(subscriber);
    }

    void unsubscribe(const TEventId& eventId,
                     std::shared_ptr<TSubscriber> subscriber) {
        auto entry = subscribersMap_.find(eventId);
        if (entry == subscribersMap_.end()) {
            return;
        }

        std::erase(entry->second, subscriber);
        if (entry->second.empty()) {
            subscribersMap_.erase(entry);
        }
    }

    void notify(const TEventId& eventId, const TEventInfo& eventInfo,
                ThreadPool& threadPool) const {
        auto entry = subscribersMap_.find(eventId);
        if (entry == subscribersMap_.end()) {
            return;
        }

        // edge case: if a subscriber unsubscribes itself during notification
        // this modifies the vector and makes my iterators invalid
        auto subscribersCopy = entry->second;
        for (auto& subscriber : subscribersCopy) {
            // Enqueue each notification as a separate task in the thread pool
            // capture by value next to copy the shared_ptr
            threadPool.enqueue([subscriber, eventId, eventInfo]() {
                subscriber->onNotification(eventId, eventInfo);
            });
        }
    }
};