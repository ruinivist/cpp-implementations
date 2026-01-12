#include "app/services.hpp"

#include <chrono>
#include <iostream>
#include <thread>

void SmsService::onNotification(const ProductId& productId,
                                const ProductCount& quantity) {
    std::cout << "[SMS] Starting notification for Product: " << productId
              << ", Quantity: " << quantity << std::endl;
    // Simulate slow SMS API call
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[SMS] Completed notification for Product: " << productId
              << std::endl;
}

void EmailService::onNotification(const ProductId& productId,
                                  const ProductCount& quantity) {
    std::cout << "[EMAIL] Starting notification for Product: " << productId
              << ", Quantity: " << quantity << std::endl;
    // Simulate slow Email API call
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[EMAIL] Completed notification for Product: " << productId
              << std::endl;
}

void LogService::onNotification(const ProductId& productId,
                                const ProductCount& quantity) {
    std::cout << "[LOG] Starting log entry for Product: " << productId
              << ", Quantity: " << quantity << std::endl;
    // Simulate slower log write
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[LOG] Completed log entry for Product: " << productId
              << std::endl;
}