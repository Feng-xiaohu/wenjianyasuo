#include <Windows.h>  
#include "MyLZ77.h"  
  
  
#define MY_LZ77_DICTIONARY_LEN 0x06 //动态字典的长度设置为6字节  
#define MY_LZ77_ENCODE_BUFFER_LEN 0x04 //动态编码的buf长度设备为4字节  
  
//更新链表  
PLZ77_ENCODE_LIST UpdataLZ77Link(PLZ77_ENCODE_LIST *lz77Header,PLZ77_ENCODE_LIST *pCur,PLZ77_ENCODE_ITEM item)  
{  
    PLZ77_ENCODE_LIST p = new LZ77_ENCODE_LIST;  
    if(!p)  
    {  
        return NULL;  
    }  
  
    p->item = item;  
    p->next = NULL;  
    if(!*pCur)  
    {  
        *pCur = p;  
        *lz77Header = p;  
    }  
    else  
    {  
        (*pCur)->next = p;  
        *pCur = p;  
    }  
  
    return p;  
}  
  
//通过m*n的时间复杂度搜索最长匹配，可以优化该函数  
PLZ77_ENCODE_SEARCH_RESULT MyLZ77Search(unsigned char *pDictionary,  
                                        unsigned short dictionarySize,  
                                        unsigned char* pEncodeBuffer,  
                                        unsigned short encodeBufferSize)  
{  
    PLZ77_ENCODE_SEARCH_RESULT searchResult =(PLZ77_ENCODE_SEARCH_RESULT)new LZ77_ENCODE_SEARCH_RESULT;  
    if(!searchResult)  
    {  
        return NULL;  
    }  
  
    //在字典中搜索  
    for(int i=0;i<dictionarySize;i++)  
    {  
        int pos = i;  
        int index = i;  
        for(int j=0;j<encodeBufferSize;j++)  
        {  
            if(pEncodeBuffer[j] == pDictionary[index])  
            {  
                ++index;  
                continue;  
            }  
            else  
            {  
                break;  
            }  
        }  
        int matchLen = index - i;  
        if(matchLen && matchLen>searchResult->matchLen)  
        {  
            searchResult->pos = dictionarySize-i;  
            searchResult->matchLen = matchLen;  
        }  
    }  
  
    return searchResult;  
}  
  
//标准的LZ77编码输出,这里使用链表保存编码的结果  
PLZ77_ENCODE_LIST MyLZ77Encode(unsigned char* data,int dataLen)  
{  
    if(!data || dataLen<=0)  
    {  
        return NULL;  
    }  
  
    int hasbeenEncodeSize = 0;//已经编码的数据大小  
  
    PLZ77_ENCODE_LIST lz77Header = NULL;  
    PLZ77_ENCODE_LIST pCur = NULL;  
  
    unsigned short dictionarySize = 0;//开始时字典长度为0  
    unsigned short encodeBufferSize = MY_LZ77_ENCODE_BUFFER_LEN;  
    if(dataLen<MY_LZ77_ENCODE_BUFFER_LEN)  
    {  
        encodeBufferSize = dataLen;  
    }  
  
    unsigned char *pDictionary = data;  
    unsigned char *pEncodeBuffer = data;  
  
    while(encodeBufferSize>0)  
    {  
        PLZ77_ENCODE_SEARCH_RESULT pResult = MyLZ77Search(pDictionary,dictionarySize,pEncodeBuffer,encodeBufferSize);  
        if(!pResult)  
        {  
            break;  
        }  
  
        PLZ77_ENCODE_ITEM item = new LZ77_ENCODE_ITEM;  
        if(!item)  
        {  
            break;  
        }  
  
        item->searchResult = pResult;  
        item->nextChar = pEncodeBuffer[pResult->matchLen];  
  
        hasbeenEncodeSize += (pResult->matchLen+1);//已编码的字节数同步增加  
  
        //更新链表  
        if(!UpdataLZ77Link(&lz77Header,&pCur,item))  
        {  
            break;  
        }  
  
        //更新动态字典  
        dictionarySize += pResult->matchLen+1;  
        if(dictionarySize>MY_LZ77_DICTIONARY_LEN)  
        {  
            dictionarySize = MY_LZ77_DICTIONARY_LEN;  
            pDictionary = (pEncodeBuffer+pResult->matchLen+1)-MY_LZ77_DICTIONARY_LEN;  
        }  
  
        //更新待编码的buffer指针,往后移动  
        pEncodeBuffer += (pResult->matchLen+1);  
  
        //未编码的数据已经不足MY_LZ77_ENCODE_BUFFER_LEN，更新未编码的数据，否则未编码的buffer大小仍然设置为MY_LZ77_ENCODE_BUFFER_LEN  
        if(dataLen-hasbeenEncodeSize<MY_LZ77_ENCODE_BUFFER_LEN)  
        {  
            encodeBufferSize = ((dataLen-hasbeenEncodeSize)>=0)?(dataLen-hasbeenEncodeSize):0;  
        }  
    }  
  
    return lz77Header;  
}  
  
//标准的LZ77解码输出,输入需要编码后的链表，输出解码后的字节数  
int MyLZ77Decode(PLZ77_ENCODE_LIST header,unsigned char *pDecodeOut)  
{  
    int decodeLen = 0;  
    PLZ77_ENCODE_LIST p = header;  
    unsigned char* pCursorData = pDecodeOut;  
    while(p)  
    {     
        unsigned char *pCopy = pCursorData-p->item->searchResult->pos;  
        for(int i=0;i<p->item->searchResult->matchLen;i++)  
        {  
            *pCursorData = *pCopy;  
            pCursorData++;  
            pCopy++;  
        }  
        *pCursorData = p->item->nextChar;  
        pCursorData++;  
  
        decodeLen += (p->item->searchResult->matchLen+1);  
  
        p = p->next;  
    }  
  
    return decodeLen;  
}  
     这个例子可以看到，其实标准的原始
