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

#include "../sql/sql.h"
#include "../utils/read_conf.h"



int dir_monitor();			//监控指定目录
int dir_all_monitor(char *path, int fd);
