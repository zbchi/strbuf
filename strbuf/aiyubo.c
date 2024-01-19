#include "strbuf.h"
//2A
void strbuf_init(struct strbuf *sb, size_t alloc){
    sb->len=0;
    sb->alloc=alloc;
    sb->buf=(char*)malloc(sizeof(char)*(alloc+1));
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc){
    sb->buf=(char*)str;
    sb->len = len;
    sb->alloc = alloc;
    sb->buf[sb->len]='\0';
}
void strbuf_release(struct strbuf *sb){
    free(sb->buf);
    sb->buf = NULL;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b){
    char*ret_t=a->buf;
    a->buf=b->buf;
    b->buf=ret_t;
    int temp=a->len;
    a->len=b->len;
    b->len=temp;
    temp=a->alloc;
    a->alloc=b->alloc;
    b->alloc=temp;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz){
    *sz=sb->alloc ;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second){
    if (first->len != second->len){
        return -1; 
    }
    return memcmp(first->buf, second->buf, first->len) ;
}
void strbuf_reset(struct strbuf *sb){
    for(int i=0;i<sb->alloc;i++)
        sb->buf[i]='\0';
    sb->len = 0;
}
//2B
void strbuf_grow(struct strbuf *sb, size_t extra){
    if(sb->len+extra>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+extra+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+extra;
        }
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len){
    if(sb->len+len>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+len+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+len;
        }
    }
    memcpy(sb->buf+sb->len, data, len);
    sb->len= sb->len+len;
    sb->buf[sb->len]='\0';
}
void strbuf_addch(struct strbuf *sb, int c)
{
    if(sb->len+1>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+2));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+1;
        }
    }
    sb->len= sb->len+1;
    sb->buf[sb->len-1]=c;
    sb->buf[sb->len]='\0';
}
void strbuf_addstr(struct strbuf *sb, const char *s){
    int len=strlen(s);
     if(sb->len+len>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+len+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+len;
        }
    }
    memcpy(sb->buf+sb->len, s, len);
    sb->len= sb->len+len;
    sb->buf[sb->len]='\0';
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2){
    if(sb->len+sb2->len>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+sb2->len+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+sb2->len;
        }
    }
    memcpy(sb->buf+sb->len, sb2->buf, sb2->len);
    sb->len=sb->len+sb2->len;
    sb->buf[sb->len]='\0';
}
void strbuf_setlen(struct strbuf *sb, size_t len){
    if(sb->alloc<len){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(len+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=len;
        }
    }
    sb->len=len;
    sb->buf[sb->len]='\0';
}
size_t strbuf_avail(const struct strbuf *sb){
    return sb->alloc-sb->len-1;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len){
     if(sb->len+len>sb->alloc){
        char*ret=(char*)realloc(sb->buf,sizeof(char)*(sb->len+len+1));
        if(ret!=NULL){
            sb->buf=ret;
            sb->alloc=sb->len+len;
        }
    }
    memmove(sb->buf + pos + len, sb->buf + pos, sb->len - pos);
    memcpy(sb->buf + pos, data, len);
    sb->len += len;
    sb->buf[sb->len]='\0';
}
//2C
void strbuf_ltrim(struct strbuf *sb){
   if(sb==NULL||sb->buf==NULL||sb->len==0)
        return ;
    int i=0;
    while(i<sb->len&&(sb->buf[i]==' '||sb->buf[i]=='\t'))
        i++;
    if(i>0){
        for(int j=0;j<sb->len-i;j++)
            sb->buf[j]=sb->buf[i+j];
        sb->len=sb->len-i;
    }
}
void strbuf_rtrim(struct strbuf *sb){
    while(sb->buf[sb->len-1]==' '||sb->buf[sb->len-1]=='\t'){
            sb->len--;
            sb->buf[sb->len]='\0';
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
    memmove(sb->buf + pos, sb->buf + pos + len, sb->len - pos - len);
    sb->len = sb->len - len;
}
//2D
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint){
    FILE *fp;
    char ch;
    if (((fp = fdopen(fd, "r")) == NULL) || (ch = fgetc(fp)) == EOF)
        return sb->len;
    else{
        sb->alloc += (hint ? hint : 8192);
        char*ret = (char *) realloc(sb->buf, sizeof(char) * (sb->alloc));
        sb->buf=ret;
        sb->buf[sb->len] = ch;
        sb->len++;
        while ((ch = fgetc(fp)) != EOF){
            sb->buf[sb->len++] = ch;
        }
        sb->buf[sb->len] = '\0';
        return sb->len;
    }
}
int strbuf_getline(struct strbuf *sb, FILE *fp){
    char ch;
    while ((ch=fgetc(fp)) != EOF){
        if (ch == '\n'  || ch == '\r')  
             break;
        strbuf_addch(sb, ch);
    }
    return sb->len;
}
//CHALLENGE
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max){
    char str2[2];
    str2[0] = (char) terminator,str2[1] = '\0';
    int i,count = 0;
    struct strbuf **ret= (struct strbuf **) malloc(sizeof(struct strbuf *)*(max+1) );
    struct strbuf *ret_t;
    char str3[len + 1];
    memcpy(str3, str, len + 1);
    for (i = 0; i < len; i++) {
        if (str3[i] == '\0')
        str3[i] = '=';
    }
    char *ch = strtok(str3, str2);
    while (ch != NULL && count < max) {
        int len2 = strlen(ch);
        for (i = 0; i < len2; i++) {
            if (ch[i] == '=')
            ch[i] = '\0';
        }
        ret_t = (struct strbuf *) malloc(sizeof(struct strbuf));
        strbuf_init(ret_t, len2 + 1);
        strbuf_add(ret_t, ch, len2);
        ret[count] = ret_t;
        count++;
        ch = strtok(NULL, str2);
    }
    ret[count] = NULL;
    return ret;
}
bool strbuf_begin_judge(char* target_str, const char* str, int strnlen){

    if(str==NULL||strncmp(target_str,str,strlen(str))==0)
        return true;
    return false;
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len){
    if(target_buf==NULL||begin>end)
        return NULL;
    char* ret = (char *)malloc(sizeof(char)*(end-begin+1));

    for(int i=begin, j=0;i<end;i++,j++)/* 引用*/
        ret[j]=target_buf[i];

    /*memmove(ret,target_buf+begin,end-begin+1);*//*拷贝*/

    ret[end-begin]='\0';
    return ret;
}