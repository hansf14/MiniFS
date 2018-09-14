#include "fileoperate.h"
#include <stdio.h>
#include <stdlib.h>


void MiniFS::initSystem()
{
	//创建空间
	systemStartAddr = (BYTE*)malloc(SYSTEM_SIZE * sizeof(BYTE));
	//初始化盘块的位示图
	for (int i = 0; i < BLOCK_COUNT; i++)
		systemStartAddr[i] = '0';
	//用于存放位示图的空间已被占用
	int bitMapSize = BLOCK_COUNT * sizeof(BYTE) / BLOCK_SIZE;//λʾͼռ���̿���:1024
	printf("bitmapSize : [%d]\n", bitMapSize);
	for (int i = 0; i < bitMapSize; i++) //从零开始分配
		systemStartAddr[i] = '1'; //盘块已被使用
}

void MiniFS::exitSystem()
{
	free(systemStartAddr);
}

int MiniFS::getBlock(int blockSize)
{
	int startBlock = 0;
	int sum = 0;
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		if (systemStartAddr[i] == '0') //可用盘块
		{
			if (sum == 0) //刚开始，设置开始盘块号
				startBlock = i;
			sum++;
			if (sum == blockSize) //连续盘块是否满足需求
			{
				//满足分配，置1
				for (int j = startBlock; j < startBlock + blockSize; j++)
					systemStartAddr[j] = '1';
				return startBlock;
			}
		}
		else //已被使用,连续已经被打断
			sum = 0;
	}
	printf("not found such series memory Or memory is full\n");
	return -1;
}

BYTE *MiniFS::getBlockAddr(int blockNum) const
{
	return (systemStartAddr + blockNum * BLOCK_SIZE); //偏移量单位为字节
}

int MiniFS::getBlockIdx(BYTE* addr) const
{
	return (addr - systemStartAddr) / BLOCK_SIZE;
}

int MiniFS::releaseBlock(int blockNum, int blockSize)
{
	int endBlock = blockNum + blockSize;
	//修改位示图盘块的位置为0
	for (int i = blockNum; i < endBlock; i++)
		systemStartAddr[i] = '0';
	return 0;
}