#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
struct strbuf
{
    int len;
    int alloc;
    char *buf;
};
// 1.初始化 sb 结构体，容量为 alloc
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->buf = (char *)malloc(alloc * sizeof(char));
    if (sb->buf == NULL)
    {
        exit(EXIT_FAILURE);
    }
    sb->len = 0;
    sb->alloc = alloc;
}
// 2.将字符串填充到 sb 中，长度为 len, 容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char *)str;
}
// 3.释放 sb 结构体的内存（！！释放缓冲区，而不是整个结构体）
void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf = NULL;
}
// 4.交换两个 strbuf
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    //    int temp;
    //    temp=a->len;
    //    a->len=b->len;
    //    b->len=temp;
    //    temp=a->alloc;
    //    a->alloc=b->alloc;
    //    b->alloc=temp;
    //    char *tmp;
    //    tmp=a->buf;
    //    a->buf=b->buf;
    //    b->buf=tmp;
    //    a->buf=(char*)realloc(a->buf,(a->len+1)*sizeof(char));
    //    b->buf=(char*)realloc(b->buf,(b->len+1)*sizeof(char));
    //    if(a->buf==NULL||b->buf==NULL)
    //    {
    //     free(a->buf);
    //     free(b->buf);
    //     exit(EXIT_FAILURE);
    //    }
    struct strbuf temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
