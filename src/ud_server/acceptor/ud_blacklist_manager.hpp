#ifndef ud_blacklist_manager_hpp
#define ud_blacklist_manager_hpp

#include <iostream>
#include <string>
#include <vector>

class ud_blacklist_manager
{
private:    
    std::vector<std::string> m_blacklisted_ips;    

public:
    ud_blacklist_manager(const std::vector<std::string> &blacklisted_ips = {}) : m_blacklisted_ips(blacklisted_ips) {}
    ~ud_blacklist_manager() {}

    bool is_ip_blacklisted(const std::string &ip)
    {
        return std::find(m_blacklisted_ips.begin(), m_blacklisted_ips.end(), ip) != m_blacklisted_ips.end();
    }

    void set_blacklisted_ip(const std::string &ip)
    {
        m_blacklisted_ips.emplace_back(ip);
    }
};

#endif