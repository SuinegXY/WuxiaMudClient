#pragma once
// Base64����

//���뺯����pOutString == NULL  nBuffLen == 0�������ɱ�����ַ����Ŀռ��С��������Чʱ�򣬷��ر�����ַ��ĳ��ȣ�������NULL�ַ���
unsigned int Base64Encode(const char *pInString, unsigned int nStrLen, char *pOutString, unsigned int nBufferLen);
//���뺯��,pOutString == NULL  nBuffLen == 0�������ɱ�����ַ����Ŀռ��С��������Чʱ�򣬷��ر�����ַ��ĳ��ȣ�������NULL�ַ���
unsigned int Base64Decode(const char *pInString, unsigned int nStrLen, char *pOutString, unsigned int nBufferLen);
