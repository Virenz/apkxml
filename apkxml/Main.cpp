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

	// ����Ŀ¼  
	for (i = 0; i < iLen; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			file_path[i] = '\0';

			//���������,����  
			iRet = _access(file_path, 0);
			if (iRet != 0)
			{
				iRet = _mkdir(file_path);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//֧��linux,������\����/  
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
		printf("[ERROR] ��ȡ��ǰ�ļ���Ϣʧ��\n");
		return -1;
	}
	char *temp = new char[fileName_BufSize];
	//�޸�fileName_WithPath��ʹ��extractdirectory������ǰ��
	strcpy_s(temp, 512, extractdirectory);
	strcat_s(temp, 512, fileName_WithPath);
	fileName_WithPath = temp;
	//�жϵ�ǰ�ļ���Ŀ¼�����ļ�
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
		printf("[INFO] �ɹ���ȡ��ǰĿ¼:%s\n", fileName_WithPath);
		printf("[INFO] ��ʼ����Ŀ¼:%s\n", fileName_WithPath);
		//����Ŀ¼
		int err = _mkdir(fileName_WithPath);
		if (err != 0)
			printf("[ERROR] ����Ŀ¼%sʧ��\n", fileName_WithPath);
		else
			printf("[INFO] �ɹ�����Ŀ¼%s\n", fileName_WithPath);
	}
	else
	{
		printf("[INFO] �ɹ���ȡ��ǰ�ļ�: %s\n", fileName_WithoutPath);
		printf("[INFO] ��ʼ��ѹ��ǰ�ļ�: %s\n",fileName_WithoutPath);
		//�򿪵�ǰ�ļ�
		if (UNZ_OK != unzOpenCurrentFile(zfile))
		{
			//��������Ϣ  
			printf("[ERROR] �򿪵�ǰ�ļ�%sʧ��", fileName_WithoutPath);
		}
		//����һ��fstream��������д���ļ�
		fstream file;
		createMultipleDirectory(fileName_WithPath);
		file.open(fileName_WithPath, ios_base::out | ios_base::binary);
		ZPOS64_T fileLength = zFileInfo.uncompressed_size;
		//����һ���ַ�������fileData����ȡ�����ļ����ݽ������ڸñ�����
		char *fileData = new char[fileLength];
		//��ѹ���ļ�  
		ZPOS64_T err = unzReadCurrentFile(zfile, (voidp)fileData, fileLength);
		if (err < 0)
			printf("[ERROR] ��ѹ��ǰ�ļ�%sʧ��\n", fileName_WithoutPath);
		else
			printf("[INFO] ��ѹ��ǰ�ļ�%s�ɹ�\n", fileName_WithoutPath);
		file.write(fileData, fileLength);
		file.close();
		free(fileData);
	}
	return 0;
}

int main()
{
	unzFile zfile;//����һ��unzFile���͵Ľṹ��zfile
				  //����zip�ļ���·��������ʹ�����·��
	char filepath[] = "83fba62fbb414fa09c2110e2b426501e1859a046.apk";
	//����unzOpen64()������zip�ļ�
	zfile = unzOpen64(filepath);
	if (zfile == NULL)
	{
		printf("%s [INFO] ��ѹ���ļ�ʧ��\n", filepath);
		return -1;
	}
	else
	{
		printf("[INFO] �ɹ���ѹ���ļ�\n");
	}
	unz_global_info64 zGlobalInfo;
	//unz_global_info64��һ���ṹ��
	//��������Ҫ����number_entry��Ա
	//���������ʾ��ѹ���ļ��е������ļ���Ŀ�������ļ��С�
	//�ļ����Լ����ļ��к����ļ����е��ļ���
	//���������������ѭ����ȡzip�ļ��е������ļ�
	if (UNZ_OK != unzGetGlobalInfo64(zfile, &zGlobalInfo))
		//ʹ��unzGetGlobalInfo64������ȡzip�ļ�ȫ����Ϣ
	{
		printf("[ERROR] ��ȡѹ���ļ�ȫ����Ϣʧ��\n");
		return -1;
	}
	//ѭ����ȡzip���е��ļ���
	//��extract_currentfile������
	//�����ļ���ѹ��������д�롢����Ȳ���
	for (int i = 0; i < zGlobalInfo.number_entry; i++)
	{
		//extract_currentfile�����ĵڶ�������ָ���ļ���ѹ������
		//����ʹ��extract/����ʾ�����ѹ������Ŀ¼�µ�extract�ļ�����
		int err = extract_currentfile(zfile, "extract/");
		//�رյ�ǰ�ļ�
		unzCloseCurrentFile(zfile);
		//ʹָ��ָ����һ���ļ�
		unzGoToNextFile(zfile);
	}
	//�ر�ѹ���ļ�
	unzClose(zfile);

	//GetManifest("java", "Tools/AXMLPrinter2.jar", "extract/");
	ManifestData("extract/manifest.xml");

	system("pause");
	return 0;
}