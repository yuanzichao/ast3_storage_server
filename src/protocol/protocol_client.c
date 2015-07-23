#include "protocol_client.h"

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

int
tcp_connect(const char *hostname, const char *servname, SA *sockaddr, socklen_t *addrlen)
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd;
    int ret;
    
    memset(&hint, '\0', sizeof(struct addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    
    if ((ret = getaddrinfo(hostname, servname, &hint, &ailist)) != 0) {
        fprintf(stderr, "%s", gai_strerror(ret));
        return -1;
    }
    
    aip = ailist;
    do {
        sockfd = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
        if (sockfd < 0)
            continue;
        if (connect(sockfd, aip->ai_addr, aip->ai_addrlen) == 0) {
            if (addrlen != NULL)
                *addrlen = aip->ai_addrlen;
            if (sockaddr != NULL)
                memcpy(sockaddr, aip->ai_addr, aip->ai_addrlen);
            break;
        }
        close(sockfd);
    } while ((aip = aip->ai_next) != NULL);
    
    if (aip == NULL) {
        sockfd = -1;
    }
    
    freeaddrinfo(ailist);
    
    return sockfd;
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
    }
    else if (n == 0)
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

int
ast3array_send_req(int sockfd, const char *data)
{
    uint32_t protocol = ARRAY_PROTOCOL, flag = 0, code = AST3_OK, len = (uint32_t) strlen(data);
    return ast3_send(sockfd, protocol, flag, code, data, len);
}

int
ast3array_recv_ack(int sockfd, uint32_t *code, char *data, size_t size)
{
    uint32_t protocol, flag = 0, len;
    return ast3_recv(sockfd, &protocol, &flag, code, &len, data, (uint32_t) size);
}


/**
 * 和服务器通信，获取相关文件
 *
 * 参数：请求消息（JSON）
 *
 */

int
get_files(char* request) {

	int sockfd, ret;
    uint32_t code;

	sockfd = tcp_connect("localhost", PORT, NULL, NULL);

	ret = ast3array_send_req(sockfd, request);


	while(1) {

		char buf[BUFSIZE];
		memset(buf, 0, BUFSIZE);

		ast3array_recv_ack(sockfd, &code, buf, BUFSIZE);

		char file_name[BUFSIZE];
		memset(file_name, 0, BUFSIZE);
		sprintf(file_name, "/home/yuan/test/%s", buf);


		FILE *dest_fp;
		if ((dest_fp = fopen(file_name, "w")) == NULL){
			printf("fopen %s failed.\n", file_name);
			return -1;
		}


		 char dest_buf[BUFSIZE];
		 memset(dest_buf, 0, BUFSIZE);


		 // 从服务器接收数据到buffer中
		 // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
		 int length = 0;

		 while((length = recv(sockfd, dest_buf, BUFSIZE, 0)) > 0) {
			 if(fwrite(dest_buf, sizeof(char), length, dest_fp) < length) {
				 printf("File:\t%s Write Failed\n", file_name);
				 break;
			 }

			 bzero(dest_buf, BUFSIZE);
		 }

		printf("Recieve File:\t %s From Server Finished\n", file_name);

		fclose(dest_fp);

	}

	return EXIT_SUCCESS;
}
