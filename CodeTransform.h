/** 
* @file CodeTransform.h
* @brief ת�����ݱ��룬�������� Windows ƽ̨
* @date 2016/8/24 �������ĵ�
*/

#ifdef WIN32

#pragma once

// ��Unicode����ת����ANSI����(��Ҫ�ֶ�deleteת�����)
char* UnicodeConvert2ANSI(LPCWCH strEncodeData, int &ncLength);

// ��ANSI����ת����Unicode����
CString ANSIConvert2Unicode(char *strANSI, int &ncLength);

// ��Unicode����ת����UTF-8����(��Ҫ�ֶ�deleteת�����)
char* UnicodeConvert2UTF8(LPCWCH strUnicode, int &ncLength);

// ��utf-8����ת����Unicode����
CString UTF8Convert2Unicode(const char* strUtf8, int &ncLength);

// ��ANSI����ת����UTF-8����
char* ANSIConvert2UTF8(char* strANSI, int &ncLength);

// ��UTF8����ת����ANSI����
char* UTF8Convert2ANSI(char* strUtf8, int &ncLength);

#endif