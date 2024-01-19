#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include <unistd.h>
#include<sys/types.h>
struct strbuf {
  int len;     //当前缓冲区（字符串）长度
  int alloc;   //当前缓冲区（字符串）容量
  char *buf;   //缓冲区（字符串）
};
void strbuf_init(struct strbuf *sb, size_t alloc);  //初始化 sb 结构体，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);  //将字符串填充到 sb 中，长度为 len, 容量为 alloc。
void strbuf_release(struct strbuf *sb);  //释放 sb 结构体的内存
void strbuf_swap(struct strbuf *a, struct strbuf *b); 	//交换两个 strbuf。
char *strbuf_detach(struct strbuf *sb, size_t *sz); 	//将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小 。
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second); 	//比较两个 strbuf 的内存是否相同。
void strbuf_reset(struct strbuf *sb); 	//清空 sb。



void strbuf_grow(struct strbuf *sb, size_t extra); 	//确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。
void strbuf_add(struct strbuf *sb, const void *data, size_t len); 	//向 sb 追加长度为 len 的数据 data。
void strbuf_addch(struct strbuf *sb, int c); 	//向 sb 追加一个字符 c。
void strbuf_addstr(struct strbuf *sb, const char *s); 	//向 sb 追加一个字符串 s。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2); 	//向一个 sb 追加另一个 strbuf的数据。
void strbuf_setlen(struct strbuf *sb, size_t len); 	//设置 sb 的长度 len。
size_t strbuf_avail(const struct strbuf *sb); 	//计算 sb 目前仍可以向后追加的字符串长度。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len); 	//向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。



void strbuf_ltrim(struct strbuf *sb); 	//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
void strbuf_rtrim(struct strbuf *sb); 	//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len); 	//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。



size_t strbuf_read(struct strbuf *sb, int fd, size_t hint); 	//sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
int strbuf_getline(struct strbuf *sb, FILE *fp);    //将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb 。

//实现字符串切割（C 系字符串函数的一个痛点）。
bool strbuf_begin_judge(char* target_str, const char* str, int strlen); 	//target_str : 目标字符串，str : 前缀字符串，strlen : target_str 长度 ，前缀相同返回 true 失败返回 false

//实现判断一个 strbuf 是否以指定字符串开头的功能（C 系字符串函数的另一个痛点）。
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max); 	//将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回，max 可以用来限定最大切割数量。返回 struct strbuf 的指针数组，数组的最后元素为 NULL

//获取字符串从坐标 [begin, end) 的所有内容（可以分成引用和拷贝两个模式） 。
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len); 	//target_str : 目标字符串，begin : 开始下标，end 结束下标。len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，[begin, end)区间。


