#include "strbuf.h"
#include<assert.h>
#include<unistd.h>

void strbuf_init(struct strbuf*sb,size_t alloc)
{
    sb->alloc=alloc;
    sb->buf=(char *)malloc(sizeof(char)*alloc);
    assert(sb->buf!=NULL);
    sb->len=0;
}

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    sb->len=len;
    sb->alloc=alloc;
    sb->buf=(char *)str;
    /* if(sb->alloc<len+sb->len+1)
    {
        sb->buf=(char *)realloc(sb->buf,sizeof(char)*(sb->len+len+1));
        assert(sb->buf!=NULL);
        sb->alloc=sb->len+len+1;
    }
    sb->len+=len;
    strcat(sb->buf,(const char*)str); */
}

void strbuf_release(struct strbuf *sb)
{
    if(sb->buf!=NULL)
    {
        free(sb->buf);
        sb->buf=NULL;
    }
}

void strbuf_swap(struct strbuf*a,struct strbuf*b)
{
    int tmp_len=a->len;
    a->len=b->len;
    b->len=tmp_len;
    int tmp_alloc=a->alloc;
    a->alloc=b->alloc;
    b->alloc=tmp_alloc;
    char *buf=a->buf;
    a->buf=b->buf;
    b->buf=buf;
}

char * strbuf_detach(struct strbuf *sb,size_t *sz)
{
    char *start=sb->buf;
    *sz=sb->alloc;
    return start;
}

int strbuf_cmp(const struct strbuf *first,const struct strbuf *second)
{
    if(first->len==second->len)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void strbuf_reset(struct strbuf*sb)
{
    if(sb->buf!=NULL)
    {
        free(sb->buf);
    }
    sb->buf=NULL;
    sb->len=0;
}

void strbuf_grow(struct strbuf *sb,size_t extra)
{
    if(sb->alloc-sb->len-1<extra)
    {
        sb->alloc=extra+sb->len+1;
        sb->buf=(char *)realloc(sb->buf,sizeof(char)*(sb->alloc));
        assert(sb->buf!=NULL);
    }
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    if(sb->len==0)
    {
        sb->len+=len;
        strcpy(sb->buf,(char *)data);
    }
    else
    {
        sb->len+=len;
        strncat(sb->buf,(char *)data,len);
    }
}

void strbuf_addch(struct strbuf *sb, int c)//strcpy,strcat用来拷贝或追加单个字符串会发生意外的行为.
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
    int lens=strlen(s);
    if(sb->len==0)
    {    
        while (sb->alloc <= lens)
        {
            sb->alloc*=2;
            sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
            assert(sb->buf!=NULL);
        }
        strcpy(sb->buf,s);
        sb->len+=lens;
    }
    else
    {
        while(sb->len+lens>=sb->alloc)
        {
             sb->alloc*=2;
             sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
             assert(sb->buf!=NULL);
        }
        strcat(sb->buf,s);
        sb->len+=lens;
    }
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//前面的函数错误会影响后面的函数的测试.
{
    sb->len+=sb2->len;
    sb->alloc+=sb2->alloc;
    sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
    assert(sb->buf!=NULL);
    if(sb->len==sb2->len)
    {
        strcpy(sb->buf,sb2->buf);
    }
    else
    {
        strcat(sb->buf,sb2->buf);
    }
    /* sb->len+=sb2->len;
    if(sb->len>=sb->alloc)
    {
        sb->alloc+=sb2->alloc;
        sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
        assert(sb->buf!=NULL); 
    }
    if(sb->len==sb2->len)
    {
        strcpy(sb->buf,sb2->buf); 
    }
    else
    {
        strncat(sb->buf,sb2->buf,sb2->len);
    }
    sb->alloc=sizeof(char)*sb->alloc; */
}	

void strbuf_setlen(struct strbuf *sb,size_t len)//将strbuf的len设置为len,并将stbuf中的buf的字符串的下标为了len的设置为'\0'
{
    sb->len=len;
    sb->buf[len]='\0';
}

size_t strbuf_avail(const struct strbuf *sb)
{
    int more=0;
    more=sb->alloc-sb->len-1;
    return more;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    int lendata=strlen((char *)data);
    int fisrtlen=sb->len;
    sb->len+=lendata;
    if(sb->len>=sb->alloc)
    {
        sb->alloc*=2;
        sb->buf=(char *)realloc(sb->buf,sizeof(char)*sb->alloc);
        assert(sb->buf!=NULL);
    }
    char s[fisrtlen-(int)pos+1];
    for(int i=0;i<fisrtlen-(int )pos;i++)
    {
        s[i]=sb->buf[pos+i];
    }
        
    strcat(sb->buf+pos, (char *)data );
    for(int i=0;i<fisrtlen-(int) pos;i++)
    {
        sb->buf[pos+lendata+i]=s[i];
    }
    int count=0;
    for(int i=pos;i<pos+lendata;i++)
    {
        sb->buf[i]=*((char *)data+count);
        count++;
    }
}

void strbuf_ltrim(struct strbuf *sb)
{
    int j=0;
    while(sb->buf[j]==' '||sb->buf[j]=='\t')
    {
        j++;
    }
    sb->len-=j;
    memmove(sb->buf,sb->buf+j,sb->len);
}

void strbuf_rtrim(struct strbuf *sb)//原先的只是把后面的空格所对应的长度减短了，并没有进行将strbuf的buf的尾部去掉，
{
    int j=sb->len-1;
    int count=0;
    while(sb->buf[j]==' '||sb->buf[j]=='\t')
    {
        j--;
        count++;
    }
    sb->len-=count;
    sb->buf[sb->len]='\0';//只要加后面的这句话即可.
}

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    if(pos+len>sb->len)
    {
        len=sb->len-pos;
    }
    memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
    sb->len-=len;
}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)//要往strbuf的buf中读数据.buf最后一个要变为'\0'。
{
    if (hint == 0)
    {
        hint = 8192;
    }
    if (sb->len + hint >= sb->alloc)
    {
        sb->alloc += hint;
        sb->buf = (char *)realloc(sb->buf, sizeof(char) * sb->alloc);
        assert(sb->buf != NULL);
    }
    char *str = (char *)malloc(sizeof(char) * hint);
    ssize_t ret = read(fd, str, hint);
    if (ret > 0)
    {
        // Ensure that sb->buf is null-terminated before strcat
        sb->buf = (char *)realloc(sb->buf, sb->len + ret + 1);
        assert(sb->buf != NULL);
        memcpy(sb->buf + sb->len, str, ret);
        sb->len += ret;
        sb->buf[sb->len] = '\0';  // Null-terminate the buffer
    }
    free(str);
    return ret;
}

int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    int c=0;
    while((c=fgetc(fp))!=EOF&&c!='\n')
    {
        strbuf_addch(sb,c);
    }
    return 0;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    //这里采取一遍遍历和赋值同时进行.
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
    //不用在这里free,后面有该功能
    // Free memory allocated for each struct strbuf in the result array
    // Free memory allocated for the result array
    return result;
}


bool strbuf_begin_judge(char* target_str, const char* str, int othstrlen)
{   
    if(othstrlen==0)
    {
        return true;
    }
    int len=strlen(str);
    return (strncmp(target_str,str,len)==0);
}

char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(target_buf==NULL)
    {
        return NULL;
    }
    char *s=(char *)malloc(sizeof(char)*len+1);
    if(end-begin>len)
    {
        return NULL;
    }
    memcpy(s,target_buf+begin,end-begin);
    s[end-begin]='\0';
    return s;
}