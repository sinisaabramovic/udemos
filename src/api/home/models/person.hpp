#ifndef person_hpp
#define person_hpp

#include <iostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class person
{
private:
    std::string m_name;
    int m_age;
    std::string m_address;

public:
    person(/* args */);
    ~person();

    bool fromJson(const Value &json)
    {                        
        if (json.HasMember("name") && json["name"].IsString())
        {
            this->m_name = json["name"].GetString();
        }
        else
        {
            return false;
        }

        if (json.HasMember("age") && json["age"].IsInt())
        {
            this->m_age = json["age"].GetInt();
        }
        else
        {
            return false;
        }

        if (json.HasMember("address") && json["address"].IsString())
        {
            this->m_address = json["address"].GetString();
        }
        else
        {
            return false;
        }
        return true;
    }

    std::string get_name() const
    {
        return this->m_name;
    }

    std::string get_address() const
    {
        return this->m_address;
    }

    int get_age() const
    {
        return this->m_age;
    }

    void set_name(const std::string &name)
    {
        this->m_name = name;
    }

    void set_address(const std::string &address)
    {
        this->m_address = address;
    }

    void set_age(int age)
    {
        this->m_age = age;
    }

    std::string toJSON()
    {
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        writer.StartObject();
        writer.String("name");
        writer.String(this->m_name.c_str());
        writer.String("address");
        writer.String(this->m_address.c_str());
        writer.String("age");
        writer.Int(this->m_age);
        writer.EndObject();
        return buffer.GetString();
    }
};

person::person(/* args */)
{
}

person::~person()
{
}

#endif