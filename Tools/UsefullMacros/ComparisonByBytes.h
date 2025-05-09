#pragma once
#define ComparisonByBytesMacro(nameOfClass, comparisonOper)\
inline bool operator comparisonOper(const nameOfClass& inst) const {\
	for (unsigned int i = 0; i < sizeof(nameOfClass); i++)\
		if (!(i[(unsigned char*)this] comparisonOper i[(unsigned char*)&inst])) return false;\
			return true; }
