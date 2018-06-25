#ifndef _ST_AES_H
#define _ST_AES_H

////////////////////////////////////////////////////////////////////////////
//	如果需要AES加密、解密在不同平台如PHP，JAVA等互通，必须要保证三个地方参数
//	统一，加密算法：均为AES；加密模式：CBC，CFB，ECB，OFB，PCBC其中之一；
//	填充模式；NoPadding，PKCS5Padding，ISO10126Padding其中之一.
//	注意：CBC，ECB，PCBC加密模式是不支持NoPadding填充模式的
//	
//	填充模式：
//	AES加密数据以16字节为单位，如果数据不满16字节则根据选择的模式进行填充。
//	NoPadding：不进行填充，保持数据原始长度
//	PKCS5Padding：使用16-数据长度的数值填充剩余的字节
//	ISO10126Padding：使用随机数据填充剩余字节
//	
//	算法/模式/填充                16字节加密后数据长度        不满16字节加密后长度
//	AES/CBC/NoPadding             16                          不支持
//	AES/CBC/PKCS5Padding          32                          16
//	AES/CBC/ISO10126Padding       32                          16
//	AES/CFB/NoPadding             16                          原始数据长度
//	AES/CFB/PKCS5Padding          32                          16
//	AES/CFB/ISO10126Padding       32                          16
//	AES/ECB/NoPadding             16                          不支持
//	AES/ECB/PKCS5Padding          32                          16
//	AES/ECB/ISO10126Padding       32                          16
//	AES/OFB/NoPadding             16                          原始数据长度
//	AES/OFB/PKCS5Padding          32                          16
//	AES/OFB/ISO10126Padding       32                          16
//	AES/PCBC/NoPadding            16                          不支持
//	AES/PCBC/PKCS5Padding         32                          16
//	AES/PCBC/ISO10126Padding      32                          16
///////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

enum KEYSIZE{KS_128,KS_192,KS_256}; /* 密钥长度 */
enum PADDINGMODE{NOPADDING,PKCS5PADDING,ISO12106PADDING};	/*填充模式*/

class CAES
{
public:
	/* 缺省构造函数 */
	CAES();

	/* 构造函数,初始化设置密钥长度,密钥及其长度 */
	CAES(KEYSIZE keySize,unsigned char* key,int iSize);

	/* 析构函数 */
	~CAES();

public:
	/* 设置参数 */
	void SetParams(KEYSIZE keySize,unsigned char* key,int isize);

	/* AES加密 */
	void Encrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* AES解密 */
	void Decrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* AES加密(带随机数) */
	void Encrypt2(void* in,unsigned long nsize,void* out,unsigned long& outSize,void* key,int iSize,KEYSIZE keySize,int RandNum = 0);

