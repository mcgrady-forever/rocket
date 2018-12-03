/*
 @note:
 读取配置文件, #后面的内容为注释,  支持[section]章节
 @eg:
 #define CONFIG_NOTIFY_TEST (*CSingleton<CBitMapConfigFile>::instance())("notify", "test").c_str())
 @config file:
 [notify] #notify section
 test=123
 */

#ifndef _FILE_CONFIG_H_
#define _FILE_CONFIG_H_
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
namespace hy_base {

class FileConfig {
public:
    FileConfig();
    FileConfig(const char* szFileName);
    virtual ~FileConfig();

public:
    string& operator[](const char* szName);
    string& operator()(const char* szSection, const char* szName);

    /****
     * 获取全部匹配数据
     * [log]
     *  file=xx
     *  level=1
     *
     * config.GetInt("log.level");
     *
     */
    int GetInt(const char* szName, int defValue = 0);
    /****
     * 获取全部匹配数据
     * [log]
     *  file=xx
     *  level=1
     *
     * config.GetStr("log.file");
     *
     */
    string GetStr(const string& str, const string& strDefault);


    int ParseFile(const char* szConfigFile);


    /****
     * 获取全部的配置信息
     * [log]
     *  file=xx
     *  level=1
     *
     *  map<string,string> map = config.GetMap();
     *  map vlaue is:
     *  [log.file = xx]
     *  [log.level= 1]
     *
     */
    map<string, string> GetMap();


    /***
     * 通过前缀来获取配置map
     * [log]
     *  file=xx
     *  level=1
     *
     *  map<string,string> map = config.GetMapByPrefix("log")
     *  map value is:
     *  [file =1]
     *  [level=1]
     *
     */
    map<string, string> GetMapByPrefix(const string& prefix);

private:
    static int StrimString(char* szLine);
    int ParseFile();

private:
    ifstream m_ConfigFile;
    map<string, string> m_ConfigMap;

    map<string, map<string, string> > m_Level_ConfigMap;

};

}

#endif
