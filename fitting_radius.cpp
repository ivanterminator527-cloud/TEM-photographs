#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <functional>
#include <cmath>
#include <tuple>
#include <numeric>
#include <algorithm>

std::vector<std::vector<float>> fitting(std::vector<std::vector<float>> sort,
std::vector<std::vector<int>> indices, std::vector<std::vector<int>> place){

    std::vector<std::vector<float>> centers(2, std::vector<float>(sort.size(), 0.0f));
    std::vector<float> radius(sort.size(), 0.0f);
    int n;
        
    for (size_t i=0; i<sort.size(); i++){ 
        //Make centers
        n = std::min(indices[i].back(), int(indices[i].size()-1));
        n = std::max(1, n);
        if (n>10){
        for (size_t j=0; j<n; j++){
            centers[0][i] += place[indices[i][j]][1];
            centers[1][i] += place[indices[i][j]][0];
        }
        centers[0][i] /= n;
        centers[1][i] /= n;

        //Make radius from that
        for (size_t j=0; j<n; j++){
            radius[i]+= std::sqrt(std::pow(centers[0][i]-place[indices[i][j]][1], 2)+
            std::pow(centers[1][i]-place[indices[i][j]][0], 2));
        }
        radius[i] /= n;
        }
        
    }

    std::vector<std::vector<float>> ABC = {centers[0], centers[1], radius};
    
    return ABC;

}

PYBIND11_MODULE(fitting_rad, m){
    m.def("fitting_radius", &fitting, "array, index, coord");
}
