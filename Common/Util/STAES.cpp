#include "STAES.h"
#include "Base64Encoder.h"
/***********************************************************************
* 全局变量申明
***********************************************************************/
//S盒
static unsigned char SBox[16][16] = 
{
				/* 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
	/*0*/  {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
	/*1*/  {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
	/*2*/  {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
	/*3*/  {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
	/*4*/  {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
	/*5*/  {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
	/*6*/  {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
	/*7*/  {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
	/*8*/  {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
	/*9*/  {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
	/*a*/  {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
	/*b*/  {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
	/*c*/  {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
	/*d*/  {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
	/*e*/  {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
	/*f*/  {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

//逆S盒
static unsigned char InvSBox[16][16] = 
{
				/* 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
	/*0*/  {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
	/*1*/  {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
	/*2*/  {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
	/*3*/  {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
	/*4*/  {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
	/*5*/  {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
	/*6*/  {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
	/*7*/  {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
	/*8*/  {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
	/*9*/  {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
	/*a*/  {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
	/*b*/  {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
	/*c*/  {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
	/*d*/  {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
	/*e*/  {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
	/*f*/  {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d} 
};

/* 16字节对齐位 */
const int ALIGNBIT = 16;

/* 随机数 + 结束符 */
const int PASSBIT = 9;

/*******************************************************
* 函数名: CAES
* 函数功能: 构造函数
* 参数个数: 无
* 返回值: 无
*******************************************************/
CAES::CAES()
{
	m_Nb = 4;
	m_Nk = 4;
	m_Nr = 10;

	m_Mode = PKCS5PADDING;

	m_w = NULL;
	m_State = NULL;
	m_key = NULL;
	m_rcon = NULL;
}

/*******************************************************
* 函数名: CAES
* 函数功能: 构造函数
* 参数个数: 3
**** @Param[IN]: keySize ---- 密钥长度类型(KS_128,KS_192,KS_256)
**** @Param[IN]: key ---- 解密用的密钥
**** @Param[IN]: isize ---- 密钥的长度
* 返回值: 无
*******************************************************/
CAES::CAES(KEYSIZE keySize,unsigned char* key,int isize)
{
	m_w = NULL;
	m_State = NULL;
	m_key = NULL;
	m_rcon = NULL;

	m_Mode = PKCS5PADDING;

	SetParams(keySize,key,keySize);
}

/*******************************************************
* 函数名: ~CAES
* 函数功能: 析构函数
* 参数个数: 无
* 返回值: 无
*******************************************************/
CAES::~CAES()
{
	Clear();
}

/*******************************************************
* 函数名: SetParams
* 函数功能: 根据密钥及长度设置参数
* 参数个数: 3
**** @Param[IN]: keySize ---- 密钥长度类型(KS_128,KS_192,KS_256)
**** @Param[IN]: key ---- 解密用的密钥
**** @Param[IN]: isize ---- 密钥的长度
* 返回值: 无
*******************************************************/
void CAES::SetParams(KEYSIZE keySize,unsigned char* key,int isize)
{
	//此处加密块固定设置为16字节块
	m_Nb = 4;
	switch (keySize)
	{
	case KS_128:
		{
			m_Nk = 4;
			m_Nr = 10;
		}break;
	case KS_192:
		{
			m_Nk = 6;
			m_Nk = 12;
		}break;
	case KS_256:
		{
			m_Nk = 8;
			m_Nr = 12;
		}break;
	}

	//初始化密钥
	InitializeKey(key,isize);

	//初始化轮数
	InitializeRcon();

	//计算密钥调度表
	KeyExpansion(m_key);

}

/*******************************************************
* 函数名: InitializeKey
* 函数功能: 初始化密钥
* 参数个数: 2
**** @Param[IN]: key ---- 解密用的密钥
**** @Param[IN]: isize ---- 密钥的长度
* 返回值: 无
*******************************************************/
void CAES::InitializeKey(unsigned char* key,int isize)
{
	int len = 4 * m_Nk;

	if(m_key)
		delete[] m_key;

	//密钥长度为128,192,256
	m_key = new unsigned char[len];
	memset(m_key,0,len);

	memcpy(m_key,key,(isize<len ? isize:len));
}

/*******************************************************
* 函数名: InitializeRcon
* 函数功能: 初始化轮数
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::InitializeRcon()
{
	int len = (m_Nr+1) * 4;

	if(m_rcon)
		delete[] m_rcon;
	m_rcon = new unsigned char[len];
	memset(m_rcon,0,sizeof(unsigned char)*len);
	
	m_rcon[0] = 0;
	m_rcon[4] = 1;

	for(int i=2;i<=m_Nr;i++)
	{
		m_rcon[4*i] = gfmultby02(m_rcon[4*(i-1)]);
	}
}

/*******************************************************
* 函数名: Clear
* 函数功能: 释放所有资源
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::Clear()
{
	if(m_w)
		delete[] m_w;
	m_w = NULL;

	if(m_State)
		delete[] m_State;
	m_State = NULL;

	if(m_key)
		delete[] m_key;
	m_key = NULL;

	if(m_rcon)
		delete[] m_rcon;
	m_rcon = NULL;
}

/*******************************************************
* 函数名: KeyExpansion
* 函数功能: 计算密钥调度表
* 参数个数: 1
**** @Param[IN]: key ---- 密钥
* 返回值: 无
*******************************************************/
void CAES::KeyExpansion(void *key)
{
	int len = m_Nb * (m_Nr+1)*4;

	if(m_w)
		delete[] m_w;
	m_w = new unsigned char[len];  

	//前M_NK行保存原始密钥
	for (int row = 0; row < m_Nk; ++row)
	{
		m_w[4*row+0] = m_key[4*row];
		m_w[4*row+1] = m_key[4*row+1];
		m_w[4*row+2] = m_key[4*row+2];
		m_w[4*row+3] = m_key[4*row+3];
	}

	unsigned char* temp = new unsigned char[4];
	int keylen = m_Nb * (m_Nr + 1);

	//根据原始密钥计算轮密钥
	for (int row = m_Nk; row < keylen; ++row)
	{
		temp[0] = m_w[4*(row-1)+0]; 
		temp[1] = m_w[4*(row-1)+1];
		temp[2] = m_w[4*(row-1)+2]; 
		temp[3] = m_w[4*(row-1)+3];

		if (row % m_Nk == 0)  
		{
			SubBytes(RowBytes(temp));

			temp[0] = (unsigned char)( (int)temp[0] ^ (int)m_rcon[4*(row/m_Nk)+0] );
			temp[1] = (unsigned char)( (int)temp[1] ^ (int)m_rcon[4*(row/m_Nk)+1] );
			temp[2] = (unsigned char)( (int)temp[2] ^ (int)m_rcon[4*(row/m_Nk)+2] );
			temp[3] = (unsigned char)( (int)temp[3] ^ (int)m_rcon[4*(row/m_Nk)+3] );
		}
		else if ( m_Nk > 6 && (row % m_Nk == 4) )  
		{
			SubBytes(temp);
		}

		m_w[4*row+0] = (unsigned char) ( (int)m_w[4*(row-m_Nk)+0] ^ (int)temp[0] );
		m_w[4*row+1] = (unsigned char) ( (int)m_w[4*(row-m_Nk)+1] ^ (int)temp[1] );
		m_w[4*row+2] = (unsigned char) ( (int)m_w[4*(row-m_Nk)+2] ^ (int)temp[2] );
		m_w[4*row+3] = (unsigned char) ( (int)m_w[4*(row-m_Nk)+3] ^ (int)temp[3] );

	}

	delete[] temp;
}

/*******************************************************
* 函数名: AddRoundKey
* 函数功能: 轮密钥加,使用密钥调度表中的前四行对State状态矩阵实行一个字节一个字节的异或
				 操作,并用轮密钥表w[c,r] 异或输入State[r,c]
* 参数个数: 1
**** @Param[IN]: round ---- 轮数
* 返回值: 无
*******************************************************/
void CAES::AddRoundKey(const int round)
{
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < m_Nb; ++c)
		{
			m_State[r*m_Nb+c] = (unsigned char) ( (int)m_State[r*m_Nb+c] ^
				(int)m_w[(round*m_Nb+c)*4+r] );
		}
	}
}

/*******************************************************
* 函数名: SubBytes
* 函数功能: 字节替换,将 State 矩阵中的每个字节替换成一个由 Sbox 决定的新字节
* 参数个数: 1
**** @Param[IN]: bytes[] ---- 需要替换的字节(一个字大小)
* 返回值: 无
*******************************************************/
void CAES::SubBytes(unsigned char bytes[])
{
	bytes[0] = SBox[ bytes[0] >> 4][bytes[0] & 0x0f ];
	bytes[1] = SBox[ bytes[1] >> 4][bytes[1] & 0x0f ];
	bytes[2] = SBox[ bytes[2] >> 4][bytes[2] & 0x0f ];
	bytes[3] = SBox[ bytes[3] >> 4][bytes[3] & 0x0f ];
}

/*******************************************************
* 函数名: RowBytes
* 函数功能: 循环左移移位
* 参数个数: 1
**** @Param[IN]: bytes[] ---- 需要移位的字节(一个子大小)
* 返回值: 返回移位后的字
*******************************************************/
unsigned char* CAES::RowBytes(unsigned char bytes[])
{
	unsigned char temp = bytes[0];

	bytes[0] = bytes[1];
	bytes[1] = bytes[2];
	bytes[2] = bytes[3];
	bytes[3] = temp;

	return bytes;
}

/*******************************************************
* 函数名: ShiftRows
* 函数功能: 行位移变换,将 State 矩阵中的字节向左旋转
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::ShiftRows(void)
{
	unsigned char* temp = new unsigned char[4*m_Nb];

	for (int r = 0; r < 4; ++r)  
	{
		for (int c = 0; c < m_Nb; ++c)
		{
			temp[m_Nb*r + c] = m_State[m_Nb*r + c];
		}
	}

	for (int r = 1; r < 4; ++r) 
	{
		for (int c = 0; c < m_Nb; ++c)
		{
			m_State[m_Nb*r + c] = temp[ m_Nb*r + (c + r) % m_Nb ];
		}
	}

	delete[] temp;
}

/*******************************************************
* 函数名: MixColumns
* 函数功能: 列混合变换,用 State 字节列的值进行数学域加和域乘的结果代替每个字节
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::MixColumns(void)
{
	unsigned char* temp = new unsigned char[4*m_Nb];

	for (int r = 0; r < 4; ++r)  
	{
		for (int c = 0; c < 4; ++c)
		{
			temp[m_Nb*r + c] = m_State[m_Nb*r + c];
		}
	}

	for (int c = 0; c < 4; ++c)
	{
		m_State[m_Nb*0+c] = (unsigned char) ( (int)gfmultby02(temp[m_Nb*0+c]) ^
			(int)gfmultby03(temp[m_Nb*1+c]) ^
			(int)gfmultby01(temp[m_Nb*2+c]) ^
			(int)gfmultby01(temp[m_Nb*3+c]) );

		m_State[m_Nb*1+c] = (unsigned char) ( (int)gfmultby01(temp[m_Nb*0+c]) ^
			(int)gfmultby02(temp[m_Nb*1+c]) ^
			(int)gfmultby03(temp[m_Nb*2+c]) ^
			(int)gfmultby01(temp[m_Nb*3+c]) );

		m_State[m_Nb*2+c] = (unsigned char) ( (int)gfmultby01(temp[m_Nb*0+c]) ^
			(int)gfmultby01(temp[m_Nb*1+c]) ^
			(int)gfmultby02(temp[m_Nb*2+c]) ^
			(int)gfmultby03(temp[m_Nb*3+c]) );

		m_State[m_Nb*3+c] = (unsigned char) ( (int)gfmultby03(temp[m_Nb*0+c]) ^
			(int)gfmultby01(temp[m_Nb*1+c]) ^
			(int)gfmultby01(temp[m_Nb*2+c]) ^
			(int)gfmultby02(temp[m_Nb*3+c]) );
	}

	delete[] temp;
}

/*******************************************************
* 函数名: gfmultby(*)
* 函数功能: 伽罗瓦域运算(GF(28))
* 参数个数: 1
**** @Param[IN]: b ---- 被乘数
* 返回值: 返回运算后的结果
*******************************************************/
unsigned char CAES::gfmultby01(const unsigned char b)
{
	return b;
}

unsigned char CAES::gfmultby02(const unsigned char b)
{
	if (b < 0x80)
		return (unsigned char)(int)(b <<1);
	else
		return (unsigned char)( (int)(b << 1) ^ (int)(0x1b) );
}

unsigned char CAES::gfmultby03(const unsigned char b)
{
	return (unsigned char) ( (int)gfmultby02(b) ^ (int)b );
}

unsigned char CAES::gfmultby09(const unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^(int)b );
}

unsigned char CAES::gfmultby0b(const unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(b) ^
		(int)b );
}

unsigned char CAES::gfmultby0d(const unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(gfmultby02(b)) ^
		(int)(b) );
}

unsigned char CAES::gfmultby0e(const unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(gfmultby02(b)) ^
		(int)gfmultby02(b) );
}

/*******************************************************
* 函数名: InvSubBytes
* 函数功能: 逆字节替换,用于解密
* 参数个数: 1
**** @Param[IN]: bytes[] ---- 一个字大小的字节数组
* 返回值: 无
*******************************************************/
void CAES::InvSubBytes(unsigned char bytes[])
{
	bytes[0] = InvSBox[ bytes[0] >> 4][bytes[0] & 0x0f ];
	bytes[1] = InvSBox[ bytes[1] >> 4][bytes[1] & 0x0f ];
	bytes[2] = InvSBox[ bytes[2] >> 4][bytes[2] & 0x0f ];
	bytes[3] = InvSBox[ bytes[3] >> 4][bytes[3] & 0x0f ];
}

/*******************************************************
* 函数名: InvShiftRows
* 函数功能: 逆行位移变换,用于解密
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::InvShiftRows(void)
{
	unsigned char* temp = new unsigned char[4*m_Nb];

	for (int r = 0; r < 4; ++r)  
	{
		for (int c = 0; c < m_Nb; ++c)
		{
			temp[m_Nb*r + c] = m_State[m_Nb*r + c];
		}
	}
	
	for (int r = 1; r < 4; ++r)
	{
		for (int c = 0; c < m_Nb; ++c)
		{
			m_State[m_Nb*r + (c + r) % m_Nb ] = temp[m_Nb*r+c];
		}
	}

	delete[] temp;
}

/*******************************************************
* 函数名: InvMixColumns
* 函数功能: 逆列混合变换,用于解密
* 参数个数: 0
* 返回值: 无
*******************************************************/
void CAES::InvMixColumns(void)
{
	unsigned char* temp = new unsigned char[4*m_Nb];

	for (int r = 0; r < 4; ++r)  
	{
		for (int c = 0; c < 4; ++c)
		{
			temp[m_Nb*r + c] = m_State[m_Nb*r + c];
		}
	}

	for (int c = 0; c < 4; ++c)
	{
		m_State[m_Nb*0+c] = (unsigned char) ( (int)gfmultby0e(temp[m_Nb*0+c]) ^
			(int)gfmultby0b(temp[m_Nb*1+c]) ^
			(int)gfmultby0d(temp[m_Nb*2+c]) ^
			(int)gfmultby09(temp[m_Nb*3+c]) );

		m_State[m_Nb*1+c] = (unsigned char) ( (int)gfmultby09(temp[m_Nb*0+c]) ^
			(int)gfmultby0e(temp[m_Nb*1+c]) ^
			(int)gfmultby0b(temp[m_Nb*2+c]) ^
			(int)gfmultby0d(temp[m_Nb*3+c]) );

		m_State[m_Nb*2+c] = (unsigned char) ( (int)gfmultby0d(temp[m_Nb*0+c]) ^
			(int)gfmultby09(temp[m_Nb*1+c]) ^
			(int)gfmultby0e(temp[m_Nb*2+c]) ^
			(int)gfmultby0b(temp[m_Nb*3+c]) );

		m_State[m_Nb*3+c] = (unsigned char) ( (int)gfmultby0b(temp[m_Nb*0+c]) ^
			(int)gfmultby0d(temp[m_Nb*1+c]) ^
			(int)gfmultby09(temp[m_Nb*2+c]) ^
			(int)gfmultby0e(temp[m_Nb*3+c]) );
	}

	delete[] temp;
}

/*******************************************************
* 函数名: Cipher
* 函数功能: 加密
* 参数个数: 2
**** @Param[IN]: in ---- 需要加密的明文,只能为4*m_Nb字节
**** @Param[OUT]: out ---- 加密后的密文
* 返回值: 无
*******************************************************/
void CAES::Cipher(void *in,void *out)
{
	if(m_State)
		delete[] m_State;
	m_State = new unsigned char[4*m_Nb];

	unsigned char* pInput = (unsigned char*)in;

	//首先将明文的前4*m_Nb个字节拷贝到state数组
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		m_State[m_Nb*(i % 4) + i / 4] = pInput[i];
	}

	//第一次轮密钥加
	AddRoundKey(0);

	//循环进行m_Nr次轮变换
	for (int round = 1; round < m_Nr; ++round)  
	{
		for(int row=0;row<4;row++)
		{
			SubBytes(&m_State[m_Nb*row]); 
		}

		ShiftRows();  
		MixColumns(); 
		AddRoundKey(round);
	}  

	//最后一次变换,不需要进行列混合变换
	for(int row=0;row<4;row++)
	{
		SubBytes(&m_State[m_Nb*row]); 
	}

	ShiftRows();
	AddRoundKey(m_Nr);

	//将状态数组的前4*m_Nb个字节最为密文输出
	unsigned char* pOut = (unsigned char*)out;
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		pOut[i] = m_State[m_Nb*(i % 4) + i / 4];
	}
}

/*******************************************************
* 函数名: InvCipher
* 函数功能: 解密,ciper的逆过程
* 参数个数: 2
**** @Param[IN]: in ---- 需要解密的密文,只能为4*m_Nb字节
**** @Param[OUT]: out ---- 解密后的明文
* 返回值: 无
*******************************************************/
void CAES::InvCipher(void *in,void *out)
{
	if(m_State)
		delete[] m_State;
	m_State = new unsigned char[4*m_Nb];

	unsigned char* pInput = (unsigned char*)in;
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		m_State[m_Nb*(i % 4) + i / 4] = pInput[i];
	}

	AddRoundKey(m_Nr);


	for (int round = m_Nr-1; round >= 1; --round) 
	{
		InvShiftRows();

		for(int row=0;row<4;row++)
		{
			InvSubBytes(&m_State[m_Nb*row]); 
		}

		AddRoundKey(round);
		InvMixColumns();
	}  

	InvShiftRows();

	for(int row=0;row<4;row++)
	{
		InvSubBytes(&m_State[m_Nb*row]); 
	}

	AddRoundKey(0);

	unsigned char* pOut = (unsigned char*)out;
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		pOut[i] = m_State[m_Nb*(i % 4) + i / 4];
	}
}

/*******************************************************
* 函数名: Encrypt
* 函数功能: AES加密
* 参数个数: 6
**** @Param[IN]: in ---- 需要解密的缓冲区
**** @Param[IN]: nsize ---- 缓冲区大小
**** @Param[OUT]: out ---- 输出缓冲区
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: iSize ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥长度类型
* 返回值: 无
*******************************************************/
void CAES::Encrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize)
{
	//首先初始化各参数
	SetParams(keySize,(unsigned char*)key,iSize);

	unsigned char OutText[16];
	unsigned char* pText = NULL;
	
	//将明文拷贝到新的数组,不足的用NULL填充
	int ncount = nsize % 16;
	int totalSize = (nsize / 16 + 1) * 16;
	pText = new unsigned char[totalSize];
	if (m_Mode == PKCS5PADDING)
	{
		memset(pText,16-ncount,totalSize);
	}
	else if (m_Mode == NOPADDING)
	{
		memset(pText,0,totalSize);
	}

	memcpy(pText,in,nsize);

	//每4*m_Nb个字节一组,进行加密
	unsigned char* pOut = (unsigned char*)out;
	for (int i=0;i<totalSize;i+=4*m_Nb)
	{
		Cipher(&pText[i],OutText);
		memcpy(&pOut[i],OutText,4*m_Nb);
	}

	delete[] pText;
}

/*******************************************************
* 函数名: Decrypt
* 函数功能: AES解密
* 参数个数: 6
**** @Param[IN]: in ---- 需要解密的缓冲区
**** @Param[IN]: nsize ---- 缓冲区大小
**** @Param[OUT]: out ---- 输出缓冲区
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: iSize ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥长度类型
* 返回值: 无
*******************************************************/
void CAES::Decrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize)
{
	SetParams(keySize,(unsigned char*)key,iSize);

	unsigned char OutText[16];
	unsigned char* pText = NULL;

	int ncount = nsize % 16;
	int totalSize = nsize + ncount;

	pText = new unsigned char[totalSize];
	memset(pText,16-ncount,totalSize);

	memcpy(pText,in,nsize);

	unsigned char* pOut = (unsigned char*)out;
	for (int i=0;i<totalSize;i+=16)
	{
		InvCipher(&pText[i],OutText);
		memcpy(&pOut[i],OutText,16);
	}

	delete[] pText;
}

/*******************************************************
* 函数名: Encrypt2
* 函数功能: 加密(PHP用)
* 参数个数: 2
**** @Param[IN]: in ---- 需要加密的缓冲区
**** @Param[IN]: nsize ---- 缓冲区大小
**** @Param[OUT]: out ---- 输出缓冲区
**** @Param[IN]: outSize ---- 输出大小
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: iSize ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥长度类型
**** @Param[IN]: RandNum ---- 随机数(0表示由系统生成,否则使用提供的值)
* 返回值: 无
*******************************************************/
void CAES::Encrypt2(void* in,unsigned long nsize,void* out,unsigned long& outSize,void* key,int iSize,KEYSIZE keySize,int RandNum /*= 0*/)
{
	/////////////////////////////////////////////////////////////////////
	//先对密钥进行填充到合适的大小,并使用AES对其进行一次加密
	int len = 4 * m_Nk;

	unsigned char* pkey = (unsigned char*)key;
	unsigned char* temp = new unsigned char[len];
	unsigned char* cipherkey = new unsigned char[len];

	for (int i=0; i<len; i++)
	{
		char ch = '\0';

		if(i <= iSize)
			ch = (char)pkey[i];

		temp[i] = (int)ch & 0xff;
	}

	Encrypt(temp,4*m_Nk,cipherkey,temp,len,keySize/*KS_128*/);
	delete[] temp;
	/////////////////////////////////////////////////////////////////////
	//使用随机数生成一个8字节大小的数组
	unsigned char* counterBlock = new unsigned char[16];

	int nonce = 0;
	int nonceSec = 0;
	int nonceMs = 0;

	if(RandNum == 0)
		nonce = RangedRandDemo(11111111,99999999)*1000;
	else
		nonce = RandNum;

	nonceSec = asmifloor((float)nonce/1000);
	nonceMs = nonce % 1000;

	for (int i=0;i<4;i++)
	{
		counterBlock[i] = URS(nonceSec,i*8) & 0xff;
		counterBlock[i+4] = nonceMs & 0xff;
	}

	char* ctrTxt = new char[9];
	memcpy(ctrTxt,counterBlock,8);
	ctrTxt[8] = '\0';
	//////////////////////////////////////////////////////////////////////
	//利用加密后的密文作为当前使用的密钥
	SetParams(keySize/*KS_128*/,cipherkey,len);

	unsigned char* plainText = (unsigned char*)in;
	unsigned char* outText = (unsigned char*)out;
	int blockCount = asmiceil((float)nsize / 16);

	int tolSize = 0;
	for (int b=0; b<blockCount; b++) 
	{
		//填充后8个字节
		for (int c=0; c<4; c++) 
		{
			counterBlock[15-c] = URS(b,c*8) & 0xff;
			counterBlock[15-c-4] = URS((float)b/0x100000000, (float)c*8);
		}

		//对该块进行加密
		unsigned char tout[16];
		Cipher(counterBlock,tout);

		int blockLength = b<blockCount-1 ? 16 : (nsize-1)%16+1;
		
		//利用该加密块对明文进行一个字节一个字节的异或,生成密文
		unsigned char* cipherByte = new unsigned char[blockLength];
		for (int i=0; i<blockLength; i++) 
		{ 
			cipherByte[i] = tout[i] ^ (int)plainText[b*16+i];
			cipherByte[i] = (char)cipherByte[i];
		}

		memcpy(&outText[tolSize],cipherByte,blockLength);
		tolSize += blockLength;
		delete[] cipherByte;
	}

	memmove(&outText[8],&outText[0],tolSize);
	memcpy(outText,ctrTxt,8);
	outSize = tolSize + 8;

	delete[] counterBlock;
	delete[] ctrTxt;
	delete[] cipherkey;
}

/*******************************************************
* 函数名: Decrypt2
* 函数功能: AES解密(PHP用)
* 参数个数: 6
**** @Param[IN]: in ---- 需要加密的缓冲区
**** @Param[IN]: nsize ---- 缓冲区大小
**** @Param[OUT]: out ---- 输出缓冲区
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: iSize ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥长度类型
* 返回值: 无
*******************************************************/
void CAES::Decrypt2(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize)
{
	/////////////////////////////////////////////////////////////////
	//和加密过程一样
	int len = 4 * m_Nk;
	unsigned char* pkey = (unsigned char*)key;
	unsigned char* temp = new unsigned char[len];
	unsigned char* cipherkey = new unsigned char[len];

	for (int i=0; i<len; i++)
	{
		char ch = '\0';

		if(i <= iSize)
			ch = (char)pkey[i];

		temp[i] = (int)ch & 0xff;
	}

	Encrypt(temp,4*m_Nk,cipherkey,temp,len,keySize/*KS_128*/);
	delete[] temp;
	///////////////////////////////////////////////////////////////////////
	//取出前八个字节,此八个字节为加密时随机数生成的
	char* ctrTxt = new char[9];
	memcpy(ctrTxt,in,8);
	ctrTxt[8] = '\0';

	unsigned char* counterBlock = new unsigned char[16];
	for (int i=0; i<8; i++) 
	{
		counterBlock[i] = (int)ctrTxt[i];
	}
	//////////////////////////////////////////////////////////////////////
	unsigned char* pIn = (unsigned char*)in;
	unsigned char* pOut = (unsigned char*)out;

	SetParams(keySize/*KS_128*/,cipherkey,len);

	int nBlocks = asmiceil((float)(nsize-8) / 16);
	
	char* ct = new char[nBlocks*16+1];
	memset(ct,0,nBlocks*16+1);
	memcpy(ct,&pIn[8],nsize - 8);

	//与加密过程一致
	unsigned char plaintxtByte[16];
	int tolSize = 0;
	for (int b=0; b<nBlocks; b++) 
	{
		//此处还原原后8个字节
		for (int c=0; c<4; c++) 
		{
			counterBlock[15-c] = URS(b, c*8, false) & 0xff;
			counterBlock[15-c-4] = URS((float)(b+1)/4294967296-1, c*8, false) & 0xff;
		}

		unsigned char out[16];
		Cipher(counterBlock,out);

		memset(plaintxtByte,0,16);
		char* temp = &ct[b*16];

		//再一次进行异或,还原明文
		int isize = (nsize - 8 - b*16) > 16 ? 16:(nsize - 8 - b*16); 
		for (int i=0; i<isize; i++) 
		{
			plaintxtByte[i] = out[i] ^ (int)temp[i];
			plaintxtByte[i] = (char)plaintxtByte[i];
		}

		memcpy(&pOut[tolSize],plaintxtByte,16);
		tolSize += 16;
	}

	delete[] ct;
	delete[] counterBlock;
	delete[] cipherkey;
	delete[] ctrTxt;
}

/*******************************************************
* 函数名: URS
* 函数功能: 移位
* 参数个数: 3
**** @Param[IN]: a ---- 需要移位的变量
**** @Param[IN]: b ---- 移位数
**** @Param[IN]: c ---- 控制变量
* 返回值: 返回移位后的值
*******************************************************/
int CAES::URS(int a,int b,bool c /*= true*/)
{
	if(c )
	{
		if(a>0)
		{
			a &= 0xffffffff; 
			b &= 0x1f;
			if (a & 0x80000000) 
			{  
				a = (a>>1) | 0x7fffffff;
				a = a >> (b-1);
			} else 
			{ 
				a = (a>>b); 
			}
		}
		else if(a<0)
		{
			int bases = 4294967295;
			a = bases+a+1;
			a = a/pow(2,b);
		}else
		{
			a=0;
		}

	}
	else 
	{
		a &= 0xffffffff; 
		b &= 0x1f;
		if (a & 0x80000000) 
		{  
			a = (a>>1) | 0x7fffffff;
			a = a >> (b-1);
		} else 
		{ 
			a = (a>>b);
		}
	}
	return (int)a;
}

/*******************************************************
* 函数名: RangedRandDemo
* 函数功能: 生成随机数
* 参数个数: 2
**** @Param[IN]: range_min ---- 随机数下限
**** @Param[IN]: range_max ---- 随机数上限
* 返回值: 返回生成的随机数
*******************************************************/
int CAES::RangedRandDemo( int range_min, int range_max)
{
	//以当前时间作为种子
	srand((unsigned int )time( NULL ));

	int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)+ range_min;

	return u;
}

/*******************************************************
* 函数名: AESEncrypt
* 函数功能: AES加密
* 参数个数: 7
**** @Param[IN]: plainTextBuf ---- 需要加密的文本
**** @Param[IN]: bufLen ---- 文本长度
**** @Param[OUT]: pCipherTxt ---- 加密后的文本
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: keyLen ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥类型
**** @Param[IN]: RandNum ---- 随机数
* 返回值: 返回加密后的文本地址
*******************************************************/
char* CAES::AESEncrypt(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize/* = KS_128*/,int RandNum /*= 0*/)
{
	CAES Aes;

	unsigned short ciperLen = bufLen + ALIGNBIT + PASSBIT;
	unsigned char* CipherText = new unsigned char[ciperLen];
	unsigned long outSize = 0;

	memset(CipherText,0,ciperLen);

	Aes.Encrypt2(plainTextBuf,bufLen,(void*)CipherText,outSize,key,keyLen,keySize,RandNum);

	*pCipherTxt = new char[2*outSize + 1];
	memset(*pCipherTxt,0,2*outSize+1);
	CharStr2HexStr(CipherText,outSize,*pCipherTxt);

	delete[] CipherText;

	return *pCipherTxt;
}

/*******************************************************
* 函数名: AESDecrypt
* 函数功能: AES解密
* 参数个数: 5
**** @Param[IN]: cipherText ---- 加密文本
**** @Param[OUT]: PlainTxt ---- 解密后的文本
**** @Param[IN]: key ---- 密钥
**** @Param[IN]: keyLen ---- 密钥长度
**** @Param[IN]: keySize ---- 密钥类型
* 返回值: 返回解密文本地址
*******************************************************/
char* CAES::AESDecrypt(char* cipherText,char** PlainTxt,void* key,unsigned long keyLen,KEYSIZE keySize /*= KS_128*/)
{
	CAES Aes;

	// 长度不合法(长度至少为18个字节,因为有8个字节的随机数)
	int hexStrlen = strlen(cipherText);
	if(hexStrlen < 2*PASSBIT)
		return NULL;

	// 字符串如果不是合法的16进制的字符串,则直接返回NULL
	if(Aes.IsNotValidHexStr(cipherText))
		return NULL;

	int StrLen = hexStrlen>>1; 

	unsigned char* pCipherText = new unsigned char[StrLen+1];
	memset(pCipherText,0,StrLen+1);
	
	HexStr2CharStr(cipherText,StrLen,pCipherText);

	*PlainTxt = new char[StrLen +ALIGNBIT];
	memset(*PlainTxt,0,StrLen +ALIGNBIT);

	//开始解密
	Aes.Decrypt2((void*)pCipherText,StrLen,(void*)*PlainTxt,key,keyLen,keySize);

	delete[] pCipherText;

	return *PlainTxt;
}

/*******************************************************
* 函数名: IsNotValidHexStr
* 函数功能: 检查字符串是否为合法16进制串
* 参数个数: 1
**** @Param[IN]: pucCharStr ---- 16进制串
* 返回值: 不是合法的16进制串则返回TRUE
*******************************************************/
bool CAES::IsNotValidHexStr(const char* pucCharStr)
{
	int len = strlen(pucCharStr);

	if(len <= 0 || len % 2 != 0)
		return true;

	for (int i=0;i<len;i++)
	{
		if((pucCharStr[i] >= '0' && pucCharStr[i] <= '9') || (pucCharStr[i] >= 'A' && pucCharStr[i] <= 'F'))
			continue;
		return true;
	}

	return false;
}

/*******************************************************
* 函数名: HexStr2CharStr
* 函数功能: 16进制字串----->字符串
* 参数个数: 3
**** @Param[IN]: pszHexStr ---- 16进制串
**** @Param[IN]: iSize ---- 串长度
**** @Param[OUT]: pucCharStr ---- 字符串
* 返回值: 无
*******************************************************/
void CAES::HexStr2CharStr(char const* pszHexStr, int iSize,unsigned char* pucCharStr)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

/*******************************************************
* 函数名: Hex2Char
* 函数功能: 16进制----->字符
* 参数个数: 2
**** @Param[IN]: szHex ---- 16进制串
**** @Param[OUT]: rch ---- 字符
* 返回值: 无
*******************************************************/
void CAES::Hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}

/*******************************************************
* 函数名: Char2Hex
* 函数功能: 字符----->16进制
* 参数个数: 2
**** @Param[IN]: ch ---- 字符
**** @Param[OUT]: szHex ---- 16进制串
* 返回值: 无
*******************************************************/
void CAES::Char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch % 16;
	for(int i=0;i<2;i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

/*******************************************************
* 函数名: CharStr2HexStr
* 函数功能: 字符串----->十六进制字串
* 参数个数: 3
**** @Param[IN]: pucCharStr ---- 字符串
**** @Param[IN]: iSize ---- 串长
**** @Param[OUT]: pszHexStr ---- 16进制串
* 返回值: 无
*******************************************************/
void CAES::CharStr2HexStr(unsigned char* pucCharStr, int iSize,char* pszHexStr)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0;i<iSize;i++)
	{
		Char2Hex(pucCharStr[i],szHex);
		strcat(pszHexStr,szHex);
	}
}

//	* 函数名: AESEncryptOutputBase64Code
//	* 函数功能: AES加密之后输出Base64编码结果
//	* 参数个数: 6
//	**** @Param[IN]: plainTextBuf ---- 需要加密的文本
//	**** @Param[IN]: bufLen ---- 文本长度
//	**** @Param[OUT]: pCipherTxt ---- 加密后的文本
//	**** @Param[IN]: key ---- 密钥
//	**** @Param[IN]: keyLen ---- 密钥长度
//	**** @Param[IN]: keySize ---- 密钥类型
//	* 返回值: 返回加密后的文本地址
char* CAES::AESEncryptOutputBase64Code(void* plainTextBuf,unsigned long bufLen,char** pCipherTxt,void* key,unsigned long keyLen,KEYSIZE keySize)
{
	CAES Aes;

	unsigned short ciperLen = bufLen + ALIGNBIT + PASSBIT;
	unsigned char* CipherText = new unsigned char[ciperLen];
	unsigned long outSize = (bufLen / 16) * 16 + 16;

	memset(CipherText,0,ciperLen);

	Aes.Encrypt(plainTextBuf,bufLen,(void*)CipherText,key,keyLen,keySize);

	int nSize = Base64Encode((const char*)CipherText, outSize, NULL, 0);
	*pCipherTxt = (char*)malloc(nSize+1);
	memset(*pCipherTxt,0,nSize+1);
	Base64Encode((const char*)CipherText, outSize, *pCipherTxt, nSize);

	delete[] CipherText;

	return *pCipherTxt;
}


wchar_t * CAES::AESDecryptOutputBase64Code(void* pCipherTxt,unsigned long bufLen,wchar_t * plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize)
{
	if (plainTextBuf == NULL)
	{
		return NULL;
	}
	
	int nSize = Base64Decode((const char*)pCipherTxt, bufLen, NULL, 0);
	//OutputDebugStringA("AESDecryptOutputBase64Code 1\n");
	unsigned short ciperLen = nSize + 1;
	unsigned char* CipherText = new unsigned char[ciperLen];
	memset(CipherText,0,ciperLen);
	unsigned int nDecodeLength = Base64Decode((const char*)pCipherTxt, bufLen, (char*)CipherText, nSize);
	//OutputDebugStringA("AESDecryptOutputBase64Code 2\n");
	CAES Aes;
	memset(plainTextBuf,0,wcslen(plainTextBuf));
	Aes.Decrypt(CipherText,nDecodeLength,(void*)plainTextBuf,key,keyLen,keySize);
	//OutputDebugStringA("AESDecryptOutputBase64Code 3\n");
	return plainTextBuf;
}

char* CAES::AESDecryptOutputBase64Code( void* pCipherTxt,unsigned long bufLen,char* plainTextBuf,void* key,unsigned long keyLen,KEYSIZE keySize /*= KS_128*/ )
{
	if (plainTextBuf == NULL)
	{
		return NULL;
	}

	int nSize = Base64Decode((const char*)pCipherTxt, bufLen, NULL, 0);
	//OutputDebugStringA("AESDecryptOutputBase64Code 1\n");
	unsigned short ciperLen = nSize + 1;
	unsigned char* CipherText = new unsigned char[ciperLen];
	memset(CipherText,0,ciperLen);
	unsigned int nDecodeLength = Base64Decode((const char*)pCipherTxt, bufLen, (char*)CipherText, nSize);
	//OutputDebugStringA("AESDecryptOutputBase64Code 2\n");
	memset(plainTextBuf,0,nSize+1);
	CAES Aes;
	Aes.Decrypt(CipherText,nDecodeLength,(void*)plainTextBuf,key,keyLen,keySize);
	//OutputDebugStringA("AESDecryptOutputBase64Code 3\n");
	return plainTextBuf;
}
