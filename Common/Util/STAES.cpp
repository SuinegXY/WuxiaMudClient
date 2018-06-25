#include "STAES.h"
#include "Base64Encoder.h"
/***********************************************************************
* ȫ�ֱ�������
***********************************************************************/
//S��
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

//��S��
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

/* 16�ֽڶ���λ */
const int ALIGNBIT = 16;

/* ����� + ������ */
const int PASSBIT = 9;

/*******************************************************
* ������: CAES
* ��������: ���캯��
* ��������: ��
* ����ֵ: ��
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
* ������: CAES
* ��������: ���캯��
* ��������: 3
**** @Param[IN]: keySize ---- ��Կ��������(KS_128,KS_192,KS_256)
**** @Param[IN]: key ---- �����õ���Կ
**** @Param[IN]: isize ---- ��Կ�ĳ���
* ����ֵ: ��
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
* ������: ~CAES
* ��������: ��������
* ��������: ��
* ����ֵ: ��
*******************************************************/
CAES::~CAES()
{
	Clear();
}

/*******************************************************
* ������: SetParams
* ��������: ������Կ���������ò���
* ��������: 3
**** @Param[IN]: keySize ---- ��Կ��������(KS_128,KS_192,KS_256)
**** @Param[IN]: key ---- �����õ���Կ
**** @Param[IN]: isize ---- ��Կ�ĳ���
* ����ֵ: ��
*******************************************************/
void CAES::SetParams(KEYSIZE keySize,unsigned char* key,int isize)
{
	//�˴����ܿ�̶�����Ϊ16�ֽڿ�
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

	//��ʼ����Կ
	InitializeKey(key,isize);

	//��ʼ������
	InitializeRcon();

	//������Կ���ȱ�
	KeyExpansion(m_key);

}

/*******************************************************
* ������: InitializeKey
* ��������: ��ʼ����Կ
* ��������: 2
**** @Param[IN]: key ---- �����õ���Կ
**** @Param[IN]: isize ---- ��Կ�ĳ���
* ����ֵ: ��
*******************************************************/
void CAES::InitializeKey(unsigned char* key,int isize)
{
	int len = 4 * m_Nk;

	if(m_key)
		delete[] m_key;

	//��Կ����Ϊ128,192,256
	m_key = new unsigned char[len];
	memset(m_key,0,len);

	memcpy(m_key,key,(isize<len ? isize:len));
}

