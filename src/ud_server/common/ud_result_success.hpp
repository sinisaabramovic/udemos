#ifndef ud_result_success_hpp
#define ud_result_success_hpp

#include <string>

class ud_result_success
{
private:
    std::string m_result;

public:
    ud_result_success() {}
    ud_result_success(const std::string &result) : m_result(result) {}
    ~ud_result_success() {}

    std::string get_description() const
    {
        return m_result;
    }
};

#endif