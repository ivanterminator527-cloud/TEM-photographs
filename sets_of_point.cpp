#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <functional>
#include <cmath>
#include <tuple>
#include <numeric>
#include <algorithm>

std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> statistic (
    std::vector<std::vector<float>> intersection, std::vector<std::vector<int>> indices){
    int l =5000; int n=400; bool check=false; float x, y, r;
    std::vector<std::vector<float>> coord(l, std::vector<float>(4, -100.0f)); //4 число - число, сколько вошло в это пересечение точек
    std::vector<std::vector<int>> ind_of_ind(l, std::vector<int>(n+1, 0));
    int cur = 0;
    if (intersection.size()>=1){
    for (int i=0; i<3; i++){
        coord[0][i] = intersection[0][i];
    }
    coord[0][3]=1.0f;
    cur++;
    for (size_t i = 1; i<intersection.size(); i+=int(intersection.size()/l)/2+1){
        x = intersection[i][0];
        y = intersection[i][1];
        r = intersection[i][2];
        check = false;
        for (int j=0; j<cur ; j++){
            if (std::sqrt(std::pow(x-coord[j][0], 2)+std::pow(y-coord[j][1], 2))<(r+coord[j][2])*0.3+10.0f and 
            std::max(r, coord[j][2])/(std::min(r, coord[j][2])+2)<1.4){
                check = true;
                for (int k=0; k<3; k++){
                    coord[j][k] = (intersection[i][k]/5+coord[j][k]*4/5);
                }
                if (ind_of_ind[j][n]<=n-2){
                    ind_of_ind[j][ind_of_ind[j][n]]=indices[i][0];
                    ind_of_ind[j][ind_of_ind[j][n]+1]=indices[i][1];
                }
                ind_of_ind[j][n]+=2;
                coord[j][3]++;
            }
        }
        if (!(check) and cur<l-1){
            for (int k=0; k<3; k++){
                coord[cur][k] = intersection[i][k];
            }
            coord[cur][3]=1.0f;
            cur++;
        }
    }
    }
    std::vector<std::vector<float>> newcoord(coord.begin(), coord.begin()+cur);
    std::vector<std::vector<int>> newind_of(ind_of_ind.begin(), ind_of_ind.begin()+cur);
    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> AB = {newcoord, newind_of};
    return AB;
}


std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>  sort_stat (
    std::vector<std::vector<float>> stat, std::vector<std::vector<int>> indices){
    std::vector<std::vector<float>> sort(1000, std::vector<float>(4, -100.0f)); // из 4 элементов, где 4 - количество пересечений, которые вошло в это число
    int n=400*2; int src_count, dst_count;
    std::vector<std::vector<int>> indices_all(sort.size(), std::vector<int>(n+1, 0));

    std::vector<int> place(stat.size(), -1);
    int now=-1;
    float x, y, r;
    size_t curr=0;


    while (curr<stat.size()){
        if (place[curr]==-1 and now<int(sort.size())-1){
            now++;
            for (int i=0; i<3; i++){
                sort[now][i] = stat[curr][i];
            }
            sort[now][3] = stat[curr][3];


            for (size_t i=curr+1; i<stat.size(); i++){
                x = stat[i][0];
                y = stat[i][1];
                r = stat[i][2];
                if (std::sqrt(std::pow(x-sort[now][0], 2)+std::pow(y-sort[now][1], 2))<(r+sort[now][2])/5*0.8f+2.0f){
                    place[i]=now;
                    for (int k=0; k<3; k++){
                        sort[now][k] = (stat[i][k]*stat[i][3]+sort[now][k]*sort[now][3])/(stat[i][3]+sort[now][3]);
                    }
                    src_count = std::max(0, std::min(indices[i].back(), static_cast<int>(indices[i].size()) - 1));
                    dst_count = indices_all[now][n];
                    if (dst_count<n-2 ){
                        for (int k=0; k<n-dst_count && k<src_count; k++){
                            indices_all[now][k+dst_count] = indices[i][k];
                        }
                        indices_all[now][n]+= std::max(0, std::min(n-dst_count, src_count));
                    }
                    sort[now][3]+=stat[i][3];

                    
                }
            }
        }
        curr++;

    }
    /*
    std::vector<std::vector<float>> newcoord(stat.size(), std::vector<float>(4, 0.0f));
    //std::vector<std::vector<float>> newindices(stat.size(), std::vector<float>(4, 0.0f));
    now = 0;
    for (size_t i=0; i<stat.size(); i++){
        if (sort[i][3]>20){
            for (size_t j=0; j<4; j++){
                newcoord[now][j]=sort[i][j];
            }
            now++;
        }
    }
    */
    std::vector<std::vector<float>> newnewcoord(sort.begin(), sort.begin()+now+1);
    std::vector<std::vector<int>> newnewindices(indices_all.begin(), indices_all.begin()+now+1);
    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> AB = {newnewcoord, newnewindices};
    return AB;
}

PYBIND11_MODULE(sets_of_point, m){
    m.def("statistick", &statistic, "int from sets");
    m.def("sort_dat", &sort_stat, "stat from statictick");
}
