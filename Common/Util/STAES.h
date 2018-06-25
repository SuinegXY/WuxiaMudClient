#ifndef _ST_AES_H
#define _ST_AES_H

////////////////////////////////////////////////////////////////////////////
//	�����ҪAES���ܡ������ڲ�ͬƽ̨��PHP��JAVA�Ȼ�ͨ������Ҫ��֤�����ط�����
//	ͳһ�������㷨����ΪAES������ģʽ��CBC��CFB��ECB��OFB��PCBC����֮һ��
//	���ģʽ��NoPadding��PKCS5Padding��ISO10126Padding����֮һ.
//	ע�⣺CBC��ECB��PCBC����ģʽ�ǲ�֧��NoPadding���ģʽ��
//	
//	���ģʽ��
//	AES����������16�ֽ�Ϊ��λ��������ݲ���16�ֽ������ѡ���ģʽ������䡣
//	NoPadding����������䣬��������ԭʼ����
//	PKCS5Padding��ʹ��16-���ݳ��ȵ���ֵ���ʣ����ֽ�
//	ISO10126Padding��ʹ������������ʣ���ֽ�
//	
//	�㷨/ģʽ/���                16�ֽڼ��ܺ����ݳ���        ����16�ֽڼ��ܺ󳤶�
//	AES/CBC/NoPadding             16                          ��֧��
//	AES/CBC/PKCS5Padding          32                          16
//	AES/CBC/ISO10126Padding       32                          16
//	AES/CFB/NoPadding             16                          ԭʼ���ݳ���
//	AES/CFB/PKCS5Padding          32                          16
//	AES/CFB/ISO10126Padding       32                          16
//	AES/ECB/NoPadding             16                          ��֧��
//	AES/ECB/PKCS5Padding          32                          16
//	AES/ECB/ISO10126Padding       32                          16
//	AES/OFB/NoPadding             16                          ԭʼ���ݳ���
//	AES/OFB/PKCS5Padding          32                          16
//	AES/OFB/ISO10126Padding       32                          16
//	AES/PCBC/NoPadding            16                          ��֧��
//	AES/PCBC/PKCS5Padding         32                          16
//	AES/PCBC/ISO10126Padding      32                          16
///////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

enum KEYSIZE{KS_128,KS_192,KS_256}; /* ��Կ���� */
enum PADDINGMODE{NOPADDING,PKCS5PADDING,ISO12106PADDING};	/*���ģʽ*/

class CAES
{
public:
	/* ȱʡ���캯�� */
	CAES();

	/* ���캯��,��ʼ��������Կ����,��Կ���䳤�� */
	CAES(KEYSIZE keySize,unsigned char* key,int iSize);

	/* �������� */
	~CAES();

public:
	/* ���ò��� */
	void SetParams(KEYSIZE keySize,unsigned char* key,int isize);

	/* AES���� */
	void Encrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* AES���� */
	void Decrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* AES����(�������) */
	void Encrypt2(void* in,unsigned long nsize,void* out,unsigned long& outSize,void* key,int iSize,KEYSIZE keySize,int RandNum = 0);

	/* AES���� */
	void Decrypt2(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize);

	/* ���������Դ */
	void Clear();

protected:
	/* ��ʼ����Կ */
	void InitializeKey(unsigned char* key,int keysize);

	/* ��ʼ������ */
	void InitializeRcon();

	/* ������Կ���ȱ� */
	void KeyExpansion(void *key);

	/* ����Կ�� */
	void AddRoundKey(const int round);

	/* �ֽ��滻 */
	void SubBytes(unsigned char bytes[]);

	/* ѭ������ */
	unsigned char* RowBytes(unsigned char bytes[]);
	
	/* ��λ�Ʊ任 */
	void ShiftRows(void);

	/* �л�ϱ任 */
	void MixColumns(void);
	
	/* ٤���������� */
	unsigned char gfmultby01(const unsigned char b);
	unsigned char gfmultby02(const unsigned char b);
	unsigned char gfmultby03(const unsigned char b);
	unsigned char gfmultby09(const unsigned char b);
	unsigned char gfmultby0b(const unsigned char b);
	unsigned char gfmultby0d(const unsigned char b);
	unsigned char gfmultby0e(const unsigned char b);

	/* ���ֽ��滻 */
	void InvSubBytes(unsigned char bytes[]);

	/* ����λ�Ʊ任 */
	void InvShiftRows(void);

	/* ���л�ϱ任 */
	void InvMixColumns(void);

	/* ���� */
	void Cipher(void *in,void *out);

	/* ���� */
	void InvCipher(void *in,void *out);

	/* ��λ */
	int URS(int a,int b,bool c = true);

	/* ��������� */
	int RangedRandDemo(int range_min, int range_max);

	/* ����ַ����Ƿ�Ϊ�Ϸ�16���ƴ� */
	bool IsNotValidHexStr(const char* pucCharStr);

protected:
	/* �ݺ��� */
	inline int pow(int base, int exp) ;

	/* �������� */
	inline int asmifloor(float f) ;

	/* �������� */
	inline int asmiceil(float f) ;

public:
	/* 16�����ִ�----->�ַ��� */
	static void HexStr2CharStr(char const* pszHexStr, int iSize,unsigned char* pucCharStr);

	/* �ַ���----->ʮ�������ִ� */
	static void CharStr2HexStr(unsigned char* pucCharStr, int iSize,char* pszHexStr);

	/* 16����----->�ַ� */
	static void Hex2Char(char const* szHex, unsigned char& rch);

	/* �ַ�----->16���� */
	static void Char2Hex(unsigned char ch, char* szHex);

	/* ���� */
	static char* AESEncrypt(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128,int RandNum = 0);

	/* ���� */
	static char* AESDecrypt(char* cipherText,char** PlainTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);

	/*����֮�����Base64���*/
	static char* AESEncryptOutputBase64Code(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);

	static wchar_t* AESDecryptOutputBase64Code(void* pCipherTxt,unsigned long bufLen,wchar_t * plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);
	static char* AESDecryptOutputBase64Code(void* pCipherTxt,unsigned long bufLen,char* plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize = KS_128);
private:
	int m_Nr;				//����
	int m_Nb;				//���鳤��
	int m_Nk;				//��Կ����
	int m_Mode;				//���ģʽ
	
	unsigned char* m_w;			//��Կ���ȱ�,ÿ��ȡ��Nb�����֣�����ҪȡNr+1��,����=Nb+6
	unsigned char* m_State;		//�������ݿ�,ÿ��ȡ��,Nb��
	unsigned char* m_key;			//��Կ
	unsigned char* m_rcon;		//�ֳ�����
};

/*******************************************************
* ������: pow
* ��������: �ݺ���
* ��������: 2
**** @Param[IN]: base ---- ����
**** @Param[OUT]: exp ---- ��ָ��
* ����ֵ: ������ֵ
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
* ������: asmifloor
* ��������: ��������
* ��������: 1
**** @Param[IN]: f ---- ������
* ����ֵ: �ø�������������������
*******************************************************/
inline int CAES::asmifloor( float f )
{
	static float Half = 0.5;
	int i; 

	__asm fld [f]				// ȡ������,ѹ��ջ��
	__asm fsub [Half]		// ����������
	__asm fistp [i]			// ����������������ʽ�����������

	return i;
}

/*******************************************************
* ������: asmiceil
* ��������: ��������
* ��������: 1
**** @Param[IN]: f ---- ������
* ����ֵ: �ø�������������������
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