/*
 * virtualdirectory.c
 *
 *  Created on: 2015年5月28日
 *      Author: jermaine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sql/sql.h"
#include "../welcome/welcome.h"
#include "virtualdirectory.h"

/*
 * format()：命令预处理
 * 参数：char* command
 * 返回值：基本上需要两个：命令本身及所带参数（ls -l 或者 cd 目录名）或者定义全局变量在函数中赋值
 *
 */
int format(char* input){

	char* command;
	char* parameter1;
	char* parameter2;
	char* parameter3;
	char* parameter4;

	command=strtok(input," ");
	parameter1=strtok(NULL," ");
	parameter2=strtok(NULL," ");
	parameter3=strtok(NULL," ");
	parameter4=strtok(NULL," ");


	if((strcmp(command,"ls")==0)&&(parameter1==NULL)) {
		key=0;
	} else if((strcmp(command,"ls")==0)&&(strcmp(parameter1,"-l")==0)&&(parameter2==NULL)) {
		key=10;
	} else if((strcmp(command,"ls")==0)&&(strcmp(parameter1,"-l")==0)&&(parameter2!=NULL)&&(strcmp(parameter2,"*.fits")!=0)&&(parameter3==NULL)) {
		key=11;
		fileName=parameter2;
	} else if((strcmp(command,"ls")==0)&&(strcmp(parameter1,"-***")==0)&&(parameter2==NULL)) {
		key=1;
	} else if((strcmp(command,"cd")==0)&&(strcmp(parameter1,"/")==0)&&(parameter2==NULL)) {
		key=2;
		currentDisk=parameter1;
	} else if((strcmp(command,"cd")==0)&&(strcmp(parameter1,"..")==0)&&(parameter2==NULL)) {
		key=3;
		//currentDirectory=parameter1;
	} else if((strcmp(command,"cd")==0)&&(parameter1!=NULL)&&(strcmp(parameter1,"..")!=0)&&(strcmp(parameter1,"/")!=0)&&(parameter2==NULL)) {
		key=4;
		currentDirectory=parameter1;
	} else if((strcmp(command,"open")==0)&&(parameter1!=NULL)&&(parameter2==NULL)) {
		key=5;
		currentDisk=parameter1;
	} else if((strcmp(command,"ls")==0)&&(strcmp(parameter1,"-***")==0)&&(parameter2!=NULL)&&(parameter3==NULL)) {
		key=6;
		fileName = parameter2;
	} else if((strcmp(command,"pwd")==0)&&(parameter1==NULL)) {
		key=7;
	} else if((strcmp(command,"find")==0)&&(strcmp(parameter1,"time")==0)&&(parameter2!=NULL)&&(parameter3!=NULL)&&(parameter4==NULL)) {
		key=8;
		startTime = parameter2;
		endTime = parameter3;
	} else if((strcmp(command,"find")==0)&&(strcmp(parameter1,"/time")==0)&&(parameter2!=NULL)&&(parameter3!=NULL)&&(parameter4==NULL)) {
		key=13;
		startTime = parameter2;
		endTime = parameter3;
	} else if(strcmp(command,"/location")==0) {
		key=9;
		start_ra = parameter1;
		end_ra = parameter2;
		start_dec = parameter3;
		end_dec  = parameter4;
	} else if(strcmp(command,"location")==0) {
		key=14;
		start_ra = parameter1;
		end_ra = parameter2;
		start_dec = parameter3;
		end_dec  = parameter4;
	} else if((strcmp(command,"ls")==0)&&(strcmp(parameter1,"-l")==0)&&(strcmp(parameter2,"*.fits")==0)&&(parameter3==NULL)) {
		key=12;
		fileName = parameter2;
	} else if((strcmp(command,"cp")==0)&&(parameter1!=NULL)) {
		key=15;
	}else if((strcmp(command, "history")==0) && (parameter1==NULL)) {			//查询当天操作记录
		key = 16;
	}else if((strcmp(command, "history")==0) && (strcmp(parameter1,"-a")==0)&& (parameter2==NULL)) {		//查询所有操作记录
		key = 17;
	}else if((strcmp(command, "history")==0) && (parameter1!=NULL) && (parameter2==NULL)) {		//查询指定日期操作记录
		key = 18;
		spe_date = parameter1;
	}else if((strcmp(command, "get")==0) && (parameter1!=NULL) && (parameter2==NULL)) {		//查询指定日期操作记录
		key = 19;
		currentFile = parameter1;
	}else if((strcmp(command, "help")==0)) {		//帮助信息
		key = 100;
	}else {						//错误信息处理
		key = -1;
	}
	return key;

}//定义好之后反向修改主函数调用



