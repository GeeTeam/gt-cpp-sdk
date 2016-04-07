#include <vector>
#include <map>
#include <cmath>
#include "httpclient.h"
#include "md5.h"
#include "util.h"
#include "geetestlib.h"
using namespace geetest;



/**
 * 预处理失败后的返回格式串
 * 
 * @return
 */
string GeetestLib::getFailPreProcessRes() {
    int rnd1 = getRandomNum();
    int rnd2 = getRandomNum();
    string md5Str1 = md5Encode( convert<string>(rnd1) );
    string md5Str2 = md5Encode( convert<string>(rnd2) );
    string challenge = md5Str1 + md5Str2.substr(0, 2);
    setChallengeId(challenge);

    string ret = "{\"success\":0,\"gt\":\"" + this->captchaId + "\",\"challenge\":\"" + this->challengeId + "\"}";
    return ret;
}

/**
 * 预处理成功后的标准串
 * 
 * @return
 */
string GeetestLib::getSuccessPreProcessRes() {
    string ret = "{\"success\":1,\"gt\":\"" + this->captchaId + "\",\"challenge\":\"" + this->challengeId + "\"}";
    return ret;
}


/**
 * 验证初始化预处理
 *
 * @return
 */
int GeetestLib::preProcess() {
    if (registerChallenge() != 1) {
        return 0;
    }
    return 1;
}

int GeetestLib::getRandomNum() {
    int rand_num = random_num(100);
    return rand_num;
}

/**
 * 用captchaID进行注册，更新challenge
 * 
 * @return 1表示注册成功，0表示注册失败
 */
int GeetestLib::registerChallenge() {
    try {
        string GET_URL = api_url + "/register.php?gt=" + this->captchaId;
        string result_str = readContentFromGet(GET_URL);
        if (32 == result_str.length()) {
            this->challengeId = this->md5Encode(result_str + this->privateKey);
            return 1;
        } else {
            return 0;
        }
    } catch (...) {}
    return 0;
}


/**
 * 发送请求，获取服务器返回结果
 * 
 * @param getURL
 * @return 服务器返回结果
 * @throws IOException
 */
string GeetestLib::readContentFromGet(string getURL) {
    CHttpClient httpClient;
    string strResponse;
    int ret = httpClient.Get(getURL, strResponse);
    if(ret != 0)
        strResponse = "";
    return strResponse;
}


/**
 * 判断一个表单对象值是否为空
 * 
 * @param gtObj
 * @return
 */
bool GeetestLib::objIsEmpty(string gtObj) {
    if ( trim(gtObj).length() == 0 ) {
        return true;
    }
    return false;
}


/**
 * 检查客户端的请求是否合法,三个只要有一个为空，则判断不合法
 * 
 * @param request
 * @return
 */
bool GeetestLib::resquestIsLegal(string challenge, string validate, string seccode) {
    if (objIsEmpty(challenge)) {
        return false;
    }
    if (objIsEmpty(validate)) {
        return false;
    }
    if (objIsEmpty(seccode)) {
        return false;
    }
    return true;
}
    
    
/**
 * 服务正常的情况下使用的验证方式,向gt-server进行二次验证,获取验证结果
 * 
 * @param challenge
 * @param validate
 * @param seccode
 * @return
 */
string GeetestLib::enhencedValidateRequest(string challenge, string validate, string seccode) {
    if ( !resquestIsLegal(challenge, validate, seccode) ) {
        return geetest::fail_res;
    }

    string host = geetest::baseUrl;
    string path = "/validate.php";
    int port = 80;

    string query = "seccode=" + seccode + "&sdk=" + (geetest::sdkLang + "_" + geetest::verName);

    string response = "";

    try 
    {
        if (validate.length() <= 0) {
            return geetest::fail_res;
        }

        if ( !checkResultByPrivate(challenge, validate) ) {
            return geetest::fail_res;
        }

        response = postValidate(host, path, query, port);
    } catch (...) {}

    if ( response == md5Encode(seccode) ) {
        return geetest::success_res;
    } else {
        return response;
    }
}

/**
 * failback使用的验证方式
 * 
 * @param challenge
 * @param validate
 * @param seccode
 * @return
 */
