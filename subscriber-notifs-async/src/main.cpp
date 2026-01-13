#include <chrono>
#include <iostream>
#include <memory>

#include "app/inventory_manager.hpp"
#include "app/services.hpp"
#include "core/thread_pool.hpp"

int main() {
    auto threadPool = std::make_shared<ThreadPool>(4);
    InventoryManager inventoryManager(threadPool);

    auto smsService = std::make_shared<SmsService>();
    auto emailService = std::make_shared<EmailService>();
    auto logService = std::make_shared<LogService>();

    inventoryManager.subscribe("ProductA", smsService);
    inventoryManager.subscribe("ProductB", emailService);
    inventoryManager.subscribe("ProductA", logService);

    auto start = std::chrono::steady_clock::now();

    inventoryManager.setInventory("ProductA", 100);
    inventoryManager.setInventory("ProductB", 200);

    auto end = std::chrono::steady_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Inventory updates: " << duration.count() << "ms\n";

    threadPool->wait_for_completion();

    return 0;
}