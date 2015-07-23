/*
 * virtualdirectory.c
 *
 * 接口伪代码快速书写范例
 *
 *  Created on: 2015年5月8日
 *      Author: yuan
 */

//头文件区域
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//主函数
int main(){

	/*
	 * 功能描述：
	 * 等待命令输入，得到命令后进行响应。
	 *
	 */

	//声明命令
	char command[100];

	//为了防止未知命令出现，提示命令输出及范例
	printf("ls-----------------1\n");
	printf("cd-----------------2\n");


	//如果你能决定用什么结构就直接写，认为需要函数就直接用，描述清功能，后期在实现，先把流程写通
	while(1){
		gets(command);   //获取指令，以回车结束

		//命令解析：主要对command进行格式化处理，指令格式有限，所以可以一一对应
		format(command);  //命令处理，可以先不实现

		//执行响应的动作，key为命令解析后结果，决定调用哪个函数
		switch (key) {
			case value0:
				//新开一个线程执行相应操作
				execute_ls("参数");    //执行ls操作
				break;
			case value1:
				//新开一个线程执行相应操作
				execute_cd("参数");    //执行cd操作
				break;
				//等等
			default:
				break;
		}//指令执行结束，重新循环

	}


	//如果不能决定用什么结构，就用自然语言描述流程，尽量清楚。例如：
	/*
	 * 1、等待用户输入命令
	 * 2、处理命令
	 * 3、执行相应的动作
	 */
	//然后咱们再商量

}



/*
 * 函数伪代码说明：
 * 1、明确函数功能
 * 2、确定参数
 * 3、确定返回值
 * 4、注意变量的作用域（局部变量还是全局变量）
 */

//例如
/*
 * format()：命令预处理
 * 参数：char* command
 * 返回值：基本上需要两个：命令本身及所带参数（ls -l 或者 cd 目录名）或者定义全局变量在函数中赋值
 *
 */
void format(char* command){
	//如果不能实现，继续描述，例如
	/*
	 * 将command以空格分割，分成命令和参数两部分
	 * 分别赋值给全局变量
	 */
}//定义好之后反向修改主函数调用


/*
 * execute_ls(char* parameter):执行ls
 * 参数：char* parameter
 * 返回值：无
 * 功能描述：在终端显示当前目录结构
 */
void execute_ls(char* parameter){
	char* currentDirectory;
	currentDirectory = getcurrent();   //获取当前目录位置，需要函数就先定义

	//显示当前目录信息，调用数据库接口
	query();                           //查询数据库，这些我都会写
	print_result();                    //打印结果

	//或者直接描述，我来实现
	/*
	 * 1、活取当前目录信息
	 * 2、查询
	 * 3、打印结果
	 */
}
