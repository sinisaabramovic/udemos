#ifndef ud_result_failure_hpp
#define ud_result_failure_hpp

#include <string>

class ud_result_failure
{
public:
    ud_result_failure() {}
    ud_result_failure(const std::string &result) : m_result(result) {}
    ~ud_result_failure() {}

    std::string get_description() const
    {
        return m_result;
    }
    
private:
    std::string m_result;
};

#endif