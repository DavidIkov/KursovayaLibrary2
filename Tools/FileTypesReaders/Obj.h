#pragma once
#include"DLL.h"
#include<vector>
#include<string>
//todo made a fucking good .obj reader, not this slow bullshit
//will return array of floats, first 3 will be position and another 3 will be normal
DLLTREATMENT std::vector<float> ReadObjFileType(const char* filePath);