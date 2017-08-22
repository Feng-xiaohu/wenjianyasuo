#include <Windows.h>  
#include <stdio.h>  
#include "MyLZ77.h"  
  
  
void TestMyLZ77()  
{  
    //为了方便查看，使用字符串测试，MyLZ77Encode函数是支持二进制压缩的  
    char *pTestStr = "aacaacabcabaaac";  
    int testStrLen = strlen(pTestStr);  
    PLZ77_ENCODE_LIST header = MyLZ77Encode((unsigned char*)pTestStr,testStrLen);//最后一个字节看能会多出来，暂时未处理  
    PLZ77_ENCODE_LIST p = header;  
    if(header)  
    {  
         while(p)  
         {  
             printf("(%2d,%2d,%2c)\r\n",p->item->searchResult->pos,p->item->searchResult->matchLen,p->item->nextChar);    
             p = p->next;  
         }  
    }  
    else  
    {  
        printf("Error: MyLZ77Encode return NULL\r\n");  
    }  
  
    char decodeBuf[1024] = {'\0'};  
    int decodeLen = MyLZ77Decode(header,(unsigned char*)decodeBuf); //最后一个字节看能会多出来，暂时未处理  
  
    if(decodeLen != testStrLen)  
    {  
        printf("Error:Decode len is not match data len before encode.May be add a byte in the end!!!\r\n");  
    }  
  
    if(strncmp(pTestStr,decodeBuf,testStrLen))  
    {  
        printf("Error:Decode data is not match data len before encode\r\n");  
    }  
    else  
    {  
        printf("Decode Success!!!\r\n");  
    }  
}  
  
  
int main(int agrc,char* argv[])  
{  
    TestMyLZ77();  
  
    getchar();  
    return 0;  
}
