#include "app/inventory_manager.hpp"

void InventoryManager::setInventory(const ProductId& productId,
                                    ProductCount quantity) {
    ProductCount oldQuantity = inventory_[productId];
    inventory_[productId] = quantity;
    if (oldQuantity != quantity) {
        this->notify(productId, quantity);
    }
}