#ifndef __HTTP_GEETEST_H__  
#define __HTTP_GEETEST_H__  

#include<string>

namespace geetest{
    /**
     * SDK版本编号
     */
    // const int verCode = 8;
    /**
     * SDK版本名称
     */
    static const string verName = "2.0.0";
    static const string sdkLang = "c++";// SDK的语言类型

    static const string gt_challenge_session_key = "geetest_challenge";// challenge的key值(单实例)
    static const string gt_server_status_session_key = "gt_server_status";// 极验服务器状态key值（单实例）

    static const string baseUrl = "api.geetest.com";
    static const string api_url = "http://" + baseUrl;
    static const string https_api_url = "https://" + baseUrl;// 一些页面是https
    static const int com_port = 80;// 通讯端口号

    static const int defaultIsMobile = 0;
    // private static const int defaultMobileWidth = 260;// the default width of the
    // mobile id

    // 一些常量
    static const string success_res = "success";
    static const string fail_res = "fail";
    static const string forbidden_res = "forbidden";

    // 前端验证的表单值--属于接口，不允许修改
    static const string fn_geetest_challenge = "geetest_challenge";
    static const string fn_geetest_validate = "geetest_validate";
    static const string fn_geetest_seccode = "geetest_seccode";


    class GeetestLib
    {
    public:
        GeetestLib(string captchaId, string privateKey):
        debugCode(true), validateLogPath(""),
        captchaId(captchaId), privateKey(privateKey),
        challengeId(""), productType("embed"),
        isHttps(false), submitBtnId("submit-button"),
        isMobile(defaultIsMobile)
        {}

    public:
        bool debugCode;// 调试开关，是否输出调试日志
        string validateLogPath;// 服务器端保存日志的目录//var/log/，请确保有可读写权限
    
    private:
        /**
         * 公钥
         */
        string captchaId;

        /**
         * 私钥
         */
        string privateKey;

        /**
         * the challenge
         */
        string challengeId;

        /**
         * he captcha product type,default is 'embed'
         */
        string productType;

        /**
         * is secure,default is false
         */
        bool isHttps;
        /**
         * when the productType is popup,it needs to set the submitbutton, default is "submit-button"
         */
        string submitBtnId;
        /**
         * 是否是移动端的 defaultIsMobile
         */
        int isMobile;// 1--true,0-false

    public:
        inline bool getIsHttps() {
            return isHttps;
        }
        inline void setIsHttps(bool isHttps) {
            this->isHttps = isHttps;
        }

        inline string getSubmitBtnId() {
            return submitBtnId;
        }
        inline void setSubmitBtnId(string submitBtnId) {
            this->submitBtnId = submitBtnId;
        }


        inline bool getDebugCode() {
            return debugCode;
        }
        inline void setDebugCode(bool debugCode) {
            this->debugCode = debugCode;
        }
        
        inline string getChallengeId() {
            return challengeId;
        }
        inline void setChallengeId(string challengeId) {
            this->challengeId = challengeId;
        }
        

        inline string getProductType() {
            return productType;
        }
        inline void setProductType(string productType) {
            this->productType = productType;
        }

        inline int getIsMobile() {
            return isMobile;
        }
        inline void setIsMobile(int isMobile) {
            this->isMobile = isMobile;
        }

        inline string getPrivateKey() {
            return privateKey;
        }
        inline void setPrivateKey(string privateKey) {
            this->privateKey = privateKey;
        }

        inline string getVerName() {
            return verName;
        }

        inline string getCaptchaId() {
            return captchaId;
        }
        inline void setCaptchaId(string captchaId) {
            this->captchaId = captchaId;
        }

        /**
         * 获取版本编号
         */
        inline string getVersionInfo() {
            return verName;
        }

        /**
         * 设置服务器端保存日志的目录
         */
        inline void setValidateLogPath(string validateLogPath) {
            this->validateLogPath = validateLogPath;
        }
        
        /**
         * 获取服务器端保存日志的目录
         */
        inline string getValidateLogPath() {
            return validateLogPath;
        }


        /////////functions///////////
        /**
         * 预处理失败后的返回格式串
         * 
         * @return
         */
        string getFailPreProcessRes() ;

        /**
         * 预处理成功后的标准串
         * 
         * @return
         */
        string getSuccessPreProcessRes() ;

        /**
         * 验证初始化预处理
         * @return
         */
        int preProcess();

        int getRandomNum();

        /**
         * 用captchaID进行注册，更新challenge
         * @return 1表示注册成功，0表示注册失败
         */
        int registerChallenge();

        /**
         * 发送请求，获取服务器返回结果
         * @param getURL
         * @return 服务器返回结果
         * @throws IOException
         */
        string readContentFromGet(string getURL);

        /**
         * 判断一个表单对象值是否为空
         * @param gtObj
         * @return
         */
        bool objIsEmpty(string gtObj);

        /**
         * 检查客户端的请求是否合法,三个只要有一个为空，则判断不合法
         * @param request
         * @return
         */
        bool resquestIsLegal(string challenge, string validate, string seccode);
            
        /**
         * 服务正常的情况下使用的验证方式,向gt-server进行二次验证,获取验证结果
         * @param challenge
         * @param validate
         * @param seccode
         * @return
         */
        string enhencedValidateRequest(string challenge, string validate, string seccode);

        /**
         * failback使用的验证方式
         * @param challenge
         * @param validate
         * @param seccode
         * @return
         */
        string failbackValidateRequest(string challenge, string validate, string seccode);
            
        /**
         * @param ans
         * @param full_bg_index
         * @param img_grp_index
         * @return
         */
        string validateFailImage(int ans, int full_bg_index, int img_grp_index);
        
        /**
         * 解码随机参数
         * @param encodeStr
         * @param challenge
         * @return
         */
        int decodeResponse(string challenge, string str) ;
        
        /**
         * 输入的两位的随机数字,解码出偏移量
         * @param randStr
         * @return
         */
        int decodeRandBase(string challenge) ;

        bool checkResultByPrivate(string challenge, string validate) ;
        
        /**
         * 貌似不是Post方式，后面重构时修改名字
         * @param host
         * @param path
         * @param data
         * @param port
         * @return
         * @throws Exception
         */
        string postValidate(string host, string path, string data, int port) ;
        
        /**
         * md5 加密
         * @param plainText
         * @return
         */
        string md5Encode(const string &plainText);


    };

}

#endif  