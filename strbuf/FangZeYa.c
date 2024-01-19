#include "strbuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strbuf_init(struct strbuf *sb, size_t alloc) {
    sb->alloc = alloc;
    sb->len = 0;
    sb->buf = (char *)malloc(sizeof(char) * alloc);
}

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc) {
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char *)str;
}

void strbuf_release(struct strbuf *sb) {
    free(sb->buf);
    sb->buf = NULL;
}

void strbuf_swap(struct strbuf *a, struct strbuf *b) {
    struct strbuf temp = *a;
    *a = *b;
    *b = temp;
}

char *strbuf_detach(struct strbuf *sb, size_t *sz) {
    char *str = sb->buf;
    *sz = sb->alloc;
    sb->len = 0;
    sb->buf = NULL;
    return str;
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second) {
    return memcmp(first->buf, second->buf, first->alloc);
}

void strbuf_reset(struct strbuf *sb) {
    if (sb->buf != NULL) {
        free(sb->buf);
    }
    sb->buf = NULL;
    sb->len = 0;
}

//----------------------------------------------------------------------

void strbuf_grow(struct strbuf *sb, size_t extra) {
    if (sb->alloc - sb->len < extra) {
        sb->buf = (char *)realloc(sb->buf, sizeof(char) * (sb->len + extra));
        sb->alloc = sb->len + extra;
    }
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len) {
    strbuf_grow(sb, len + 1);
    memcpy(sb->buf + sb->len, data, len);
    sb->buf[sb->len + len] = '\0';
    sb->len += len;
}

void strbuf_addch(struct strbuf *sb, int c) { strbuf_add(sb, &c, 1); }

void strbuf_addstr(struct strbuf *sb, const char *s) {
    strbuf_add(sb, s, strlen(s));
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2) {
    sb->alloc = sb->len + sb2->len + 1;
    sb->buf = (char *)realloc(sb->buf, sizeof(char) * sb->alloc);
    strcpy(sb->buf + sb->len, sb2->buf);
    sb->len += sb2->len;
}

void strbuf_setlen(struct strbuf *sb, size_t len) {
    if (len > sb->alloc) {
        sb->buf = (char *)realloc(sb->buf, len * sizeof(char));
        sb->len = len;
        sb->alloc = len;
        return;
    }
    sb->len = len;
    sb->buf[sb->len] = '\0';
}

size_t strbuf_avail(const struct strbuf *sb) { return sb->alloc - sb->len - 1; }

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data,
        size_t len) {
    if (sb->alloc < sb->len + len) {
        sb->buf = (char *)realloc(sb->buf, (sb->len + len + 1) * sizeof(char));
    }
    char *str = sb->buf + pos;
    memmove(sb->buf + pos + len, str, sb->len - pos + 1);
    memcpy(sb->buf + pos, data, len);
    sb->len += len;
}

//--------------------------------------------------------------------------

void strbuf_rtrim(struct strbuf *sb) {
    while (sb->len > 0 &&
            (sb->buf[sb->len - 1] == ' ' || sb->buf[sb->len - 1] == '\t')) {
        sb->len--;
    }
    sb->buf[sb->len] = '\0';
}

void strbuf_ltrim(struct strbuf *sb) {
    int pos = 0;

    while (pos < sb->len && (sb->buf[pos] == ' ' || sb->buf[pos] == '\t')) {
        pos++;
    }

    if (pos > 0) {
        memmove(sb->buf, sb->buf + pos, sb->len - pos);
        sb->len -= pos;
        sb->buf[sb->len] = '\0';
    }
}

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len) {
    size_t end = pos + len;
    if (pos >= sb->len) {
        return;
    }
    if (end >= sb->len) {
        end = sb->len;
    }
    memmove(sb->buf + pos, sb->buf + end, sb->len - end + 1);
    sb->len -= len;
}

//----------------------------------------------------------------------------------

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint) {
    FILE *fp = fdopen(fd, "r");
    char ch;
    if (fp == NULL || (ch = fgetc(fp)) == EOF)
        return sb->len;
    strbuf_grow(sb, (hint ? hint : 8192) + 1);
    sb->buf[sb->len++] = ch;
    while ((ch = fgetc(fp)) != EOF) {
        sb->buf[sb->len] = ch;
        sb->len++;
    }
    sb->buf[sb->len] = '\0';
    return sb->len;
}

int strbuf_getline(struct strbuf *sb, FILE *fp) {
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n' || ch == '\r')
            break;
        strbuf_addch(sb, ch);
    }
    return 1;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator,int max) {
    struct strbuf **result = (struct strbuf **) malloc(sizeof(struct strbuf *) * (max + 1));

    const char * begin = str;
    const char * end = str + len;
    const char * next;
    int count = 0;
    int length;

    while(*begin == terminator)
        begin++;

    for(next = begin; next <= end; next++)
    {
        if(* next == terminator || next == end)
        {
            length = next - begin;
            result[count] = (struct strbuf *)malloc(sizeof(struct strbuf));
            result[count]->len = length;
            result[count]->alloc = length + 1;
            result[count]->buf = (char *)malloc(sizeof(char)*result[count]->alloc);
            memcpy(result[count]->buf, begin, length);
            *(result[count]->buf + length) = '\0';
            count++;

            while (*next == terminator && next <= end)
                next++;

            begin = next;
        }

        if(count == max)
            break;
    }
    result[count] = NULL;
    return result;
}

bool strbuf_begin_judge(char *target_str, const char *str, int strnlen) {
    return (str==NULL||strncmp(target_str,str,strlen(str))==0);
}
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len) {
    if(target_buf == NULL)
        return 0;
    char * str;
    str = (char *)malloc(end - begin + 1);
    memcpy(str, target_buf + begin, end - begin);
    str[end - begin] = '\0';
    return str;
}
