/*
 * dir_monitor.h
 *
 *  Created on: 2015年7月29日
 *      Author: yuan
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#include <fcntl.h>             // 提供open()函数
#include <unistd.h>
#include <dirent.h>            // 提供目录流操作函数
#include <sys/stat.h>        // 提供属性操作函数
#include <sys/types.h>         // 提供mode_t 类型

#define MAX_BUF_SIZE 1024 // 缓冲区最大字节数

#define EVENT_NUM 12

char *event_str[EVENT_NUM] = {
		"IN_ACCESS",
		"IN_MODIFY",
		"IN_ATTRIB",
		"IN_CLOSE_WRITE",
		"IN_CLOSE_NOWRITE",
		"IN_OPEN",
		"IN_MOVED_FROM",
		"IN_MOVED_TO",
		"IN_CREATE",
		"IN_DELETE",
		"IN_DELETE_SELF",
		"IN_MOVE_SELF"
};


int dir_monitor(char* path);			//监控指定目录
int dir_all_monitor(char *path, int fd);
