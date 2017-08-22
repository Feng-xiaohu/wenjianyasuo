#define _CRT_SECURE_NO_WARNINGS 
#include "appdefine.h"
#include "heap.h"
typedef struct
{  /* 结点型*/
	int weight ; /* 权值*/
	int pos;
}HeapNode;
typedef struct MinHeapNode
{
	HeapNode Elem[LEAFNUM+2];
	int size;
}MinHeap;
MinHeap heap;

//初始化堆
void InitHP()
{
	for (int i=0; i<=LEAFNUM+1; i++)
	{
		heap.Elem[i].weight = heap.Elem[i].pos = -1;
		heap.Elem[i].pos = -1;
	}
	heap.size = 0;
}
//判断堆是否满
bool HeapFulls()
{
	return (heap.size >= LEAFNUM-1);
}
//判断原始数否是叶子
bool IsLeaf(int pos)
{
	return (pos>heap.size/2) && (pos<=heap.size);
}
//交换堆中两个元素
void Swap(int pos1, int pos2)
{
	HeapNode temp;
	temp.pos = heap.Elem[pos1].pos;
	temp.weight = heap.Elem[pos1].weight;
	heap.Elem[pos1].pos = heap.Elem[pos2].pos;
	heap.Elem[pos1].weight = heap.Elem[pos2].weight;
	heap.Elem[pos2].pos = temp.pos;
	heap.Elem[pos2].weight = temp.weight;
}
//将元素从下往上移，直到正确的位置
bool SiftUp(int pos) //这里
{
	if(pos<=0 || pos>LEAFNUM+1) return false;
	int parent = pos/2;
	while (pos!=1 && heap.Elem[parent].weight > heap.Elem[pos].weight)
	{ //pos=1时，已经在最顶端了。
		Swap(pos, parent);
		pos = parent;
		parent = parent/2;
	}
	return true;
}
//将元素从上往下移，直到正确的位置
bool SiftDown(int pos)
{ //放元素在它正确的位置
	if((pos <= 0) && (pos>LEAFNUM+1))  return false;
	while (!IsLeaf(pos))
	{
		int child = 2*pos;
		if ((child<heap.size-1) && (heap.Elem[child].weight > heap.Elem[child+1].weight))
			child++; //使j是儿子中最小的那个
		if (heap.Elem[pos].weight <= heap.Elem[child].weight) return true;
		Swap(pos, child);
		pos = child;   //向下移
	}
	return true;
}
//压栈
void Push(int pos, int weight)
{
	int size = ++heap.size;
	heap.Elem[size].pos = pos;
	heap.Elem[size].weight = weight;
	SiftUp(size);
}
//弹栈
int Pop()
{
	if (heap.size<=0) return 0;
	int x = heap.Elem[1].pos;
	Swap(1, heap.size);
	heap.size--;
	if (heap.size>1) SiftDown(1);
	heap.Elem[heap.size+1].weight = heap.Elem[heap.size+1].pos = -1;
	return x;
}
//求两个最小值
void SelectMin (int &x, int &y)
{
	x = Pop();
	y = Pop();
}
//建堆
void BuildMinHeap(int pos, int weight)
{
	for (int i=1; i<=LEAFNUM && !HeapFulls(); i++)
		if (weight>0)
			Push(pos,weight);
}
