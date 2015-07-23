/*
 * protocol_client.h
 *
 *  Created on: 2015年7月21日
 *      Author: yuan
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>

#include <sys/errno.h>
#include <sys/socket.h>

#define BACKLOG 10
#define PORT "5000"
#define ARRAY_PROTOCOL 1
#define BUFSIZE 4096

#define MIN(a,b) (((a)<(b)?(a):(b)))
#define MAX(a,b) (((a)>(b)?(a):(b)))

typedef struct sockaddr SA;

struct Node {
    void *data;
    struct Node *next;
};

typedef struct Node *List;
typedef struct Node *Position;


typedef struct {
    uint32_t protocol;
    uint32_t flag;
    uint32_t code;
    uint32_t len;
    char data[];
} ast3_data_t;


enum {AST3_OK, AST3_EACCESS, AST3_EAGAIN, AST3_EEXIST, AST3_ENETDOWN, AST3_ENETRESET, AST3_ENETUNREACH, AST3_ENOENT, AST3_ENOMEM, AST3_EPERM, AST3_EPIPE, AST3_ESRCH, AST3_ETIMEDOUT, AST3_EABORT, AST3_EALREADY, AST3_ECLOSED, AST3_EHARDWARE, AST3_ENEQUIP, AST3_ENREGF, AST3_ENREGST, AST3_EPROTOCOL, AST3_ESENDFILE, AST3_ERROR = 255};


ssize_t Readn(int fd, void *vptr, size_t n);
ssize_t Writen(int fd, const void *vptr, size_t n);
int tcp_connect(const char *hostname, const char *servname, SA *sockaddr, socklen_t *addrlen);
static void ast3data_set(ast3_data_t *ast3data, uint32_t protocl, uint32_t flag, uint32_t code, uint32_t len);
static int ast3_send(int sockfd, uint32_t protocol, uint32_t flag, uint32_t code, const void *data, uint32_t len);
static int ast3_recv(int sockfd, uint32_t *protocol, uint32_t *flag, uint32_t *code, uint32_t *len, void *data, size_t length);
int ast3array_send_req(int sockfd, const char *data);
int ast3array_recv_ack(int sockfd, uint32_t *code, char *data, size_t size);


int get_files(char* request);				//和服务器通信获取相关文件



