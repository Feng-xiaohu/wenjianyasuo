#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <string.h>
#include "heap.h"
#include "huffman.h"

using namespace std;

//第一个参数，源文件路径
//第二个参数，目标文件路径（压缩时不要）
int main(int argc, char** argv)
{
	bool flag = false;
	if (!strcmp(argv[1],"-c"))
	{
		cout << "Compressing...\n";
		time_t timeStart = time(NULL);
		flag = Compress(argc, argv);
		time_t timeEnd = time(NULL);
		time_t min = (timeEnd-timeStart)/60;
		time_t sec = (timeEnd-timeStart)%60;
		if (flag)
			cout << "Compress succeed! " << "It cost " << min << " minutes " << sec <<" seconds.\n";
		else
			cout << "Compress failed!\n";
	}
	else if (!strcmp(argv[1],"-d"))
	{
		cout << "Decompressing...\n";
		time_t timeStart = time(NULL);
		flag = Decompress(argc, argv);
		time_t timeEnd = time(NULL);
		time_t min = (timeEnd-timeStart)/60;
		time_t sec = (timeEnd-timeStart)%60;
		if (flag)
			cout << "Decompress succeed! " << "It cost " << min << " minutes " << sec <<" seconds.\n";
		else
			cout << "Decompress failed!\n";
	}
	else
		cout << "Arguments error!\n";
	return 0;
}
