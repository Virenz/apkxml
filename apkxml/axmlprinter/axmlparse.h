#ifndef __AXMLPARSE_H__
#define __AXMLPARSE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum AttrType {
	ATTR_NULL = 0,
	ATTR_REFERENCE = 1,
	ATTR_ATTRIBUTE = 2,
	ATTR_STRING = 3,
	ATTR_FLOAT = 4,
	ATTR_DIMENSION = 5,
	ATTR_FRACTION = 6,
	ATTR_FIRSTINT = 16,
	ATTR_DEC = 16,
	ATTR_HEX = 17,
	ATTR_BOOLEAN = 18,
	ATTR_FIRSTCOLOR = 28,
	ATTR_ARGB8 = 28,
	ATTR_RGB8 = 29,
	ATTR_ARGB4 = 30,
	ATTR_RGB4 = 31,
	ATTR_LASTCOLOR = 31,
	ATTR_LASTINT = 31,
};

enum TagType {
	TAG_START, TAG_END,
};

///�ļ�ͷ
typedef struct FileHeader {
	int32_t magicNumber;
	int32_t fileSize;
} FileHeader;

//string in stringPool
typedef struct StringItem {
	int16_t charNum;
	int16_t *chars;
}StringItem;

///String Chunk
typedef struct StringChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t stringCount;
	int32_t styleCount;
	int32_t unknown;
	int32_t stringPoolOffset;
	int32_t stylePoolOffset;
	int32_t *stringOffsets;
	int32_t *styleOffsets;
	StringItem* stringPool;
	/*�����һ��stylePool,��ʵ������յ�*/
}StringChunk;

typedef struct ResourceIDChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t *resourceIDs;
}ResourceIDChunk;

typedef struct StartNamespaceChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t lineNum;
	int32_t unknown;
	int32_t prefix;
	int32_t uri;
}StartNamespaceChunk;

typedef struct EndNamespaceChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t lineNum;
	int32_t unknown;
	int32_t prefix;
	int32_t uri;
}EndNamespaceChunk;

typedef struct AttrItem {
	int32_t spaceUri;
	int32_t name;
	int32_t valueString;
	int32_t type; //ʵ��ֵΪ������24λ
	int32_t data;
}AttrItem;

typedef struct StartTagChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t lineNum;
	int32_t unknown;
	int32_t nameSpaceUri;
	int32_t name;///��ǩ����(���ַ����е�����ֵ)
	int32_t flags;///��ǩ������,�����ǿ�ʼ��ǩ���ǽ�����ǩ��
	int32_t atrrCount;
	int32_t classAtrr;///��ǩ������������
	AttrItem *attrs;
}StartTagChunk;

typedef struct EndTagChunk {
	int32_t chunkType;
	int32_t chunkSize;
	int32_t lineNum;
	int32_t unknown;
	int32_t nameSpaceUri;
	int32_t name;
}EndTagChunk;

typedef struct XMlContentChunk {
	int type; ///1����startTag  0����endTag
	void *TagChunk;
	struct XMlContentChunk *next;
	struct XMlContentChunk *prev;
}XMlContentChunk;

typedef struct _MainfestXML {
	FileHeader header;
	StringChunk stringChunk;
	ResourceIDChunk resourceIDChunk;
	StartNamespaceChunk startNamespaceChunk;
	XMlContentChunk xmlContentChunk;///һ������Ԫ�ر�ǩ������
	EndNamespaceChunk endNamespaceChunk;
}MainfestXML;

void parseFileHearder();
void parseStringChunk();
void printStringItem(StringItem*);
void parseResourceIDChunk();
void parseStartNamespaceChunk();
void parseEndNamespaceChunk();
void parseStartTagChunk();
char* getAttrType(int32_t);
void parseEndTagChunk();
void addTagChunk(int, void*);
void parseXmlContent();
void writeFormatXmlToFile();
void freeMainfestXML();
void freeXMlContentChunk(XMlContentChunk*);

void initFile(int, char *path);
void freeFile();

#endif