string GeetestLib::failbackValidateRequest(string challenge, string validate, string seccode) {
    if ( !resquestIsLegal(challenge, validate, seccode) ) {
        return geetest::fail_res;
    }

    vector<string> validateStr;
    split(validate, "_", validateStr);
    string encodeAns = validateStr[0];
    string encodeFullBgImgIndex = validateStr[1];
    string encodeImgGrpIndex = validateStr[2];

    int decodeAns = decodeResponse(this->getChallengeId(), encodeAns);
    int decodeFullBgImgIndex = decodeResponse(this->getChallengeId(),
            encodeFullBgImgIndex);
    int decodeImgGrpIndex = decodeResponse(this->getChallengeId(),
            encodeImgGrpIndex);

    string validateResult = validateFailImage(decodeAns,
            decodeFullBgImgIndex, decodeImgGrpIndex);

    if ( validateResult != geetest::fail_res ) {
        // 使用一随机标识来丢弃掉此次验证，防止重放
        int rnd1 = getRandomNum();
        string md5Str1 = md5Encode( convert<string>(rnd1) );
        this->setChallengeId(md5Str1);
    }

    return validateResult;
}
    


/**
 *
 * @param ans
 * @param full_bg_index
 * @param img_grp_index
 * @return
 */
string GeetestLib::validateFailImage(int ans, int full_bg_index, int img_grp_index) {
    const int thread = 3;// 容差值

    string full_bg_name = md5Encode( convert<string>(full_bg_index) ).substr(0, 9);
    string bg_name = md5Encode( convert<string>(img_grp_index) ).substr(10, 9);

    string answer_decode = "";

    // 通过两个字符串奇数和偶数位拼接产生答案位
    for (int i = 0; i < 9; i++) {
        if (i % 2 == 0) {
            answer_decode += full_bg_name.substr(i, 1);
        } 
        else if (i % 2 == 1) {
            answer_decode += bg_name.substr(i, 1);
        } 
        else {
            //exception
        }
    }

    string x_decode = answer_decode.substr(4);

    int x_int = hex2int(x_decode);

    int result = x_int % 200;
    if (result < 40) {
        result = 40;
    }

    if (abs(ans - result) <= thread) {
        return geetest::success_res;
    } else {
        return geetest::fail_res;
    }
}
    

/**
 * 解码随机参数
 * 
 * @param encodeStr
 * @param challenge
 * @return
 */
int GeetestLib::decodeResponse(string challenge, string str) {
    if (str.length() > 100) {
        return 0;
    }

    int shuzi[] = { 1, 2, 5, 10, 50 };
    string chongfu = "";
    map<string, int> key;
    int count = 0;

    for (unsigned i = 0; i < challenge.length(); i++) {
        string item = challenge.substr(i,1);

        if (chongfu.find(item) != string::npos) {
            continue;
        } else {
            int value = shuzi[count % 5];
            chongfu += item;
            count++;
            key[item] = value;
        }
    }

    int res = 0;

    for (unsigned j = 0; j < str.length(); j++) {
        res += key[str.substr(j,1)];
    }

    res = res - decodeRandBase(challenge);

    return res;
}


/**
 * 输入的两位的随机数字,解码出偏移量
 * 
 * @param randStr
 * @return
 */
int GeetestLib::decodeRandBase(string challenge) {

    string base = challenge.substr(32, 2);
    vector<int> tempArray;

    for (unsigned i = 0; i < base.length(); i++) {
        char tempChar = base[i];
        int tempAscii = (int) (tempChar);

        int result = (tempAscii > 57) ? (tempAscii - 87)
                : (tempAscii - 48);

        tempArray.push_back(result);
    }

    int decodeRes = tempArray[0] * 36 + tempArray[1];
    return decodeRes;
}



bool GeetestLib::checkResultByPrivate(string challenge, string validate) {
    string encodeStr = md5Encode(this->privateKey + "geetest" + challenge);
    return validate == encodeStr;
}



/**
 * 貌似不是Post方式，后面重构时修改名字
 * 
 * @param host
 * @param path
 * @param data
 * @param port
 * @return
 * @throws Exception
 */
string GeetestLib::postValidate(string host, string path, string data, int port) {
    string queryUrl = "http://" + host + path;
    CHttpClient httpClient;
    string strResponse;
    int ret = httpClient.Post(queryUrl, port, data, strResponse);
    if(ret != 0)
        strResponse = "";
    return strResponse;
}


/**
 * md5 加密
 * 
 * @param plainText
 * @return
 */
string GeetestLib::md5Encode(const string &plainText){
    CMD5 iMD5;  
    iMD5.GenerateMD5( (unsigned char*)plainText.c_str(), plainText.size() );  
    string key = iMD5.ToString(); 
    return key;
}