	/* AES解密 */
	void Decrypt2(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* 清空所有资源 */
	void Clear();

protected:
	/* 初始化密钥 */
	void InitializeKey(unsigned char* key,int keysize);

	/* 初始化轮数 */
	void InitializeRcon();

	/* 计算密钥调度表 */
	void KeyExpansion(void *key);

	/* 轮密钥加 */
	void AddRoundKey(const int round);

	/* 字节替换 */
	void SubBytes(unsigned char bytes[]);

	/* 循环左移 */
	unsigned char* RowBytes(unsigned char bytes[]);
	
	/* 行位移变换 */
	void ShiftRows(void);

	/* 列混合变换 */
	void MixColumns(void);
	
	/* 伽罗瓦域运算 */
	unsigned char gfmultby01(const unsigned char b);
	unsigned char gfmultby02(const unsigned char b);
	unsigned char gfmultby03(const unsigned char b);
	unsigned char gfmultby09(const unsigned char b);
	unsigned char gfmultby0b(const unsigned char b);
	unsigned char gfmultby0d(const unsigned char b);
	unsigned char gfmultby0e(const unsigned char b);

	/* 逆字节替换 */
	void InvSubBytes(unsigned char bytes[]);

	/* 逆行位移变换 */
	void InvShiftRows(void);

	/* 逆列混合变换 */
	void InvMixColumns(void);

	/* 加密 */
	void Cipher(void *in,void *out);

	/* 解密 */
	void InvCipher(void *in,void *out);

	/* 移位 */
	int URS(int a,int b,bool c = true);

	/* 生成随机数 */
	int RangedRandDemo(int range_min, int range_max);

	/* 检查字符串是否为合法16进制串 */
	bool IsNotValidHexStr(const char* pucCharStr);

protected:
	/* 幂函数 */
	inline int pow(int base, int exp) ;

	/* 向下舍入 */
	inline int asmifloor(float f) ;

	/* 向上舍入 */
	inline int asmiceil(float f) ;

public:
	/* 16进制字串----->字符串 */
	static void HexStr2CharStr(char const* pszHexStr, int iSize,unsigned char* pucCharStr);

	/* 字符串----->十六进制字串 */
	static void CharStr2HexStr(unsigned char* pucCharStr, int iSize,char* pszHexStr);

	/* 16进制----->字符 */
	static void Hex2Char(char const* szHex, unsigned char& rch);

	/* 字符----->16进制 */
	static void Char2Hex(unsigned char ch, char* szHex);

	/* 加密 */
	static char* AESEncrypt(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128,int RandNum = 0);

	/* 解密 */
	static char* AESDecrypt(char* cipherText,char** PlainTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);

	/*加密之后输出Base64结果*/
	static char* AESEncryptOutputBase64Code(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);

	static wchar_t* AESDecryptOutputBase64Code(void* pCipherTxt,unsigned long bufLen,wchar_t * plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);
	static char* AESDecryptOutputBase64Code(void* pCipherTxt,unsigned long bufLen,char* plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);
private:
	int m_Nr;				//轮数
	int m_Nb;				//分组长度
	int m_Nk;				//密钥长度
	int m_Mode;				//填充模式
	
	unsigned char* m_w;			//密钥调度表,每次取出Nb个密字，共需要取Nr+1轮,轮数=Nb+6
	unsigned char* m_State;		//加密数据块,每次取行,Nb列
	unsigned char* m_key;			//密钥
	unsigned char* m_rcon;		//轮常数表
};

/*******************************************************
* 函数名: pow
* 函数功能: 幂函数
* 参数个数: 2
**** @Param[IN]: base ---- 基数
**** @Param[OUT]: exp ---- 幂指数
* 返回值: 返回幂值
*******************************************************/
inline int CAES::pow( int base, int exp )
{
	int r=1;

	if ( exp>0 )
	{
		for (;;)
		{
			if ( exp & 1 ) 
				r *= base;

			exp >>= 1;

			if (exp==0) break;

			base *= base;
		}
	}

	return r;
}

/*******************************************************
* 函数名: asmifloor
* 函数功能: 向下舍入
* 参数个数: 1
**** @Param[IN]: f ---- 浮点数
* 返回值: 该浮点数的向下舍入整数
*******************************************************/
inline int CAES::asmifloor( float f )
{
	static float Half = 0.5;
	int i; 

	__asm fld [f]				// 取浮点数,压入栈顶
	__asm fsub [Half]		// 浮点数减法
	__asm fistp [i]			// 将浮点数以整数形式放入操作数中

	return i;
}

/*******************************************************
* 函数名: asmiceil
* 函数功能: 向上舍入
* 参数个数: 1
**** @Param[IN]: f ---- 浮点数
* 返回值: 该浮点数的向上舍入整数
*******************************************************/
inline int CAES::asmiceil( float f )
{
	static float Half = 0.5;
	int i; 

	__asm fld [f]
	__asm fadd [Half]
	__asm fistp [i]

	return i;
}

#endif // _ST_AES_H