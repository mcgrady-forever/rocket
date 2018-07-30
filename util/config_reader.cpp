#include "config_reader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <strings.h>

using namespace std;

bool ConfigReader::Load(const string& file)
{
    ifstream ifs(file.c_str());
    if (!ifs.good())
    {
        return false;
    }

    string line;
    string section = "";

    while (!ifs.eof())
    {
        getline(ifs, line);

        if (isSection(line, section))
        {
            continue;
        }

        size_t equ_pos = line.find('=');
        if (equ_pos == string::npos) continue;
        string key = line.substr(0, equ_pos);
        string value = line.substr(equ_pos + 1);
        key = trim(key);
        value = trim(value);

        if (key.empty()) continue;

        _m[section + "." + key] = value;
    }

    ifs.close();

    return true;
}

string ConfigReader::GetString(const string& section,
                               const string& key,
                               const string& default_value)
{
    string name = section + "." + key;

    if (_m.find(name) != _m.end())
    {
        return _m[name];
    }

    return default_value;
}

vector<string> ConfigReader::GetList(const string& section,
                                     const string& key)
{
    vector<string> v;
    string name = section + "." + key;
    string str = GetString(section, key, "");
    string sep = ", \t";
    string substr;
    string::size_type start = 0;
    string::size_type index;

    while ((index = str.find_first_of(sep, start)) != string::npos)
    {
        substr = str.substr(start, index - start);
        v.push_back(substr);

        start = str.find_first_not_of(sep, index);
        if (start == string::npos)
            return v;
    }

    substr = str.substr(start);
    v.push_back(substr);

    return v;
}

//long long ConfigReader::GetNumber(const string& section,
//                                  const string& key,
//                                  long long default_value)
//{
//    string name = section + "." + key;
//
//    if (_m.find(name) != _m.end())
//    {
//        istringstream iss(_m[name]);
//        long long num = 0;
//        iss >> num;
//        return num;
//    }
//
//    return default_value;
//}

bool ConfigReader::GetBool(const string& section,
                           const string& key,
                           bool default_value)
{
    string name = section + "." + key;

    if (_m.find(name) != _m.end())
    {
        return 0 == strcasecmp(_m[name].c_str(), "true");
    }

    return default_value;
}

string& ConfigReader::operator[](const string& name)
{
    if (_m.find(name) == _m.end())
    {
        fprintf(stderr, "config[%s] error\n", name.c_str());
    }

    return _m[name];
}

/**
 * determine whether a line contains a section name
 *
 * @param line    a line from the config file
 * @param section section description
 *
 * @return return true and set section if the line is a section, else false
 */
bool ConfigReader::isSection(string line, string& section) {
    string sec = trim(line);

    if (sec.empty() || sec.length() <= 2)
        return false;

    if (sec.at(0) != '[' || sec.at(sec.length() - 1) != ']')
        return false;

    sec = sec.substr(1, sec.length() - 2);
    sec = trim(sec);
    section = sec;

    return true;
}

string& ConfigReader::trim(string& s)
{
    if (s.empty())
    {
        return s;
    }

    // 删除注释
    string comment = "#;";
    if (n != string::npos)
    {
        s.erase(n);
    }

    // 删除空白
    string blank = " \r\n\t";
    s.erase(0, s.find_first_not_of(blank));
    s.erase(s.find_last_not_of(blank) + 1);
    
    return s;
}
