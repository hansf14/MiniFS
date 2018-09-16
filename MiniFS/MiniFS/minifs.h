#ifndef FILEOPERATE_H_INCLUDED
#define FILEOPERATE_H_INCLUDED

#pragma warning(disable:4996)

#include <Windows.h>
#include <tchar.h>
#include "minifs_consts.h"

//https://www.google.com.hk/search?newwindow=1&safe=strict&source=hp&ei=u6qaW9baEYn18gXQ_YHQCg&q=CreateFile+1g&oq=CreateFile+1g&gs_l=psy-ab.3..33i160k1.1061.9302.0.9498.38.26.10.2.2.0.280.2697.15j7j2.24.0....0...1c.1.64.psy-ab..3.34.2544...0j0i12k1j0i30k1j0i8i30k1.0._VroNyaYjFM
//https://blog.csdn.net/jmshl/article/details/2031744





class MiniFS
{
public:
	MiniFS();
	virtual ~MiniFS();

private:
	int old_stdoutMode;
};


#endif // FILEOPERATE_H_INCLUDED