void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->alloc=alloc;
    sb->buf=(char*)malloc(alloc);
    sb->len=0;
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
   
    sb->alloc=alloc;
    sb->buf=(char*)str;
    sb->len=len;
}
void strbuf_release(struct strbuf *sb)
{
    sb->alloc=0x10;
    free(sb->buf);
    sb->buf=NULL;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf temp;
    temp.alloc=a->alloc;
    temp.len=a->len;
    temp.buf=a->buf;
    a->alloc=b->alloc;
    a->buf=b->buf;
    a->len=b->len;
    b->alloc=temp.alloc;
    b->buf=temp.buf;
    b->len=temp.len;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    char *a=sb->buf;
    sb->buf=NULL;
    *sz=sb->alloc;
    return a;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    return strcmp(first->buf,second->buf);
}
void strbuf_reset(struct strbuf *sb)
{
    sb->len=0;
    sb->alloc=0x10;
    free(sb->buf);
    sb->buf=NULL;
}
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if((sb->alloc)-(sb->len)<extra)
    {
    sb->buf=(char*)realloc(sb->buf,sb->len+extra);
    sb->alloc=sb->len+extra;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    if(sb->len+len+1>sb->alloc)
    {
    sb->buf=(char*)realloc(sb->buf,sb->len+len+1);
    sb->alloc=sb->len+len+1;
    }
    memcpy(sb->buf+sb->len,(char*)data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
void strbuf_addch(struct strbuf *sb, int c)
{
    if(sb->len+2>sb->alloc)
    {   
        sb->alloc=sb->len+2;
        sb->buf=(char*)realloc(sb->buf,sb->alloc);
    }
    memcpy(sb->buf+sb->len,&c,1);
    sb->len+=1;
    sb->buf[sb->len]='\0';
}
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    size_t len=strlen(s);
    if(sb->len+len+1>sb->alloc)
    {
    sb->buf=(char*)realloc(sb->buf,sb->len+len+1);
    sb->alloc=sb->len+len+1;
    }
    memcpy(sb->buf+sb->len,s,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    size_t len=strlen(sb2->buf);
    if(sb->len+len+1>sb->alloc)
    {
    sb->buf=(char*)realloc(sb->buf,sb->len+len+1);
    sb->alloc=sb->len+len+1;
    }
    memcpy(sb->buf+sb->len,sb2->buf,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->buf[len]='\0';
    sb->len=len;
}
size_t strbuf_avail(const struct strbuf *sb)
{
    size_t left;
    left=sb->alloc-sb->len-1;
    return left;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    int k=0;
    if(sb->alloc<sb->len+len+1)
    {
        sb->buf=(char*)realloc(sb->buf,sb->len+len+1);
        sb->alloc=sb->len+len+1;
    }
    for(int i=sb->len+1;i>=pos;i--)
    {
        sb->buf[i+len]=sb->buf[i];
    }
    memcpy(sb->buf+pos,(char*)data,len);
    sb->len+=len;
}
void strbuf_ltrim(struct strbuf *sb)
{
    int i=0;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'||sb->buf[i]=='\n')
    {
        i++;
    }
    memmove(sb->buf,sb->buf+i,sb->len-i+1);
    sb->len-=i;
    //sb->buf[sb->len]='\0';
}
void strbuf_rtrim(struct strbuf *sb)
{
    int i=sb->len-1;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'||sb->buf[i]=='\n')
    {
        i--;
    }
    sb->buf[i+1]='\0';
    sb->len=i+1;
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len+1);
    sb->len-=len;
}
size_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    FILE *p;
    p=fdopen(fd,"r");
    sb->alloc+=hint ? hint : 8192;
    sb->buf=(char*)realloc(sb->buf,sb->alloc);
    int ch;
    while((ch=fgetc(p))!=EOF)
    {
        sb->buf[sb->len]=(char)ch;
        sb->len++;
    }
    sb->buf[sb->len]='\0';
    return sb->len;

}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    int ch;
    while((ch=fgetc(fp))!='\n'&&ch!=EOF&&ch!='/r')
    {
        strbuf_addch(sb,ch);
    }
    return sb->len;
}
bool strbuf_begin_judge(char* target_str, const char* str, int len)
{
   return str==NULL||(strncmp(target_str,str,strlen(str)))==0;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    struct strbuf **sb=(struct strbuf **)malloc((max+1)*sizeof(struct strbuf*));
    char ch;
    int count=0,k=0;
    while(k<len)
    {

        if(count==max)  
        {
            break;
        }
        while((ch=str[k])==(char)terminator&&k<len)
        {
            k++;
        }
        if(k>=len)  break;
        sb[count] = (struct strbuf *)malloc(sizeof(struct strbuf));
        strbuf_init(sb[count],1);
        while((ch=str[k])!=(char)terminator&&k<len)
        {
            strbuf_addch(sb[count],ch);
            k++;
        }
        count++;
    }
    sb[count]=NULL;
    sb=(struct strbuf **)realloc(sb,(count+1)*sizeof(struct strbuf*));
    return sb;
}

char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(begin>end||end>=len)  return NULL;
    char *str=(char*)malloc((end-begin+1)*sizeof(char));
    memcpy(str,target_buf+begin,end-begin);
    str[end-begin]='\0';
    return str;
}