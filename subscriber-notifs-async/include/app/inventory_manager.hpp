
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "core/subscribable.hpp"
#include "core/thread_pool.hpp"

using ProductCount = int;
using ProductId = std::string;

// inventory manager is not generic ( works with the specific types ProductId
// and ProductCount ) but the subscribable is generic
class InventoryManager : public Subscribable<ProductId, ProductCount> {
   private:
    std::unordered_map<ProductId, int> inventory_;
    std::shared_ptr<ThreadPool> thread_pool_;

   public:
    explicit InventoryManager(std::shared_ptr<ThreadPool> thread_pool);

    void setInventory(const ProductId& productId, ProductCount quantity);
};