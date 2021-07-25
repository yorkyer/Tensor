#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <cmath>

namespace yorkyer {


template<typename T, int ... Dims>
struct Tensor
{
    std::vector<T> data;
    std::array<int, sizeof...(Dims)> dims = {Dims...};

    Tensor() {
        int product = 1;
        for (auto d : dims) {
            product *= d;
        }
        data.resize(product);
    }

    Tensor(std::vector<T> data) : data(data) { }

    template<typename ... Coordinates> inline
    T & at(Coordinates ... coordinates) {
        std::array<int, sizeof...(coordinates)> coords = {coordinates...};
        assert(dims.size() == coords.size());
        size_t index = 0;
        size_t mul = 1;
        for (int i = dims.size() - 1; i >= 0; --i) {
            index += coords[i] * mul;
            mul *= dims[i];
        }
        return data[index];
    }
};


template<typename T, int I, int J> inline
std::unique_ptr<Tensor<T, I> > 
matMulVec(Tensor<T, I, J> & mat, Tensor<T, J> & vec)
{
    auto outcome = std::make_unique<Tensor<T, I> >();
    for (int i = 0; i < I; ++i) {
        T sum = 0;
        for (int j = 0; j < J; ++j) {
            sum += mat.at(i, j) * vec.at(j);
        }
        outcome->at(i) = sum;
    }
    return std::move(outcome);
}


template<typename T, int I> inline
void 
vecAddVecInplace(Tensor<T, I> & vec, Tensor<T, I> & vec1)
{
    for (int i = 0; i < I; ++i) {
        vec.at(i) += vec1.at(i);
    }
}


template<typename T, int I, int J> inline
std::unique_ptr<Tensor<T, I> > 
linearTransform(Tensor<T, I, J> & mat, Tensor<T, J> & vec, Tensor<T, I> & vec1)
{
    auto outcome = std::make_unique<Tensor<T, I> >();
    for (int i = 0; i < I; ++i) {
        T sum = 0;
        for (int j = 0; j < J; ++j) {
            sum += mat.at(i, j) * vec.at(j);
        }
        sum += vec1.at(i);
        outcome->at(i) = sum;
    }
    return std::move(outcome);
}


template<typename T, int I, int J> inline
void print(Tensor<T, I, J> & mat)
{
    for (int i = 0; i < I; ++i) {
        for (int j = 0; j < J; ++j) {
            std::cout << mat.at(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


template<typename T, int I> inline
void print(Tensor<T, I> & vec)
{
    for (int i = 0; i < I; ++i) {
        std::cout << vec.at(i) << ' ';
    }
    std::cout << std::endl;
    std::cout << std::endl;
}


template<typename T, int I> inline
bool isEqual(Tensor<T, I> & vec, Tensor<T, I> & vec1, double tolerance=0.01)
{
    for (int i = 0; i < I; ++i) {
        if (abs(vec.at(i) - vec1.at(i)) >= tolerance) {
            return false;
        }
    }
    return true;
}


}
