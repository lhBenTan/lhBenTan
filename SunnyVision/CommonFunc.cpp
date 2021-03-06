#include <iostream>

/// 释放内存
bool _stdcall ReleaseMemUseFree(unsigned char* buf)
{
	if (nullptr == buf)
	{
		return false;
	}
	free(buf);
	buf = nullptr;
	return true;
}

/// 释放new成员时产生的内存
bool _stdcall ReleaseMemUseDelete(unsigned char* pArray)
{
	if (nullptr == pArray)
	{
		return false;
	}
	delete[] pArray;
	pArray = nullptr;
	return true;
}