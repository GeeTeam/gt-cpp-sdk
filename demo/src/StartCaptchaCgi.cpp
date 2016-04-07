#include <iostream>
#include <string>  
#include "common.h"

#include "sdk/geetestlib.h"
#include "GeetestConfig.h"

using namespace std;
using namespace geetest;
//
void preProcess(string &response)
{
    GeetestLib gtSdk = GeetestLib(config::captcha_id, config::private_key);
    string resStr = "{}";
    Json::Value session = load_session();

    //进行验证预处理
    if (gtSdk.preProcess() == 1) {
        // gt-server服务正常,预处理完成
        resStr = gtSdk.getSuccessPreProcessRes(); //预处理成功，获取标准返回
        session["gt_server_status_code"] = 1;//在session中设置gt-server服务状态
    } else {
        // 预处理失败
        resStr = gtSdk.getFailPreProcessRes(); //无法连接到gt-server服务器，进行相应处理, 获得返回
        session["gt_server_status_code"] = 0;//在session中设置gt-server服务状态
    }
    
    session["challengeId"] = gtSdk.getChallengeId(); //将challenge设置到session中，二次验证进行challenge比对
    save_session(session);

    response = resStr;
}


int main ()
{
    string response;
    preProcess(response);

    cout << "Content-type:text/text\r\n\r\n";
    cout << response;
   
    return 0;
}




