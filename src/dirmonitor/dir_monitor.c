/*
 * monitor.c
 *
 *  Created on: 2015年7月29日
 *      Author: yuan
 */


#include "dir_monitor.h"

int
dir_monitor(char* path) {

	int fd;
	int len;
	int nread;

	int i;

	char buf[BUFSIZ];

	struct inotify_event *event;



	fd = inotify_init();

	if( fd < 0 ) {
		fprintf(stderr, "inotify_init failed\n");
		return EXIT_FAILURE;
	}


	dir_all_monitor(path, fd);


	buf[sizeof(buf) - 1] = 0;

	while( (len = read(fd, buf, sizeof(buf) - 1)) > 0 ) {
		nread = 0;
		while( len > 0 ) {
			event = (struct inotify_event *)&buf[nread];
			for(i=0; i<EVENT_NUM; i++) {
				if((event->mask >> i) & 1) {
					if(event->len > 0)
						fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
					else
						fprintf(stdout, "%s --- %s\n", " ", event_str[i]);
				}
			}

			nread = nread + sizeof(struct inotify_event) + event->len;
			len = len - sizeof(struct inotify_event) - event->len;
		}
	}

	return EXIT_SUCCESS;
}


int
dir_all_monitor(char *path, int fd) {

	char ab_path[MAX_BUF_SIZE];			//绝对路径
	memset(ab_path, 0, sizeof(ab_path));
	sprintf(ab_path, "%s", path);		//存储绝对路径

	int wd;

	DIR *dp;                       // 定义子目录流指针
	struct dirent *entry;         // 定义dirent结构指针保存后续目录
	struct stat statbuf;          // 定义statbuf结构保存文件属性
	if((dp = opendir(ab_path)) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功
	{
		puts("can't open path.");
		return EXIT_FAILURE;
	}


	wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);

	if(wd < 0) {
		fprintf(stderr, "inotify_add_watch %s failed\n", path);
		return EXIT_FAILURE;
	}


	chdir (ab_path);                     // 切换到当前目录
	while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环
	{
		lstat(entry->d_name, &statbuf); // 获取下一级成员属性
		if(S_IFDIR &statbuf.st_mode)    // 判断下一级成员是否是目录
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			{
				continue;
			}

			strcat(ab_path, "/");
			strcat(ab_path, entry->d_name);

			wd = inotify_add_watch(fd, ab_path, IN_ALL_EVENTS);

			if(wd < 0) {
				fprintf(stderr, "inotify_add_watch %s failed\n", ab_path);
				return EXIT_FAILURE;
			}

			//组装目录信息


			printf("%s/\n", ab_path);  // 输出目录名称
			dir_all_monitor(ab_path, fd);              // 递归调用自身，扫描下一级目录的内容
		}
	}
	chdir("..");                                                  // 回到上级目录
	closedir(dp);                                                 // 关闭子目录流


	return EXIT_SUCCESS;
}

int
main() {
	dir_monitor("/home/yuan/test/");

	return 0;
}

