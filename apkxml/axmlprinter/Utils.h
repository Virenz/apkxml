#pragma once

#include <string>
typedef char byte;

int numberOfLeadingZeros(int i) {
	// HD, Figure 5-6  
	if (i == 0)
		return 32;
	unsigned int _i = (unsigned)i;
	unsigned int n = 1;
	if (_i >> 16 == 0) { n += 16; _i <<= 16; }
	if (_i >> 24 == 0) { n += 8; _i <<= 8; }
	if (_i >> 28 == 0) { n += 4; _i <<= 4; }
	if (_i >> 30 == 0) { n += 2; _i <<= 2; }
	n -= _i >> 31;
	return (int)n;
}

int byte2int(byte res[]) {
	unsigned char u_res = (unsigned)(res[2] << 24);
	int targets = (res[0] & 0xff) | ((res[1] << 8) & 0xff00)
		| (u_res >> 8) | (res[3] << 24);
	return targets;
}

byte* int2Byte(int integer) {
	int byteNum = (40 - numberOfLeadingZeros(integer < 0 ? ~integer : integer)) / 8;
	byte* byteArray = new byte[4];

	for (int n = 0; n < byteNum; n++)
		byteArray[3 - n] = (byte)(((unsigned)integer) >> (n * 8));

	return (byteArray);
}

short byte2Short(byte b[]) {
	short s = 0;
	short s0 = (short)(b[0] & 0xff);
	short s1 = (short)(b[1] & 0xff);
	s1 <<= 8;
	s = (short)(s0 | s1);
	return s;
}

int hexCharToInt(char c)
{
	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
	return 0;
}

char* hexstringToBytes(std::string s)
{
	int sz = s.length();
	char *ret = new char[sz / 2];
	for (int i = 0; i < sz; i += 2) {
		ret[i / 2] = (char)((hexCharToInt(s.at(i)) << 4)
			| hexCharToInt(s.at(i + 1)));
	}
	return ret;
}

std::string bytesToHexString(char* bytes, int bytelength)
{
	std::string str("");
	std::string str2("0123456789abcdef");
	for (int i = 0; i < bytelength; i++) {
		int b;
		b = 0x0f & (bytes[i] >> 4);
		char s1 = str2.at(b);
		str.append(1, str2.at(b));
		b = 0x0f & bytes[i];
		str.append(1, str2.at(b));
		char s2 = str2.at(b);
	}
	return str;
}

byte* copyByte(byte src[], int start, int len) {
	if (src == nullptr) {
		return nullptr;
	}
	if (start > sizeof(src) / sizeof(char)) {
		return nullptr;
	}
	if ((start + len) > sizeof(src) / sizeof(char)) {
		return nullptr;
	}
	if (start < 0) {
		return nullptr;
	}
	if (len <= 0) {
		return nullptr;
	}
	byte* resultByte = new byte[len];
	for (int i = 0; i < len; i++) {
		resultByte[i] = src[i + start];
	}
	return resultByte;
}