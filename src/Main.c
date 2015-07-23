//头文件区域
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vdir/virtualdirectory.h"
#include "sql/sql.h"
#include "welcome/welcome.h"
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


	//欢迎界面
	welcome();

	//功能列表
	main_menu();


	//初始化磁盘和目录
    memset(curr_disk, 0, sizeof(curr_disk));
    memcpy(curr_disk, "home", sizeof(curr_disk));
    memset(curr_dir, 0, sizeof(curr_dir));
    memcpy(curr_dir, "unknown", sizeof(curr_dir));
    memset(curr_file, 0, sizeof(curr_file));
//    memcpy(curr_file, "", sizeof(curr_file));

    //输出当前根目录
    printf("%s/", curr_disk);


    //执行目录功能操作
	while(1){

        memset(input, 0,sizeof(input));

		fgets(input, MAX_BUF_SIZE, stdin);   //获取用户输入指令，以回车结束

		insert_history(strtok(input,"\n"));				//操作日志记录

		key = format(&input);//命令解析：主要对input进行格式化处理

		//执行响应的动作，key为命令解析后结果，决定调用哪个函数
		switch (key) {

			case 0://浏览目录结构
			{
				if(strcmp(curr_disk,"home")==0)
					excute_show();    //执行show操作，列出磁盘信息
				else if(strcmp(curr_dir,"unknown")==0)
					excute_list(curr_disk);//执行list操作，列出目录信息
				else
					excute_ls(curr_dir,curr_disk);//执行ls操作，列出文件信息
				break;
			}
			case 1://浏览目录详细信息
			{
				if(strcmp(curr_disk,"home")==0)
					excute_show_all();
				else if(strcmp(curr_dir,"unknown")==0)
					excute_list_all(curr_disk);
				else
					excute_ls_all(curr_dir,curr_disk);
				break;
			}
			case 2://返回根目录
			{
				memset(curr_disk, 0, sizeof(curr_disk));
				memcpy(curr_disk, "home", sizeof(curr_disk));
			    memset(curr_dir, 0, sizeof(curr_dir));
			    memcpy(curr_dir, "unknown", sizeof(curr_dir));
				break;
			}
			case 3://返回父目录
			{
				/*excute_return_parent_directory(curr_disk,curr_dir);
				memset(curr_dir, 0, sizeof(char)*100);
				memcpy(curr_dir, currentDirectory, strlen(currentDirectory));*/
				memset(curr_disk, 0, sizeof(curr_disk));
				memcpy(curr_disk, "home", sizeof(curr_disk));
				memset(curr_dir, 0, sizeof(curr_dir));
				memcpy(curr_dir, "unknown", sizeof(curr_dir));
				break;
			}
			case 4://打开指定硬盘
			{
				memset(curr_dir, 0, sizeof(curr_disk));
				memcpy(curr_dir, currentDirectory, sizeof(curr_disk));
				break;
			}
			case 5://切换到指定目录
			{
				memset(curr_disk, 0, sizeof(curr_disk));
				memcpy(curr_disk, currentDisk, sizeof(currentDisk));
				break;
			}
			case 6:
			{
				excute_ls_file_all(fileName,curr_dir,curr_disk);
				break;
			}
			case 7:
			{
				if(strcmp(curr_dir,"unknown")==0)
					printf("%s\n",curr_disk);
				else
				    printf("%s\n",curr_dir);
				break;
			}
			case 8:
			{
				query_time_from_curr(curr_dir,curr_disk,startTime, endTime);
				break;
			}
			case 9:
			{
				query_location(start_ra, end_ra, start_dec, end_dec);
				break;
			}
			case 10:
			{
				if(strcmp(curr_disk,"home")==0)
					excute_show_primary();
				else if(strcmp(curr_dir,"unknown")==0)
					excute_list_primary(curr_disk);
				else
					excute_ls_primary(curr_dir,curr_disk);
				break;
			}
			case 11:
			{
				excute_ls_file_primary(fileName,curr_dir,curr_disk);
				break;
			}
			case 12:
			{
				excute_ls_file_wildcard(fileName,curr_dir,curr_disk);
				break;
			}
			case 13:
			{
				query_time_from_home(startTime, endTime);
				break;
			}
			case 14:
			{
				query_location_curr(curr_dir, curr_disk, start_ra, end_ra, start_dec, end_dec);
				break;
			}
			case 15:
			{
				excute_copy_file();
				break;
			}
			case 16:
			{
				//查询当天所有操作记录
				if(get_cur_history() == EXIT_FAILURE) {
					excute_error();
				}
				break;
			}
			case 17:
			{
				//查询当天所有操作记录
				if(get_all_history() == EXIT_FAILURE) {
					excute_error();
				}
				break;
			}
			case 18:
			{
				//查询当天所有操作记录
				if(get_spe_history(spe_date) == EXIT_FAILURE) {
					excute_error();
				}
				break;
			}
			case 19:
			{
				memset(curr_file, 0, sizeof(curr_file));
				memcpy(curr_file, currentFile, sizeof(curr_file));

				//生成请求信息
				if(gen_request(curr_file, curr_dir, curr_disk) == EXIT_FAILURE) {
					excute_error();
				}

				break;
			}


			case 100:
			{
				excute_help();						//帮助菜单
				break;
			}

			default :
			{
				excute_error();
				break;
			}

		}//指令执行结束，重新循环


		//打印当前目录
		printf("%s/",curr_disk);

		if(strcmp(curr_dir,"unknown")!=0)
		{
			printf("%s",curr_dir);
			printf("%s","/");
		}
	}

}













