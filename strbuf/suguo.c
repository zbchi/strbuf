#include "strbuf.h"
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->alloc=alloc;
    sb->len=0;
    sb->buf=(char*)calloc(alloc,sizeof(char));
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    sb->alloc=alloc;
    strbuf_grow(sb,len+1);
    sb->buf=(char*)str;
    strbuf_setlen(sb,len);
}
void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
    sb->alloc=0;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf temp=*a;
    *a=*b;
    *b=temp;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    *sz=sb->alloc;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    int n;
    if(first->len==second->len)
    {      
        if(memcmp(first->buf,second->buf,first->len)==0)
        {
            n=0;
        }
        else
        {
            n=1;
        }
    }
    else
    {
        n=first->len-second->len;
    }
    return n;
}
void strbuf_reset(struct strbuf *sb)
{
    for(int i=0;i<=sb->len;i++)
    {
        sb->buf[i]='0';
    }
    sb->len=0;
}
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(sb->alloc-sb->len-1<extra)
    {
        sb->buf=(char*)realloc(sb->buf,(sb->len+extra+1)*sizeof(char));
        sb->alloc=sb->len+extra+1;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb,len);
    memcpy(sb->buf+sb->len,(char*)data,len);
    (sb->buf)[sb->len+len]='\0';
    strbuf_setlen(sb,sb->len+len);
}
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_grow(sb,1);
    (sb->buf)[sb->len]=c;
    strbuf_setlen(sb,sb->len+1);
}
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    strbuf_grow(sb,strlen(s));
    strcat(sb->buf,s);
    strbuf_setlen(sb,sb->len+strlen(s));
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_grow(sb,sb2->len);
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    //strcat(sb->buf,sb2->buf);
    strbuf_setlen(sb,sb->len+sb2->len);
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len=len;
    sb->buf[len]='\0';
}
size_t strbuf_avail(const struct strbuf *sb)
{
    size_t avail=sb->alloc-sb->len-1;
    return avail;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    strbuf_grow(sb,len);
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
    memcpy(sb->buf+pos,(char*)data,len);
    strbuf_setlen(sb,sb->len+len);
}
void strbuf_ltrim(struct strbuf *sb)
{
    while((sb->buf[0]==' '||sb->buf[0]=='\t')&&sb->buf[0]!='\0')
    {
        memmove(sb->buf,sb->buf+1,sb->len-1);
        sb->len--;
    }
    // int slow=0;
    // for(int fast=0;fast<=sb->len;fast++)
    // {
    //     if(sb->buf[fast]==' '||sb->buf[fast]=='\t')
    //     {
    //         sb->buf[slow]=sb->buf[fast];
    //         slow++;
    //     }
    // }
}
void strbuf_rtrim(struct strbuf *sb)
{
    while((sb->buf[sb->len-1]==' '||sb->buf[sb->len-1]=='\t')&&sb->buf[sb->len-1]!='\0')
    {
        sb->len--;
    }
    sb->buf[sb->len]='\0';
    // int slow=0;
    // for(int fast=sb->len;fast<=sb->alloc;fast++)
    // {
    //     if(sb->buf[fast]==' '||sb->buf[fast]=='\t')
    //     {
    //         sb->buf[slow]=sb->buf[fast];
    //         slow++;
    //     }
    // }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
    strbuf_setlen(sb,sb->len-len);
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    strbuf_grow(sb,hint?hint:8192);
    FILE *fp=fdopen(fd,"r");
    char ch=getc(fp);
    while (ch!=EOF)
    {
        strbuf_addch(sb,ch);
        ch=getc(fp);
    }
    return sb->len;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    char ch=getc(fp);
    while (ch!=EOF&&ch!='\n')
    {
        strbuf_addch(sb,ch);
        ch=getc(fp);
    } 
    return sb->len;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    char t=(char)terminator;
    struct strbuf **a=(struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
    char b[len+1];
    memcpy(b,str,len);
    for(int i=0;i<len;i++)
    {
        if(b[i]=='\0')
        {
            b[i]='#';
        }
    }
    b[len]='\0';
    int num=0;
    char*r=strtok(b,&t);
    while(r!=NULL)
    {
        a[num]=(struct strbuf*)malloc(sizeof(struct strbuf));
        a[num]->len=strlen(r);
        a[num]->alloc=strlen(r)+1;
        a[num]->buf=(char*)malloc(sizeof(char)*(a[num]->alloc));
        memcpy(a[num]->buf,r,a[num]->len);
        a[num]->buf[a[num]->len]='\0';
        // strbuf_init(a[num],strlen(r)+1);
        // strbuf_attach(a[num],r,strlen(r),strlen(r)+1);
        for(int i=0;i<a[num]->len;i++)
        {
            if(a[num]->buf[i]=='#')
            {
                a[num]->buf[i]='\0';
            }
        }
        r=strtok(NULL,&t);
        num++;
        if(num==max)
        {
            break;
        }
    }
    a[num]=NULL;
    return a;

    // struct strbuf **a=(struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
    // int n=0,num=0,i=0;//n为应切割的字符串的首字符的标记点，num为切割后二维数组的下标，i为原字符串下标
    // while(str[i]==terminator)//判断开头是否会无意义切割或连续无意义切割
    // {
    //     i++;
    // }
    // n=i;//标记
    // for(;i<=len;i++)
    // {
    //     if(str[i]==terminator||(i==len&&str[i-1]!=terminator))//考虑遍历到结束或遍历到后面没有有效字符串
    //     {
    //         a[num]=(struct strbuf*)malloc(sizeof(struct strbuf));
    //         a[num]->len=i-n;
    //         a[num]->alloc=i-n+1;
    //         a[num]->buf=(char*)malloc(sizeof(char)*(a[num]->alloc));
    //         memcpy(a[num]->buf,str+n,a[num]->len);
    //         a[num]->buf[a[num]->len]='\0';
    //         num++;
    //         while(i<=len&&str[i+1]==terminator)//判断是否连续切割
    //         {
    //             i++;
    //         }
    //         n=i+1;//标记
    //     }
    //     if(num==max)//切割数检查
    //     {
    //         break;
    //     }
    // }
    // a[num]=NULL;
    // return a;
}
bool strbuf_begin_judge(char* target_str, const char* str, int strnlen)
{
    if(str==NULL||memcmp(target_str,str,strlen(str))==0)//str==NULL必须放前面，否则会在str为空指针时非法访问 strncmp
    {
        return true;
    }
    else
    {
        return false;
    }
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(target_buf==NULL)
    {
        return target_buf;
    }
    char*a=(char*)malloc(sizeof(char)*(end-begin+1));
    memcpy(a,target_buf+begin,end-begin);
    a[end-begin]='\0';
    return a;
}