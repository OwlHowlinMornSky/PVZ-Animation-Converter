#include "jsonio.h"

void loadJsonFile(Json::Value& val, std::string path) {
	 //printf(">>>>load %s\n", path.c_str());
	 std::ifstream ifs;
	 ifs.open(path);
	 assert(ifs.is_open());
	 Json::Reader reader;
	 if (!(reader.parse(ifs, val, false))) {
		 printf_s("main::readFile failed!\n");
	 }
	 ifs.close();
	 return;
}

void loadJsonString(Json::Value& val, const std::string& document) {
	Json::Reader reader;
	if (!(reader.parse(document, val, false))) {
		printf_s("main::readFile failed!\n");
	}
	return;
}
