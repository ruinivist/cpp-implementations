#pragma once

template <typename TEventId, typename TEventInfo>
class Subscriber {
   public:
    virtual void onNotification(const TEventId& eventId,
                                const TEventInfo& eventInfo) = 0;

    virtual ~Subscriber() = default;
};