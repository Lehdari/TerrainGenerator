/**
    PerlinNoise.hpp

    @author Miika Lehtimäki
    @date   2015-06-07
**/

#ifndef MULTIDIMARRAY_HPP
#define MULTIDIMARRAY_HPP


#include <array>
#include <random>


namespace Perlin {

    //  Structs & classes
    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    class MultiDimArray {
    public:
        MultiDimArray(void);
        MultiDimArray(MultiDimArray<T, EdgeWidth_T, 3>* left,
                      MultiDimArray<T, EdgeWidth_T, 3>* right,
                      MultiDimArray<T, EdgeWidth_T, 3>* above,
                      MultiDimArray<T, EdgeWidth_T, 3>* below);

        void setBoundaries(MultiDimArray<T, EdgeWidth_T, 3>* left,
                           MultiDimArray<T, EdgeWidth_T, 3>* right,
                           MultiDimArray<T, EdgeWidth_T, 3>* above,
                           MultiDimArray<T, EdgeWidth_T, 3>* below);

        void setPrev(MultiDimArray<T, EdgeWidth_T, NDims_T-1>* prev_);
        void setNext(MultiDimArray<T, EdgeWidth_T, NDims_T-1>* next_);

        MultiDimArray<T, EdgeWidth_T, NDims_T-1>& operator[](unsigned id);

        T sampleCubic(float edgeWidth, std::array<float, NDims_T> point) const;

        const std::array<MultiDimArray<T, EdgeWidth_T, NDims_T-1>, EdgeWidth_T>& getArray(void) const;

    private:
        MultiDimArray<T, EdgeWidth_T, NDims_T-1>* prev_;
        MultiDimArray<T, EdgeWidth_T, NDims_T-1>* next_;

        std::array<MultiDimArray<T, EdgeWidth_T, NDims_T-1>, EdgeWidth_T> arr_;
    };

    template<typename T, unsigned EdgeWidth_T>
    class MultiDimArray<T, EdgeWidth_T, 1> {
    public:
        T& operator[](unsigned id);

        T sampleCubic(float edgeWidth, std::array<float, 1> point) const;

    private:
        std::array<T, EdgeWidth_T> arr_;
    };


    //  Helper functions
    template<typename T, unsigned EdgeWidth_T, unsigned ArrLevel_T>
    void randomizeArray(MultiDimArray<T, EdgeWidth_T, ArrLevel_T>& arr, std::default_random_engine& rnd, T minAmp, T maxAmp);

    template<typename T, unsigned EdgeWidth_T, unsigned ArrLevel_T>
    void randomizeArray(MultiDimArray<T, EdgeWidth_T, ArrLevel_T>& arr,
                        std::default_random_engine& rnd, T minAmp, T maxAmp,
                        MultiDimArray<T, EdgeWidth_T, ArrLevel_T>* left,
                        MultiDimArray<T, EdgeWidth_T, ArrLevel_T>* above);


