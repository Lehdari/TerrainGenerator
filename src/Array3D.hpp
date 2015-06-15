/**
    Array3D.hpp

    @author Miika Lehtimäki
    @date   2015-06-07
**/

#ifndef ARRAY3D_HPP
#define ARRAY3D_HPP


#include <array>
#include <vector>
#include <cstdio>
#include <memory>


#define PI 3.14159265359


//  Structs & classes
template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
class Array3D {
public:
    Array3D(void);

    //std::array<std::array<T, ZSize_T>, YSize_T>& operator[](unsigned id);
    T& operator()(unsigned x, unsigned y, unsigned z);

    void remapValues(T lower, T upper, const std::vector<T>& curve);

    void saveToFile(const std::string& fileName) const;
    void loadFromFile(const std::string& fileName);

private:
    std::unique_ptr<std::array<std::unique_ptr<std::array<std::unique_ptr<std::array<T, ZSize_T>>, YSize_T>>, XSize_T>> arr_;
};


//  Member definitions
/*template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
std::array<std::array<T, ZSize_T>, YSize_T>& Array3D<T, XSize_T, YSize_T, ZSize_T>::operator[](unsigned id) {
    return arr_[id];
}*/

template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
Array3D<T, XSize_T, YSize_T, ZSize_T>::Array3D(void) :
    arr_(new std::array<std::unique_ptr<std::array<std::unique_ptr<std::array<T, ZSize_T>>, YSize_T>>, XSize_T>())
{
    for (auto i=0u; i<XSize_T; ++i) {
        (*arr_)[i] = std::unique_ptr<std::array<std::unique_ptr<std::array<T, ZSize_T>>, YSize_T>>(new std::array<std::unique_ptr<std::array<T, ZSize_T>>, YSize_T>());
        for (auto j=0u; j<YSize_T; ++j) {
            (*(*arr_)[i])[j] = std::unique_ptr<std::array<T, ZSize_T>>(new std::array<T, ZSize_T>());
        }
    }
}

template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
T& Array3D<T, XSize_T, YSize_T, ZSize_T>::operator()(unsigned x, unsigned y, unsigned z) {
    return (*(*(*arr_)[x])[y])[z];
}

template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
void Array3D<T, XSize_T, YSize_T, ZSize_T>::remapValues(T lower, T upper, const std::vector<T>& curve) {
    if (curve.size() < 2)
        return;

    float m = (curve.size()-1)/(upper-lower);

    for (auto i=0u; i<XSize_T; ++i) {
        for (auto j=0u; j<YSize_T; ++j) {
            for (auto k=0u; k<ZSize_T; ++k) {
                float p = ((*(*(*arr_)[i])[j])[k]-lower)*m;
                if (p < 0.0f) p = 0.0f;
                if (p > (float)curve.size()) p = (float)curve.size();

                unsigned id = (int)p;
                float ip = (1.0f-cosf((p-id)*PI))*0.5;

                (*(*(*arr_)[i])[j])[k] = curve[id]+ip*(curve[std::min(id+1, curve.size()-1)]-curve[id]);
            }
        }
    }
}

template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
void Array3D<T, XSize_T, YSize_T, ZSize_T>::saveToFile(const std::string& fileName) const {
    FILE* f = fopen(fileName.c_str(), "wb");

    if (f) {
        for (auto i=0u; i<XSize_T; ++i)
            for (auto j=0u; j<YSize_T; ++j)
                fwrite(&(*(*(*arr_)[i])[j])[0], sizeof(T), ZSize_T, f);

        fclose(f);
    }
}

template<typename T, unsigned XSize_T, unsigned YSize_T, unsigned ZSize_T>
void Array3D<T, XSize_T, YSize_T, ZSize_T>::loadFromFile(const std::string& fileName) {
    FILE* f = fopen(fileName.c_str(), "rb");

    if (f) {
        for (auto i=0u; i<XSize_T; ++i)
            for (auto j=0u; j<YSize_T; ++j)
                fread(&(*(*(*arr_)[i])[j])[0], sizeof(T), ZSize_T, f);

        fclose(f);
    }
}


#endif // 3DARRAY_HPP
