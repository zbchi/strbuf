#include "strbuf.h"

// struct strbuf {
//   int len;     //当前缓冲区（字符串）长度
//   int alloc;   //当前缓冲区（字符串）容量
//   char *buf;   //缓冲区（字符串）
// };

void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->alloc=alloc;
    sb->buf=(char *)malloc(sizeof(char)*sb->alloc);
    sb->len=0;
}

void strbuf_attach(struct strbuf *sb,  void *str, size_t len, size_t alloc)
{
    sb->alloc=alloc;
    sb->len=len;
    sb->buf=(char*)str;
    sb->buf[sb->len]='\0';
}

void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    sb->len=0;
}

void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    int len,alloc;
    /*交换长度*/
    len=a->len;
    a->len=b->len;
    b->len=len;
    /*交换容量*/
    alloc=a->alloc;
    a->alloc=b->alloc;
    b->alloc=alloc;
    /*交换字符串*/
    char *C=a->buf;
    a->buf=b->buf;
    b->buf=C;
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
   return first->len-second->len;
}

char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    char *ptr=sb->buf;
    *sz=sb->alloc;
    return ptr;
}

void strbuf_reset(struct strbuf *sb)
{
    for(int a  = 0; a<sb->len;a++)
        sb->buf[a]='\0';
    sb->len=0;
}

/*Part B*/
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len=len;
    sb->buf[len]='\0';
}

void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(sb->alloc-sb->len-1 > extra)
    return ;
    else 
    {
        sb->buf=(char *)realloc(sb->buf,(sb->alloc+extra)*sizeof(char));
        sb->alloc=(sb->alloc+extra);
    }
    return ;
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb,len+1);
    memcpy(sb->buf+sb->len,data,len);
    sb->len=sb->len+len;
    sb->buf[sb->len]='\0';
    return ;
}

void strbuf_addch(struct strbuf *sb, int c)
{
   strbuf_grow(sb,2);
    sb->buf[sb->len]=c;
    sb->len++; 
    sb->buf[sb->len]='\0';
    return ;
}

void strbuf_addstr(struct strbuf *sb, const char *s)
{
    int len =strlen(s);
    strbuf_grow(sb,len+1);
    memcpy(sb->buf+sb->len,s,len);
    sb->len=sb->len+len;
    sb->buf[sb->len]='\0';
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_grow(sb,sb2->len+1);
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    sb->len+=sb2->len;
    sb->buf[sb->len]='\0';
    return ;
}

size_t strbuf_avail(const struct strbuf *sb)
{
    size_t number;
    number=sb->alloc-sb->len-1;
    return number;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    strbuf_grow(sb,len+1);
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
    memmove(sb->buf+pos,(char*)data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}

/*Part C*/

void strbuf_ltrim(struct strbuf *sb) 
{
    size_t i;
    for (i = 0; i < sb->alloc; i++)
    {
    if (sb->buf[i] != ' ' && sb->buf[i] != '\t' && sb->buf[i] != '\n')
        break;
    }
    if (i > 0) 
    {
        memmove(sb->buf, sb->buf + i, strlen(sb->buf + i) + 1);
        sb->len-=i;   
    }
}

void strbuf_rtrim(struct strbuf *sb)
{
  int i=sb->len-1;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'){
        sb->len--;
        sb->buf[i]='\0';
        i--;
    }
}


void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+ pos, sb->buf + pos + len, strlen(sb->buf+ pos + len) + 1);
    sb->len=sb->len-len;
}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    ssize_t result;
    size_t old_len = sb->len;
    // 增长缓冲区
  if (hint) {
    strbuf_grow(sb, hint);
  } else {
    strbuf_grow(sb, 8192);
  }

  // 读取文件内容并追加到缓冲区
  result = read(fd, sb->buf + old_len, sb->alloc - old_len - 1);
  if (result < 0) {
    sb->len = old_len;
    return result;
  }
  sb->len += result;
  sb->buf[sb->len] = '\0';  
  return result;
}

int strbuf_getline(struct strbuf *sb, FILE *fp)
{
   char ch;
    while ((ch=fgetc(fp)) != EOF)
    {
        if (ch == '\n' || feof(fp) != 0 || ch == '\r')    break;
        strbuf_addch(sb, ch);
    }
    return 1;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    struct strbuf **list=(struct strbuf **)malloc(sizeof(struct strbuf *)*(max+1));
    const char *begin=str;
    const char *start=str;
    const char *end=str+len;
    int count=0;
    while(1)
    {
        int len=0;
        while(*(begin)==terminator&&begin<end)
        begin++;
        if(begin==end)
        break;
        start=begin;
        while(*begin!=terminator&&begin<end)
        {
            begin++;
            len++;
        }
        list[count]=(struct strbuf *)malloc(sizeof(struct strbuf));
        list[count]->buf=(char *)malloc(sizeof(char)*len+1);
        list[count]->len=len;
        list[count]->alloc=len+1;
        list[count]->buf=(char *)memmove(list[count]->buf,start,len);
        list[count]->buf[len]='\0';
        count++;
        if(count>=max)
        break;
    }
    list[count]=NULL;
    return list;
}

bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
{
    int i;
    for(i=0;i<strlen;i++){
        if(str[i]=='\0'){
            break;
        }
        if(target_str[i]!=str[i]){
            return false;
        }
    }
    return true;

}

char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    if(begin>end||target_buf==NULL)
    return NULL;
    char *ptr;
    ptr=(char *)malloc(sizeof(char)*(end-begin+1));
    ptr=(char *)memcpy(ptr,target_buf+begin,end-begin);
    ptr[end-begin]='\0';
    return ptr;
}
