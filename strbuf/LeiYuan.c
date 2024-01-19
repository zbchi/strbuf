#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
struct strbuf {
    int len;  // 当前缓冲区（字符串）长度
    int alloc;// 当前缓冲区（字符串）容量
    char *buf;// 缓冲区（字符串）
};

// PART     A
void strbuf_init(struct strbuf *sb, size_t alloc);
// 初始化
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);
// 字符串填充到sb,长度为len,容量为alloc
void strbuf_release(struct strbuf *sb);
// 释放sb的内存
void strbuf_swap(struct strbuf *a, struct strbuf *b);
// 交换两个strbuf
char *strbuf_detach(struct strbuf *sb, size_t *sz);
// 将sb中的原始内存取出,sz设置为其alloc大小
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
// 比较两个strbuf内存是否相同
void strbuf_reset(struct strbuf *sb);
// 清空sb
void strbuf_init(struct strbuf *sb, size_t alloc) {
    sb->len = 0;
    sb->alloc = alloc;
    sb->buf = (char *) malloc(sizeof(char) * alloc);
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc) {
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char *) str;
}
void strbuf_release(struct strbuf *sb) {
    free(sb->buf);
    sb->buf = NULL;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b) {
    int temp = a->len;
    a->len = b->len;
    b->len = temp;
    int all_temp = a->alloc;
    a->alloc = b->alloc;
    b->alloc = all_temp;
    char *str = a->buf;
    a->buf = b->buf;
    b->buf = str;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz) {
    char *str;
    str = sb->buf;
    *sz = sb->alloc;
    return str;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second) {
    if (first->len > second->len) {
        return 1;
    } else if (first->len < second->len) {
        return -1;
    } else {
        return 0;
    }
}
void strbuf_reset(struct strbuf *sb) {
    int i = 0;
    for (i = 0; i < sb->len; i++) {
        sb->buf[i] = '\0';
    }
    sb->len = 0;
}

