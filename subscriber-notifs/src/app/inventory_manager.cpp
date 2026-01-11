#include "app/inventory_manager.hpp"

void InventoryManager::setInventory(const ProductId& productId,
                                    ProductCount quantity) {
    inventory_[productId] = quantity;
    this->notify(productId, quantity);
}