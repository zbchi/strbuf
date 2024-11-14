#include <stddef.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
//设置缓冲区属性结构体
struct strbuf 
{
    int len;//长度
    int alloc;//容量
    char *buf;//缓冲区（字符串）
};
//struct strbuf *sb=malloc(sizeof(struct strbuf));

// 初始化 sb 结构体，容量为 alloc
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    if(alloc<=0||sb==NULL)return;
    //长度初始化
    sb->len=0;
    //容量初始化
    sb->alloc=alloc;
    //给属性初始化
    sb->buf=(char*)malloc(sizeof(char)*alloc);

}

// 将字符串填充到 sb 中，长度为 len，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    // 释放旧的缓冲区内存
    if (sb->buf != NULL) 
    {
        free(sb->buf);
    }
    
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char *) str;
}

// 释放 sb 结构体的内存
void strbuf_release(struct strbuf *sb)
{
    if(sb==NULL)return ;

    //释放缓冲区
    free(sb->buf);
    sb->buf=NULL;

    //重置其他属性
    sb->len=0;
    sb->alloc=0;
}

// 交换两个 strbuf
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    char*temp_buf=a->buf;
    size_t temp_alloc=a->alloc;
    int temp_len=a->len;

    a->buf=b->buf;
    a->alloc=b->alloc;
    a->len=b->len;

    b->buf=temp_buf;
    b->len=temp_len;
    b->alloc=temp_alloc;
}

// 将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    //提取容量值
    *sz=sb->alloc;
    
    //提取sb的原始内存
    char *str=sb->buf;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return sb->buf;
}

// 比较两个 strbuf 的内存是否相同
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
   if(first->len!=second->len)
   return 1;
   if(memcmp(first->buf,second->buf,first->len)==0)
   return 0;
   else
   return 1;

}

// 清空 sb
void strbuf_reset(struct strbuf *sb)
{
    //长度归零
    sb->len=0;
    //内存数据清空
    memset(sb->buf,0,sb->alloc);
}

// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    //判断是否需要增加容量
    if(extra+sb->len>sb->alloc)
    {
        sb->buf=(char*)realloc(sb->buf,extra+sb->alloc);

        //更新容量
        sb->alloc+=extra;
    }

}

// 向 sb 追加长度为 len 的数据 data。
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    //保证容量足够
    strbuf_grow(sb,len+1);

    //追加数据date
    memcpy(sb->buf+sb->len,data,len);

    //更新长度
    sb->len+=len;

    //写入/0
    sb->buf[sb->len]='\0';
}

// 向 sb 追加一个字符 c。
void strbuf_addch(struct strbuf *sb, int c)
{
    //保证空间足够
    strbuf_grow(sb,2);
    //追加字符
    sb->buf[sb->len]=(char)c;
    //更新长度
    sb->len++;
    //写入\0
    sb->buf[sb->len]='\0';

}

// 向 sb 追加一个字符串 s。
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    //直接调用add函数
    strbuf_add(sb,s,strlen(s));
}

// 向一个 sb 追加另一个 strbuf 的数据。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_add(sb,sb2->buf,sb2->len);
}

// 设置 sb 的长度 len。
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    if(len>sb->alloc)return;
    sb->len=len;
    sb->buf[len]='\0';
}

// 计算 sb 目前仍可以向后追加的字符串长度。
size_t strbuf_avail(const struct strbuf *sb)
{
    return sb->alloc-sb->len-1;
}

// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    //确保足够空间
    strbuf_grow(sb,len);

    //移动后半数据以便插入
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);

    //插入新数据到pos位置
    memcpy(sb->buf+pos,data,len);

    //更新长度
    sb->len+=len;
    //写入\0
    sb->buf[sb->len]='\0';
}


// 去除 sb 缓冲区左端的所有空格、制表符和'\t'字符。
void strbuf_ltrim(struct strbuf *sb)
{
    size_t i=0;
    //查找到非空白字符为首地址
    while(i<sb->len&&(sb->buf[i]==' '||sb->buf[i]=='\t'||sb->buf[i]=='\n'))
    i++;
    //将剩余的字符向左移动
    memmove(sb->buf,sb->buf+i,sb->len-i);
    //更新长度
    sb->len-=i;
    //
}

// 去除 sb 缓冲区右端的所有空格、制表符和'\t'字符。
void strbuf_rtrim(struct strbuf *sb)
{
    size_t i=sb->len;
    //从末尾查找到第一个非空格字符
    while(i>0&&(sb->buf[i-1]==' '||sb->buf[i-1]=='\n'||sb->buf[i-1]=='\t'))
    i--;
    //更新长度
    sb->len=i;
    //保证末尾为\0
    sb->buf[sb->len]='\0';

}

// 删除 sb 缓冲区从 pos 坐标开始长度为 len 的内容。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    //如果从删除的长度超过剩余长度，修正len
    if(len>sb->len-pos)
    len=sb->len-pos;

    //从pos+len开始移动到pos位置
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);

    //更新长度
    sb->len-=len;
    //保证结尾为\0
    sb->buf[sb->len]='\0';
}


// 将文件描述符为 fd 的所有文件内容追加到 sb 中。sb 增长 hint ? hint : 8192 大小。
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    size_t grow_size=hint?hint:8192;
    //增长hint大小
    strbuf_grow(sb,grow_size);

    //读取数据
    size_t bytes_read;
    size_t total_read=0;
    //读到结尾为止
    while(bytes_read=read(fd,sb->buf+sb->len,sb->alloc-sb->len))
    {
        //更新数据
        sb->len+=bytes_read;
        total_read+=bytes_read;

        //如果满容量再扩容
        if(sb->len==sb->alloc)
        {
            strbuf_grow(sb,grow_size);
        }
    }
    return total_read;//返回读取的文件长度

}

// 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb。
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    //临时字符
    char c;
    while((c=fgetc(fp))!=EOF||c!='\n')
    {
        strbuf_addch(sb,c);
    }
    return sb->len;
}
/*
* @brief 将指定长度的字符串按切割字符切割成多个 strbuf
*
* @param str 要切割的字符串
* @param len 字符串的长度
* @param terminator 切割字符
* @param max 最大切割数量（可选）
* @return struct strbuf** 指向 struct strbuf 的指针数组，数组的最后一个元素为 NULL
*
* @note 函数将字符串 str 根据切割字符 terminator 切割成多个 strbuf，并返回结果。可选参数 max 用于限定最大切割数量。
*/
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{

}
/*
* @brief 判断目标字符串是否以指定前缀开头
*
* @param target_str 目标字符串
* @param str 前缀字符串
* @param strlen target_str 的长度
* @return bool 前缀相同返回 true，否则返回 false
*/
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
{

}
/*
brief 获取目标字符串的指定子串

param target_buf 目标字符串
param begin 开始下标（包含）
param end 结束下标（不包含）
param len target_buf 的长度
return char* 指向获取的子串的指针，如果参数不合法则返回 NULL

note 下标从0开始，[begin, end)表示左闭右开区间
*/
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{

}