/*******************************************************
* ������: InitializeRcon
* ��������: ��ʼ������
* ��������: 0
* ����ֵ: ��
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
* ������: Clear
* ��������: �ͷ�������Դ
* ��������: 0
* ����ֵ: ��
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
* ������: KeyExpansion
* ��������: ������Կ���ȱ�
* ��������: 1
**** @Param[IN]: key ---- ��Կ
* ����ֵ: ��
*******************************************************/
void CAES::KeyExpansion(void *key)
{
	int len = m_Nb * (m_Nr+1)*4;

	if(m_w)
		delete[] m_w;
	m_w = new unsigned char[len];  

	//ǰM_NK�б���ԭʼ��Կ
	for (int row = 0; row < m_Nk; ++row)
	{
		m_w[4*row+0] = m_key[4*row];
		m_w[4*row+1] = m_key[4*row+1];
		m_w[4*row+2] = m_key[4*row+2];
		m_w[4*row+3] = m_key[4*row+3];
	}

	unsigned char* temp = new unsigned char[4];
	int keylen = m_Nb * (m_Nr + 1);

	//����ԭʼ��Կ��������Կ
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
* ������: AddRoundKey
* ��������: ����Կ��,ʹ����Կ���ȱ��е�ǰ���ж�State״̬����ʵ��һ���ֽ�һ���ֽڵ����
				 ����,��������Կ��w[c,r] �������State[r,c]
* ��������: 1
**** @Param[IN]: round ---- ����
* ����ֵ: ��
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
* ������: SubBytes
* ��������: �ֽ��滻,�� State �����е�ÿ���ֽ��滻��һ���� Sbox ���������ֽ�
* ��������: 1
**** @Param[IN]: bytes[] ---- ��Ҫ�滻���ֽ�(һ���ִ�С)
* ����ֵ: ��
*******************************************************/
void CAES::SubBytes(unsigned char bytes[])
{
	bytes[0] = SBox[ bytes[0] >> 4][bytes[0] & 0x0f ];
	bytes[1] = SBox[ bytes[1] >> 4][bytes[1] & 0x0f ];
	bytes[2] = SBox[ bytes[2] >> 4][bytes[2] & 0x0f ];
	bytes[3] = SBox[ bytes[3] >> 4][bytes[3] & 0x0f ];
}

/*******************************************************
* ������: RowBytes
* ��������: ѭ��������λ
* ��������: 1
**** @Param[IN]: bytes[] ---- ��Ҫ��λ���ֽ�(һ���Ӵ�С)
* ����ֵ: ������λ�����
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
* ������: ShiftRows
* ��������: ��λ�Ʊ任,�� State �����е��ֽ�������ת
* ��������: 0
* ����ֵ: ��
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
* ������: MixColumns
* ��������: �л�ϱ任,�� State �ֽ��е�ֵ������ѧ��Ӻ���˵Ľ������ÿ���ֽ�
* ��������: 0
* ����ֵ: ��
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
* ������: gfmultby(*)
* ��������: ٤����������(GF(28))
* ��������: 1
**** @Param[IN]: b ---- ������
* ����ֵ: ���������Ľ��
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
* ������: InvSubBytes
* ��������: ���ֽ��滻,���ڽ���
* ��������: 1
**** @Param[IN]: bytes[] ---- һ���ִ�С���ֽ�����
* ����ֵ: ��
*******************************************************/
void CAES::InvSubBytes(unsigned char bytes[])
{
	bytes[0] = InvSBox[ bytes[0] >> 4][bytes[0] & 0x0f ];
	bytes[1] = InvSBox[ bytes[1] >> 4][bytes[1] & 0x0f ];
	bytes[2] = InvSBox[ bytes[2] >> 4][bytes[2] & 0x0f ];
	bytes[3] = InvSBox[ bytes[3] >> 4][bytes[3] & 0x0f ];
}

/*******************************************************
* ������: InvShiftRows
* ��������: ����λ�Ʊ任,���ڽ���
* ��������: 0
* ����ֵ: ��
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
* ������: InvMixColumns
* ��������: ���л�ϱ任,���ڽ���
* ��������: 0
* ����ֵ: ��
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
* ������: Cipher
* ��������: ����
* ��������: 2
**** @Param[IN]: in ---- ��Ҫ���ܵ�����,ֻ��Ϊ4*m_Nb�ֽ�
**** @Param[OUT]: out ---- ���ܺ������
* ����ֵ: ��
*******************************************************/
void CAES::Cipher(void *in,void *out)
{
	if(m_State)
		delete[] m_State;
	m_State = new unsigned char[4*m_Nb];

	unsigned char* pInput = (unsigned char*)in;

	//���Ƚ����ĵ�ǰ4*m_Nb���ֽڿ�����state����
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		m_State[m_Nb*(i % 4) + i / 4] = pInput[i];
	}

	//��һ������Կ��
	AddRoundKey(0);

	//ѭ������m_Nr���ֱ任
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

	//���һ�α任,����Ҫ�����л�ϱ任
	for(int row=0;row<4;row++)
	{
		SubBytes(&m_State[m_Nb*row]); 
	}

	ShiftRows();
	AddRoundKey(m_Nr);

	//��״̬�����ǰ4*m_Nb���ֽ���Ϊ�������
	unsigned char* pOut = (unsigned char*)out;
	for (int i = 0; i < (4 * m_Nb); ++i)
	{
		pOut[i] = m_State[m_Nb*(i % 4) + i / 4];
	}
}

/*******************************************************
* ������: InvCipher
* ��������: ����,ciper�������
* ��������: 2
**** @Param[IN]: in ---- ��Ҫ���ܵ�����,ֻ��Ϊ4*m_Nb�ֽ�
**** @Param[OUT]: out ---- ���ܺ������
* ����ֵ: ��
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
* ������: Encrypt
* ��������: AES����
* ��������: 6
**** @Param[IN]: in ---- ��Ҫ���ܵĻ�����
**** @Param[IN]: nsize ---- ��������С
**** @Param[OUT]: out ---- ���������
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: iSize ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ��������
* ����ֵ: ��
*******************************************************/
void CAES::Encrypt(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize)
{
	//���ȳ�ʼ��������
	SetParams(keySize,(unsigned char*)key,iSize);

	unsigned char OutText[16];
	unsigned char* pText = NULL;
	
	//�����Ŀ������µ�����,�������NULL���
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

	//ÿ4*m_Nb���ֽ�һ��,���м���
	unsigned char* pOut = (unsigned char*)out;
	for (int i=0;i<totalSize;i+=4*m_Nb)
	{
		Cipher(&pText[i],OutText);
		memcpy(&pOut[i],OutText,4*m_Nb);
	}

	delete[] pText;
}

