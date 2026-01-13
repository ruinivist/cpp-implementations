#include "app/inventory_manager.hpp"

InventoryManager::InventoryManager(std::shared_ptr<ThreadPool> thread_pool)
    : thread_pool_(std::move(thread_pool)) {}

void InventoryManager::setInventory(const ProductId& productId,
                                    ProductCount quantity) {
    ProductCount oldQuantity = inventory_[productId];
    inventory_[productId] = quantity;
    if (oldQuantity != quantity) {
        this->notify(productId, quantity, *thread_pool_);
    }
}