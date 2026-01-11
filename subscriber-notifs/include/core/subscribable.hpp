#pragma once

#include <unordered_map>
#include <vector>

#include "subscriber.hpp"

template <typename TEventId, typename TEventInfo>
class Subscribable {
    using TSubscriber = Subscriber<TEventId, TEventInfo>;

   private:
    std::unordered_map<TEventId, std::vector<TSubscriber*>> subscribersMap_;

   public:
    void subscribe(const TEventId& eventId, TSubscriber* subscriber) {
        subscribersMap_[eventId].push_back(subscriber);
    }

    void unsubscribe(const TEventId& eventId, TSubscriber* subscriber) {
        auto entry = subscribersMap_.find(eventId);
        if (entry == subscribersMap_.end()) {
            return;
        }

        std::erase(entry->second, subscriber);
        if (entry->second.empty()) {
            subscribersMap_.erase(entry);
        }
    }

    void notify(const TEventId& eventId, const TEventInfo& eventInfo) {
        auto entry = subscribersMap_.find(eventId);
        if (entry == subscribersMap_.end()) {
            return;
        }

        for (TSubscriber* subscriber : entry->second) {
            subscriber->onNotification(eventId, eventInfo);
        }
    }
};