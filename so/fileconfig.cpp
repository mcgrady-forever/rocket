#include "fileconfig.h"
#include <cstring>
using namespace std;
namespace hy_base {
FileConfig::FileConfig() {
}

FileConfig::FileConfig(const char* szFileName) {
    m_ConfigFile.open(szFileName);

    ParseFile();
}

FileConfig::~FileConfig() {
    m_ConfigFile.close();
}

// delete pre- or -end white space
int FileConfig::StrimString(char* szLine) {
    int i, j;

    // ignor -end comment
    char *p;
    p = strstr(szLine, "//");
    if (p != NULL) {
        *p = 0;
    }

    // delete -end white space
    j = strlen(szLine) - 1;
    while (isspace(szLine[j])) {
        if (j == 0)
            return -1;
        szLine[j] = 0;
        j--;
    }
    // delete pre- white space
    i = 0;
    j = 0;
    while (isspace(szLine[j])) {
        if (szLine[j] == 0)
            return -1;
        j++;
    }
    // shift string
    while (szLine[j] != 0) {
        szLine[i] = szLine[j];
        i++;
        j++;
    }
    szLine[i] = 0;

    // whole comment line
    if ((szLine[0] == '/') && (szLine[1] == '/'))
        return 1;

    return 0;
}

int FileConfig::ParseFile(const char* szConfigFile) {
    m_ConfigFile.open(szConfigFile);
    if (m_ConfigFile.fail()) {
        return -1;
    }
    return ParseFile();
}

int FileConfig::ParseFile() {
    char szLine[10240];
    char szSection[640], szParam[1280];
    char *pColon;
    int iLen;

    while (m_ConfigFile.getline(szLine, sizeof(szLine))) {
        if (StrimString(szLine) != 0)
            continue;
        iLen = strlen(szLine);
        if (((szLine[0] == '[') && (szLine[iLen - 1] == ']')) || ((szLine[0] == '<') && (szLine[iLen - 1] == '>'))) {
            pColon = szLine + 1;
            szLine[iLen - 1] = 0;

            strncpy(szSection, pColon, sizeof(szSection));
            // section name
            continue;
        }
        if ((pColon = index(szLine, '=')) == NULL) {
            pColon = index(szLine, ':');
        }
        if (pColon == NULL)
            continue;
        *pColon = 0;
        pColon++;
        if (StrimString(pColon) < 0)
            continue;
        if (StrimString(szLine) < 0)
            continue;
        if (szSection[0] != 0) {
            snprintf(szParam, sizeof(szParam), "%s.%s", szSection, szLine);
        } else {
            snprintf(szParam, sizeof(szParam), "%s", szLine);
        }
        m_ConfigMap[szParam] = pColon;

//        printf("m_ConfigMap   szParam:[%s],szValue:[%s] ----- szSection:[%s],szLine:[%s]\n", szParam, pColon, szSection, szLine);

        map<string, map<string, string> >::iterator multitr;
        multitr =  m_Level_ConfigMap.find(szSection);
        map<string, string>  value_map;
        if (multitr != m_Level_ConfigMap.end()) {
            value_map =  multitr->second;
        }
        value_map[szLine] = pColon;
        m_Level_ConfigMap[szSection] = value_map;
    }
    return 0;
}

map<string, string> FileConfig::GetMap() {

//    cout<< "   ----    "<<endl;
//    cout<< "   ----    "<<endl;
//    cout<< "   ----    "<<endl;
//    cout<< "   ----    "<<endl;
//
//    map<string, map<string, string> >::iterator multitr;  // 以下是如何遍历本multiMap
//    map<string, string>::iterator intertr;
//    for (multitr = m_Level_ConfigMap.begin(); multitr != m_Level_ConfigMap.end(); multitr++) {
//        for (intertr = multitr->second.begin(); intertr != multitr->second.end(); intertr++)
//            cout << multitr->first << " " << intertr->first << " (" << intertr->second << ")" << endl;
//    }
    return m_ConfigMap;
}

string& FileConfig::operator[](const char* szParam) {
    if (szParam == NULL) {
//        fprintf(stderr, "config operator[] error: param null");

        m_ConfigMap[""] = "";
        return m_ConfigMap[""];
    }

    if (m_ConfigMap[szParam].empty()) {
        fprintf(stderr, "config[%s] error.\n", szParam);
        m_ConfigMap[szParam] = "";
    }
    return m_ConfigMap[szParam];
}

string& FileConfig::operator()(const char* szSection, const char* szName) {
    if (szSection == NULL || szName == NULL) {
//        fprintf(stderr, "config operator() error: param null");
        m_ConfigMap[""] = "";
        return m_ConfigMap[""];
    }

    char szParam[64];
    snprintf(szParam, sizeof(szParam), "%s.%s", szSection, szName);

    if (m_ConfigMap.find(szParam) == m_ConfigMap.end()) {
//        fprintf(stderr, "config[%s.%s] error.\n", szSection, szName);
        m_ConfigMap[szParam] = "";
    }
    return m_ConfigMap[szParam];
}

int FileConfig::GetInt(const char* szName, int defValue) {
    if (szName == NULL) {
        m_ConfigMap[""] = "";
//        fprintf(stderr, "config GetInt error: param null\n");
        return defValue;
    }
    if (m_ConfigMap[szName].empty()) {
//        fprintf(stderr, "config GetInt(%s) error.\n", szName);
        return defValue;
    }
    return atoi(m_ConfigMap[szName].data());
}

string FileConfig::GetStr(const string& str, const string& strDefault) {
    map<string, string>::iterator it = m_ConfigMap.find(str);
    if (m_ConfigMap.end() != it) {
        return it->second;
    } else {
//        fprintf(stderr, "config GetStr(%s) error.\n", str.c_str());
        return strDefault;
    }
}


map<string, string> FileConfig::GetMapByPrefix(const string& prefix) {

    map<string, map<string, string> >::iterator multitr;
    multitr = m_Level_ConfigMap.find(prefix);
    map<string, string> value_map;
    if (multitr != m_Level_ConfigMap.end()) {  //没有找到
        value_map = multitr->second;
    }
    return value_map;

}

}
