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
static void ast3data_set(ast3_data_t *ast3data, uint32_t protocl, uint32_t flag, uint32_t code, uint32_t len);
static int ast3_send(int sockfd, uint32_t protocol, uint32_t flag, uint32_t code, const void *data, uint32_t len);
static int ast3_recv(int sockfd, uint32_t *protocol, uint32_t *flag, uint32_t *code, uint32_t *len, void *data, size_t length);
List list_init(void);
int is_list_empty(List l);
int is_list_last(Position p);
Position find_list(List l, void *data);
Position find_list_previous(List l, void *data);
void delete_list_item(List l, void *data);
Position find_list_item_accurate(List l, void *data, int (*compar)(const void *, const void *));
Position find_list_previous_accurate(List l, void *data, int (*compar)(const void *, const void *));
void delete_list_accurate(List l, void *data, int (*compar)(const void *, const void *));
int insert_list(Position p, void *data);
int list_append(List l, void *data);
void delete_list(List l);
void list_clean_and_destroy(List l, void (*free_data)(void *));
void list_destroy(List l);
int tcp_listen(const char *hostname, const char *servname, SA *sockaddr, socklen_t *addrlen);
int ast3array_recv_req(int sockfd, char *data, size_t size);
int ast3array_send_ack(int sockfd, uint32_t code, const char *data);


int send_files();				//和客户端通信，发送相关文件


