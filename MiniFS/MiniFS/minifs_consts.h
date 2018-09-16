#pragma once


namespace RM
{
	//磁盘操作接口
	int const SYSTEM_SIZE = 1024 * 1024 * 1024;			//系统大小
	int const BLOCK_SIZE = 1024;						//磁盘分配
	int const BLOCK_COUNT = SYSTEM_SIZE / BLOCK_SIZE;	//系统盘块数目
	int const DIRTABLE_MAX_SIZE = 63;			//15	//目录表项最大值

	//指令数目
	char const command[13][7] = { "fmt","close","dr","cp","dl","map","opt","att","help","touch","mk","rm","cd" };

}

namespace MMM
{
	
}