/*
 * execute_show():执行show操作
 * 参数：无
 * 返回值：无
 * 功能描述：列出所有磁盘信息
 */
void excute_show(){

	query_disks();     //查询所有磁盘信息
	print_result();    //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There is no disk information");
	}*/
	free_result();     //释放结果集

}


/*
 * execute_show_primary():执行show操作
 * 参数：无
 * 返回值：无
 * 功能描述：列出所有磁盘详细信息
 */
void excute_show_primary(){

	query_primary_disks();     //查询所有磁盘信息
	print_result();    //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There is no disk information");
	}*/
	free_result();     //释放结果集

}


/*
 * execute_show_all():执行show操作
 * 参数：无
 * 返回值：无
 * 功能描述：列出所有磁盘详细信息
 */
void excute_show_all(){

	query_all_disks();     //查询所有磁盘信息
	print_result();    //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There is no disk information");
	}*/
	free_result();     //释放结果集

}


/*
 * execute_use(char* diskName):执行use操作
 * 参数：char* diskName
 * 返回值：无
 * 功能描述：进入指定磁盘"

void excute_use(char* diskName){

	currentDisk=diskName;

} */


/*
 * execute_list(char* diskName):执行list操作
 * 参数：char* diskName
 * 返回值：无
 * 功能描述：列出指定磁盘下的目录信息
 */
void excute_list(char* diskName){

	query_disks_info(diskName);  //查询目录信息
	print_result();              //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=11)
	{
		printf("%s\n","There are no directories in the disk");
	}*/
	free_result();               //释放结果集

}


/*
 * execute_list_primary(char* diskName):执行list操作
 * 参数：char* diskName
 * 返回值：无
 * 功能描述：列出指定磁盘下的目录基本信息
 */
void excute_list_primary(char* diskName){

	query_disks_primary_info(diskName);  //查询目录信息
	print_result();              //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=11)
	{
		printf("%s\n","There are no directories in the disk");
	}*/
	free_result();               //释放结果集

}


/*
 * execute_list_all(char* diskName):执行list操作
 * 参数：char* diskName
 * 返回值：无
 * 功能描述：列出指定磁盘下的目录信息
 */
void excute_list_all(char* diskName){

	query_disks_all_info(diskName);  //查询目录信息
	print_result();              //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=11)
	{
		printf("%s\n","There are no directories in the disk");
	}*/
	free_result();               //释放结果集

}


/*
 * execute_ls(char* parameter):执行ls
 * 参数：char* parameter
 * 返回值：无
 * 功能描述：在终端显示当前目录结构
 */
void excute_ls(char* dirName, char* diskName){

	query_directory_info(dirName, diskName);//查询当前目录内文件和目录信息
	print_result();       //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There are no files in the directory");
	}*/
	free_result();        //释放结果集

}


/*
 * execute_ls_primary(char* parameter):执行ls
 * 参数：char* parameter
 * 返回值：无
 * 功能描述：在终端显示当前目录基本信息
 */
void excute_ls_primary(char* dirName, char* diskName){

	query_directory_primary_info(dirName, diskName);//查询当前目录内文件和目录详细信息
	print_result();       //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There are no files in the directory");
	}*/
	free_result();        //释放结果集

}


/*
 * execute_ls_all(char* dirName, char* diskName):执行ls
 * 参数：char* parameter
 * 返回值：无
 * 功能描述：在终端显示当前目录详细信息
 */
