#ifndef ud_common_result_hpp
#define ud_common_result_hpp

#include <utility>
#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
struct is_error_empty : std::false_type
{
};

struct ud_error
{
    std::string message;
};

template <>
struct is_error_empty<ud_error> : std::true_type
{
};

template <typename T, typename E>
class result
{
public:
    result() : m_error{} {}
    result(const T &value) : m_value(value), m_error(E{}) {}
    result(T &&value) : m_value(std::move(value)), m_error(E{}) {}
    result(const E &error) : m_value(T{}), m_error(error) {}
    result(E &&error) : m_value(T{}), m_error(std::move(error)) {}

    bool is_ok() const
    {
        return is_error_empty<E>::value ? !m_error.message.length() : true;
    }

    const T &value() const
    {
        return m_value;
    }

    const E &error() const
    {
        return m_error;
    }

private:
    T m_value;
    E m_error;
};

#endif