#include <iostream>
#include <string>  
#include <cgicc/Cgicc.h> 

#include "common.h"
#include "sdk/geetestlib.h"
#include "GeetestConfig.h"

using namespace std;
using namespace geetest;


string do_validate(string challenge, string validate, string seccode)
{
    GeetestLib gtSdk = GeetestLib(config::captcha_id, config::private_key);
    //从session中获取gt-server状态
    Json::Value session = load_session();
    int gt_server_status_code = -1;
    if( session.isMember("gt_server_status_code") ){
        gt_server_status_code = session["gt_server_status_code"].asInt();
    } else {
        return "session do not has gt_server_status_code";
    }
    //从session中获取challenge
    // gtSdk.getChallengeSession(request);
    string challengeId = "";
    if( session.isMember("challengeId") ){
        challengeId = session["challengeId"].asString();
    } else {
        return "session do not has challengeId";
    }
    gtSdk.setChallengeId(challengeId);

    string gtResult = "fail";

    if (gt_server_status_code == 1) {
        //gt-server正常，向gt-server进行二次验证
        gtResult = gtSdk.enhencedValidateRequest(challenge, validate, seccode);
    } else {
        // gt-server非正常情况下，进行failback模式验证
        gtResult = "fail";
        gtResult = gtSdk.failbackValidateRequest(challenge, validate, seccode);
    }

    string response;

    if (gtResult == geetest::success_res) {
        // 验证成功
        response = geetest::success_res + ":" + gtSdk.getVersionInfo();
    } else if (gtResult == geetest::forbidden_res) {
        // 验证被判为机器人
        response = geetest::forbidden_res + ":" + gtSdk.getVersionInfo();
    } else {
        // 验证失败
        response = geetest::fail_res + ":" + gtSdk.getVersionInfo();
    }
    return response;
}

/**
 * 使用post方式，返回验证结果, request表单中必须包含challenge, validate, seccode
 */
int main ()
{
    // 获取CGI传递参数
    cgicc::Cgicc formData;
    string challenge = query_param(formData, geetest::fn_geetest_challenge);
    string validate = query_param(formData, geetest::fn_geetest_validate);
    string seccode = query_param(formData, geetest::fn_geetest_seccode);
    // 验证
    string response = do_validate(challenge, validate, seccode);

    cout << "Content-type:text/text\r\n\r\n";
    cout << response;
   
    return 0;
}





