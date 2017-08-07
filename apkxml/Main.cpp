#include <stdio.h>
#include <io.h>
#include <fstream>
#include <direct.h>

#include "zlib/zlib.h"
#include "zlib/unzip.h"
#include "manifest\manifestdata.h"

using namespace std;
#pragma comment (lib,"zlibwapi.lib")

int GetManifest(string java_path, string jar_path, string APP_path)
{
	FILE    * f;
	char    buff[1024];
	string xml_path = APP_path + "AndroidManifest.xml";
	memset(buff, '\0', sizeof(buff));
	string cmd = java_path + " -jar " + jar_path + " " + xml_path;
	f = _popen(cmd.c_str(), "r");
	if (f == NULL)
	{
		return(-1);
	}
	fstream file;
	file.open((APP_path + "manifest.xml").c_str(), ios_base::out);
	while (fgets(buff, sizeof(buff), f) != NULL)
	{
		file.write(buff, strlen(buff));
	}
	file.close();
	_pclose(f);

	return(0);
}

int createMultipleDirectory(char* file_path)
{
	int i = 0;
	int iRet;
	int iLen = strlen(file_path);

	// 创建目录  
	for (i = 0; i < iLen; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			file_path[i] = '\0';

			//如果不存在,创建  
			iRet = _access(file_path, 0);
			if (iRet != 0)
			{
				iRet = _mkdir(file_path);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/  
			file_path[i] = '/';
		}
	}
	return 0;
}

int extract_currentfile(unzFile zfile, char * extractdirectory)
{
	unsigned int fileName_BufSize = 512;
	char *fileName_WithPath = new char[fileName_BufSize];
	char *p, *fileName_WithoutPath;
	unz_file_info64 zFileInfo;
	p = fileName_WithoutPath = fileName_WithPath;
	if (UNZ_OK != unzGetCurrentFileInfo64(zfile,
		&zFileInfo, fileName_WithPath,
		fileName_BufSize, NULL, 0, NULL, 0))
	{
		printf("[ERROR] 获取当前文件信息失败\n");
		return -1;
	}
	char *temp = new char[fileName_BufSize];
	//修改fileName_WithPath，使得extractdirectory加在其前面
	strcpy_s(temp, 512, extractdirectory);
	strcat_s(temp, 512, fileName_WithPath);
	fileName_WithPath = temp;
	//判断当前文件是目录还是文件
	while ((*p) != '\0')
	{
		if (((*p) == '/') || ((*p) == '\\'))
		{
			fileName_WithoutPath = p + 1;
		}
		p++;
	}
	if (*fileName_WithoutPath == '\0')
	{
		printf("[INFO] 成功读取当前目录:%s\n", fileName_WithPath);
		printf("[INFO] 开始创建目录:%s\n", fileName_WithPath);
		//创建目录
		int err = _mkdir(fileName_WithPath);
		if (err != 0)
			printf("[ERROR] 创建目录%s失败\n", fileName_WithPath);
		else
			printf("[INFO] 成功创建目录%s\n", fileName_WithPath);
	}
	else
	{
		printf("[INFO] 成功读取当前文件: %s\n", fileName_WithoutPath);
		printf("[INFO] 开始解压当前文件: %s\n",fileName_WithoutPath);
		//打开当前文件
		if (UNZ_OK != unzOpenCurrentFile(zfile))
		{
			//错误处理信息  
			printf("[ERROR] 打开当前文件%s失败", fileName_WithoutPath);
		}
		//定义一个fstream对象，用来写入文件
		fstream file;
		createMultipleDirectory(fileName_WithPath);
		file.open(fileName_WithPath, ios_base::out | ios_base::binary);
		ZPOS64_T fileLength = zFileInfo.uncompressed_size;
		//定义一个字符串变量fileData，读取到的文件内容将保存在该变量中
		char *fileData = new char[fileLength];
		//解压缩文件  
		ZPOS64_T err = unzReadCurrentFile(zfile, (voidp)fileData, fileLength);
		if (err < 0)
			printf("[ERROR] 解压当前文件%s失败\n", fileName_WithoutPath);
		else
			printf("[INFO] 解压当前文件%s成功\n", fileName_WithoutPath);
		file.write(fileData, fileLength);
		file.close();
		free(fileData);
	}
	return 0;
}

int main()
{
	unzFile zfile;//定义一个unzFile类型的结构体zfile
				  //定义zip文件的路径，可以使用相对路径
	char filepath[] = "83fba62fbb414fa09c2110e2b426501e1859a046.apk";
	//调用unzOpen64()函数打开zip文件
	zfile = unzOpen64(filepath);
	if (zfile == NULL)
	{
		printf("%s [INFO] 打开压缩文件失败\n", filepath);
		return -1;
	}
	else
	{
		printf("[INFO] 成功打开压缩文件\n");
	}
	unz_global_info64 zGlobalInfo;
	//unz_global_info64是一个结构体
	//其中最重要的是number_entry成员
	//这个变量表示了压缩文件中的所有文件数目（包括文件夹、
	//文件、以及子文件夹和子文件夹中的文件）
	//我们用这个变量来循环读取zip文件中的所有文件
	if (UNZ_OK != unzGetGlobalInfo64(zfile, &zGlobalInfo))
		//使用unzGetGlobalInfo64函数获取zip文件全局信息
	{
		printf("[ERROR] 获取压缩文件全局信息失败\n");
		return -1;
	}
	//循环读取zip包中的文件，
	//在extract_currentfile函数中
	//进行文件解压、创建、写入、保存等操作
	for (int i = 0; i < zGlobalInfo.number_entry; i++)
	{
		//extract_currentfile函数的第二个参数指将文件解压到哪里
		//这里使用extract/，表示将其解压到运行目录下的extract文件夹中
		int err = extract_currentfile(zfile, "extract/");
		//关闭当前文件
		unzCloseCurrentFile(zfile);
		//使指针指向下一个文件
		unzGoToNextFile(zfile);
	}
	//关闭压缩文件
	unzClose(zfile);

	//GetManifest("java", "Tools/AXMLPrinter2.jar", "extract/");
	ManifestData("extract/manifest.xml");

	system("pause");
	return 0;
}