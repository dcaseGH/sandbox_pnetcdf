#include <iostream>
#include <vector>
#include <cstddef>
#include <cmath>

extern void laplace_step(const double* in, double* out, std::size_t rows, std::size_t cols);

int main() {
    const std::size_t rows = 5;
    const std::size_t cols = 6;
    std::vector<double> in(rows * cols);
    std::vector<double> out(rows * cols, -1.0);

    // initialize in with simple increasing pattern
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            in[i * cols + j] = static_cast<double>(i * cols + j);

    laplace_step(in.data(), out.data(), rows, cols);

    auto approx_equal = [](double a, double b) {
        return std::fabs(a - b) <= 1e-12;
    };

    // verify boundaries copied
    for (std::size_t j = 0; j < cols; ++j) {
        if (!approx_equal(out[0 * cols + j], in[0 * cols + j])) {
            std::cerr << "Top boundary mismatch at j=" << j << "\n";
            return 1;
        }
        if (!approx_equal(out[(rows - 1) * cols + j], in[(rows - 1) * cols + j])) {
            std::cerr << "Bottom boundary mismatch at j=" << j << "\n";
            return 2;
        }
    }

    for (std::size_t i = 1; i + 1 < rows; ++i) {
        if (!approx_equal(out[i * cols + 0], in[i * cols + 0])) {
            std::cerr << "Left boundary mismatch at i=" << i << "\n";
            return 3;
        }
        if (!approx_equal(out[i * cols + (cols - 1)], in[i * cols + (cols - 1)])) {
            std::cerr << "Right boundary mismatch at i=" << i << "\n";
            return 4;
        }
    }

    // verify interior stencil
    for (std::size_t i = 1; i + 1 < rows; ++i) {
        for (std::size_t j = 1; j + 1 < cols; ++j) {
            double up    = in[(i - 1) * cols + j];
            double down  = in[(i + 1) * cols + j];
            double left  = in[i * cols + (j - 1)];
            double right = in[i * cols + (j + 1)];
            double expected = 0.25 * (up + down + left + right);
            if (!approx_equal(out[i * cols + j], expected)) {
                std::cerr << "Interior mismatch at (" << i << "," << j << "): got="
                          << out[i * cols + j] << " expected=" << expected << "\n";
                return 5;
            }
        }
    }

    std::cout << "laplace_step test passed\n";
    return 0;
}
