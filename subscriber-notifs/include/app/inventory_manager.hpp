
#pragma once

#include <string>
#include <unordered_map>

#include "core/subscribable.hpp"

using ProductCount = int;
using ProductId = std::string;

// inventory manager is not generic ( works with the specific types ProductId
// and ProductCount ) but the subscribable is generic
class InventoryManager : public Subscribable<ProductId, ProductCount> {
   private:
    std::unordered_map<ProductId, int> inventory_;

   public:
    void setInventory(const ProductId& productId, ProductCount quantity);
};