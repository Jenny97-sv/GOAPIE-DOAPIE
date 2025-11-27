#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <optional>

class Blackboard
{
public:
    //using Value = std::variant<int, float, bool, std::string>;

    // Set a value
    template<typename T>
    void SetValue(const std::string& key, const T& value)
    {
        data[key] = value;
    }

    // Get a value (returns std::optional<T>)
    template<typename T>
    T GetValue(const std::string& key) const
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            try
            {
                return std::any_cast<T>(it->second);
            }
            catch (const std::bad_any_cast&)
            {
                return T(); // wrong type stored
            }
        }
        return T(); // not found
    }


    // Check if a key exists
    bool HasKey(const std::string& key) const
    {
        return data.find(key) != data.end();
    }

    // Remove a key
    void Remove(const std::string& key)
    {
        data.erase(key);
    }

private:
    std::unordered_map<std::string, std::any> data;
};
