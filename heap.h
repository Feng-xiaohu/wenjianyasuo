#define _CRT_SECURE_NO_WARNINGS 

#ifndef _MINHEAP_H_
#define _MINHEAP_H_

void InitHP();//初始化堆
bool HeapFulls();//判断堆是否满
bool IsLeaf(int pos);//判断原始数是否是叶子
void Swap(int pos1, int pos2); //交换堆中俩个元素
bool SiftUp(int pos); // 将元素从下往上移，直到正确的位置
bool SiftDown(int pos);//将元素从上往下移，直到正确的位置
void Push(int pos,int weight); //压栈
int Pop();  //弹栈
void SelectMin(int &x, int &y);  //求俩个最小值
void BuildMinHeap(int pos,int weight);//建堆

#endif
