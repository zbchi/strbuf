#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "strbuf.h"

/*struct strbuf
{
    int len;
    int alloc;
    char *buf;
};*/

//Part 2A
void strbuf_init(struct strbuf *sb, size_t alloc){
    sb->len = 0;
    sb->alloc = alloc;
    sb->buf = (char*)malloc(sizeof(char)*alloc);
}

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc){
    sb->alloc = alloc;
    sb->len = len;
    sb->buf = (char*)str;
}

void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf = NULL;
    sb->alloc = 0;
    sb->len = 0;
}

void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    char *temp;
    int cnt;

    //先交换容量与长度
    cnt = a->len;
    a->len = b->len;
    b->len = cnt;
    cnt = a->alloc;
    a->alloc = b->alloc;
    b->alloc = cnt;

    temp = a->buf;
    a->buf =b->buf;
    b->buf = temp;

}

char *strbuf_detach(struct strbuf *sb, size_t *sz){
    char *source;
    source = sb->buf;
    *sz = sb->alloc;
    return source; 
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second){
    if(first->len == second->len)
        return memcmp(first->buf,second->buf,first->len);
    else 
        return 1;
}

void strbuf_reset(struct strbuf *sb)
{
    memset(sb->buf,0,sb->len);
    sb->len = 0;
}

//Part 2B
void strbuf_grow(struct strbuf *sb, size_t extra){
    if(sb->len + extra > sb->alloc){
        sb->buf = (char*)realloc(sb->buf,(sb->alloc)*2);
        sb->alloc = sb->len + extra;
    }
    else 
        return;
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len){
    if(data == NULL)
        return;
    while(sb->len + len >= sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sb->alloc);
    }
    //strncat(sb->buf,(char*)data,len);
    memcpy(sb->buf + sb->len,data,len);//将缓冲指针向后移动sb->len个字节，确保不覆盖原来的数据
    sb->len += len;
    sb->buf[sb->len] = '\0';
    
}

void strbuf_addch(struct strbuf *sb, int c){
    if(sb->len + 2 >= sb->alloc){
        sb->buf = (char*)realloc(sb->buf,(sb->alloc)*2);
        sb->alloc *= 2;
    }
    sb->buf[sb->len] = c;
    sb->len++;
    sb->buf[sb->len] = '\0';

}

void strbuf_addstr(struct strbuf *sb, const char *s){
    int len = strlen(s);
    strbuf_add(sb,s,len);

}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2){
    while(sb->len + sb2->len >= sb->alloc){
        sb->buf = (char*)realloc(sb->buf,(sb->alloc)*2);
        sb->alloc *= 2;
    } 
    strcpy(sb->buf + sb->len,sb2->buf);
    sb->len += sb2->len;
    
}

void strbuf_setlen(struct strbuf *sb, size_t len){
    sb->len = len;
    sb->buf[sb->len] = '\0';

}

size_t strbuf_avail(const struct strbuf *sb){
    int remain = sb->alloc - sb->len - 1;
    return remain;

}


void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len){
    while(sb->len + len >= sb->alloc){
        sb->buf = (char*)realloc(sb->buf,(sb->alloc)*2);
        sb->alloc *= 2;
    } 
    memmove(sb->buf + (pos + len),sb->buf + pos,sb->len - pos);
    memcpy(sb->buf + pos,data,len);
    sb->len += len;
    sb->buf[sb->len] = '\0';

}


//Part 2C
void strbuf_ltrim(struct strbuf *sb){
    int index = 0,i;
    for(i=0; i<sb->len; i++){
        if(sb->buf[i] != ' ' && sb->buf[i] != '\t'){
            index = i;
            break;
        }
    }
    sb->len -= index; 
    for(int j=0; j<sb->len; j++){
        sb->buf[j] = sb->buf[index++];
    }
    sb->buf[sb->len] = '\n';

}

void strbuf_rtrim(struct strbuf *sb){
    int index = 0,i;
    for(i=sb->len-1; i>=0; i--){
        if(sb->buf[i] != ' ' && sb->buf[i] != '\t'){
            index = i;
            break;
        }
    }
    sb->len = sb->len-(sb->len - index)+1;
    sb->buf[sb->len] = '\n';

}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
    memmove(sb->buf+pos, sb->buf+(pos + len), sb->len-(len + pos));
    sb->len -= len;
}


//Part 2D
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint){
    sb->alloc += (hint ? hint : 8192);
    sb->buf = (char*)realloc(sb->buf,sb->alloc);
    size_t filelen = read(fd, sb->buf + sb->len, sb->alloc);
    if(filelen == -1 || filelen == EOF){
        exit(EXIT_FAILURE);
        return sb->len;
    }
    else{
        sb->len += filelen; 
        sb->buf[sb->len] = '\0';
        return sb->len;  
    }
}

int strbuf_getline(struct strbuf *sb, FILE *fp){
    char c;
    while((c=fgetc(fp)) != EOF && c != '\n'){//一次只读一个！
        strbuf_addch(sb,c);
    }
    return 0;
}


//CHALLENGE
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max){
    struct strbuf **ret = (struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
    const char *begin = str;
    //char *end;
    int j,i = 0,tokenlen = 0;
    for(j=0; j<max;){
        tokenlen = 0;
        int flag = 0;
        while(i<len){
            if(str[i] != terminator){
                tokenlen++;
            }//算长度
            if(str[i] != terminator && (str[i+1] == terminator || i+1 == len)){
                //end = i + 1;
                begin = str + (i + 1) - tokenlen;
                i++;
                flag = 1;
                break;
            }//切割操作
            i++;
        }
        if(flag == 1){
            ret[j] = (struct strbuf*)malloc(sizeof(struct strbuf));
            ret[j]->buf = (char*)malloc(tokenlen + 1);
            ret[j]->len = tokenlen;
            ret[j]->alloc = tokenlen + 1;
            memcpy(ret[j]->buf,begin,tokenlen);
            ret[j]->buf[tokenlen] = '\0';
            j++;
        }
        else
            break;
    }
    ret[j] = NULL;
    return ret;
   
}

bool strbuf_begin_judge(char *target_str, const char *str, int strnlen){
    //size_t len = strlen(str);
    if(strnlen == 0 || (memcmp(target_str,str,strlen(str))==0))//顺序不能改变
        return true;
    else
        return false;

}

char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len){
    int i,j;
    if(target_buf == NULL || begin > end){
        return NULL;
    }
    char *result = (char*)malloc(end - begin + 1);//先判断再分配空间，否则目标字符串为空指针时会出现内存泄漏！
    for(i=begin,j=0; i<end; i++){
        result[j++]=target_buf[i];
    }
    result[j] = '\0';
    /*memcpy(result,target_buf + begin,end - begin);
    result[end - begin] = '\0';*/
    return result;

}
