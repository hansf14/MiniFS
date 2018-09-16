/*
*磁盘空间采用连续分配的方式，以1kB作为盘快为分配单位，采用位示图记录盘块分配情况（位于盘块0）
*系统共有100k个盘块，前100个盘块用于存放位图表（char[]）
*文件目录采用树状目录，系统创始化一张根目录表（用户初始位置）；占用一个盘块
*目录项：文件名，FCB盘块号，文件类型
*文件控制块FCB：文件数据起始盘块号，文件大小，链接数,读指针，数据大小；占用一个盘块
*文件数据和FCB分开存放
*/

#include "minifs_rm.h"
#include "minifs_mmm.h"
#include <iostream>
#include <string>

int runMiniFS_RM();
int runMiniFS_MMM();

int main()
{
	//runMiniFS_MMM();
	runMiniFS_RM();
	

	return 0;
}

int runMiniFS_RM()
{
	MiniFS_RM miniFS_RM;

	miniFS_RM.help();
	char judge[20];
	char SName[20];
	while (1)
	{
		wprintf(L"Mini-FS>");
		scanf("%s", judge);
		if (strcmp(judge, "create") == 0)
		{
			scanf("%s", SName);
			wprintf(L"succeed!\n");
			break;
		}
		else if (strcmp(judge, "help") == 0)
			miniFS_RM.help();
		else
			wprintf(L"\tYou have not created and named your operating space!!\n\t\tPlease try again and create the space first.\n");
	}
	//初始化系统
	//miniFS.initSystem();
	//创建根目录
	miniFS_RM.initRootDir();


	char comm[10], name[20];
	int choice = 0;
	bool bClose = false;
	while (1)
	{
		wprintf(L"%s:\\>", SName);
		scanf("%s", comm);
		choice = -1;
		int i;
		for (i = 0; i <= 13; ++i)
		{
			if (strcmp(comm, RM::command[i]) == 0)
			{
				choice = i;
				break;
			}
		}
		switch (choice)
		{
		case (int)MiniFS_RM::COMMAND::fmt: //格式化文件系统
			miniFS_RM.initSystem();
			miniFS_RM.initRootDir();
			wprintf(L"succeed!\n");
			break;

		case (int)MiniFS_RM::COMMAND::close: //退出文件系统
			bClose = true;
			break;

		case (int)MiniFS_RM::COMMAND::dr: //显示文件目录
			miniFS_RM.showDir();
			break;

		case (int)MiniFS_RM::COMMAND::cp: //复制文件
			char name2[59];
			scanf("%s%s", name, name2);
			miniFS_RM.copy(name, name2);
			break;

		case (int)MiniFS_RM::COMMAND::dl:
			scanf("%s", name);
			miniFS_RM.deleteFile(name);
			break;

		case (int)MiniFS_RM::COMMAND::map: //显示文本占用的全部空间块号
			miniFS_RM.showMap();
			break;

		case (int)MiniFS_RM::COMMAND::opt: //优化整个空间
			wprintf(L"敬请期待！！！\n");
			break;

		case (int)MiniFS_RM::COMMAND::att: //显示文本属性
			miniFS_RM.showAtt();
			break;

		case (int)MiniFS_RM::COMMAND::help: //帮助信息
			miniFS_RM.help();
			break;

		case (int)MiniFS_RM::COMMAND::touch: //创建文件
			scanf("%s", name);
			int fileSize;
			scanf("%d", &fileSize);
			miniFS_RM.createFile(name, fileSize);
			break;

		case (int)MiniFS_RM::COMMAND::mk:
			scanf("%s", name);
			miniFS_RM.createDir(name);
			break;

		case (int)MiniFS_RM::COMMAND::rm:
			scanf("%s", name);
			miniFS_RM.deleteDir(name);
			break;

		case (int)MiniFS_RM::COMMAND::cm:
			scanf("%s", name);
			miniFS_RM.changeDir(name);
			break;

		default:
			wprintf(L"%s command not defined\n", comm);
		}
		if (bClose)
			break;
	}

	return 0;
}

int runMiniFS_MMM()
{
	int ret = 0;

	MiniFS_MMM miniFS_MMM;
	ret = miniFS_MMM.create();

	return ret;
}