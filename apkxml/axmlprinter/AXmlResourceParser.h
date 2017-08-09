#pragma once
/**
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

#include <string>
#include "StringBlock.h"
#include "TypedValue.h"
#include "XmlResourceParser.h"

/**
* @author Dmitry Skiba
*
* Binary xml files parser.
*
* Parser has only two states:
* (1) Operational state, which parser obtains after first successful call
* to next() and retains until open(), close(), or failed call to next().
* (2) Closed state, which parser obtains after open(), close(), or failed
* call to next(). In this state methods return invalid values or throw exceptions.
*
* TODO:
* 	* check all methods in closed state
*
*/
class AXmlResourceParser : public XmlResourceParser 
{

public:
	AXmlResourceParser();
	void open(char* stream);
	void close();

	/////////////////////////////////// iteration
	int next();
	int nextToken();
	int nextTag();
	std::string nextText();
	void require(int type, std::string _namespace, std::string name);
	int getDepth();
	int getEventType();
	int getLineNumber();
	std::string getName();
	std::string getText();
	const char* getTextCharacters(int holderForStartAndLength[]);
	std::string getNamespace();
	std::string getPrefix();
	std::string getPositionDescription();
	int getNamespaceCount(int depth);
	std::string getNamespacePrefix(int pos);
	std::string getNamespaceUri(int pos);

	/////////////////////////////////// attributes
	std::string getClassAttribute();
	std::string getIdAttribute();
	int getIdAttributeResourceValue;
	int getStyleAttribute();
	int getAttributeCount();
	std::string getAttributeNamespace(int index);
	std::string getAttributePrefix(int index);
	std::string getAttributeName(int index);
	int getAttributeNameResource(int index);
	int getAttributeValueType(int index);
	int getAttributeValueData(int index);
	std::string getAttributeValue(int index);
	bool getAttributeBooleanValue(int index, bool defaultValue);
	float getAttributeFloatValue(int index, float defaultValue);
	int getAttributeIntValue(int index, int defaultValue);
	int getAttributeUnsignedIntValue(int index, int defaultValue);
	int getAttributeResourceValue(int index, int defaultValue);
	std::string getAttributeValue(std::string _namespace, std::string attribute);
	bool getAttributeBooleanValue(std::string _namespace, std::string attribute, bool defaultValue);
	float getAttributeFloatValue(std::string _namespace, std::string attribute, float defaultValue);
	int getAttributeIntValue(std::string _namespace, std::string attribute, int defaultValue);
	int getAttributeUnsignedIntValue(std::string _namespace, std::string attribute, int defaultValue);
	int getAttributeResourceValue(std::string _namespace, std::string attribute, int defaultValue);
	int getAttributeListValue(int index, std::string options[], int defaultValue);
	int getAttributeListValue(std::string _namespace, std::string attribute, std::string options[], int defaultValue);
	std::string getAttributeType(int index);
	bool isAttributeDefault(int index);

	/////////////////////////////////// dummies
	void setInput(char* stream, std::string inputEncoding);
	std::string getInputEncoding();
	int getColumnNumber();
	bool isEmptyElementTag();
	bool isWhitespace();
	void defineEntityReplacementText(std::string entityName, std::string replacementText);
	std::string getNamespace(std::string prefix);
	void* getProperty(std::string name);
	void setProperty(std::string name, void* value);
	bool getFeature(std::string feature);
	void setFeature(std::string name, bool value);
	StringBlock* getStrings();
	///////////////////////////////////
	int getAttributeOffset(int index);
	int findAttribute(std::string _namespace, std::string attribute);
	void resetEventInfo();
	void doNext();

	/////////////////////////////////// data

	/*
	* All values are essentially indices, e.g. m_name is
	* an index of name in m_strings.
	*/
private:
	StringBlock* m_strings;
	IntReader* m_reader;
	bool m_operational = false;
	
	int* m_resourceIDs;
	bool m_decreaseDepth;

	int m_event;
	int m_lineNumber;
	int m_name;
	int m_namespaceUri;
	int* m_attributes;
	int m_idAttribute;
	int m_classAttribute;
	int m_styleAttribute;

	//static std::string E_NOT_SUPPORTED = "Method is not supported.";
	static const int
			ATTRIBUTE_IX_NAMESPACE_URI = 0,
			ATTRIBUTE_IX_NAME = 1,
			ATTRIBUTE_IX_VALUE_STRING = 2,
			ATTRIBUTE_IX_VALUE_TYPE = 3,
			ATTRIBUTE_IX_VALUE_DATA = 4,
			ATTRIBUTE_LENGHT = 5;

	static const int
			CHUNK_AXML_FILE = 0x00080003,
			CHUNK_RESOURCEIDS = 0x00080180,
			CHUNK_XML_FIRST = 0x00100100,
			CHUNK_XML_START_NAMESPACE = 0x00100100,
			CHUNK_XML_END_NAMESPACE = 0x00100101,
			CHUNK_XML_START_TAG = 0x00100102,
			CHUNK_XML_END_TAG = 0x00100103,
			CHUNK_XML_TEXT = 0x00100104,
			CHUNK_XML_LAST = 0x00100104;

	///////////////////////////////////////////// implementation

	/**
	* Namespace stack, holds prefix+uri pairs, as well as
	*  depth information.
	* All information is stored in one int[] array.
	* Array consists of depth frames:
	*  Data=DepthFrame*;
	*  DepthFrame=Count+[Prefix+Uri]*+Count;
	*  Count='count of Prefix+Uri pairs';
	* Yes, count is stored twice, to enable bottom-up traversal.
	* increaseDepth adds depth frame, decreaseDepth removes it.
	* push/pop operations operate only in current depth frame.
	* decreaseDepth removes any remaining (not pop'ed) _namespace pairs.
	* findXXX methods search all depth frames starting
	* from the last _namespace pair of current depth frame.
	* All functions that operate with int, use -1 as 'invalid value'.
	*
	* !! functions expect 'prefix'+'uri' pairs, not 'uri'+'prefix' !!
	*
	*/
public:
	static class NamespaceStack {
	public :
		NamespaceStack();
		void reset();
		int getTotalCount();
		int getCurrentCount();
		int getAccumulatedCount(int depth);
		void push(int prefix, int uri);
		bool pop(int prefix, int uri);
		bool pop();
		int getPrefix(int index);
		int getUri(int index);
		int findPrefix(int uri);
		int findUri(int prefix);
		int getDepth();
		void increaseDepth();
		void decreaseDepth();

		void ensureDataCapacity(int capacity);

		int find(int prefixOrUri, bool prefix);

		int get(int index, bool prefix);

	private:
		int* m_data;
		int* m_dataLength;
		int m_count;
		int m_depth;
	};
public:
	NamespaceStack* m_namespaces = new NamespaceStack();
};
