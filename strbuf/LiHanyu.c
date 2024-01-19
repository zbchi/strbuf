#include "strbuf.h"
void strbuf_init(struct strbuf *sb, size_t alloc){
    sb->buf = (char*)malloc(alloc*sizeof(char));
    sb->alloc = alloc;
    sb->len = 0;
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc){
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char*)str;
}
void strbuf_release(struct strbuf *sb){
    free(sb->buf);
}
void strbuf_swap(struct strbuf *a, struct strbuf *b){
    struct strbuf p  = *a;
    *a = *b;
    *b = p;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz){
    *sz = sb->alloc;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second){
    int i = memcmp(first->buf, second->buf,second->len);
    if (first->len == second->len && i == 0)
        return 0;
    else 
        return 1; 
}
void strbuf_reset(struct strbuf *sb){
    sb->len = 0;
    sb->buf[0] = '\0';
}
void strbuf_grow(struct strbuf *sb, size_t extra){
    sb->buf = (char*)realloc(sb->buf,extra);
    sb->alloc = sb->len + extra;
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len){
    if(sb->len+len >= sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sb->alloc);
    }
    memcpy(sb->buf + sb->len,data,len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}
void strbuf_addch(struct strbuf *sb, int c){
    if (sb->len + 1 >= sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf, sb->alloc);
    }
    sb->buf[sb->len] = c;
    sb->len++;
    sb->buf[sb->len] = '\0';
}
void strbuf_addstr(struct strbuf *sb, const char *s){
    int sl = strlen(s);
    while (sb->len + sl >= sb->alloc) {
        sb->alloc *= 2;
        sb->buf = (char *)realloc(sb->buf, sb->alloc);
    }
    strcpy(sb->buf + sb->len, s);
    sb->len += sl;
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2){
    while (sb->len + sb2->len >= sb->alloc) {
        sb->alloc *= 2;
        sb->buf = (char *)realloc(sb->buf, sb->alloc);
    }
    strcpy(sb->buf + sb->len, sb2->buf);
    sb->len += sb2->len;
}
void strbuf_setlen(struct strbuf *sb, size_t len){
    sb->len = len;
    sb->buf[sb->len] = '\0';
}
size_t strbuf_avail(const struct strbuf *sb){
    return sb->alloc - sb->len - 1;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len){
    while (sb->len + len >= sb->alloc) {
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sb->alloc);
    }
    for (int i = sb->len; i >= pos; i--) 
        sb->buf[i + len] = sb->buf[i];
    char *p = (char*)data;
    for (int i = 0; i < len; i++)
        sb->buf[pos + i] = p[i];
    sb->len += len;
}
void strbuf_rtrim(struct strbuf *sb){
    int i = sb->len - 1;
    while (i >= 0 && sb->buf[i] == ' ' || sb->buf[i] == '\t')
        i--;
    if (i < sb->len - 1){
        int len = i + 2;
        sb->len = len - 1;
        sb->buf[len - 1] = '\0';
    }
}
void strbuf_ltrim(struct strbuf *sb){
    int i = 0;
    while(sb->buf[i] == ' ' || sb->buf[i] == '\t')
        i++;
    if (i > 0) {
        int len = sb->len - i + 1; 
        memmove(sb->buf, sb->buf + i,len);
        sb->len = sb->len - i; 
        sb->buf[sb->len] = '\0';
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
    while(pos + len <= sb->len){
        sb->buf[pos] = sb->buf[pos + len];
        pos++;
    } 
    sb->len -= len;
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint){
    int i = hint ? hint : 8192;
    sb->alloc += i;
    sb->buf = (char*)realloc(sb->buf,sb->alloc);
    ssize_t r = read(fd,sb->buf+sb->len,sb->alloc-sb->len-1);
    if(r != -1){
        sb->len += r;
        sb->buf[sb->len] = '\0';
    }
    return r;
}
int strbuf_getline(struct strbuf *sb, FILE *fp){
    char *line = NULL;
    size_t len = 0;
    ssize_t r = getline(&line, &len, fp);
    if(r != -1){
        if (line[r - 1] == '\n') {
            line[r - 1] = '\0';
            r--;
        }
    }
    size_t i = sb->len + r;
    sb->buf = (char *)realloc(sb->buf, i + 1);
    strcpy(sb->buf + sb->len, line);
    sb->len = i;
    free(line);
    return r;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max){
    struct strbuf **p = (struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
    const char *begin = str;
    const char *end = str+len;  
    while(*begin == terminator)
        begin++;
    int i = 0;
    for (const char *j = begin;j <= end;j++){
        if (*j == terminator || j == end){
            int l2=j-begin;
            p[i]=(struct strbuf*)malloc(sizeof(struct strbuf));
            p[i]->len=l2;
            p[i]->alloc=l2+1;
            p[i]->buf=(char*)malloc(l2+1);
            memcpy(p[i]->buf,begin,l2);
            *(p[i++]->buf + l2) = '\0';
            while(*j == terminator && j <= end)
                j++;
            begin = j;
        }
        if(i == max)
            break;
    }
    p[i]=NULL;
    return p;
}
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen){
    if(strnlen == 0 || memcmp(target_str,str,strlen(str)) == 0)
        return true;
    else 
        return false;
}
char* strbuf_get_mid_buf(char *target_buf, int begin, int end, int len){
    if(end < begin||target_buf == NULL)
        return NULL;
    char *p = (char*)malloc(end-begin + 1);
    memcpy(p,target_buf + begin,end-begin);
    p[end-begin]='\0';
    return p;
}
