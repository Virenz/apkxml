#pragma once

#include <string>
/*@
* Global variables
*/
extern std::string	minsdk;
extern std::string	maxsdk;
extern std::string	targetsdk;

extern std::string	package;
extern std::string	androidversioncode;
extern std::string	androidversionname;

extern std::string	activity_list[1024];
extern std::string	mainact;

extern std::string service_list[1024];

extern std::string provider_list[1024];

extern std::string receiver_list[1024];

extern std::string library_list[1024];

void ManifestData(std::string manifest_path);