#pragma once
#define DIR_NAME_SIZE 256

#include <fstream>
#include "File.h"

//TODO
class Dir //DAG
{
	char dir_name[DIR_NAME_SIZE];
	//TODO
};

// 1 partition : 1GB = 2^30B
// 1 block = 512B = 2^9B
// Total :  2^21 blocks
//2048 * 512B = 1M(2^20)B

class Interpreter
{
	void interpret(char *command);
};

class FileSystem
{
public:

	FileSystem();
	~FileSystem();

	int create(char *name);
	//create
	//mount
	//fmt : format SName
	//dr : dir : show file directory
	//cp : copy file
	//dl : delete file
	//tp : show text files
	//more
	//att : show file attributes
	//help
	//close

	//Block **getBlocks();
	//void addBlock(Block block);
	//Directory *getRoot();

private:
	/////////Directory *cur_dir;

	//PCB *getPCB();
	//Block *blocks[NUM_OF_BLOCKS];
};