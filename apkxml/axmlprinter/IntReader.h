#pragma once
/**
* @author Dmitry Skiba
*
* Simple helper class that allows reading of integers.
*
* TODO:
* 	* implement buffering
*
*/

typedef char byte;

class IntReader {

public: 
	IntReader();
	IntReader(char* stream, bool bigEndian);
	void reset(char* stream, bool bigEndian);
	void close();

	char* getStream();
	bool isBigEndian();
	void setBigEndian(bool bigEndian);
	int readByte();
	int readShort();
	int readInt();
	int readInt(int length);
	int* readIntArray(int length);
	void readIntArray(int array[], int offset, int length);
	byte* readByteArray(int length);

	void skip(int bytes);
	void skipInt();
	int available();
	int getPosition();

	/////////////////////////////////// data

private:
	char* m_stream;
	bool m_bigEndian;
	int m_position;
};