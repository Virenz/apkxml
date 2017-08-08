#pragma once
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

/**
* @author Dmitry Skiba
*
* Block of strings, used in binary xml and arsc.
*
* TODO:
* - implement get()
*
*/
#include <string>
#include "IntReader.h"

class StringBlock {

	/**
	* Reads whole (including chunk type) string block from stream.
	* Stream must be at the chunk type.
	*/
public:

	static void readCheckType(IntReader reader, int expectedType);
	static StringBlock* read(IntReader reader);

	/**
	* Returns number of strings in block.
	*/
	int getCount();

	/**
	* Returns raw string (without any styling information) at specified index.
	*/
	std::string getString(int index);

	/**
	* Not yet implemented.
	*
	* Returns string with style information (if any).
	*/
	const char* get(int index);

	/**
	* Returns string with style tags (html-like).
	*/
	std::string getHTML(std::string & html,  int index);

	/**
	* Finds index of the string.
	* Returns -1 if the string was not found.
	*/
	int find(std::string string);

	///////////////////////////////////////////// implementation
	StringBlock();

	/**
	* Returns style information - array of int triplets,
	* where in each triplet:
	* 	* first int is index of tag name ('b','i', etc.)
	* 	* second int is tag start index in string
	* 	* third int is tag end index in string
	*/
private:
	int getStyle(int index, int * stype);

	static int getShort(int array[], int offset);

	int* m_stringOffsets;
	int stringsOffset;

	int* m_strings;
	int stringCount;

	int* m_styleOffsets;
	int styleOffsetCount;

	int* m_styles;
	int stylesOffset;

	static const int CHUNK_TYPE = 0x001C0001;
};