/*******************************************************
* ������: Decrypt
* ��������: AES����
* ��������: 6
**** @Param[IN]: in ---- ��Ҫ���ܵĻ�����
**** @Param[IN]: nsize ---- ��������С
**** @Param[OUT]: out ---- ���������
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: iSize ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ��������
* ����ֵ: ��
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
* ������: Encrypt2
* ��������: ����(PHP��)
* ��������: 2
**** @Param[IN]: in ---- ��Ҫ���ܵĻ�����
**** @Param[IN]: nsize ---- ��������С
**** @Param[OUT]: out ---- ���������
**** @Param[IN]: outSize ---- �����С
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: iSize ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ��������
**** @Param[IN]: RandNum ---- �����(0��ʾ��ϵͳ����,����ʹ���ṩ��ֵ)
* ����ֵ: ��
*******************************************************/
void CAES::Encrypt2(void* in,unsigned long nsize,void* out,unsigned long& outSize,void* key,int iSize,KEYSIZE keySize,int RandNum /*= 0*/)
{
	/////////////////////////////////////////////////////////////////////
	//�ȶ���Կ������䵽���ʵĴ�С,��ʹ��AES�������һ�μ���
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
	//ʹ�����������һ��8�ֽڴ�С������
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
	//���ü��ܺ��������Ϊ��ǰʹ�õ���Կ
	SetParams(keySize/*KS_128*/,cipherkey,len);

	unsigned char* plainText = (unsigned char*)in;
	unsigned char* outText = (unsigned char*)out;
	int blockCount = asmiceil((float)nsize / 16);

	int tolSize = 0;
	for (int b=0; b<blockCount; b++) 
	{
		//����8���ֽ�
		for (int c=0; c<4; c++) 
		{
			counterBlock[15-c] = URS(b,c*8) & 0xff;
			counterBlock[15-c-4] = URS((float)b/0x100000000, (float)c*8);
		}

		//�Ըÿ���м���
		unsigned char tout[16];
		Cipher(counterBlock,tout);

		int blockLength = b<blockCount-1 ? 16 : (nsize-1)%16+1;
		
		//���øü��ܿ�����Ľ���һ���ֽ�һ���ֽڵ����,��������
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
* ������: Decrypt2
* ��������: AES����(PHP��)
* ��������: 6
**** @Param[IN]: in ---- ��Ҫ���ܵĻ�����
**** @Param[IN]: nsize ---- ��������С
**** @Param[OUT]: out ---- ���������
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: iSize ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ��������
* ����ֵ: ��
*******************************************************/
void CAES::Decrypt2(void* in,unsigned long nsize,void* out,void* key,int iSize,KEYSIZE keySize)
{
	/////////////////////////////////////////////////////////////////
	//�ͼ��ܹ���һ��
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
	//ȡ��ǰ�˸��ֽ�,�˰˸��ֽ�Ϊ����ʱ��������ɵ�
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

	//����ܹ���һ��
	unsigned char plaintxtByte[16];
	int tolSize = 0;
	for (int b=0; b<nBlocks; b++) 
	{
		//�˴���ԭԭ��8���ֽ�
		for (int c=0; c<4; c++) 
		{
			counterBlock[15-c] = URS(b, c*8, false) & 0xff;
			counterBlock[15-c-4] = URS((float)(b+1)/4294967296-1, c*8, false) & 0xff;
		}

		unsigned char out[16];
		Cipher(counterBlock,out);

		memset(plaintxtByte,0,16);
		char* temp = &ct[b*16];

		//��һ�ν������,��ԭ����
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
* ������: URS
* ��������: ��λ
* ��������: 3
**** @Param[IN]: a ---- ��Ҫ��λ�ı���
**** @Param[IN]: b ---- ��λ��
**** @Param[IN]: c ---- ���Ʊ���
* ����ֵ: ������λ���ֵ
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
* ������: RangedRandDemo
* ��������: ���������
* ��������: 2
**** @Param[IN]: range_min ---- ���������
**** @Param[IN]: range_max ---- ���������
* ����ֵ: �������ɵ������
*******************************************************/
int CAES::RangedRandDemo( int range_min, int range_max)
{
	//�Ե�ǰʱ����Ϊ����
	srand((unsigned int )time( NULL ));

	int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)+ range_min;

	return u;
}

/*******************************************************
* ������: AESEncrypt
* ��������: AES����
* ��������: 7
**** @Param[IN]: plainTextBuf ---- ��Ҫ���ܵ��ı�
**** @Param[IN]: bufLen ---- �ı�����
**** @Param[OUT]: pCipherTxt ---- ���ܺ���ı�
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: keyLen ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ����
**** @Param[IN]: RandNum ---- �����
* ����ֵ: ���ؼ��ܺ���ı���ַ
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
* ������: AESDecrypt
* ��������: AES����
* ��������: 5
**** @Param[IN]: cipherText ---- �����ı�
**** @Param[OUT]: PlainTxt ---- ���ܺ���ı�
**** @Param[IN]: key ---- ��Կ
**** @Param[IN]: keyLen ---- ��Կ����
**** @Param[IN]: keySize ---- ��Կ����
* ����ֵ: ���ؽ����ı���ַ
*******************************************************/
char* CAES::AESDecrypt(char* cipherText,char** PlainTxt,void* key,unsigned long keyLen,KEYSIZE keySize /*= KS_128*/)
{
	CAES Aes;

	// ���Ȳ��Ϸ�(��������Ϊ18���ֽ�,��Ϊ��8���ֽڵ������)
	int hexStrlen = strlen(cipherText);
	if(hexStrlen < 2*PASSBIT)
		return NULL;

	// �ַ���������ǺϷ���16���Ƶ��ַ���,��ֱ�ӷ���NULL
	if(Aes.IsNotValidHexStr(cipherText))
		return NULL;

	int StrLen = hexStrlen>>1; 

	unsigned char* pCipherText = new unsigned char[StrLen+1];
	memset(pCipherText,0,StrLen+1);
	
	HexStr2CharStr(cipherText,StrLen,pCipherText);

	*PlainTxt = new char[StrLen +ALIGNBIT];
	memset(*PlainTxt,0,StrLen +ALIGNBIT);

	//��ʼ����
	Aes.Decrypt2((void*)pCipherText,StrLen,(void*)*PlainTxt,key,keyLen,keySize);

	delete[] pCipherText;

	return *PlainTxt;
}

/*******************************************************
* ������: IsNotValidHexStr
* ��������: ����ַ����Ƿ�Ϊ�Ϸ�16���ƴ�
* ��������: 1
**** @Param[IN]: pucCharStr ---- 16���ƴ�
* ����ֵ: ���ǺϷ���16���ƴ��򷵻�TRUE
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
* ������: HexStr2CharStr
* ��������: 16�����ִ�----->�ַ���
* ��������: 3
**** @Param[IN]: pszHexStr ---- 16���ƴ�
**** @Param[IN]: iSize ---- ������
**** @Param[OUT]: pucCharStr ---- �ַ���
* ����ֵ: ��
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
* ������: Hex2Char
* ��������: 16����----->�ַ�
* ��������: 2
**** @Param[IN]: szHex ---- 16���ƴ�
**** @Param[OUT]: rch ---- �ַ�
* ����ֵ: ��
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
* ������: Char2Hex
* ��������: �ַ�----->16����
* ��������: 2
**** @Param[IN]: ch ---- �ַ�
**** @Param[OUT]: szHex ---- 16���ƴ�
* ����ֵ: ��
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
* ������: CharStr2HexStr
* ��������: �ַ���----->ʮ�������ִ�
* ��������: 3
**** @Param[IN]: pucCharStr ---- �ַ���
**** @Param[IN]: iSize ---- ����
**** @Param[OUT]: pszHexStr ---- 16���ƴ�
* ����ֵ: ��
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

//	* ������: AESEncryptOutputBase64Code
//	* ��������: AES����֮�����Base64������
//	* ��������: 6
//	**** @Param[IN]: plainTextBuf ---- ��Ҫ���ܵ��ı�
//	**** @Param[IN]: bufLen ---- �ı�����
//	**** @Param[OUT]: pCipherTxt ---- ���ܺ���ı�
//	**** @Param[IN]: key ---- ��Կ
//	**** @Param[IN]: keyLen ---- ��Կ����
//	**** @Param[IN]: keySize ---- ��Կ����
//	* ����ֵ: ���ؼ��ܺ���ı���ַ
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
