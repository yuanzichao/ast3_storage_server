#include "protocol_server.h"


ssize_t
Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;
    
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        
        nleft -= nread;
        ptr += nread;
    }
    
    return (n - nleft);
}

ssize_t
Writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    
    ptr = vptr;
    
    nleft = n; 
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <=0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else 
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

static void
ast3data_set(ast3_data_t *ast3data, uint32_t protocl, uint32_t flag, uint32_t code, uint32_t len)
{
#ifdef BENDIAN
    protocol = swap_int32(protocol);
    flag = swap_int32(flag);
    code = swap_int32(code);
    len = swap_int32(len);
#endif
    ast3data->protocol = protocl;
    ast3data->flag = flag;
    ast3data->code = code;
    ast3data->len = len;
}

static int
ast3_send(int sockfd, uint32_t protocol, uint32_t flag, uint32_t code, const void *data, uint32_t len)
{
    ast3_data_t *ast3data;
    ssize_t n;
    
    if (data == NULL)
        len = 0;
    
    ast3data = (ast3_data_t *) malloc(sizeof(ast3_data_t) + len);
    if (ast3data == NULL)
        return AST3_ENOMEM;
    memcpy(ast3data->data, data, len);
    ast3data_set(ast3data, protocol, flag, code, len);
    n = Writen(sockfd, ast3data, sizeof(ast3_data_t) + len);
    if (n < 0) {
        free(ast3data);
        switch (errno) {
            case ENETDOWN:
                return AST3_ENETDOWN;
                break;
            case ENETUNREACH:
                return AST3_ENETUNREACH;
                break;
            case EPIPE:
                return AST3_EPIPE;
                break;
            default:
                return AST3_ERROR;
                break;
        }
    }
    free(ast3data);
    return AST3_OK;
}

static int
ast3_recv(int sockfd, uint32_t *protocol, uint32_t *flag, uint32_t *code, uint32_t *len, void *data, size_t length)
{
    ast3_data_t ast3data;
    ssize_t n;
    n = Readn(sockfd, &ast3data, sizeof(ast3_data_t));
    if (n < 0) {
        switch (errno) {
            case ENETDOWN:
                return AST3_ENETDOWN;
                break;
            case ENETUNREACH:
                return AST3_ENETUNREACH;
                break;
            case EPIPE:
                return AST3_EPIPE;
                break;
            default:
                return AST3_ERROR;
                break;
        }
    } else if (n == 0)
        return AST3_ECLOSED;
    
#ifdef BENDIAN
    ast3data.protocol = swap_int32(ast3data.protocol);
    ast3data.flag = swap_int32(ast3data.flag);
    ast3data.code = swap_int32(ast3data.code);
    ast3data.len = swap_int32(ast3data.len);
#endif
    *protocol = ast3data.protocol;
    *flag = ast3data.flag;
    *code = ast3data.code;
    *len = ast3data.len;
    
    if (data != NULL) {
        char *buf = (char *) malloc(ast3data.len);
        if (buf == NULL)
            return AST3_ENOMEM;
        n = Readn(sockfd, buf, ast3data.len);
        if (n < 0) {
            free(buf);
            switch (errno) {
                case ENETDOWN:
                    return AST3_ENETDOWN;
                    break;
                case ENETUNREACH:
                    return AST3_ENETUNREACH;
                    break;
                default:
                    return AST3_ERROR;
                    break;
            }
        } else if (n == 0) {
            free(buf);
            return AST3_ECLOSED;
        }
        size_t actual_len = MIN(length, *len);
        if (length != 0) {
            memset(data, 0, length);
            memcpy(data, buf, actual_len);
        }
        *len = (uint32_t)actual_len;
        free(buf);
    }
    return AST3_OK;
}


List
list_init(void)
{
    List l = NULL;
    if ((l = (List) malloc(sizeof(struct Node))) != NULL)
        l->next = NULL;
    return l;
}

int
is_list_empty(List l)
{
    return (l->next == NULL);
}

int
is_list_last(Position p)
{
    return (p->next == NULL);
}

Position
find_list(List l, void *data)
{

    if (data == NULL)
        return NULL;

    Position p = l->next;
    while ((p != NULL) && p != data)
        p = p->next;
    return p;
}

Position
find_list_previous(List l, void *data)
{
    if (data == NULL)
        return NULL;

    Position p = l;
    while (p->next != NULL && p->next->data != data)
        p = p->next;
    return p;
}

void
delete_list_item(List l, void *data)
{
    Position p, tmp = NULL;

    p = find_list_previous(l, data);
    if (p->next != NULL) {
        tmp = p->next;
        p->next = tmp->next;
    }

    free(tmp);
}

Position
find_list_item_accurate(List l, void *data, int (*compar)(const void *, const void *))
{
    if (data == NULL)
        return NULL;

    Position p = l->next;
    while ((p != NULL) && compar(p->data, data) != 0)
        p = p->next;
    return p;
}

