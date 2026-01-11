#include <memory>

#include "app/inventory_manager.hpp"
#include "app/services.hpp"

int main() {
    InventoryManager inventoryManager;

    auto smsService = std::make_shared<SmsService>();
    auto emailService = std::make_shared<EmailService>();
    auto logService = std::make_shared<LogService>();

    inventoryManager.subscribe("ProductA", smsService);
    inventoryManager.subscribe("ProductB", emailService);
    inventoryManager.subscribe("ProductA", logService);

    inventoryManager.setInventory("ProductA", 100);
    inventoryManager.setInventory("ProductB", 200);

    return 0;
}