// PART     B
void strbuf_grow(struct strbuf *sb, size_t extra);
// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
// 向 sb 追加长度为 len 的数据 data
void strbuf_addch(struct strbuf *sb, int c);
// 向 sb 追加一个字符 c
void strbuf_addstr(struct strbuf *sb, const char *s);
// 向 sb 追加一个字符串 s
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
// 向一个 sb 追加另一个 strbuf的数据
void strbuf_setlen(struct strbuf *sb, size_t len);
// 设置 sb 的长度 len
size_t strbuf_avail(const struct strbuf *sb);
// 计算 sb 目前仍可以向后追加的字符串长度
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);
// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data
void strbuf_grow(struct strbuf *sb, size_t extra)
// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用
{
    if (sb->alloc > sb->len + extra)
        return;
    sb->buf = (char *) realloc(sb->buf, (sb->len + extra + 1) * sizeof(char));
    sb->alloc = sb->len + extra;
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
// 向 sb 追加长度为 len 的数据 data
{
    // if(sb->len+len>sb->alloc){
    //     sb->buf=(char *)realloc(sb->buf,sb->len+len+1);
    //     sb->alloc=sb->len+len+1;
    // }
    // char *str=(char *)data;
    // strcat(sb->buf, str);
    // sb->len = sb->len + len;
    // sb->buf[sb->len]='\0';
    // if (data == NULL) {
    //     return;
    // }
    // if (sb->alloc - sb->len < len) {
    //     sb->buf = (char *)realloc(sb->buf, sb->len + len + 1);
    //     sb->alloc = sb->len + len + 1;
    // }
    // memcmp(sb->buf + sb->len, data, len);
    // sb->len += len;
    // sb->buf[sb->len] = '\0';
    strbuf_grow(sb, len);
    memcpy(sb->buf + sb->len, data, len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}
void strbuf_addch(struct strbuf *sb, int c)
// 向 sb 追加一个字符 c
{
    strbuf_grow(sb, 2);
    (sb->buf)[sb->len] = (char) c;
    sb->len += 1;
    (sb->buf)[sb->len] = '\0';
}
void strbuf_addstr(struct strbuf *sb, const char *s)
// 向 sb 追加一个字符串 s
{
    strbuf_add(sb, s, strlen(s));
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
// 向一个 sb 追加另一个 strbuf的数据
{
    strbuf_add(sb, sb2->buf, sb2->len);
}
void strbuf_setlen(struct strbuf *sb, size_t len)
// 设置 sb 的长度 len
{
    if (len >= sb->alloc) {
        sb->buf = (char *) realloc(sb->buf, len + 1);
        sb->alloc = len + 1;
    }
    sb->len = len;
    sb->buf[sb->len] = '\0';
}
size_t strbuf_avail(const struct strbuf *sb)
// 计算 sb 目前仍可以向后追加的字符串长度
{
    size_t i = sb->alloc - sb->len - 1;
    return i;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data
{
    if (sb->len + len > sb->alloc) {
        strbuf_grow(sb, len + 1);
    }
    memmove(sb->buf + pos + len, sb->buf + pos, sb->len - pos);
    memcpy(sb->buf + pos, data, len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}


//PART      C
void strbuf_ltrim(struct strbuf *sb);
// 去除 sb 缓冲区左端的所有 空格，tab, '\t'。
void strbuf_rtrim(struct strbuf *sb);
//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);
//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
void strbuf_ltrim(struct strbuf *sb)
// 去除 sb 缓冲区左端的所有 空格，tab, '\t'。
{
    while ((*(sb->buf) == ' ' || *(sb->buf) == '\t') && sb->buf[0] != '\0') {
        sb->buf = (char *) memmove(sb->buf, sb->buf + 1, sb->len - 1);
        sb->len--;
    }
    return;
}
void strbuf_rtrim(struct strbuf *sb)
//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
{
    while (((sb->buf)[sb->len - 1] == ' ' || (sb->buf)[sb->len - 1] == '\t') && sb->buf[sb->len - 1] != '\0') {
        sb->buf[sb->len - 1] = '\0';
        sb->len--;
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
{
    memmove(sb->buf + pos, sb->buf + pos + len, sb->len - len - pos);
    sb->len -= len;
    sb->buf[sb->len] = '\0';
    return;
}


//PART    D
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
//sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
int strbuf_getline(struct strbuf *sb, FILE *fp);
//将 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb 。
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
//sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
{
    ssize_t re;
    sb->buf = (char *) realloc(sb->buf, sb->alloc + (hint ? hint : 8192));
    sb->alloc += (hint ? hint : 8192);
    re = read(fd, sb->buf + sb->len, sb->alloc - sb->len - 1);
    if (re) {
        sb->len += re;
        sb->buf[sb->len] = '\0';
    }
    return re;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
//将 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb 。
{
    int c;
    if (fp == NULL)
        return 0;
    while ((c = getc(fp)) != EOF) {
        if (c == '\n')
            break;
        strbuf_add(sb, &c, 1);
    }
    return sb->len;
}


// 挑战
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max);
/*将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 
strbuf,并从结果返回，max 可以用来限定最大切割数量。
返回 struct strbuf 的指针数组，数组的最后元素为 NULL*/
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen);
/*target_str : 目标字符串，str : 前缀字符串，
strlen : target_str 长度 ，前缀相同返回 true 失败返回 false*/
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len);
/*target_str : 目标字符串，begin : 开始下标，end 结束下标。
len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，
[begin, end)区间。*/

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
/*将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 
strbuf,并从结果返回，max 可以用来限定最大切割数量。
返回 struct strbuf 的指针数组，数组的最后元素为 NULL*/
{
    //分配内存
    struct strbuf **res = (struct strbuf **) malloc(sizeof(struct strbuf *) * (max + 1));

    const char *begin = str;
    const char *end = str + len;
    const char *next;
    int cnt = 0;
    int length;
    //跳过字符串开始部分的切割字符
    while (*begin == terminator) {
        begin++;
    }
    for (next = begin; next <= end; next++) {
        //遇到切割字符或者字符串结束的时候进行处理
        if (*next == terminator || next == end) {
            //当前长度
            length = next - begin;
            //分配并初始化一个结构体
            res[cnt] = (struct strbuf *) malloc(sizeof(struct strbuf));
            res[cnt]->len = length;
            //长度为当前记录的长度
            res[cnt]->alloc = length + 1;
            //总长为记录长度加一
            res[cnt]->buf = (char *) malloc(sizeof(char) * res[cnt]->alloc);
            //创建一个buf缓冲区，将剪切的内容写到buf中
            memcpy(res[cnt]->buf, begin, length);
            //手动添加'\0'
            res[cnt]->buf[length] = '\0';
            cnt++;//记录剪切片段个数
            //跳过连续的切割字符
            while (*next == terminator && next <= end) {
                next++;
            }
            //更新指针位置并继续遍历
            begin = next;
        }
        //当剪切数量达到最大值时结束该过程
        if (cnt == max) {
            break;
        }
    }
    //手动添加NULL
    res[cnt] = NULL;
    return res;
}

bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
/*target_str : 目标字符串，str : 前缀字符串，
strnlen : target_str 长度 ，前缀相同返回 true 失败返回 false*/
{
    if (strnlen == 0 || memcmp(target_str, str, strlen(str)) == 0) {
        return true;
    } else {
        return false;
    }
}

char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
/*target_str : 目标字符串，begin : 开始下标，end 结束下标。
len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，
[begin, end)区间。*/
{
    if (end < begin || target_buf == NULL) {
        return NULL;
    }//特判
    char *str;
    str = (char *) malloc(len * sizeof(char));
    memcpy(str, target_buf + begin, end - begin);
    str[end - begin] = '\0';
    return str;
}