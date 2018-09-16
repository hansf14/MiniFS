#pragma once

#include "minifs.h"

//Memory-Mapping-Managed method
namespace MMM
{

}

//Mini File System (in Memory-Mapping-Managed method)
class MiniFS_MMM : public MiniFS
{
public:
	/////////////////////////////////////////////////////////////////////
	// minifs.cpp
	int create(); //create 1G space to store files and the filesystem info
	//int mount(); //mount the filesystem with the mount-point at the current directory
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

protected:
	HANDLE hFile; //the handle of the total created space as one file (operating space SName)
	HANDLE hFMap; //the handle of memory-mapped file object
};