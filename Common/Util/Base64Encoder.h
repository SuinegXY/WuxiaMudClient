#pragma once
// Base64编码

//编码函数，pOutString == NULL  nBuffLen == 0返回容纳编码后字符串的空间大小；参数有效时候，返回编码后字符的长度（不包括NULL字符）
unsigned int Base64Encode(const char *pInString, unsigned int nStrLen, char *pOutString, unsigned int nBufferLen);
//解码函数,pOutString == NULL  nBuffLen == 0返回容纳编码后字符串的空间大小；参数有效时候，返回编码后字符的长度（不包括NULL字符）
unsigned int Base64Decode(const char *pInString, unsigned int nStrLen, char *pOutString, unsigned int nBufferLen);
