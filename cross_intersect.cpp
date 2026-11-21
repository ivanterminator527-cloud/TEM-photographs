#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <functional>
#include <cmath>
#include <tuple>
#include <numeric>
#include <algorithm>

static bool check(int xi, int yi, float gradx, float grady, float x, float y){
    if ((((gradx>0) && x<xi) || ((gradx<0) &&  x>xi)) && (((grady>0) && y<yi) || ((grady<0) && y>yi)) &&
        std::sqrt((x-float(xi))*(x-float(xi))+(y-float(yi))*(y-float(yi)))<130.0f){
        return true;
    } else {
        return false;
    }
}

std::vector<std::vector<std::vector<float>>> cross_instersect( std::vector<float> k, std::vector<float> b, 
    std::vector<std::vector<int>> places, std::vector<std::vector<float>> grad){
    int n = k.size(); int k = int(std::sqrt(n*n/2/coord.size()))+1;
    int l =1;
    float x, y, R1, R2, R;

    std::vector<std::vector<float>> coord(1000000, std::vector<float>(3, 0.0f));
    std::vector<std::vector<int>> indices(coord.size(),std::vector<int>(2, -1));

    for (int i=0; i < n; i+=k){
        for (int j = i+1; j< n; j+=k-1){
            if (k[j]!=k[i]){
            x = (b[i]-b[j])/(k[j]-k[i]);
            y = (b[i]*k[j]-k[i]*b[j])/(k[j]-k[i]);
            R1 = std::sqrt((x-places[i][1])*(x-places[i][1])+(y-places[i][0])*(y-places[i][0]));
            R2 = std::sqrt((x-places[j][1])*(x-places[j][1])+(y-places[j][0])*(y-places[j][0]));
            if (R2>R1){
                R=R1;
                R1=R2;
                R2=R;
            }
            R = R1/5 + R2*4/5;


            if (check(places[i][1], places[i][0], grad[i][1], grad[i][0], x, y) and 
            check(places[j][1], places[j][0], grad[j][1], grad[j][0], x, y) and R1/(R2+5.0f)<1.2){
                coord[l-1][0] = x;
                coord[l-1][1] = y;
                coord[l-1][2] = R;
                indices[l-1][0] = i; indices[l-1][1]=j;
                l++;
                if (l==coord.size()){
                    break;
                }
            }
        }   
        }
        if (l==coord.size()){
            break;
        }

    }
    std::vector<std::vector<float>> newcoord(coord.begin(), coord.begin()+l-1);
    std::vector<std::vector<float>> newind(indices.begin(), indices.begin()+l-1);
    std::vector<std::vector<std::vector<float>>> news = {newcoord, newind};
    return news;
}

PYBIND11_MODULE(cross_intersection, m){
    m.def("cross_intersect", &cross_instersect, "k, b, places, grad");
}
