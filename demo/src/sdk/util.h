#include <sstream>
#include <iostream>  
#include <string>  
#include <vector>
#include <cstdlib>
#include <ctime>

//double d = convert<double>(s)
template<class out_type,class in_value>
inline out_type convert(const in_value & t)
{
    stringstream stream;
    stream<<t;//向流中传值
    out_type result;//这里存储转换结果
    stream>>result;//向result中写入值
    return result;
}


inline std::string trim(std::string s)   
{  
    if (s.empty())   
        return s;  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
}  

inline void split(const std::string& s, const std::string& delim, std::vector<std::string> &ret)  
{  
    size_t last = 0;  
    size_t index=s.find_first_of(delim,last);  
    while (index!=std::string::npos)  
    {  
        ret.push_back(s.substr(last,index-last));  
        last=index+1;  
        index=s.find_first_of(delim,last);  
    }  
    if (index-last>0)  
    {  
        ret.push_back(s.substr(last,index-last));  
    }  
}  


inline int random_num(int max)
{
    srand( (unsigned)time( NULL ) );
    int rand_num = rand() % 100;   //其随机域为0~MAX-1
    return rand_num;
}

//十六进制转十进制
inline int hex2int(string s)
{
    return strtol(s.c_str(), (char**)NULL, 16);
}
