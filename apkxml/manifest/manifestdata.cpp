#include <iostream>
#include <string>
#include "manifestdata.h"
#include "../tinyxml2/tinyxml2.h"

using namespace std;

void ManifestData(char* manifest_path)
{
	tinyxml2::XMLDocument doc;
	
	if (doc.LoadFile(manifest_path) != 0)
		return;
	tinyxml2::XMLElement* root = doc.FirstChildElement();

	/* MANIFEST */
	printf("manifest\n");
	if (root)
	{
		const char* value = root->Attribute("package");
		printf("package : %s\n", value);

		value = root->Attribute("android:versionCode");
		printf("android:versionCode : %s\n", value);

		value = root->Attribute("android:versionName");
		printf("android:versionName : %s\n", value);
	}

	/* SDK */
	printf("uses-sdk\n");
	tinyxml2::XMLElement *surface = root->FirstChildElement("uses-sdk");
	if (surface)
	{
		const char* value = surface->Attribute("android:minSdkVersion");
		printf("android:minSdkVersion : %s\n", value);

		value = surface->Attribute("android:maxSdkVersion");
		printf("android:maxSdkVersion : %s\n", value);

		value = surface->Attribute("android:targetSdkVersion");
		printf("android:targetSdkVersion : %s\n", value);
	}

	/* ACTIVITY */
	printf("activity\n");
	tinyxml2::XMLElement *activity = root->FirstChildElement("application")->FirstChildElement("activity");
	while (activity)
	{
		const char* value = activity->Attribute("android:name");
		printf("android:name : %s\n", value);

		tinyxml2::XMLElement* actions = activity->FirstChildElement("intent-filter")->FirstChildElement("action");
		printf("\t action\n");
		value = actions->Attribute("android:name");
		printf("\t\t android:name : %s\n", value);
		
		activity = activity->NextSiblingElement();
	}
	
	/* SERVICE */
	printf("service\n");
	tinyxml2::XMLElement *service = root->FirstChildElement("service");
	while (service)
	{
		const char* value = service->Attribute("android:name");
		printf("android:name : %s\n", value);

		service = service->NextSiblingElement();
	}
	
	/* PROVIDER */
	printf("provider\n");
	tinyxml2::XMLElement *provider = root->FirstChildElement("provider");
	while (provider)
	{
		const char* value = provider->Attribute("android:name");
		printf("android:name : %s\n", value);

		provider = provider->NextSiblingElement();
	}
	
	/* RECERVER */
	printf("receiver\n");
	tinyxml2::XMLElement *receiver = root->FirstChildElement("receiver");
	while (receiver)
	{
		const char* value = receiver->Attribute("android:name");
		printf("android:name : %s\n", value);

		receiver = receiver->NextSiblingElement();
	}
	
	/* LIBRARY */
	printf("uses-library\n");
	tinyxml2::XMLElement *useslibrary = root->FirstChildElement("uses-library");
	while (useslibrary)
	{
		const char* value = useslibrary->Attribute("android:name");
		printf("android:name : %s\n", value);

		useslibrary = useslibrary->NextSiblingElement();
	}
	
	/* PERMISSION */
	printf("uses-permission\n");
	tinyxml2::XMLElement *usespermission = root->FirstChildElement("uses-permission");
	while (usespermission)
	{
		const char* value = usespermission->Attribute("android:name");
		printf("android:name : %s\n", value);

		usespermission = usespermission->NextSiblingElement();
	}
}