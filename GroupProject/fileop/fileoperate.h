#ifndef FILEOPERATE_H_INCLUDED
#define FILEOPERATE_H_INCLUDED

#include <Windows.h>
#include <tchar.h>
#include "diskoperate.h"
//https://www.google.com.hk/search?newwindow=1&safe=strict&source=hp&ei=u6qaW9baEYn18gXQ_YHQCg&q=CreateFile+1g&oq=CreateFile+1g&gs_l=psy-ab.3..33i160k1.1061.9302.0.9498.38.26.10.2.2.0.280.2697.15j7j2.24.0....0...1c.1.64.psy-ab..3.34.2544...0j0i12k1j0i30k1j0i8i30k1.0._VroNyaYjFM
//https://blog.csdn.net/jmshl/article/details/2031744

#define dirTable_max_size	63//15	//目录表项最大值


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
	dirUnit dirs[dirTable_max_size];	//目录项列表
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

class MiniFS
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
	HANDLE hFile; //the handle of the total created space as one file (operating space SName)
	HANDLE hFMap; //the handle of memory-mapped file object

	
	BYTE *systemStartAddr;		//系统起始地址
	dirTable *rootDirTable;		//根目录
	dirTable *currentDirTable;	//当前位置
	char path[200]; //保存当前绝对路径

public:
	/////////////////////////////////////////////////////////////////////
	// minifs.cpp
	int create(); //create 1G space to store files and the filesystem info
	//int mount();
	//int fmt();
	void close();
	// dr,
	//	cp,
	//	dl,
	//	map,
	//	opt,
	//	att,
	//	help,
	//	touch,
	//	mk,
	//	rm,
	//	cm


	/////////////////////////////////////////////////////////////////////
	// diskoperate.cpp
	//初始化系统
	void initSystem();	
	//退出系统
	void exitSystem();

	//磁盘分配
	//allocate and get a chunk of blocks and return the start block index
	int getBlock(int blockSize);
	//获得盘块的物理地址
	//get the address of the "blockNum" indexed block
	BYTE *getBlockAddr(int blockNum) const;
	//获得物理地址的盘块号
	//get the index of the "addr" addressed blocked
	int getBlockIdx(BYTE* addr) const;
	//释放盘块
	//release a chunk of blocks, start from "blockNum" indexed block, in total of "blockSize" count
	int releaseBlock(int blockNum, int blockSize);

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

	//******************创建和删除文件
	//创建文件 touch
	int createFile(char fileName[], int fileSize);
	//删除文件 rm
	int deleteFile(char fileName[]);


	//释放文件内存
	int releaseFile(int FCBBlock);
	//添加目录项
	int addDirUnit(dirTable* currentDirTable, char fileName[], int type, int FCBBlockNum);

	//创建FCB
	int createFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
	//指定目录删除
	int deleteFileInTable(dirTable* myDirTable, int unitIndex);
	//删除目录项
	int deleteDirUnit(dirTable* currentDirTable, int unitIndex);
	//从目录中查找目录项目 //TODO : Search by hash table
	int findUnitInTable(dirTable* currentDirTable, char unitName[]) const;
	//展示当前占用块 map
	void showMap() const;
	//复制文件
	int copy(char oldName[], char newName[]);
	//输出帮助
	void help() const;
};

#endif // FILEOPERATE_H_INCLUDED
