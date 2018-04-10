#ifndef _CONFIG_READER_H
#define _CONFIG_READER_H

#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

namespace thunder
{

class ConfigReader
{
public:
    bool Load(const string& file);

    string GetString(const string& section,
                     const string& key,
                     const string& default_value = "");

    vector<string> GetList(const string& section, const string& key);

    template<typename T> T GetNumber(const string& section,
                                     const string& key,
                                     T default_value = 0)
    {
        string name = section + "." + key;

        if (_m.find(name) != _m.end())
        {
            istringstream iss(_m[name]);
            T num = 0;
            iss >> num;
            return num;
        }

        return default_value;
    }

    bool GetBool(const string& section,
                 const string& key,
                 bool default_value = false);

    string& operator[](const string& name);

private:
    bool isSection(string line, string& section);
    string& trim(string& s);

private:
    map<string, string> _m;
};

}

#endif

