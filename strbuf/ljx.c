#include"strbuf.h"
//#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// struct strbuf
// {
//     int len;     //当前缓冲区（字符串）长度
//     int alloc;   //当前缓冲区（字符串）容量
//     char *buf;   //缓冲区（字符串）
// };
// int main()
// {
//     struct strbuf sb;
//     strbuf_init(&sb, 10);
//     strbuf_attach(&sb, "xiyou", 5, 10);
//     assert(strcmp(sb.buf, "xiyou") == 0);
//     strbuf_addstr(&sb, "linux");
//     assert(strcmp(sb.buf, "xiyoulinux") == 0);
//     strbuf_release(&sb);
// }
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->buf=(char *)malloc(sizeof(char)*alloc);
    sb->alloc=alloc;
    sb->len=0;
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    sb->len=len;
    sb->alloc=alloc;
    sb->buf=(char*)str;
}
void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf swap=*a;
    *a=*b;
    *b=swap;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    *sz=sb->alloc;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    return memcmp(first->buf,second->buf,first->alloc);
}
void strbuf_reset(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
}
//-----------------------------------------------------------------------------------------------------
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    int n=sb->alloc-sb->len;
    if(n<extra)
    {
        sb->buf=(char *)realloc(sb->buf,(sb->len+extra)*sizeof(char));
        sb->alloc=sb->len+extra;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb,len+1);
    memcpy(sb->buf+sb->len,data,len);
    sb->buf[sb->len+len]='\0';
    sb->len+=len;
}
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_add(sb,&c,1);
}
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    strbuf_add(sb,s,strlen(s));
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_add(sb,sb2->buf,sb2->len);
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    if(len>sb->len)
    {
        sb->buf=(char*)realloc(sb->buf,len);
        sb->alloc=len;
        sb->len=len;
    } else
        {
            sb->len=len;
            sb->buf[len]='\0';
        }
}
size_t strbuf_avail(const struct strbuf *sb)
{
    return sb->alloc-sb->len-1;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    if(pos+len>sb->alloc) strbuf_grow(sb,pos+len-sb->alloc+1);;
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
    memmove(sb->buf+pos,data,len);
    sb->len+=(int)len;
    *(sb->buf+sb->len)='\0';
}
//-----------------------------------------------------------------------------------------------------------------
void strbuf_ltrim(struct strbuf *sb)
{
    int n=0;
    while(sb->buf[n]==' '||sb->buf[n]=='\t') n++;
    memmove(sb->buf,sb->buf+n,sb->len-n);
    sb->len-=n;
}
void strbuf_rtrim(struct strbuf *sb)
{
    int n=sb->len-1,x=0;
    while(sb->buf[n]==' '||sb->buf[n]=='\t')
    {
        x++;
        n--;
    }
    sb->len=n+1;
    memmove(sb->buf+sb->len,sb->buf+sb->len+x,x);
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    if(pos>sb->len) return;
    if(pos+len>=sb->len)
    {
        memmove(sb->buf+pos,sb->buf+sb->len,len);
        sb->len=pos;
    } else
        {
            memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
            sb->len-=len;
        }
}
//-----------------------------------------------------------------------------------
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    char ch;
    FILE *fp=fdopen(fd,"r");
    if(fp==NULL) return sb->len;
    strbuf_grow(sb,(hint ? hint : 8192)+1);
    while((ch=fgetc(fp))!=EOF)
    {
        sb->buf[sb->len++]=ch;
    }
    sb->buf[sb->len]='\0';
    fclose(fp);
    return sb->len;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    char ch;
    while((ch=fgetc(fp))!=EOF)
    {
        if(ch=='\n') break;
        strbuf_addch(sb,ch);
    }
    return sb->len; 
}
//---------------------------------------------------------------------------------------------------
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    struct strbuf **re=(struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
    const char *end=str+len;
    while(*str==terminator) str++;
    const char *begin=str;
    const char *i;
    int sum=0,n;
    for(i=begin;i<=end;i++)
    {
        if(*i==terminator||i==end)
        {
            n=i-begin;
            re[sum]=(struct strbuf*)malloc(sizeof(struct strbuf));
            re[sum]->len=n;
            re[sum]->alloc=n+1;
            re[sum]->buf=(char*)malloc((n+1)*sizeof(char*));
            memcpy(re[sum]->buf,begin,n);
            *(re[sum]->buf+n)='\0';
            sum++;
            while(*i==terminator&&i<=end) i++;
            begin=i;
        }
        if(sum==max) break;
    }
    re[sum]=NULL;
    return re;
}
bool strbuf_begin_judge(char* target_str, const char* str, int strnlen)
{
    if(str==NULL||strncmp(target_str,str,strlen(str))==0) return 1;
    else return 0;
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(target_buf==NULL||(begin>end)) return NULL;
    int n=end-begin;
    char *re=(char*)malloc(sizeof(char)*n+1);
    memcpy(re,target_buf+begin,n);
    re[n]='\0';
    return re;
}