//
//  CFunctional.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 12.03.2023..
//

#ifndef CFunctional_h
#define CFunctional_h

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

template<typename T, typename F>
auto operator|(T t, F f) {
    return f(t);
}

template<typename Func, typename... Args>
auto c_send(Func func, Args... args) {
    return [=](auto&&... params) -> decltype(func(args..., forward<decltype(params)>(params)...)) {
        return func(args..., forward<decltype(params)>(params)...);
    };
}

template<typename T, typename Func>
auto c_map(vector<T> vec, Func func) -> vector<decltype(func(T()))> {
    vector<decltype(func(T()))> result;
    result.reserve(vec.size());
    std::transform(vec.begin(), vec.end(), std::back_inserter(result), func);
    return result;
}

template<typename T, typename Func>
auto c_reduce(vector<T> vec, Func func, T initial) -> T {
    return std::accumulate(vec.begin(), vec.end(), initial, func);
}

template<typename T, typename Func>
auto c_filter(vector<T> vec, Func func) -> vector<T> {
    vector<T> result;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(result), func);
    return result;
}

#endif /* CFunctional_h */
