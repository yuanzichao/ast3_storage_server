//头文件区域
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sql/sql.h"
#include "history/history.h"
#include "request/request.h"


//主函数
int main()
{

	init_mysql();		//初始化数据库


	//声明命令
	char input[MAX_BUF_SIZE];
	char curr_disk[MAX_BUF_SIZE];
	char curr_dir[MAX_BUF_SIZE];
	char curr_file[MAX_BUF_SIZE];





	//初始化磁盘和目录
    memset(curr_disk, 0, sizeof(curr_disk));
    memcpy(curr_disk, "home", sizeof(curr_disk));
    memset(curr_dir, 0, sizeof(curr_dir));
    memcpy(curr_dir, "unknown", sizeof(curr_dir));
    memset(curr_file, 0, sizeof(curr_file));
//    memcpy(curr_file, "", sizeof(curr_file));

    //输出当前根目录
    printf("%s/", curr_disk);

}













