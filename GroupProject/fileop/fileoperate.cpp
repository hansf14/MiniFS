#include "FileOperate.h"

#include <stdio.h>
#include <string.h>

void MiniFS::initRootDir()
{
	//分配一个盘块空间给rootDirTable
	int startBlock = getBlock(1);
	if (startBlock == -1)
		return;
	rootDirTable = (dirTable*)getBlockAddr(startBlock);
	rootDirTable->dirUnitAmount = 0;
	//将自身作为父级目录
	//addDirUnit(rootDirTable, "..", 0, startBlock);

	currentDirTable = rootDirTable;
	//初始化初始绝对路径
	path[0] = '\\';
	path[1] = '\0';
}

char const* MiniFS::getPath() const
{
	return path;
}

//展示当前目录 ls
void MiniFS::showDir() const
{
	int unitAmount = currentDirTable->dirUnitAmount;
	printf("total:%d\n", unitAmount);
	printf("name\ttype\tsize\tFCB\tdataStartBlock\n");
	//遍历所有表项
	for (int i = 0; i < unitAmount; i++)
	{
		//获取目录项
		dirUnit unitTemp = currentDirTable->dirs[i];
		printf("%s\t%d\t", unitTemp.fileName, unitTemp.type);
		//该表项是文件，继续输出大小和起始盘块号
		if (unitTemp.type == 1)
		{
			int FCBBlock = unitTemp.startBlock;
			FCB* fileFCB = (FCB *)getBlockAddr(FCBBlock);
			printf("%d\t%d\t%d\n", fileFCB->fileSize, FCBBlock, fileFCB->blockNum);
		}
		else {
			int dirBlock = unitTemp.startBlock;
			dirTable* myTable = (dirTable *)getBlockAddr(dirBlock);
			printf("%d\t%d\n", myTable->dirUnitAmount, unitTemp.startBlock);
		}
	}
}

void MiniFS::showAtt() const
{
	int unitAmount = currentDirTable->dirUnitAmount;
	printf("name\ttype\tsize\tcreate date\ttime\n");
	//遍历所有表项
	for (int i = 0; i < unitAmount; i++)
	{
		//获取目录项
		dirUnit unitTemp = currentDirTable->dirs[i];
		printf("%s\t%d\t", unitTemp.fileName, unitTemp.type);
		//该表项是文件，继续输出大小和起始盘块号
		if (unitTemp.type == 1)
		{
			int FCBBlock = unitTemp.startBlock;
			FCB* fileFCB = (FCB*)getBlockAddr(FCBBlock);
			printf("%d\t", fileFCB->fileSize);
			printf("%4d/%02d/%02d\t%02d:%02d:%02d\n", fileFCB->time_create.wYear, fileFCB->time_create.wMonth, fileFCB->time_create.wDay, \
				fileFCB->time_create.wHour, fileFCB->time_create.wMinute, fileFCB->time_create.wSecond);
		}
		else {
			int dirBlock = unitTemp.startBlock;
			dirTable* myTable = (dirTable*)getBlockAddr(dirBlock);
			printf("%d\t", myTable->dirUnitAmount);
			printf("%4d/%02d/%02d\t%02d:%02d:%02d\n", myTable->time_create.wYear, myTable->time_create.wMonth, myTable->time_create.wDay, \
				myTable->time_create.wHour, myTable->time_create.wMinute, myTable->time_create.wSecond);
		}
	}
}

//******************������ɾ���ļ�
//�����ļ� touch
int MiniFS::createFile(char fileName[], int fileSize)
{
	//检测文件名字长度
	if (strlen(fileName) >= 59)
	{
		printf("file name too long\n");
		return -1;
	}
	//获得FCB的空间
	int FCBBlock = getBlock(1);
	if (FCBBlock == -1)
		return -1;
	//获取文件数据空间
	int FileBlock = getBlock(fileSize);
	if (FileBlock == -1)
		return -1;
	//创建FCB
	if (createFCB(FCBBlock, FileBlock, fileSize) == -1)
		return -1;
	//添加到目录项
	if (addDirUnit(currentDirTable, fileName, 1, FCBBlock) == -1)
		return -1;
	printf("succeed!\n");
	return 0;
}

int MiniFS::createDir(char dirName[])
{
	//为目录表分配空间
	if (strlen(dirName) >= 59)
	{
		printf("file name too long\n");
		return -1;
	}
	//将目录作为目录项添加到当前目录
	int dirBlock = getBlock(1);
	if (dirBlock == -1)
		return -1;
	//为新建的目录添加一个到父目录的目录项
	if (addDirUnit(currentDirTable, dirName, 0, dirBlock) == -1)
		return -1;
	//Ϊ�½���Ŀ¼���һ������Ŀ¼��Ŀ¼��
	dirTable* newTable = (dirTable*)getBlockAddr(dirBlock);
	newTable->dirUnitAmount = 0;
	char parent[] = "..";
	if (addDirUnit(newTable, parent, 0, getBlockIdx((BYTE*)currentDirTable)) == -1)
		return -1;
	printf("secceed!\n");
	return 0;
}

