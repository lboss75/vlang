#ifndef __TARGETVER_H_
#define __TARGETVER_H_

#ifdef _WIN32

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>


#else

#include <sys/types.h>

#endif

#include "stddef.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifndef _WIN32
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#endif//_WIN32

#include <memory>
#include <exception>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <fstream>
#include <iostream>
#include <functional>

#endif// __TARGETVER_H_