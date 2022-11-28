#pragma once

#include "json.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <assert.h>

void loadJsonFile(Json::Value& val, std::string path);

void loadJsonString(Json::Value& val, const std::string& document);
