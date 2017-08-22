#ifndef  _HUDDMAN_T_
#define  _HUFFMAN_T_

//初始霍夫曼树
void InitHT();
//构造huffman树，T[m-1]为其根
void CreartHT();
//编码
void Encoding();
//压缩
bool Compress(int argc, char** argv);
//解压
bool Decompress(int argc, char** argv); 

#endif
