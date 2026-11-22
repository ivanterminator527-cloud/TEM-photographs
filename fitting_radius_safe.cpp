#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

std::vector<std::vector<float>> fitting(
    const std::vector<std::vector<float>>& sort,
    const std::vector<std::vector<int>>& indices,
    const std::vector<std::vector<int>>& place) {
    if (sort.size() != indices.size()) {
        throw std::invalid_argument("sort and indices must have the same number of groups");
    }

    const float nan = std::numeric_limits<float>::quiet_NaN();
    std::vector<float> center_x(sort.size(), nan);
    std::vector<float> center_y(sort.size(), nan);
    std::vector<float> radius(sort.size(), nan);

    for (std::size_t i = 0; i < sort.size(); ++i) {
        // The final entry is the stored-index count; earlier entries are point indices.
        if (indices[i].empty()) continue;
        const std::size_t capacity = indices[i].size() - 1;
        const int stored_count = indices[i].back();
        if (stored_count <= 0 || capacity == 0) continue;

        const std::size_t count = std::min(static_cast<std::size_t>(stored_count), capacity);
        std::vector<int> points;
        points.reserve(count);
        for (std::size_t j = 0; j < count; ++j) {
            const int point_index = indices[i][j];
            if (point_index < 0 || static_cast<std::size_t>(point_index) >= place.size()) continue;
            if (place[point_index].size() < 2) continue;
            if (std::find(points.begin(), points.end(), point_index) == points.end()) {
                points.push_back(point_index);
            }
        }
        if (points.empty()) continue;

        double sum_x = 0.0;
        double sum_y = 0.0;
        for (int point_index : points) {
            sum_x += place[point_index][1];
            sum_y += place[point_index][0];
        }
        center_x[i] = static_cast<float>(sum_x / points.size());
        center_y[i] = static_cast<float>(sum_y / points.size());

        double radius_sum = 0.0;
        for (int point_index : points) {
            const double dx = center_x[i] - place[point_index][1];
            const double dy = center_y[i] - place[point_index][0];
            radius_sum += std::hypot(dx, dy);
        }
        radius[i] = static_cast<float>(radius_sum / points.size());
    }

    return {center_x, center_y, radius};
}

PYBIND11_MODULE(fitting_rad_safe, m) {
    m.def("fitting_radius", &fitting, "Estimate centers and mean radii with checked indices");
}
