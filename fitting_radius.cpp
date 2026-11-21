#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <functional>
#include <cmath>
#include <tuple>
#include <numeric>
#include <algorithm>

std::vector<std::vector<float>> fitting(std::vector<std::vector<float> sort,
std::vector<std::vector<int> indices, std::vector<std::vector<int>> place){

    std::vector<std::vector<float>> centers(2, std::vector<float>(sort.size(), 0.0f));
    std::vector<float> radius(sort.size(), 0.0f);
        
    for (size_t i=0; i<sort.size(), i++){ 
        //Пересмотреть модель
        centers[0][i]=sort[i][0];
        centers[1][i]=sort[i][1];

        for (size_t j=0; j<indices[i].size()-1; j++){
            radius[i]+= std::sqrt(std::pow(centers[i][0]-place[indices[j]][1], 2)+
            std::pow(centers[i][1]-place[indices[j]][0], 2));
        }
        radius[i] /= indices[i][indices[i].size()-1];
        
    }

    std::vector<std::vector<float>> ABC = {centers[0], centers[1], radius};
    
    return ABC

}