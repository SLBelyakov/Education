#include<TCHAR.H>
#include<atlstr.h>
#include <fcntl.h>   
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <share.h>
#include <vector>

#include "CSubtractorSentencesByWord.h"


CSubtractorSentencesByWord::CSubtractorSentencesByWord()
{
}

/* 
��������� ���� �� ����� � ���� � �� ��� ����������� �� ������ "�����". ������ ���� "report.txt". ���� ��������� ��� ������ �����������.
���������: 
const CString& strPath - ���� �� ���������� �����.
����������:
0 - ���� �� ������ �������
1 - ���� ��� ���� ������� ����, ������������� �� ����, ����������� ���������� ��� ���� ������� ����� ���.
� ������ ������ ������ � ������ ���������� ���������� errno ����� ���������� � ������������ � ���, ������� �� ������ �� ���������� �������/�������/�������������.
*/
int CSubtractorSentencesByWord::SubtractSentencesByWord(const CString& strPath) // exception
{

	int readableFileDescriptor = 0;
	int writableFileDescriptor = 0;

	TCHAR* tPathToFile = new TCHAR[strPath.GetLength() + 1];
	_tcscpy_s(tPathToFile, strPath.GetLength() + 1, strPath);

	if (_tsopen_s(&readableFileDescriptor, tPathToFile, _O_RDONLY | _O_U16TEXT, _SH_DENYWR, _S_IREAD))
	{
		perror("open failed on input file");
		delete[]tPathToFile;
		return(1);
	}
	if (_tsopen_s(&writableFileDescriptor, _T("report.txt"), _O_RDWR | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE))
	{
		perror("open failed on output file");
		delete[]tPathToFile;
		return(1);
	}


	TCHAR tLetter = ' ';
	TCHAR *tKeyWord = _T("�����");
	TCHAR *tUpperKeyWord = _T("�����");
	int LetterCounter = 0;
	bool isSentenceContainKeyWord = 0;
	bool isSentenceContainLetterSequence = 0;
	std::vector<TCHAR> Sentence;


	while (_read(readableFileDescriptor, &tLetter, sizeof(TCHAR)))
	{
		
		Sentence.push_back(tLetter);
		if (tLetter == _T('.') | tLetter == TCHAR(_T('!')) | tLetter == TCHAR(_T('?')) | tLetter == TCHAR(_T(';')))
		{
			if (isSentenceContainKeyWord | isSentenceContainLetterSequence)
			{
				_write(writableFileDescriptor, &Sentence[0], Sentence.size() * sizeof(TCHAR));

			}
			LetterCounter = 0;
			isSentenceContainKeyWord = false;
			isSentenceContainLetterSequence = false;
			Sentence.clear();

		}
		else if (!isSentenceContainKeyWord)
		{
			if (!isSentenceContainLetterSequence)
			{
				if (tLetter == tKeyWord[LetterCounter] | tLetter == (tUpperKeyWord[LetterCounter]))
				{
					if (LetterCounter == _tcslen(tKeyWord)-1)
					{
						LetterCounter = 0;
						isSentenceContainLetterSequence = true;
					}
					else
					{
						LetterCounter++;
					}
				}
			}
			else
			{
				if (tLetter == _T(' ') | tLetter == _T(',') | tLetter == _T(':') | tLetter == _T('-') | tLetter == _T("'")[0] | tLetter == _T(' " '))
				{
					isSentenceContainKeyWord = true;
				}
				else
				{
					LetterCounter = 0;
					isSentenceContainLetterSequence = false;
				}
			}

		}




	}

	_close(readableFileDescriptor);
	_close(writableFileDescriptor);
	delete[]tPathToFile;
	return 0;
}