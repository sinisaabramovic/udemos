#ifndef ud_result_hpp
#define ud_result_hpp

#include <iostream>
#include <string>
#include <vector>
#include <variant>

template <typename T, typename E>
class ud_result {
public:
    ud_result(const T& value) : m_success(true), m_value(value) {}
    ud_result(const E& error) : m_success(false), m_error(error) {}

    bool is_success() const { return m_success; }
    bool is_error() const { return !m_success; }

    const T& get_value() const { return m_value; }
    const E& get_error() const { return m_error; }

    ~ud_result() {}

private:
    bool m_success;
    union {
        T m_value;
        E m_error;
    };
};

// template <typename E>
// class ud_result<void, E>
// {
// public:
//     ud_result() {}
//     ud_result(const E &error) : m_variant(error) {}

//     bool is_success() const { return std::holds_alternative<success_type>(m_variant); }
//     bool is_error() const { return std::holds_alternative<E>(m_variant); }

//     const E &get_error() const { return std::get<E>(m_variant); }

// private:
//     struct success_type
//     {
//     };

//     std::variant<success_type, E> m_variant;
// };

#endif