Position
find_list_previous_accurate(List l, void *data, int (*compar)(const void *, const void *))
{
    if (data == NULL)
        return NULL;

    Position p = l;
    while ((p->next != NULL) && compar(p->next->data, data) != 0)
        p = p->next;
    return p;
}

void
delete_list_accurate(List l, void *data, int (*compar)(const void *, const void *))
{
    Position p, tmp = NULL;

    p = find_list_previous_accurate(l, data, compar);
    if (p->next != NULL) {
        tmp = p->next;
        p->next = tmp->next;
    }

    free(tmp);
}

int
insert_list(Position p, void *data)
{
    Position tmp;

    if ((tmp = (Position) malloc(sizeof(struct Node))) == NULL)
        return -1;

    tmp->data = data;
    tmp->next = p->next;
    p->next = tmp;
    return 0;
}

int
list_append(List l, void *data)
{
    Position p = l, tmp;

    if ((tmp = (Position) malloc(sizeof(struct Node))) == NULL)
        return -1;

    while (p->next != NULL)
        p = p->next;
    tmp->data = data;
    tmp->next = NULL;
    p->next = tmp;

    return 0;
}

void
delete_list(List l)
{
    Position p, tmp;
    p = l->next;
    l->next = NULL;

    while (p != NULL) {
        tmp = p->next;
        free(p);
        p = tmp->next;
    }
}

void
list_clean_and_destroy(List l, void (*free_data)(void *))
{
    Position tmp;
    do {
        tmp = l->next;
        free_data(l->data);
        free(l);
        l = tmp;
    } while (l != NULL);
}

void
list_destroy(List l)
{
    Position tmp;
    do {
        tmp = l->next;
        free(l);
        l = tmp;
    } while (l != NULL);
}

int
tcp_listen(const char *hostname, const char *servname, SA *sockaddr, socklen_t *addrlen)
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    const int optval = 1;
    int lfd, ret;

    memset(&hint, '\0', sizeof(struct addrinfo));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_addrlen = 0;
    hint.ai_addr = NULL;
    hint.ai_canonname = NULL;
    hint.ai_next = NULL;

    if ((ret = getaddrinfo(hostname, servname, &hint, &ailist)) != 0) {
        return -1;
    }

    aip = ailist;
    do {
        lfd = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
        if (lfd < 0)
            continue;
        setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        if (bind(lfd, aip->ai_addr, aip->ai_addrlen) == 0) {
            if (addrlen != NULL) {
                *addrlen = aip->ai_addrlen;
            }
            if (sockaddr != NULL && addrlen != NULL)
                memcpy(sockaddr, aip->ai_addr, *addrlen);
            break;
        }
        close(lfd);
    } while ((aip = aip->ai_next) != NULL);

    listen(lfd, BACKLOG);
    freeaddrinfo(ailist);
    return lfd;
}


int
ast3array_recv_req(int sockfd, char *data, size_t size)
{
    uint32_t protocol, flag, code, len;
    int ret = ast3_recv(sockfd, &protocol, &flag, &code, &len, data, size);
    if (protocol != ARRAY_PROTOCOL) {
        ret = AST3_EPROTOCOL;
    }
    return ret;
}

int
ast3array_send_ack(int sockfd, uint32_t code, const char *data)
{
    uint32_t protocol = ARRAY_PROTOCOL, flag = 0, len = (uint32_t) strlen(data);
    return ast3_send(sockfd, protocol, flag, code, data, len);
}



/**
 * 和客户端通信，发送相关文件
 */
int
send_files() {

	int ret;
	int lfd, cfd;
	uint32_t code;

	//
	char request[BUFSIZE];
	memset(request, 0, BUFSIZE);
	lfd = tcp_listen(NULL, PORT, NULL, NULL);
	for (; ;) {
		cfd = accept(lfd, NULL, NULL);

		ret = ast3array_recv_req(cfd, request, BUFSIZE);


		//解析request内容，解析出请求文件的文件名及个数
		int request_num = 0;
		char *argv[10] = {};

		int i;
		for(0; i<request_num; i++) {

			FILE *src_fp;
			if ((src_fp = fopen(argv[i], "rb")) == NULL) {
				printf("fopen %s failed./n", request);
				return -1;
			}


			ast3array_send_ack(cfd, code, argv[i]);


			char src_buf[BUFSIZE];
			bzero(src_buf, BUFSIZE);

			int length = 0;

			//每读取一段数据，便将其发送给客户端，循环直到文件读完为止
			while((length = fread(src_buf, sizeof(char), BUFSIZE, src_fp)) > 0) {
				if(send(cfd, src_buf, length, 0) < 0) {
					printf("Send File:%s Failed.\n", request);
					break;
				}
				bzero(src_buf, BUFSIZE);
			}

			// 关闭文件
			fclose(src_fp);
			printf("File:\t%s Transfer Finished\n", argv[i]);

			close(cfd);
		}
	}

	close(lfd);

	return EXIT_SUCCESS;
}