void excute_ls_all(char* dirName, char* diskName){

	query_directory_all_info(dirName, diskName);//查询当前目录内文件和目录详细信息
	print_result();       //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","There are no files in the directory");
	}*/
	free_result();        //释放结果集

}


/*
 * execute_ls_file_wildcard(char* fileName,char* dirName, char* diskName):执行ls
 * 参数：char* parameter
 * 返回值：无
 * 功能描述：在终端显示当前目录详细信息
 */
void excute_ls_file_wildcard(char* fileName,char* dirName, char* diskName){

	query_directory_wildcard_info(fileName,dirName,diskName);

}
/*
 * execute_pwd():执行pwd操作
 * 参数：无
 * 返回值：无
 * 功能描述：显示当前工作目录

void excute_pwd(){

	printf(currentDisk, " ",currentDirectory,"\n");

}*/


/*
 * execute_time(char *start_time, char *end_time):执行query操作
 * 参数：无
 * 返回值：无
 * 功能描述：显示当前工作目录
 */
void query_time_from_home(char *start_time,  char *end_time){

	query_file_by_time(start_time, end_time);//根据时间查询文件信息
	print_result();		  //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","No file was retrieved");
	}*/
	free_result();        //释放结果集

}


/*
 * execute_time(char *start_time, char *end_time):执行query操作
 * 参数：无
 * 返回值：无
 * 功能描述：显示当前工作目录
 */
void query_time_from_curr(char* dirName,char* diskName,char *start_time,  char *end_time){

	query_file_by_time_curr(dirName,diskName,start_time, end_time);//根据时间查询文件信息
	print_result();		  //打印结果
	//printf("%d",print_result());
	/*if((print_result())!=13)
	{
		printf("%s\n","No file was retrieved");
	}*/
	free_result();        //释放结果集

}



/**
 * query_location(char *start_ra, char *end_ra, char *start_dec, char *end_dec)
 * 参数：起止位置坐标
 * 返回值：无
 * 功能描述：根据位置查询文件信息
 */
void
query_location(char *start_ra, char *end_ra, char *start_dec, char *end_dec){

	query_file_by_location(start_ra, end_ra, start_dec, end_dec);	//根据位置查询文件信息
	print_result();		  //打印结果
	free_result();        //释放结果集
}


/**
 * query_location(char *start_ra, char *end_ra, char *start_dec, char *end_dec)
 * 参数：起止位置坐标
 * 返回值：无
 * 功能描述：根据位置查询文件信息
 */
void
query_location_curr(char* dirName,char* diskName,char *start_ra, char *end_ra, char *start_dec, char *end_dec){

	query_file_by_curr_location(dirName,diskName,start_ra, end_ra, start_dec, end_dec);	//根据位置查询文件信息
	print_result();		  //打印结果
	free_result();        //释放结果集
}



void excute_return_parent_directory(char* dirName,char* diskName){

	char* dirID;
	memset(dirID, 0, sizeof(char)*100);
	memcpy(dirID,get_parent_id(dirName,diskName),strlen(get_parent_id(dirName,diskName)));
	memset(currentDirectory, 0, sizeof(char)*100);
	memcpy(currentDirectory,return_directory(dirID,diskName),strlen(return_directory(dirID,diskName)));
	//char* dirID=get_parent_id(dirName,diskName);
	//currentDirectory=return_directory(dirID,diskName);
	print_result();
	free_result();

}



void excute_ls_file_primary(char* fileName,char* dirName,char* diskName){

	get_file_primary_info(fileName,dirName, diskName);
	print_result();
	free_result();
}


void excute_ls_file_all(char* fileName,char* dirName,char* diskName){

	get_file_all_info(fileName,dirName, diskName);
	print_result();
	free_result();
}

void excute_copy_file(){

	printf("The file has been copied to cache directory\n");
}

/*
 * execute_help()
 * 参数：无
 * 返回值：无
 * 功能描述：显示帮助信息
 */
void excute_help(){

	help_menu();

}

void excute_error(){

	printf("%s\n", "命令错误，请检查后重新输入或输入help寻求帮助");

}


