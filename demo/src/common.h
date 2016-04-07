#include <iostream>
#include <sstream>
#include <fstream>
#include <string>  
#include <cgicc/Cgicc.h> 
#include "json/json.h"
using namespace std;

inline bool read_file(const string& path, string& content)
{
    try
    {
        ifstream infile(path.c_str());
        ostringstream buf;
        char ch;
        while(buf && infile.get(ch))
            buf.put(ch);
        content = buf.str();
        infile.close();
        return true;
    }
    catch(...)
    {   
        content = "";
    }
    return false;
}

inline bool append_file(const string& path, string& content)
{
    try
    {
        ofstream outfile(path.c_str(),ios::app);
        outfile << content;
        outfile.close();
        return true;
    }
    catch(...)
    {   
        content = "";
    }
    return false;
}


inline bool write_file(const string& path, string& content)
{
    try
    {
        ofstream outfile(path.c_str(),ios::out);
        outfile<<content;
        outfile.close();
        return true;
    }
    catch(...)
    {   
        content = "";
    }
    return false;
}


//加载html模板文件
void load_template(const string& path, string& html)
{
    read_file(path, html);
}

//session用本地文件取代
Json::Value load_session()
{
    string path = "/tmp/session.dat";//session file
    string content;
    read_file(path, content);

    Json::Value session;
    Json::Reader reader;
    if( reader.parse(content, session) == true )
    {}
    return session;
}
bool save_session(Json::Value &session)
{
    Json::FastWriter writer;
    string session_str = writer.write(session);
    string path = "/tmp/session.dat";
    return write_file(path, session_str);
}

// Cgicc 获取CGI传递参数
string query_param(cgicc::Cgicc &formData, const string &param)
{
    string value;
    cgicc::form_iterator fi = formData.getElement(param);  
    if( !fi->isEmpty() && fi != (*formData).end() ) {  
        value = ** fi;
    }else{
        value = "";
    }
    return value;
}