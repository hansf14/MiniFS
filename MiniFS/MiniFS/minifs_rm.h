#pragma once

#include "minifs.h"

//RAM-Managed method
namespace RM
{
	//目录项 64B
	struct dirUnit
	{
		char fileName[59];		//文件名
		char type;				//文件类型,0目录， 1文件
		int startBlock;			//起始盘块
	};

	//目录表
	//一个目录表占用一个盘块，允许最多文件数为63
	struct dirTable
	{
		int dirUnitAmount;					//目录项数目
		dirUnit dirs[DIRTABLE_MAX_SIZE];	//目录项列表
		SYSTEMTIME time_create;
		SYSTEMTIME time_modify;
	};

	//FCB
	struct FCB
	{
		int blockNum;				//文件数据起始盘块号
		int fileSize;				//文件大小，盘块为单位
		int dataSize;				//已写入的内容大小，字节为单位
		int readptr;				//读指针，字节为单位			//Not used yet
		int link;					//文件链接数
		SYSTEMTIME time_create;
		SYSTEMTIME time_modify;
	};

	//multi-layouts for BLOCK type
	union BLOCK
	{
		FCB fcb;				//BLOCK in FCB form
		BYTE bytes[BLOCK_SIZE];	//BLOCK in bytes layout
	};
}


//Mini File System (in RAM-Managed method)
class MiniFS_RM : public MiniFS
{
public:
	enum class COMMAND
	{
		create,
		mount,
		fmt,
		close,
		dr,
		cp,
		dl,
		map,
		opt,
		att,
		help,
		touch,
		mk,
		rm,
		cm
	};
public:
	/////////////////////////////////////////////////////////////////////
	// fileoperate.cpp
	//初始化系统
	MiniFS_RM();
	//退出系统
	virtual ~MiniFS_RM();

	/////////////////////////////////////////////////////////////////////
	// diskoperate.cpp
	//初始化系统
	void initSystem();
	//退出系统
	void exitSystem();

	/////////////////////////////////////////////////////////////////////
	// fileoperate.cpp
	//文件系统操作

	//初始化根目录
	void initRootDir();
	//展示绝对路径
	char const* getPath() const;
	//展示当前目录文件
	void showDir() const;
	//展示文件属性
	void showAtt() const;
	//创建目录 mkdir
	int createDir(char dirName[]);
	//切换目录
	int changeDir(char dirName[]);
	//删除目录
	int deleteDir(char fileName[]);

	//创建文件 touch
	int createFile(char fileName[], int fileSize);
	//删除文件 rm
	int deleteFile(char fileName[]);


	//释放文件内存
	int releaseFile(int FCBBlock);
	//添加目录项
	int addDirUnit(RM::dirTable* currentDirTable, char fileName[], int type, int FCBBlockNum);

	//创建FCB
	int createFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
	//指定目录删除
	int deleteFileInTable(RM::dirTable* myDirTable, int unitIndex);
	//删除目录项
	int deleteDirUnit(RM::dirTable* currentDirTable, int unitIndex);
	//从目录中查找目录项目 //TODO : Search by hash table
	int findUnitInTable(RM::dirTable* currentDirTable, char unitName[]) const;
	//展示当前占用块 map
	void showMap() const;
	//复制文件
	int copy(char oldName[], char newName[]);
	//输出帮助
	void help() const;
	/////////////////////////////////////////////////////////////////////

protected:
	BYTE *systemStartAddr;		//系统起始地址
	RM::dirTable *rootDirTable;		//根目录
	RM::dirTable *currentDirTable;	//当前位置
	char path[200]; //保存当前绝对路径

protected:
	/////////////////////////////////////////////////////////////////////
	// diskoperate.cpp

	//磁盘分配
	//allocate and get a chunk of blocks and return the start block index
	int getBlock(int blockSize);
	//获得盘块的物理地址(内存)
	//get the address of the "blockNum" indexed block
	BYTE *getBlockAddr(int blockNum) const;
	//获得物理地址的盘块号
	//get the index of the "addr" addressed blocked
	int getBlockIdx(BYTE* addr) const;
	//释放盘块
	//release a chunk of blocks, start from "blockNum" indexed block, in total of "blockSize" count
	int releaseBlock(int blockNum, int blockSize);
};