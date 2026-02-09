#pragma once
#include <functional>
#include <vector>

#define LEFT(i) (i << 1)
#define RIGHT(i) ((i << 1) | 1)
#define IS_RIGHT_CHILD(i) ((i) & 1)

template <typename T>
class IterSegTree {
   private:
    int len_;
    std::vector<T> seg_tree_;
    std::function<T(T, T)> combine_;
    T iota_;

    void build(const std::vector<T>& base_array) {
        // n to 2n-1 => total n elements => leaves
        for (int i = len_; i < 2 * len_; i++) {
            seg_tree_[i] = base_array[i - len_];
        }
        // reverse as back to front is going up the tree
        for (int i = len_ - 1; i > 0; i--) {
            seg_tree_[i] = combine_(seg_tree_[LEFT(i)], seg_tree_[RIGHT(i)]);
        }
    }

   public:
    IterSegTree(const std::vector<T>& base_array,
                std::function<T(T, T)> combine_func, T iota_value)
        : combine_(combine_func), iota_(iota_value) {
        len_ = base_array.size();
        seg_tree_.resize(2 * len_ + 1);
        build(base_array);
    }

    IterSegTree(int len, T iota)
        : len_(len), iota_(iota), seg_tree_(2 * len_ + 1, iota_) {}

    void update(int index, std::function<T(T)> transform) {
        // 1. go to leaf and update
        index += len_;
        seg_tree_[index] = transform(seg_tree_[index]);

        while (index > 1) {  // we don't use 0
            index >>= 1;     // move to parent first
            seg_tree_[index] =
                combine_(seg_tree_[LEFT(index)], seg_tree_[RIGHT(index)]);
        }
    }

    // for range [l,r)
    T query(int left, int right) {
        left += len_;
        right += len_;

        T result = iota_;
        // NOTE: this soln assumes combine_ is commutative,
        // else you need to separarte left and right accumulators
        for (; left < right; left >>= 1, right >>= 1) {
            if (IS_RIGHT_CHILD(left))
                result = combine_(result, seg_tree_[left++]);
            if (IS_RIGHT_CHILD(right))
                result = combine_(result, seg_tree_[--right]);
        }

        // NOTE: if you separate, MUST combine at the end
        return result;
    }
};