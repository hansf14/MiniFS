#pragma once
#include <ctime>

#define NUM_OF_BLOCKS 1000 // for the time being //todo : change to 2^21
#define BLOCK_SIZE 512

//TODO : consider the case which depends on the system when sizeof(char *) != 4byte

class FCB;
class FIB;

//#0
class SIB // File system information block // 512B
{
public:
	SIB();
	~SIB();

	FCB *getFCB_free() const;
	FCB *getFCB_full() const;
	FCB *getFIB_free() const;

private:
	FCB *fcb_free;
	FCB *fcb_full;
	FIB *fib_free;
};

//#1 ~ #7
class FCB // File control block // 64/32B
{
public:
	FCB();
	FCB(char const* filename, FileType filetype);
	~FCB();
	
private:
	char filename[32];
	FileType file_type; // File identifier (file or directory)
	int file_size;
	//filetime (creation)(534p)
	//filetime (modify)(534p)
	Permission permission;
	char isDeleted;
	char *pBuf[9]; //on which disk block (position)
	//TODO : more attributes
	char reserved[];
	//reserved //not used in current version
};

//class FCB
//{
//public:
//	char name[8]; // File name
//	char ext[4]; // File extension
//	int nodeID; //File index // OS' file management index
//	FileType type;
//	char path[8];
//	int size; // File size
//	AccessLock access;
//	//char m_modify_date; //
//	FCB *index[N]; // Index table
//	FCB *parent; // Parent node
//
//	FCB();
//	~FCB();
//};

//#8 ~ #2047
class FIB // File information block
{
	char bytes[BLOCK_SIZE];
	FIB *next;
};

class Block
{
private:
	union
	{
		SIB sib;
		FDB fdb;
		FIB fib;
		char bytes[BLOCK_SIZE]; // Block in bytes layout
	};
};

enum class FileType
{
	FILE,
	FOLDER
	//LINK
};

enum class Permission
{
	READ,
	WRITE,
	READ_ONLY
};

//TODO
class File
{
	union
	{
		struct
		{

		};
		char bytes[BLOCK_SIZE]; // File in bytes layout
	};
};