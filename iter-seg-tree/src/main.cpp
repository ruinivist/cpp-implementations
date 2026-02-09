#include <iostream>
#include <vector>

// this isn't ideal
#include "../include/iter_seg_tree.hpp"

int main() {
    auto sum = [](int a, int b) { return a + b; };

    std::vector<int> vec = {1, 2, 3, 4};
    IterSegTree<int> st(vec, sum, 0);

    std::cout << st.query(0, 2) << "\n";
}