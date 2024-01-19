#include "strbuf.h"
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->alloc=alloc;
    sb->buf=(char*)malloc(sizeof(char)*alloc);
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
    free(sb->buf);
    sb->buf=NULL;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    int tmp_len=a->len;
    a->len=b->len;
    b->len=tmp_len;
    int tmp_alloc=a->alloc;
    a->alloc=b->alloc;
    b->alloc=tmp_alloc;
    char *buf=a->buf;//直接传址不用开辟新的空间，要不然会造成泄露，测试点没有free中间值操作
    a->buf=b->buf;
    b->buf=buf;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    char *back =sb->buf;
    *sz=(size_t)sb->alloc;
    return back;
};
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    if(first->len!=second->len)
        return 1;
    return memcmp(first->buf,second->buf,first->len);//一位一位比较，若相等输出0，遇到不同则前大为1后大为-1
}
void strbuf_reset(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
}
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    sb->alloc=sb->len+extra;
    sb->buf=(char*)realloc(sb->buf,(sb->len+extra)*sizeof(char));
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    sb->len += len;
    if(sb->len==len) strcpy(sb->buf,(char *)data); //原为空
    else strncat(sb->buf,(char *)data,len);//stract使用的时候需要两个字符串都有\0，如果len！=长度，则需要自动
}
void strbuf_addch(struct strbuf *sb, int c)
{
    char word[2]={0};
    word[0]=c;
    if(sb->len==0)
    {
        strcpy(sb->buf,word);
        sb->len+=1;
    }
    else
    {
        if(sb->len+1==sb->alloc)//考虑数组后面的那个'\0'
        {
            sb->alloc*=2;
            sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
            assert(sb->buf!=NULL);
        }
        strcat(sb->buf,word);
        sb->len+=1;
    }
}
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    int a=strlen(s);
    sb->len += a;
    if(sb->len>=sb->alloc) {
        sb->buf=(char *)realloc(sb->buf,(sb->alloc+a)*sizeof(char));
        sb->alloc+=a;
    }
    if(sb->len==a) strcpy(sb->buf,s);
    else strncat(sb->buf,s,a);
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    sb->len += sb2->len;
    sb->alloc+=sb2->alloc;
    sb->buf=(char *)realloc(sb->buf,(sb->alloc)*sizeof(char));
    if(sb->len==sb2->len) strcpy(sb->buf,sb2->buf);
    else strcat(sb->buf,sb2->buf);
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len=len;
    sb->buf[len]=0;
}
size_t strbuf_avail(const struct strbuf *sb){
    size_t a;
    a=sb->alloc-sb->len-1;
    return a;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    sb->len+=len;
    if(sb->len>=sb->alloc){
        sb->alloc=sb->len;
        sb->buf=(char *)realloc(sb->buf,(sb->alloc)*sizeof(char));
    }
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);//mubiaoweizhi,charulaiyuan,charuchangduhuozijiegeshu
    memcpy(sb->buf+pos,data,len);
}
void strbuf_ltrim(struct strbuf *sb)
{
    int i=0;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'){
        i++;
    }
    memmove(sb->buf,sb->buf+i,sb->len-i);
    sb->len-=i;
}
void strbuf_rtrim(struct strbuf *sb)
{
    int i=sb->len-1;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'){
        i--;
    }
    sb->buf[i+1]='\0';
    sb->len=i+1;
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
    sb->len-=len;
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    sb->alloc+=hint?hint:8192;
    sb->buf=(char*)realloc(sb->buf,sb->alloc*sizeof(char));
    int ret=read(fd,sb->buf+sb->len,hint);
    sb->len+=ret;
    sb->buf[sb->len] = '\0';//fangzhiyichuhuozhexielu?????
    return ret;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    char c=0;
    while((c=fgetc(fp))!=EOF&&c!='\n')/*这种机制，用一种形象的说法可以表述为：有一个固定的地址（句柄），指向一个固定的位置（区域A），而区域A中的值可以动态地变化，它时刻记录着当前时刻对象在内存中的地址。
    这样，无论对象的位置在内存中如何变化，只要我们掌握了句柄的值，就可以找到区域A，进而找到该对象。而句柄的值在程序本次运行期间是绝对不变的，我们（即系统）当然可以掌握它。
    jijubingshiyigeduixiang(wenjiandeng)dedizhi,womenkeyitongguoyigegudingdejubingzhaodaogudingdeneicunkongjiantongguoneicunkongjianchucundeneirongzhaodaoxuyaodeshujujiegou*/
    {
        strbuf_addch(sb,c);//jiangwenjiancunrubuf
    }
    return 0;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    struct strbuf **result = (struct strbuf **)malloc(sizeof(struct strbuf *) * (max + 1));
    const char *begin = str;
    const char *end = str + len;
    const char *next;
    int count = 0;
    int length;
    while (*begin == terminator)
        begin++;
    for (next = begin; next <= end; next++)
    {
        if (*next == terminator || next == end)
        {
            length = next - begin;
            result[count] = (struct strbuf *)malloc(sizeof(struct strbuf));
            result[count]->len = length;
            result[count]->alloc = length + 1;
            result[count]->buf = (char *)malloc(sizeof(char) * result[count]->alloc);
            memcpy(result[count]->buf, begin, length);
            result[count]->buf[length] = '\0';
            count++;

            while (*next == terminator && next <= end)
                next++;

            begin = next;
        }

        if (count == max)
            break;
    }
    result[count] = NULL;
    return result;
}
bool strbuf_begin_judge(char* target_str,const char*str, int strlens)
{
    int len;
    if(strlens==0)
    {
        return true;
    }
    else len=strlen(str);
    return (strncmp(target_str,str,len)==0);
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(target_buf==NULL)
    {
        return NULL;
    }
    char *s=(char *)malloc(sizeof(char)*len+1);
    if(end-begin>len)//buhefadetiaojian
    {
        return NULL;
    }
    memcpy(s,target_buf+begin,end-begin);
    s[end-begin]='\0';//bujiushu
    return s;
}