// 5.将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小 (不太理解)
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    *sz = sb->alloc;
    return sb->buf;
}
// 6.比较两个 strbuf 的内存是否相同(包括存的东西)
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    // return first->len-second->len;
    if (first->len == second->len)
        return 0;
    else
        return 1;
}
// 7.清空 sb
void strbuf_reset(struct strbuf *sb)
{
    for (int i = 0; i < sb->len; i++)
        sb->buf[i] = '\0';
    sb->len = 0;
}
// 1.确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if (sb->alloc < sb->len + 1 + extra)
        sb->alloc = sb->len + 1 + extra;
    sb->buf = (char *)realloc(sb->buf, sb->alloc * sizeof(char));
    if (sb->buf == NULL)
    {
        exit(EXIT_FAILURE);
    }
}
// 2.向 sb 追加长度为 len 的数据 data
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb, len);
    // strcat(sb->buf,(char*)data);不对！！
    for (int i = 0; i < len; i++)
    {
        sb->buf[sb->len + i] = ((char *)data)[i];
    }
    sb->buf[sb->len + len] = '\0';
    sb->len = sb->len + len;
}
// 3.向 sb 追加一个字符
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_grow(sb, 1);
    sb->buf[sb->len] = (char)c;
    sb->buf[sb->len + 1] = '\0';
    sb->len = sb->len + 1;
}
// 4.向 sb 追加一个字符串 s
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    int n = strlen(s);
    strbuf_grow(sb, n);
    // strcat(sb->buf,s);为啥我想出的第一个过不了！！
    for (int i = 0; i < n; i++)
        sb->buf[sb->len + i] = s[i];
    sb->buf[sb->len + n] = '\0';
    sb->len = sb->len + n;
}
// 5.向一个 sb 追加另一个 strbuf的数据
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    int n = strlen(sb2->buf);
    strbuf_grow(sb, n);
    for (int i = 0; i < n; i++)
        sb->buf[sb->len + i] = sb2->buf[i];
    sb->buf[sb->len + n] = '\0';
    sb->len = sb->len + n;
}
// 设置 sb 的长度 len
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len = len;
    strbuf_grow(sb, len + 1);
    sb->buf[len] = '\0';
}
// 7.计算 sb 目前仍可以向后追加的字符串长度
size_t strbuf_avail(const struct strbuf *sb)
{
    int n = sb->alloc - sb->len - 1;
    return n;
}
// 8.向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    strbuf_grow(sb, len);
    int n = strlen(sb->buf);
    int m = n - pos;
    char ch[m];
    int k = 0;
    for (int i = pos; i < n; i++)
    {
        ch[k] = sb->buf[i];
        k++;
    }
    for (int i = 0; i < len; i++)
        sb->buf[pos + i] = ((char *)data)[i];
    for (int i = 0; i < m; i++)
        sb->buf[pos + len + i] = ch[i];
    sb->buf[sb->len + len] = '\0';
    sb->len = sb->len + len;
}
// 1.去除 sb 缓冲区左端的所有 空格，tab, '\t'
void strbuf_ltrim(struct strbuf *sb)
{
    int i = 0;
    int n = 0;
    while (i < sb->len && isspace((unsigned char)sb->buf[i]))
    {
        if (sb->buf[i] == ' ' || sb->buf[i] == '\t')
            n++;
        i++;
    }
    for (int j = 0; j + n < sb->len; j++)
        sb->buf[j] = sb->buf[j + n];
    sb->len = sb->len - n;
}
// 2.去除 sb 缓冲区右端的所有 空格，tab, '\t'
void strbuf_rtrim(struct strbuf *sb)
{
    int i = sb->len - 1;
    int n = 0;
    while (i >= 0 && isspace((unsigned char)sb->buf[i]))
    {
        if (sb->buf[i] == ' ' || sb->buf[i] == '\t')
            n++;
        i--;
    }
    sb->len = sb->len - n;
}
// 3.删除 sb 缓冲区从 pos 坐标长度为 len 的内容
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    for (int i = pos; i + len < sb->len; i++)
        sb->buf[i] = sb->buf[i + len];
    sb->buf[sb->len - len] = '\0';
    sb->len = sb->len - len;
}
// 1.sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    int n = hint ? hint : 8192;
    strbuf_grow(sb, n);
    int k = read(fd, &sb->buf[sb->len], n);
    // if(k==-1)
    // {
    //     perror("Error");
    //     return -1;
    // }
    sb->len = sb->len + k;
    sb->buf[sb->len] = '\0';
    return k;
}
// 2.将 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    // sb->buf=(char*)realloc(sb->buf,sizeof(char)*1024);
    // while(1)
    // {
    // if(fgets(&sb->buf[sb->len],128-sb->len,fp)==NULL)
    // break;
    // }
    // int n=strlen(&sb->buf[sb->len]);
    // if(sb->buf[sb->len+n-1]=='\n')
    // sb->buf[sb->len+n-1]='\0';
    // sb->len=sb->len+n;
    //  return sb->len;
    // sb->buf=(char*)realloc(sb->buf,sizeof(char)*1024);
    // int ch;
    // while((ch=fgetc(fp))!=EOF&&ch!='\n')
    // sb->buf[sb->len++]==(char)ch;
    // sb->buf[sb->len]='\0';
    // return sb->len;
    int cnt = 0, i;
    while (1)
    {
        if (((i = fgetc(fp)) == '\n' || i == EOF))
            break; // 读取到行结束或文件结尾跳出

        if (strbuf_avail(sb) >= 1) // 可追加字符数量达标
        {
            strbuf_addch(sb, i);
            cnt++;
        }
        else if (strbuf_avail(sb) < 1)
        {
            sb->buf = (char *)realloc(sb->buf, sizeof(char) * (sb->alloc + 1));
            strbuf_addch(sb, i);
            cnt++;
        }
    }
    sb->len = cnt;
    return 1;
}
// 1.将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回，max 可以用来限定最大切割数量。返回 struct strbuf 的指针数组，数组的最后元素为 NULL
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    int i = 0;
    char *ch = (char *)str;
    int m = sizeof(str);
    for (int j = 0; j < m - 1; j++)
    {
        if (ch[j] == '\0')
            ch[j] = '`';
    }
    const char s[2] = {(char)terminator, '\0'};
    struct strbuf **p = (struct strbuf **)malloc(sizeof(struct strbuf *) * (max + 1));
    char *r = strtok(ch, s);
    while (r != NULL && i < max)
    {
        int n = strlen(r);
        struct strbuf *q = (struct strbuf *)malloc(sizeof(struct strbuf));
        strbuf_init(q, n + 1);
        memcpy(q->buf, r, n);
        for (int h = 0; h < n; h++)
        {
            if (q->buf[h] == '`')
                q->buf[h] = '\0';
        }
        q->buf[n] = '\0';
        q->len = n;
        p[i] = q;
        i++;
        r = strtok(NULL, s);
    }
    p[i] = NULL;
    return p;
}
// 太开心了！！
// 1.target_str : 目标字符串，str : 前缀字符串，strlen : target_str 长度 ，前缀相同返回 true 失败返回 false
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
{
    //     target_str[strnlen]='\0';
    //     if(strcmp(target_str,str)==0)
    if (str == NULL || strncmp(target_str, str, strlen(str)) == 0)
        return true;
    else
        return false;
}
// 1.target_str : 目标字符串，begin : 开始下标，end 结束下标。len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，[begin, end)区
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
    if (target_buf == NULL || begin < 0 || end < 0 || begin > end)
        return NULL;
    int n = end - begin;
    char *ret = (char *)malloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++)
        ret[i] = target_buf[begin + i];
    ret[end - begin] = '\0';
    return ret;
}
