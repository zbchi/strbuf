#include "strbuf.h"
/*A*/
// 1
void strbuf_init(struct strbuf *sb, size_t alloc)
{
     sb->len = 0;
     sb->alloc = alloc;
     sb->buf = (char *)malloc(alloc * sizeof(char));
}
// 2
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
     sb->buf = (char *)str;
     sb->len = len;
     sb->alloc = alloc;
}
// 3
void strbuf_release(struct strbuf *sb)
{
     free(sb->buf);
     sb->buf = NULL;
     sb->len = 0;
     sb->alloc = 0;
}
// 4
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
     struct strbuf temp = *a;
     *a = *b;
     *b = temp;
}
// 5
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
     char *result = sb->buf;
     *sz = sb->alloc;
     sb->buf = NULL;
     sb->len = 0;
     sb->alloc = 0;
     return result;
}
// 6
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
     if (first == NULL || second == NULL)
     {
          return 1;
     }
     if (first->len == 0 && second->len == 0)
     {
          return 0;
     }
     if (first->len != second->len)
     {
          return 1;
     }
     return memcmp(first->buf, second->buf, (size_t)first->len) != 0;
}
// 7
void strbuf_reset(struct strbuf *sb)
{
     sb->len = 0;
}





/*B*/
// 1
void strbuf_grow(struct strbuf *sb, size_t extra)
{
     size_t newalloc = sb->len + extra;
     if (newalloc > sb->alloc)
     {
          sb->alloc = newalloc * 2;
          sb->buf = (char *)realloc(sb->buf, sb->alloc);
     }
}
// 2
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
     if (!sb || len == 0)
     {
          return;
     }
     strbuf_grow(sb, len);
     if (sb->buf && sb->len + len <= sb->alloc)
     {
          if (data)
          {
               memcpy(sb->buf + sb->len, data, len);
               sb->buf[sb->len + len] = '\0';
               sb->len += len;
          }
     }
}
// 3
void strbuf_addch(struct strbuf *sb, int c)
{
     strbuf_grow(sb, 2);
     sb->buf[sb->len] = c;
     sb->len++;
     sb->buf[sb->len] = '\0';
}
// 4
void strbuf_addstr(struct strbuf *sb, const char *s)
{
     if (!sb || !s)
     {
          return;
     }
     size_t len = strlen(s);
     if (sb->len + len + 1 > sb->alloc)
     {
          strbuf_grow(sb, sb->len + len + 1);
     }
     memcpy(sb->buf + sb->len, s, len);
     sb->len += len;
     sb->buf[sb->len] = '\0';
}
// 5
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
     ssize_t newsize=sb->len+sb2->len+1;
     if(newsize>sb->alloc)
     {
          strbuf_grow(sb,newsize);
     }
     strbuf_add(sb,sb2->buf,sb2->len);
}
// 6
void strbuf_setlen(struct strbuf *sb, size_t len)
{
     if (len < sb->len)
     {
          sb->len = len;
     }
     else
     {
          strbuf_grow(sb, len - sb->len);
          memset(sb->buf+sb->len , 0, len - sb->len);
          sb->len = len;
     }
     sb->buf[sb->len] = '\0';
}
// 7
size_t strbuf_avail(const struct strbuf *sb)
{
     return sb->alloc - sb->len-1 ;
}
// 8
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
     if (pos > sb->len)
     {
          pos = sb->len;
     }
     strbuf_grow(sb, len);
     memmove(sb->buf + pos + len, sb->buf + pos, sb->len - pos);
     memcpy(sb->buf + pos, data, len);
     sb->len += len;
     sb->buf[sb->len] = '\0';
}





/*C*/
// 1
void strbuf_ltrim(struct strbuf *sb)
{
     size_t i = 0;
     while (i < sb->len && (sb->buf[i] == ' ' || sb->buf[i] == '\t'))
     {
          i++;
     }
     memmove(sb->buf, sb->buf + i, sb->len - i);
     sb->len -= i;
}
// 2
void strbuf_rtrim(struct strbuf *sb)
{
     size_t i = sb->len;
     while (i > 0 && (sb->buf[i - 1] == ' ' || sb->buf[i - 1] == '\t'))
     {
          i--;
     }
     sb->len = i;
     sb->buf[sb->len] = '\0';
}
// 3
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
     if (pos >= sb->len)
     {
          return;
     }
     size_t end_pos = pos + len;
     if (end_pos > sb->len)
     {
          end_pos = sb->len;
     }
     memmove(sb->buf + pos, sb->buf + end_pos, sb->len - end_pos);
     sb->len -= (end_pos - pos);
     sb->buf[sb->len] = '\0';
}





/*D*/
//1
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
     FILE*fp=fdopen(fd,"r");
     char ch;
     if(fp==NULL||(ch=fgetc(fp))==EOF)
     {
          return sb->len;
     }
     size_t newalloc=hint?hint:8192;
     strbuf_grow(sb,newalloc);
     sb->buf[sb->len++]=ch;
     while((ch=fgetc(fp))!=EOF)
     {
          sb->buf[sb->len++]=ch;
     }
     sb->buf[sb->len]='\0';
     return sb->len;
}
//2
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    char ch;
    while((ch=fgetc(fp))!=EOF)
    {
          if(ch=='\n')
          {
               break;
          }
          strbuf_addch(sb,ch);
    } 
    return 0;
}





/*challenge*/
//1
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max) 
 {   
     const char*cur;
     const char*start=str;
     const char*ending=str+len;
     int length,count=0;
     struct strbuf **result=(struct strbuf**)malloc((max+1)*sizeof(struct strbuf *));
     while(*start==terminator)
     {
          start++;
     }
     for(cur=start;cur<=ending;cur++)
     {
          if(*cur==terminator||cur==ending)
          {
               length=cur-start;
               result[count]=(struct strbuf*)malloc(sizeof(struct strbuf));
               result[count]->len=length;
               result[count]->alloc=length+1;
               result[count]->buf=(char*)malloc(result[count]->alloc*sizeof(char));
               memcpy(result[count]->buf,start,length);
               *(result[count]->buf+length)='\0';
               count++;
               while(*cur==terminator&&cur<=ending)
               {
                    cur++;
               }
               start=cur;
          }
          if(count==max)
          {
               break;
          }
     }
     result[count]=NULL;
     return result;
 }
//2
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
{
     
     if(str==NULL)
     {
          return true;
     }

     return strncmp(target_str,str,strlen(str))==0;
}
//3
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
     char *result;
     if(target_buf==NULL)
     {
          return 0;
     }
     result=(char *)malloc(end-begin+1);
     memcpy(result,target_buf+begin,end-begin);
     result[end-begin]='\0';
     return result;
}




