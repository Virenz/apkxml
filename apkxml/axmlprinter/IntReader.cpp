/*
 * Copyright 2008 Android4ME
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Dmitry Skiba
 *
 * Simple helper class that allows reading of integers.
 * 
 * TODO:
 * 	* implement buffering
 *
 */
#include "IntReader.h"
#include <string.h>

IntReader::IntReader() 
{
}
IntReader::IntReader(char* stream, bool bigEndian)
{
	reset(stream,bigEndian);
}
void IntReader::reset(char * stream, bool bigEndian)
{
	m_stream = stream;
	m_bigEndian = bigEndian;
	m_position = 0;
}

void IntReader::close()
{
	if (sizeof(m_stream) == 0) {
		return;
	}
	reset(nullptr, false);
}

char * IntReader::getStream()
{
	return m_stream;
}

bool IntReader::isBigEndian()
{
	return m_bigEndian;
}

void IntReader::setBigEndian(bool bigEndian)
{
	m_bigEndian = bigEndian;
}

int IntReader::readByte()
{
	return readInt(1);
}

int IntReader::readShort()
{
	return readInt(2);
}

int IntReader::readInt()
{
	readInt(4);
}

int IntReader::readInt(int length)
{
	if (length < 0 || length>4) {
		
	}
	int result = 0;
	if (m_bigEndian) {
		for (int i = (length - 1) * 8; i >= 0; i -= 8) {
			int b = m_stream[m_position];
			if (b == -1) {
				
			}
			m_position += 1;
			result |= (b << i);
		}
	}
	else {
		length *= 8;
		for (int i = 0; i != length; i += 8) {
			int b = m_stream[m_position];
			if (b == -1) {
				
			}
			m_position += 1;
			result |= (b << i);
		}
	}
	return result;
}

int * IntReader::readIntArray(int length)
{
	int* array = new int[length];
	readIntArray(array, 0, length);
	return array;
}

void IntReader::readIntArray(int array[], int offset, int length)
{
	for (; length > 0; length -= 1) {
		array[offset++] = readInt();
	}
}

byte * IntReader::readByteArray(int length)
{
	byte* array = new byte[length];
	strcpy(array, m_stream);
	m_position += length;
	
	return array;
}

void IntReader::skip(int bytes)
{
	if (bytes <= 0) {
		return;
	}
	m_position += bytes;
	if (m_position > sizeof(m_stream))
	{
		m_position = sizeof(m_stream);
	}
}

void IntReader::skipInt()
{
	skip(4);
}

int IntReader::available()
{
	return sizeof(m_stream);
}

int IntReader::getPosition()
{
	return m_position;
}