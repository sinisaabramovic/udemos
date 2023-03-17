//
//  Configuration.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef Configuration_hpp
#define Configuration_hpp

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Configuration {
public:
    static Configuration& getInstance();
    
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    
    void loadFromFile(const std::string& filePath);
    void loadFromEnvironment();
    void loadFromCommandLine(int argc, char* argv[]);
    
    template<typename T>
    T get(const std::string& key) const;
    
    template<typename T>
    void set(const std::string& key, const T& value);
    
private:
    Configuration() = default;    
    nlohmann::json config_;
};


#endif /* Configuration_hpp */
