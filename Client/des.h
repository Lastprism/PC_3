#ifndef _DES_H_
#define _DES_H_

#include <string>

#define BITS_PER_CHAR 8
#define KEY_SZIE 8
#define SUBKEY_NUM 16
#define SUBKEY_LENGHT 48
#define EXPAND_SIZE 48
#define PC_2_SIZE 48
#define PC_1_SIZE 56
#define BIT_STR_SIZE 64

class DES
{
public:
    DES();
    ~DES();

    std::string Encrypt(const std::string& plain, const std::string& key);
    std::string Decrypt(const std::string& cipher, const std::string& key);

private:
    //生成16个子秘钥
    bool CreateSubKey(const std::string& key, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);

    //加密8字节数据块
    bool EncryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);
    //解密8字节数据块
    bool DecryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);


private:
    //----------------------------------转换工具-----------------------------------------------
    bool XOR(std::string& strFirst, std::string strSecond, size_t num);
    bool PC1_Transform(const std::string& bitStr, std::string& PC1BitStr);

    bool PC2_Transform(const std::string& PC1BitStr, char subKey[SUBKEY_LENGHT]);

    bool IP_Transform(std::string& bitStr);

    bool Expand_Transform(const std::string& halfBitStr, std::string& eBitStr);

    bool SBox_Transform(const std::string& eBitStr, std::string& halfBitStr);

    bool Permute_Transform(std::string& halfBitStr);

    bool IP_1_Transform(std::string& bitStr);

private:
    //------------------------------------基础工具------------------------------------------------
    bool Char8ToBit64(const std::string& str, std::string& bitStr);
    bool Bit64ToChar8(const std::string& bitStr, std::string& str);



    bool LeftCycle(std::string& str, size_t beginSection, size_t endSection, size_t step);

private:
    //------------------------------------生成秘钥需要的表----------------------------------------
    //置换选择表1
    //压缩换位去掉每个字节的第8位，用作奇偶校检，基本上第8位可忽略
    static char PC1_Table[PC_1_SIZE];
    //左循环距离表
    static char Move_Table[SUBKEY_NUM];
    //置换选择表2
    //选择其中的某些位将其减少到48位
    static char PC2_Table[PC_2_SIZE];


    //----------------------------------加密或解密需要的表--------------------------------------------
    //初始置换表
    //表中的数值表示输入为被置换后的新位置
    static char IP_Table[BIT_STR_SIZE];
    //扩展表
    //通过重复某些位将32位的右半部分按照表扩展成48位
    static char Expand_Table[EXPAND_SIZE];
    //单纯置换表
    static char Permute_Table[BIT_STR_SIZE / 2];
    //反置换表
    static char IP_1_Table[BIT_STR_SIZE];
    //置换盒
    static char SBox_Table[KEY_SZIE][4][16];
};

#endif
