#include "app/inventory_manager.hpp"
#include "app/services.hpp"

int main() {
    InventoryManager inventoryManager;

    SmsService smsService;
    EmailService emailService;
    LogService logService;

    inventoryManager.subscribe("ProductA", &smsService);
    inventoryManager.subscribe("ProductB", &emailService);
    inventoryManager.subscribe("ProductA", &logService);

    inventoryManager.setInventory("ProductA", 100);
    inventoryManager.setInventory("ProductB", 200);

    return 0;
}