    //  Member definitions
    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    MultiDimArray<T, EdgeWidth_T, NDims_T>::MultiDimArray(void) :
        prev_(nullptr), next_(nullptr)
    {}

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    MultiDimArray<T, EdgeWidth_T, NDims_T>::MultiDimArray(MultiDimArray<T, EdgeWidth_T, 3>* left,
                                                          MultiDimArray<T, EdgeWidth_T, 3>* right,
                                                          MultiDimArray<T, EdgeWidth_T, 3>* above,
                                                          MultiDimArray<T, EdgeWidth_T, 3>* below) :
        prev_(&(*left)[EdgeWidth_T-1]), next_(&(*right)[0])
    {
        for (auto i=0u; i<EdgeWidth_T; ++i) {
            arr_[i].setPrev(&(*above)[i][EdgeWidth_T-1]);
            arr_[i].setNext(&(*above)[i][0]);
        }
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    void MultiDimArray<T, EdgeWidth_T, NDims_T>::setBoundaries(MultiDimArray<T, EdgeWidth_T, 3>* left,
                                                               MultiDimArray<T, EdgeWidth_T, 3>* right,
                                                               MultiDimArray<T, EdgeWidth_T, 3>* above,
                                                               MultiDimArray<T, EdgeWidth_T, 3>* below) {
        prev_ = left[EdgeWidth_T-1];
        next_ = right[0];

        for (auto i=0u; i<EdgeWidth_T; ++i) {
            arr_[i].setPrev(above[i][EdgeWidth_T-1]);
            arr_[i].setNext(above[i][0]);
        }
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    void MultiDimArray<T, EdgeWidth_T, NDims_T>::setPrev(MultiDimArray<T, EdgeWidth_T, NDims_T-1>* prev) {
        prev_ = prev;
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    void MultiDimArray<T, EdgeWidth_T, NDims_T>::setNext(MultiDimArray<T, EdgeWidth_T, NDims_T-1>* next) {
        next_ = next;
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    MultiDimArray<T, EdgeWidth_T, NDims_T-1>& MultiDimArray<T, EdgeWidth_T, NDims_T>::operator[](unsigned id) {
        return arr_.at(id);
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    T MultiDimArray<T, EdgeWidth_T, NDims_T>::sampleCubic(float edgeWidth, std::array<float, NDims_T> point) const {
        float p = ((EdgeWidth_T-1)/edgeWidth)*point[0];
        unsigned id = (unsigned)p;
        float ip = p-id;
        int sId[4] = { (int)id-1, (int)id, (int)id+1, (int)id+2 };

        std::array<float, NDims_T-1> newPoint;
        for (auto i=0u; i<NDims_T-1; ++i)
            newPoint[i] = point[i+1];

        T s[4];

        if (sId[0] < 0 && prev_)
            s[0] = prev_->sampleCubic(edgeWidth, newPoint);
        else
            s[0] = arr_[sId[1]].sampleCubic(edgeWidth, newPoint);

        if (sId[3] > (int)EdgeWidth_T-1 && next_)
            s[3] = next_->sampleCubic(edgeWidth, newPoint);
        else
            s[3] = arr_[sId[2]].sampleCubic(edgeWidth, newPoint);

        s[1] = arr_[sId[1]].sampleCubic(edgeWidth, newPoint);
        s[2] = arr_[sId[2]].sampleCubic(edgeWidth, newPoint);

        return s[1] + 0.5 * ip*(s[2] - s[0] + ip*(2.0*s[0] - 5.0*s[1] + 4.0*s[2] - s[3] + ip*(3.0*(s[1] - s[2]) + s[3] - s[0])));
    }

    template<typename T, unsigned EdgeWidth_T, unsigned NDims_T>
    const std::array<MultiDimArray<T, EdgeWidth_T, NDims_T-1>, EdgeWidth_T>& MultiDimArray<T, EdgeWidth_T, NDims_T>::getArray(void) const {
        return arr_;
    }

    template<typename T, unsigned EdgeWidth_T>
    T& MultiDimArray<T, EdgeWidth_T, 1>::operator[](unsigned id) {
        return arr_.at(id);
    }

    template<typename T, unsigned EdgeWidth_T>
    T MultiDimArray<T, EdgeWidth_T, 1>::sampleCubic(float edgeWidth, std::array<float, 1> point) const {
        float p = ((EdgeWidth_T-1)/edgeWidth)*point[0];
        unsigned id = (unsigned)p;
        float ip = p-id;

        int sId[4] = { std::max(0, (int)id-1), (int)id, std::min((int)EdgeWidth_T-1, (int)id+1), std::min((int)EdgeWidth_T-1, (int)id+2) };

        T s[4] = { arr_[sId[0]],
                   arr_[sId[1]],
                   arr_[sId[2]],
                   arr_[sId[3]] };

        return s[1] + 0.5 * ip*(s[2] - s[0] + ip*(2.0*s[0] - 5.0*s[1] + 4.0*s[2] - s[3] + ip*(3.0*(s[1] - s[2]) + s[3] - s[0])));
    }


    //  Helper function definitions
    template<typename T, unsigned EdgeWidth_T, unsigned ArrLevel_T>
    void randomizeArray(MultiDimArray<T, EdgeWidth_T, ArrLevel_T>& arr, std::default_random_engine& rnd, T minAmp, T maxAmp) {
        for (auto i=0u; i<EdgeWidth_T; ++i) {
            randomizeArray(arr[i], rnd, minAmp, maxAmp);
        }
    }

    template<typename T, unsigned EdgeWidth_T, unsigned ArrLevel_T>
    void randomizeArray(MultiDimArray<T, EdgeWidth_T, ArrLevel_T>& arr,
                        std::default_random_engine& rnd, T minAmp, T maxAmp,
                        MultiDimArray<T, EdgeWidth_T, ArrLevel_T>* left,
                        MultiDimArray<T, EdgeWidth_T, ArrLevel_T>* above) {

        if (above) {
            for (auto i=0u; i<EdgeWidth_T; ++i)
                for (auto k=0u; k<EdgeWidth_T; ++k)
                    arr[i][0][k] = (*above)[i][EdgeWidth_T-1][k];
        }
        else {
            for (auto i=0u; i<EdgeWidth_T; ++i) {
                for (auto k=0u; k<EdgeWidth_T; ++k) {
                    T m = (maxAmp - minAmp) / (rnd.max() - rnd.min());
                    arr[i][0][k] = minAmp + rnd()*m;
                }
            }
        }

        if (left) {
            for (auto j=0u; j<EdgeWidth_T; ++j)
                for (auto k=0u; k<EdgeWidth_T; ++k)
                    arr[0][j][k] = (*left)[EdgeWidth_T-1][j][k];
        }
        else {
            for (auto j=0u; j<EdgeWidth_T; ++j) {
                for (auto k=0u; k<EdgeWidth_T; ++k) {
                    T m = (maxAmp - minAmp) / (rnd.max() - rnd.min());
                    arr[0][j][k] = minAmp + rnd()*m;
                }
            }
        }

        for (auto i=1u; i<EdgeWidth_T; ++i) {
            for (auto j=1u; j<EdgeWidth_T; ++j) {
                for (auto k=0u; k<EdgeWidth_T; ++k) {
                    T m = (maxAmp - minAmp) / (rnd.max() - rnd.min());
                    arr[i][j][k] = minAmp + rnd()*m;
                }
            }
        }
    }

    template<typename T, unsigned EdgeWidth_T>
    void randomizeArray(MultiDimArray<T, EdgeWidth_T, 1>& arr, std::default_random_engine& rnd, T minAmp, T maxAmp) {
        for (auto i=0u; i<EdgeWidth_T; ++i) {
            T m = (maxAmp - minAmp) / (rnd.max() - rnd.min());
            arr[i] = minAmp + rnd()*m;
        }
    }

} // namespace Perlin


#endif // MULTIDIMARRAY_HPP
