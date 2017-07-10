//
//  string(RESOURCE_DIR).hpp
//  clc
//
//  Created by Grant Giesbrecht on 4/25/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#ifndef resource_dir_h

#include <string>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#define LIST_COMMAND "dir"
#define SYSTEM_OS "MS Windows"
#define HOME_DIR "C:\\"
#define RESOURCE_DIR "C:\resources/CLC_3V0/"
#else
#define CLEAR_COMMAND "clear"
#define LIST_COMMAND "ls"
#define RESOURCE_DIR "/usr/local/resources/CLC_3V0/"
#define HOME_DIR "/"
#endif

#ifdef __APPLE__
#define SYSTEM_OS "MacOS"
#elif __linux__
#define SYSTEM_OS "LINUX"
#elif __unix__
#define SYSTEM_OS "UNIX"
#endif

#define resource_dir_h


#endif /* resource_dir_h */
