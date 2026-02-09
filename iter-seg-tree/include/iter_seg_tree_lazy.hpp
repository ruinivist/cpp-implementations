#pragma once
#include <functional>
#include <vector>

#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) | 1)
#define IS_RIGHT_CHILD(i) ((i) & 1)

// REMEMBER REMEMBER REMEMBER REMEMBER
// Invariant: tree[p] will reflect the effect ot lazy[p]
// the lazy is not for the node, but for the children

template <typename T, typename L>
class IterLazySegTree {
   private:
    int n_, h_;
    std::vector<T> tree_;
    std::vector<L> lazy_;

    T iota_;
    L iota_lazy_;

    std::function<T(T, T)> combine_;
    std::function<L(L, L)> combine_lazy_;
    std::function<T(T, L, int)> apply_lazy_;

    void apply(int index, L new_lazy, int len) {
        // invariant: tree_[index] has no lazy, so just need the
        // new val to be applied
        tree_[index] = apply_lazy_(tree_[index], new_lazy, len);

        if (index < n_) {  // not leaf
            // combine lazy for children
            lazy_[index] = combine_lazy_(lazy_[index], new_lazy);
        }
    }

    void push(int index) {
        for (int cur_h = h_; cur_h > 0; cur_h--) {
            int par_at_level_cur_h = index >> cur_h;
            if (lazy_[par_at_level_cur_h] != iota_lazy_) {
            }
        }
    }
};