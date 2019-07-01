#pragma once
#include <atlstr.h>

ref class CSubtractorSentencesByWord
{
public:
	CSubtractorSentencesByWord();

	int SubtractSentencesByWord(const CString& strPath); // exception
};

