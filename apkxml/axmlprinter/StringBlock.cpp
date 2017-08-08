/*
 * Copyright 2008 Android4ME
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "StringBlock.h"

/**
 * @author Dmitry Skiba
 * 
 * Block of strings, used in binary xml and arsc.
 * 
 * TODO:
 * - implement get()
 *
 */

	
/**
 * Reads whole (including chunk type) string block from stream.
 * Stream must be at the chunk type.
 */

void StringBlock::readCheckType(IntReader reader, int expectedType)
{
	int type = reader.readInt();
	if (type != expectedType) {
		
	}
}
StringBlock* StringBlock::read(IntReader reader)
{
	readCheckType(reader, CHUNK_TYPE);
	int chunkSize = reader.readInt();
	
	StringBlock* block = new StringBlock();
	block->stringCount = reader.readInt();
	block->styleOffsetCount = reader.readInt();
	/*?*/reader.readInt();
	block->stringsOffset = reader.readInt();
	block->stylesOffset = reader.readInt();
	block->m_stringOffsets = reader.readIntArray(block->stringCount);
	if (block->styleOffsetCount != 0) {
		block->m_styleOffsets = reader.readIntArray(block->styleOffsetCount);
	}
	{
		int size = ((block->stylesOffset == 0) ? chunkSize : block->stylesOffset) - block->stringsOffset;
		if ((size % 4) != 0) {
			
		}
		block->m_strings = reader.readIntArray(size / 4);
	}
	if (block->stylesOffset != 0) {
		int size = (chunkSize - block->stylesOffset);
		if ((size % 4) != 0) {
			
		}
		block->m_styles = reader.readIntArray(size / 4);
	}

	return block;
}
	
/**
 * Returns number of strings in block. 
 */
int StringBlock::getCount()
{
	int counts = sizeof(m_stringOffsets) / sizeof(int);
	
	return counts;
}

	
/**
 * Returns raw string (without any styling information) at specified index.
 */
std::string StringBlock::getString(int index)
{
	if (index < 0 || index >= getCount())
	{
		return NULL;
	}
	int offset = m_stringOffsets[index];
	int length = getShort(m_strings, offset);
	char* result = new char(length);
	char temp;
	for (; length != 0; length -= 1) {
		offset += 2;
		temp = (char)getShort(m_strings, offset);
		strcat(result, (const char*)temp);
	}
	return std::string(result);
}
	
/**
 * Not yet implemented. 
 * 
 * Returns string with style information (if any).
 */
const char * StringBlock::get(int index)
{
	return getString(index).c_str();
}

/**
 * Returns string with style tags (html-like). 
 */
std::string StringBlock::getHTML(std::string & html, int index)
{
	std::string raw = getString(index);
	if (raw.empty()) {
		return raw;
	}
	int* style;
	int counts = getStyle(index, style);
	if (style == nullptr) {
		return raw;
	}
	//char* html = new char(raw.length() + 32);
	int offset = 0;
	while (true) {
		int i = -1;
		for (int j = 0; j != counts; j += 3) {
			if (style[j + 1] == -1) {
				continue;
			}
			if (i == -1 || style[i + 1] > style[j + 1]) {
				i = j;
			}
		}
		int start = ((i != -1) ? style[i + 1] : raw.length());
		for (int j = 0; j != counts; j += 3) {
			int end = style[j + 2];
			if (end == -1 || end >= start) {
				continue;
			}
			if (offset <= end) {
				std::string temp = raw.substr(offset, end + 1 - offset);
				html.append(temp);
				offset = end + 1;
			}
			style[j + 2] = -1;
			html.append("<");
			html.append("/");
			html.append(getString(style[j]));
			html.append(">");
		}
		if (offset < start) {
			html.append(raw, offset, start);
			offset = start;
		}
		if (i == -1) {
			break;
		}
		html.append("<");
		html.append(getString(style[i]));
		html.append(">");
		style[i + 1] = -1;
	}
	return html;
}

/**
 * Finds index of the string.
 * Returns -1 if the string was not found.
 */
int StringBlock::find(std::string string)
{
	if (string.empty()) {
		return -1;
	}
	int counts = sizeof(m_styleOffsets) / sizeof(int);
	for (int i = 0; i != counts; ++i) {
		int offset = m_stringOffsets[i];
		int length = getShort(m_strings, offset);
		if (length != string.length()) {
			continue;
		}
		int j = 0;
		for (; j != length; ++j) {
			offset += 2;
			if (string.at(j) != getShort(m_strings, offset)) {
				break;
			}
		}
		if (j == length) {
			return i;
		}
	}
	return -1;
}

///////////////////////////////////////////// implementation
StringBlock::StringBlock()
{
}
	
/**
 * Returns style information - array of int triplets,
 * where in each triplet:
 * * first int is index of tag name ('b','i', etc.)
 * * second int is tag start index in string
 * * third int is tag end index in string
 */
int StringBlock::getStyle(int index, int * style)
{
	if (m_styleOffsets == nullptr || m_styles == nullptr ||
		index >= this->styleOffsetCount)
	{
		return NULL;
	}
	int offset = m_styleOffsets[index] / 4;
	int counts = stylesOffset;
	{
		int count = 0;
		for (int i = offset; i < counts; ++i) {
			if (m_styles[i] == -1) {
				break;
			}
			count += 1;
		}
		if (count == 0 || (count % 3) != 0) {
			return NULL;
		}
		style = new int[count];
	}
	for (int i = offset, j = 0; i < counts;) {
		if (m_styles[i] == -1) {
			break;
		}
		style[j++] = m_styles[i++];
	}
	return counts;
}

int StringBlock::getShort(int array[], int offset)
{
	int value = array[offset / 4];
	if ((offset % 4) / 2 == 0) {
		return (value & 0xFFFF);
	}
	else {
		unsigned int temp = (unsigned)value;
		return (value >> 16);
	}
}