int MiniFS::createFCB(int fcbBlockNum, int fileBlockNum, int fileSize)
{
	//找到FCB的存储位置
	FCB* currentFCB = (FCB*)getBlockAddr(fcbBlockNum);
	currentFCB->blockNum = fileBlockNum; //文件数据起始位置
	currentFCB->fileSize = fileSize; //文件大小
	currentFCB->link = 1; //文件链接数
	currentFCB->dataSize = 0; //文件已写入数据长度
	currentFCB->readptr = 0; //文件读指针
	GetLocalTime(&(currentFCB->time_create)); //文件创建时间

	return 0;
}
int MiniFS::addDirUnit(dirTable* myDirTable, char fileName[], int type, int FCBBlockNum)
{
	//获得目录表
	int dirUnitAmount = myDirTable->dirUnitAmount;
	//检测目录表示是否已满
	if (dirUnitAmount == dirTable_max_size)
	{
		printf("dirTables is full, try to delete some file\n");
		return -1;
	}

	//是否存在同名文件
	if (findUnitInTable(myDirTable, fileName) != -1)
	{
		printf("file already exist\n");
		return -1;
	}
	//构建新目录项
	dirUnit* newDirUnit = &myDirTable->dirs[dirUnitAmount];
	myDirTable->dirUnitAmount++; //当前目录表的目录项数量+1
	//设置新目录项内容
	strcpy(newDirUnit->fileName, fileName);
	newDirUnit->type = type;
	newDirUnit->startBlock = FCBBlockNum;

	return 0;
}

int MiniFS::deleteFile(char fileName[])
{
	//忽略系统的自动创建的父目录
	if (strcmp(fileName, "..") == 0)
	{
		printf("can't delete ..\n");
		return -1;
	}
	//查找文件的目录项位置
	int unitIndex = findUnitInTable(currentDirTable, fileName);
	if (unitIndex == -1)
	{
		printf("file not found\n");
		return -1;
	}
	dirUnit myUnit = currentDirTable->dirs[unitIndex];
	//判断类型
	if (myUnit.type == 0) //目录
	{
		printf("not a file\n");
		return -1;
	}
	int FCBBlock = myUnit.startBlock;
	//释放内存
	releaseFile(FCBBlock);
	//从目录表中剔除
	deleteDirUnit(currentDirTable, unitIndex);
	printf("succeed!\n");
	return 0;
}

int MiniFS::deleteDirUnit(dirTable* myDirTable, int unitIndex)
{
	//迁移覆盖
	int dirUnitAmount = myDirTable->dirUnitAmount;
	for (int i = unitIndex; i < dirUnitAmount - 1; i++)
	{
		myDirTable->dirs[i] = myDirTable->dirs[i + 1];
	}
	myDirTable->dirUnitAmount--;
	return 0;
}

int MiniFS::releaseFile(int FCBBlock)
{
	FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);
	myFCB->link--; //链接数减一
	//无链接，删除文件
	if (myFCB->link == 0)
	{
		//释放文件的数据空间
		releaseBlock(myFCB->blockNum, myFCB->fileSize);
	}
	//释放FCB的空间
	releaseBlock(FCBBlock, 1);
	return 0;
}

int MiniFS::findUnitInTable(dirTable* myDirTable, char unitName[]) const
{
	//获得目录表
	int dirUnitAmount = myDirTable->dirUnitAmount;
	int unitIndex = -1;
	for (int i = 0; i < dirUnitAmount; i++) //查找目录项位置
		if (strcmp(unitName, myDirTable->dirs[i].fileName) == 0)
			unitIndex = i;
	return unitIndex;
}

