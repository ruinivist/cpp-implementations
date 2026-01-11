#include "app/services.hpp"

#include <iostream>

void SmsService::onNotification(const ProductId& productId,
                                const ProductCount& quantity) {
    std::cout << "SMS Notification - Product: " << productId
              << ", Quantity: " << quantity << std::endl;
}

void EmailService::onNotification(const ProductId& productId,
                                  const ProductCount& quantity) {
    std::cout << "Email Notification - Product: " << productId
              << ", Quantity: " << quantity << std::endl;
}

void LogService::onNotification(const ProductId& productId,
                                const ProductCount& quantity) {
    std::cout << "Log Entry - Product: " << productId
              << ", Quantity: " << quantity << std::endl;
}