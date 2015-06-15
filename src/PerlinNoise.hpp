/**
    PerlinNoise.hpp

    @author Miika Lehtimäki
    @date   2015-06-06
**/

#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP


#include "MultiDimArray.hpp"


namespace Perlin {

    //  Structs & classes
    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    class PerlinNoise {
    public:
        PerlinNoise(std::default_random_engine& rnd, T amplitude);
        PerlinNoise(std::default_random_engine& rnd, T amplitude,
                    PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* left,
                    PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* right,
                    PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* above,
                    PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* below);

        void setRight(PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* right);
        void setBelow(PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* below);

        const MultiDimArray<T, EdgeWidth_T, NDims_T>& getArray(void) const;
        const PerlinNoise<T, NDims_T, EdgeWidth_T*2, Depth_T-1>& getNext(void) const;

        T sample(float edgeWidth, std::array<float, NDims_T> point) const;

    private:
        std::default_random_engine& rnd_;

        PerlinNoise<T, NDims_T, EdgeWidth_T*2, Depth_T-1> perlin_;
        MultiDimArray<T, EdgeWidth_T, NDims_T> arr_;

        PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* left_;
        PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* right_;
        PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* above_;
        PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* below_;
    };

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T>
    class PerlinNoise<T, NDims_T, EdgeWidth_T, 1> {
    public:
        PerlinNoise(std::default_random_engine& rnd, T amplitude);
        PerlinNoise(std::default_random_engine& rnd, T amplitude,
                    PerlinNoise<T, 3, EdgeWidth_T, 1>* left,
                    PerlinNoise<T, 3, EdgeWidth_T, 1>* right,
                    PerlinNoise<T, 3, EdgeWidth_T, 1>* above,
                    PerlinNoise<T, 3, EdgeWidth_T, 1>* below);

        const MultiDimArray<T, EdgeWidth_T, NDims_T>& getArray(void) const;

        T sample(float edgeWidth, std::array<float, NDims_T> point) const;

    private:
        std::default_random_engine& rnd_;

        MultiDimArray<T, EdgeWidth_T, NDims_T> arr_;

        PerlinNoise<T, 3, EdgeWidth_T, 1>* left_;
        PerlinNoise<T, 3, EdgeWidth_T, 1>* right_;
        PerlinNoise<T, 3, EdgeWidth_T, 1>* above_;
        PerlinNoise<T, 3, EdgeWidth_T, 1>* below_;
    };


    //  Member definitions
    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::PerlinNoise(std::default_random_engine& rnd, T amplitude) :
        rnd_(rnd),
        perlin_(PerlinNoise<T, NDims_T, EdgeWidth_T*2, Depth_T-1>(rnd, amplitude/2))
    {
        randomizeArray(arr_, rnd_, -amplitude/2, amplitude/2);
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::PerlinNoise(std::default_random_engine& rnd, T amplitude,
                                                               PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* left,
                                                               PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* right,
                                                               PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* above,
                                                               PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* below) :
        rnd_(rnd),
        perlin_(PerlinNoise<T, NDims_T, EdgeWidth_T*2, Depth_T-1>(rnd, amplitude/2, left_ ? &(left->perlin_) : nullptr, right_ ? &(right->perlin_) : nullptr, above_ ? &(above->perlin_) : nullptr, below ? &(below->perlin_) : nullptr)),
        arr_(&(left->arr_), &(right->arr_), &(above->arr_), &(below->arr_)),
        left_(left), right_(right), above_(above), below_(below)
    {
        if (left_ && above_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, &(left_->arr_), &(above_->arr_));
        else if (left_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, &(left_->arr_), nullptr);
        else if (above_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, nullptr, &(above_->arr_));
        else
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2);
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    void PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::setRight(PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* right) {
        right_ = right;
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    void PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::setBelow(PerlinNoise<T, 3, EdgeWidth_T, Depth_T>* below) {
        below_ = below;
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    const MultiDimArray<T, EdgeWidth_T, NDims_T>& PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::getArray(void) const {
        return arr_;
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    const PerlinNoise<T, NDims_T, EdgeWidth_T*2, Depth_T-1>& PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::getNext(void) const {
        return perlin_;
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T, unsigned Depth_T>
    T PerlinNoise<T, NDims_T, EdgeWidth_T, Depth_T>::sample(float edgeWidth, std::array<float, NDims_T> point) const {
        //if (left_ && right_ && above_ && below_)
            return arr_.sampleCubic(edgeWidth, point) + perlin_.sample(edgeWidth, point);
        //else
        //    return arr_.sampleCubic(edgeWidth, point) + perlin_.sample(edgeWidth, point);
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T>
    PerlinNoise<T, NDims_T, EdgeWidth_T, 1>::PerlinNoise(std::default_random_engine& rnd, T amplitude) :
        rnd_(rnd)
    {
        randomizeArray(arr_, rnd_, -amplitude/2, amplitude/2);
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T>
    PerlinNoise<T, NDims_T, EdgeWidth_T, 1>::PerlinNoise(std::default_random_engine& rnd, T amplitude,
                                                         PerlinNoise<T, 3, EdgeWidth_T, 1>* left,
                                                         PerlinNoise<T, 3, EdgeWidth_T, 1>* right,
                                                         PerlinNoise<T, 3, EdgeWidth_T, 1>* above,
                                                         PerlinNoise<T, 3, EdgeWidth_T, 1>* below) :
        rnd_(rnd),
        arr_(&(left->arr_), &(right->arr_), &(above->arr_), &(below->arr_)),
        left_(left), right_(right), above_(above), below_(below)
    {
        if (left_ && above_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, &(left_->arr_), &(above_->arr_));
        else if (left_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, &(left_->arr_), nullptr);
        else if (above_)
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2, nullptr, &(above_->arr_));
        else
            randomizeArray<T, EdgeWidth_T, 3>(arr_, rnd_, -amplitude/2, amplitude/2);
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T>
    const MultiDimArray<T, EdgeWidth_T, NDims_T>& PerlinNoise<T, NDims_T, EdgeWidth_T, 1>::getArray(void) const {
        return arr_;
    }

    template<typename T, unsigned NDims_T, unsigned EdgeWidth_T>
    T PerlinNoise<T, NDims_T, EdgeWidth_T, 1>::sample(float edgeWidth, std::array<float, NDims_T> point) const {
        //if (left_ && right_ && above_ && below_)
            return arr_.sampleCubic(edgeWidth, point);
        //else
            //return arr_.sampleCubic(edgeWidth, point);
    }

}; // namespace Perlin


#endif // PERLINNOISE_HPP
