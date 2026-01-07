#include <iostream>
#include <cstddef>
#include "zero_matrix.hpp"

int main() {
    ZeroMatrix m(3, 4);

    if (m.rows != 3 || m.cols != 4) {
        std::cerr << "Size check failed\n";
        return 1;
    }

    for (std::size_t i = 0; i < m.rows; ++i) {
        for (std::size_t j = 0; j < m.cols; ++j) {
            if (m(i, j) != 0.0) {
                std::cerr << "Initial zero check failed at (" << i << "," << j << ")\n";
                return 2;
            }
        }
    }

    m(1,2) = 5.5;
    if (m(1,2) != 5.5) {
        std::cerr << "Write/read failed\n";
        return 3;
    }

    m.fill_zero();
    for (std::size_t i = 0; i < m.rows; ++i) {
        for (std::size_t j = 0; j < m.cols; ++j) {
            if (m(i, j) != 0.0) {
                std::cerr << "fill_zero failed at (" << i << "," << j << ")\n";
                return 4;
            }
        }
    }

    std::cout << "ZeroMatrix tests passed\n";
    return 0;
}
