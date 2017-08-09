#pragma once
#include <string>

class AttributeData
{
public:
	AttributeData();
	~AttributeData();
	
	std::string getNameSpaceUri();
	std::string getName();
	std::string getData();

	int nameSpaceUri;
	int name;
	int valueString;
	int type;
	int data;
};

AttributeData::AttributeData()
{
}

AttributeData::~AttributeData()
{
}

inline std::string AttributeData::getNameSpaceUri()
{
	if (nameSpaceUri < 0) {
		return "";
	}
	return ParseChunkUtils.getStringContent(nameSpaceUri);
}

inline std::string AttributeData::getName()
{
	if (name < 0) {
		return "";
	}
	return ParseChunkUtils.getStringContent(name);
}

inline std::string AttributeData::getData()
{
	if (data < 0) {
		return "";
	}
	return ParseChunkUtils.getStringContent(data);
}