void MiniFS::showMap() const
{
	int unitAmount = currentDirTable->dirUnitAmount;
	printf("name\ttype\tdataStartBlock\tdataEndBlock\n");
	//遍历所有表项
	for (int i = 0; i < unitAmount; i++)
	{
		//获取目录项
		dirUnit unitTemp = currentDirTable->dirs[i];
		printf("%s\t%d\t", unitTemp.fileName, unitTemp.type);
		//该表项是文件，继续输出大小和起始盘块号
		if (unitTemp.type == 1)
		{
			int FCBBlock = unitTemp.startBlock;
			FCB* fileFCB = (FCB*)getBlockAddr(FCBBlock);
			int temp = fileFCB->fileSize / BLOCK_SIZE;
			printf("%d\t\t%d\n", fileFCB->blockNum, fileFCB->blockNum + temp);
		}
		else {
			int dirBlock = unitTemp.startBlock;
			dirTable* myTable = (dirTable*)getBlockAddr(dirBlock);
			printf("%d\t%d\n", myTable->dirUnitAmount, unitTemp.startBlock);
		}
	}
}
int MiniFS::deleteDir(char dirName[])
{
	//忽略系统的自动创建的父目录
	if (strcmp(dirName, "..") == 0)
	{
		printf("can't delete ..\n");
		return -1;
	}
	//查找文件
	int unitIndex = findUnitInTable(currentDirTable, dirName);
	if (unitIndex == -1)
	{
		printf("file not found\n");
		return -1;
	}
	dirUnit myUnit = currentDirTable->dirs[unitIndex];
	//判断类型
	if (myUnit.type == 0) //目录
	{
		deleteFileInTable(currentDirTable, unitIndex);
	}
	else {
		printf("not a dir\n");
		return -1;
	}
	//从目录表中剔除
	deleteDirUnit(currentDirTable, unitIndex);
	printf("secceed!\n");
	return 0;
}
int MiniFS::deleteFileInTable(dirTable* myDirTable, int unitIndex)
{
	//查找文件
	dirUnit myUnit = myDirTable->dirs[unitIndex];
	//判断类型
	if (myUnit.type == 0) //目录
	{
		//找到目录位置
		int FCBBlock = myUnit.startBlock;
		dirTable* table = (dirTable*)getBlockAddr(FCBBlock);
		//递归删除目录下的所有文件
		printf("cycle delete dir %s\n", myUnit.fileName);
		int unitCount = table->dirUnitAmount;
		for (int i = 1; i < unitCount; i++) //忽略“..”
		{
			printf("delete %s\n", table->dirs[i].fileName);
			deleteFileInTable(table, i);
		}
		//释放目录表空间
		releaseBlock(FCBBlock, 1);
	}
	else
	{	//文件
		//释放文件内存
		int FCBBlock = myUnit.startBlock;
		releaseFile(FCBBlock);
	}
	return 0;
}

int MiniFS::copy(char oldName[], char newName[])
{
	int unitIndex = findUnitInTable(currentDirTable, oldName);
	if (unitIndex == -1)
	{
		printf("file not found\n");
		return -1;
	}
	int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
	FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);
	createFile(newName, myFCB->fileSize);
	return 0;
}
int MiniFS::changeDir(char dirName[])
{
	//目录项在目录位置
	int unitIndex = findUnitInTable(currentDirTable, dirName);
	//不存在
	if (unitIndex == -1)
	{
		printf("file not found\n");
		return -1;
	}
	if (currentDirTable->dirs[unitIndex].type == 1)
	{
		printf("not a dir\n");
		return -1;
	}
	//修改当前目录
	int dirBlock = currentDirTable->dirs[unitIndex].startBlock;
	currentDirTable = (dirTable*)getBlockAddr(dirBlock);
	//修改全局绝对路径
	if (strcmp(dirName, "..") == 0)
	{
		//回退绝对路径
		int len = strlen(path);
		for (int i = len - 2; i >= 0; i--)
			if (path[i] == '\\')
			{
				path[i + 1] = '\0';
				break;
			}
	}
	else {
		strcat(path, dirName);
		strcat(path, "\\");
	}
	printf("succeed!\n");
	return 0;
}

void MiniFS::help() const
{
	printf("\n----------帮助信息----------\n");
	printf("create: 创建系统\n");
	printf("输入格式： create + 系统名\n");
	printf("fmt: 格式化文件\n");
	printf("输入格式： fmt\n");
	printf("mk: 创建目录\n");
	printf("输入格式： mk + 目录名\n");
	printf("dr: 显示文件目录\n");
	printf("输入格式： dr\n");
	printf("cp: 拷贝文件目录\n");
	printf("输入格式： cp + 文件名1 + 文件名2\n");
	printf("close: 关闭文件系统\n");
	printf("输入格式： close\n");
	printf("dl: 删除文件\n");
	printf("输入格式： dl + 文件名\n");
	printf("map: 显示文件所在全部块的磁盘块号\n");
	printf("输入格式： map\n");
	printf("att: 显示文件属性\n");
	printf("输入格式： att + 文件名\n");
	printf("opt: 使用连续块号存储文件内容\n");
	printf("输入格式： opt\n");
	printf("touch: 创建文件\n");
	printf("输入格式： touch + 文件名 + 文件大小\n");
	printf("rm: 删除目录\n");
	printf("输入格式： rm + 目录名\n");
	printf("请输入 help 获得帮助！\n");
	printf("\n");
}