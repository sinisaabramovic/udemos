#ifndef ud_result_success_hpp
#define ud_result_success_hpp

#include <string>

class ud_result_success
{
public:
    ud_result_success() {}
    ud_result_success(const std::string &result) : m_result(result) {}
    ~ud_result_success() {}

    std::string get_description() const
    {
        return m_result;
    }

private:
    std::string m_result;
};

#endif