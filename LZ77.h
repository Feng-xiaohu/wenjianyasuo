#ifndef _MYLZ77_H_  
#define _MYLZ77_H_  
  
typedef struct _t_LZ77_ENCODE_SEARCH_RESULT  
{  
    _t_LZ77_ENCODE_SEARCH_RESULT()  
    {  
        pos = -1;  
        matchLen = 0;  
    }  
    short pos;//字典匹配的起始位置,相对待编码数据的偏移  
    short matchLen;//字典匹配的长度  
}LZ77_ENCODE_SEARCH_RESULT,*PLZ77_ENCODE_SEARCH_RESULT;  
  
typedef struct _t_LZ77_ENCODE_ITEM  
{  
    PLZ77_ENCODE_SEARCH_RESULT searchResult;  
    unsigned char nextChar;//匹配后的下一个未匹配的字符  
}LZ77_ENCODE_ITEM,*PLZ77_ENCODE_ITEM;  
  
typedef struct _t_LZ77_ENCODE_LIST  
{  
    PLZ77_ENCODE_ITEM item;  
    _t_LZ77_ENCODE_LIST* next;  
}LZ77_ENCODE_LIST,*PLZ77_ENCODE_LIST;  
  
  
//标准的LZ77编码输出,这里使用链表保存编码的结果  
PLZ77_ENCODE_LIST MyLZ77Encode(unsigned char* data,int dataLen);  
  
//标准的LZ77解码输出,输入需要编码后的链表，输出解码后的字节数  
int MyLZ77Decode(PLZ77_ENCODE_LIST header,unsigned char *pDecodeOut);  
  
#endif  
