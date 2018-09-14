/*
*磁盘空间采用连续分配的方式，以1kB作为盘快为分配单位，采用位示图记录盘块分配情况（位于盘块0）
*系统共有100k个盘块，前100个盘块用于存放位图表（char[]）
*文件目录采用树状目录，系统创始化一张根目录表（用户初始位置）；占用一个盘块
*目录项：文件名，FCB盘块号，文件类型
*文件控制块FCB：文件数据起始盘块号，文件大小，链接数,读指针，数据大小；占用一个盘块
*文件数据和FCB分开存放
*/
#pragma warning(disable:4996)

#include "FileOperate.h"
#include <iostream>
#include <string>

//指令数目
char const command[13][7] = { "fmt","close","dr","cp","dl","map","opt","att","help","touch","mk","rm","cd"};

int runMiniFS()
{
	MiniFS miniFS;

	miniFS.help();
	char judge[20];
	char SName[20];
	while (1)
	{
		printf("Mini-FS>");
		scanf("%s", judge);
		if (strcmp(judge, "create") == 0)
		{
			scanf("%s", SName);
			printf("succeed!\n");
			break;
		}
		else if (strcmp(judge, "help") == 0)
			miniFS.help();
		else
			printf("\tYou have not created and named your operating space!!\n\t\tPlease try again and create the space first.\n");
	}
	//初始化系统
	miniFS.initSystem();
	//创建根目录
	miniFS.initRootDir();


	char comm[10], name[20];
	int choice = 0;
	bool bClose = false;

	while (1)
	{
		printf("%s:\\>", SName);
		scanf("%s", comm);
		choice = -1;
		int i;
		for (i = 0; i <= 13; ++i)
		{
			if (strcmp(comm, command[i]) == 0)
			{
				choice = i;
				break;
			}
		}
		switch (choice)
		{
		case (int)MiniFS::COMMAND::fmt: //格式化文件系统
			miniFS.initSystem();
			miniFS.initRootDir();
			printf("succeed!\n");
			break;

		case (int)MiniFS::COMMAND::close: //退出文件系统
			bClose = true;
			break;

		case (int)MiniFS::COMMAND::dr: //显示文件目录
			miniFS.showDir();
			break;

		case (int)MiniFS::COMMAND::cp: //复制文件
			char name2[59];
			scanf("%s%s", name, name2);
			miniFS.copy(name, name2);
			break;

		case (int)MiniFS::COMMAND::dl:
			scanf("%s", name);
			miniFS.deleteFile(name);
			break;

		case (int)MiniFS::COMMAND::map: //显示文本占用的全部空间块号
			miniFS.showMap();
			break;

		case (int)MiniFS::COMMAND::opt: //优化整个空间
			printf("敬请期待！！！\n");
			break;

		case (int)MiniFS::COMMAND::att: //显示文本属性
			miniFS.showAtt();
			break;

		case (int)MiniFS::COMMAND::help: //帮助信息
			miniFS.help();
			break;

		case (int)MiniFS::COMMAND::touch: //创建文件
			scanf("%s", name);
			int fileSize;
			scanf("%d", &fileSize);
			miniFS.createFile(name, fileSize);
			break;

		case (int)MiniFS::COMMAND::mk:
			scanf("%s", name);
			miniFS.createDir(name);
			break;

		case (int)MiniFS::COMMAND::rm:
			scanf("%s", name);
			miniFS.deleteDir(name);
			break;

		case (int)MiniFS::COMMAND::cm:
			scanf("%s", name);
			miniFS.changeDir(name);
			break;

		default:
			printf("%s command not defined\n", comm);
		}
		if (bClose)
			break;
	}
	miniFS.exitSystem();

	return 0;
}

int test()
{
	int ret = 0;

	MiniFS miniFS;
	ret = miniFS.create();

	return ret;
}

int main()
{
	//test();
	runMiniFS();
	

	return 0;
}

