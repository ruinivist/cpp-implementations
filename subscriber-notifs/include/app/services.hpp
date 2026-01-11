#pragma once

#include "app/inventory_manager.hpp"
#include "core/subscriber.hpp"

class SmsService : public Subscriber<ProductId, ProductCount> {
   public:
    void onNotification(const ProductId& productId,
                        const ProductCount& quantity) override;
};

class EmailService : public Subscriber<ProductId, ProductCount> {
   public:
    void onNotification(const ProductId& productId,
                        const ProductCount& quantity) override;
};

class LogService : public Subscriber<ProductId, ProductCount> {
   public:
    void onNotification(const ProductId& productId,
                        const ProductCount& quantity) override;
};