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
Принимает путь до файла и ищет в нём все предложения со словом "почта". Создаёт файл "report.txt". куда сохраняет все данные предложения.
параметры: 
const CString& strPath - путь до текстового файла.
Возвращает:
0 - если всё прошло успешно
1 - если нет прав открыть файл, расположенный по пути, переданному аргументом или если данного файла нет.
В случае ошибки работы с файлом глобальная переменная errno будет возвращена с комментарием о том, который из файлов не получилось открыть/создать/редактировать.
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
	TCHAR *tKeyWord = _T("почта");
	TCHAR *tUpperKeyWord = _T("ПОЧТА");
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