#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "appdefine.h"
#include "heap.h"
#include "huffman.h"
#define NODENUM (2*(LEAFNUM)-2)  

typedef struct HTNODE //结点型
{
    int weight;
	int lchild;
	int rchild;
	int parent;
	char ch;
}HuffmanT;

typedef struct CodeNode
{
   char ch;
   char bits[LEAFNUM+1];
}HuffmanCode;

HuffmanT T[NODENUM];  //定义全局变量霍夫曼树
HuffmanCode H[LEAFNUM];

using namespace std;  

//初始霍夫曼树
void InitHT()
{
   for(int i = 0; i< NODENUM;i++)
	   T[i].weight = T[i].lchild = T[i].parent = T[i].rchild = -1;
   for(int i = 1; i< LEAFNUM;i++)
	   T[i].ch = static_cast<char>(static_cast<char>(i)-129);
}

void CreartHT()  //构造霍夫曼树，T[m-1]为其根
{
    int p1,p2;
	InitHP();
	for(int i =1; i<LEAFNUM &&!HeapFulls();i++)
	{
	   if(T[i].weight>0)
		   Push(i,T[i].weight);
	}
	for(int i=LEAFNUM; i<NODENUM;i++)
	{
	   SelectMin(p1,p2);   //求俩个最小值
	   if(!(p1&p2)) return ;
	   T[p1].parent = T[p2].parent = i;
	   T[i].lchild = p1;
	   T[i].rchild = p2;
	   T[i].weight =T[p1].weight + T[p2].weight;
	   Push(i, T[i].weight);
	}
}

void Encoding()  //求霍夫曼树编码
{
    int c, p,i;   //c表示T中孩子位置  p表示T中双亲的位置
	char cd[LEAFNUM+1]; //临时存放编码
	int start;  //编码在cd中的位置
	cd[LEAFNUM] = '\0';//编码结束
	for(i = 0;i<LEAFNUM; i++)
	{
	  H[i].bits[0]='\0';
	} 
	for(i=0; i<LEAFNUM; i++)  //依次求叶子T[i]的编码
	{
	 H[i].ch = static_cast<char>(i-128);  //读叶子T[i]对应的字符
		 start =LEAFNUM;//编码起始位置的初值
	    c = i+1;//从叶子T[i+1]开始上溯，因为T是从T开始存的
		while((p = T[c].parent)>=0)  //直到上溯到T[c]是根位置
		{
			cd[--start] = (T[p].lchild==c)?'0':'1'; //若T[c]是T[p]的左孩子，则生成代码0，否则生成代码1
            c = p;  //继续上溯
		}
		strcpy(H[i].bits, &cd[start]);  //复制编码为串于编码表
	}
}

bool Compress(int argc, char** argv)  //压缩
{
    ifstream fin;
	ofstream fout;
	string optfilepath(argv[2]);
    optfilepath.append(".wz");
	fout.open(optfilepath.c_str(), ios_base::out|ios_base::trunc|ios_base::binary);
    if(!fout.is_open())
	{
	  cout<<"这个文件不可以打开!\n";
	  return false;
	}
	fout<<".wz"<<' '; //写入文件格式
	fout<<argc-3<<' ';//写入总文件个数
	for(int argi = 3;argi<argc;argi++)
	{
	   string filepath(argv[argi]);
	   cout<<filepath<<"   -->";
	   fin.open(filepath.c_str(),ios_base::in|ios_base::binary);
       if(!fin)
	   {
	     cout<< "错误，无法打开"<<filepath<<"  file."<<endl;
         return false;
	   }
	   //原来打开目标文件的位置
	  char ch;
	  if(fin.eof())
		  fin.clear();
	  fin.seekg(0);
	  int total = 0;
	  	InitHT(); //初始化Huffman树
		while (fin.get(ch))		//这里能读出来
		{
			total++;
			int index;
			index = static_cast<int>(ch)+129;
			if (0<index && index<LEAFNUM)
			{
				if (T[index].weight==-1) T[index].weight = 0; //如果没有计数此字符，则初始化为0；
				T[index].weight++;
			}
		}
		fout << filepath << ' '; //把文件名写入头部
		fout << total << ' ';
		CreartHT();
		Encoding(); //建编码
		for (int i=LEAFNUM; i<NODENUM; i++)
			fout << T[i].lchild << ' ' << T[i].rchild << ' ';
		if (fin.eof())
			fin.clear();
		char cur, des;
		des ^= des; //置0
		int count = 0;
		fin.seekg(0);
		while (fin.get(ch))
		{
			for (int i=0; i<LEAFNUM && H[ch+128].bits[i] != '\0'; i++)
			{
				cur = H[ch+128].bits[i];
				des <<= 1; //左移一位
				count++;
				if (cur=='1')
					des |= 1; //置最低位为1

				if (count==8) //满8b了，写入文件
				{
					fout << des;
					count = 0;
					des ^= des; //置0
				}
			}
		}
		if (count>0) //处理最后一个字节
		{
			des <<= (8-count);
			fout << des;
		}
		cout << " complated.\n";
		fin.clear();
		fin.close();
	} //end of for

	fout.close();
	return true;
}
//解压
bool Decompress(int argc, char** argv)
{

	ifstream fin;
	ofstream fout;
	int root=1, bit, leaf;
	int total = 0, curCount = 0;
	char ch;
	
	string filepath(argv[2]);
	fin.open(filepath.c_str(), ios_base::in|ios_base::binary);
	if (!fin.is_open())
	{
		cout << "Source file can't be open!\n";
		return false;
	}
	string ff("\0");
	int filenum;
	fin.seekg(0);
	fin >> ff; //文件格式
	fin >> filenum;
	//cout << ff << "  " << filenum << endl;
	for (int n=0; n<filenum; n++)
	{
		fin >> filepath;
		fin >> total;
		cout << filepath << "  --> ";
		fout.clear();
		fout.open(filepath.c_str(), ios_base::out|ios_base::trunc|ios_base::binary);
		fout.clear();
		if (!fout.is_open())
		{
			cout << "Objective file can't be open!\n";
			return false;
		}
		InitHT();
		for (int i=LEAFNUM; i<NODENUM; i++)
		{
			fin >> T[i].lchild;
			fin >> T[i].rchild;
		}
		for (root=NODENUM-1; root>0; root--)
		{
			if (T[root].rchild!=-1 && T[root].rchild!=-1 && T[root].parent==-1)
				break;
		}
		leaf = root; //找到根节点
		//cout << "leaf:" << leaf << endl;
		fin.get(ch); //把空格读入
		curCount = 0;
		while(curCount<total && fin.get(ch))
		{
			bit ^= bit;
			for (int i=0; i<8; i++)
			{
				if (leaf>0)
				{
					bit = ((ch<0)?1:0); //取最高位
					ch <<= 1;
					if (bit)//是1，右
						leaf = T[leaf].rchild;
					else
						leaf = T[leaf].lchild;
					if (T[leaf].lchild==-1 && T[leaf].rchild==-1 && curCount<total)
					{
						char c = T[leaf].ch;
						fout << c;
						curCount++;
						leaf = root;
						//cout << "leaf:" << leaf << endl;
					}
					//else
					//	cout << T[leaf].lchild << ' ' << T[leaf].rchild << ' ' << curCount << endl;
				}
				else
				{
					cout << "Leaf position error!\n";
					fin.close();
					fout.close();
					return false;
				}
			}
		}
		fout.clear();
		fout.close();
		cout << " complated.\n";
	}
	
	fin.close();
	return true;
}
	


