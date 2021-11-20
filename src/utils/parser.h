#ifndef __PARSER_H
#define __PARSER_H

//将一个路径保持在其根目录下，不允许路径穿透到上级目录
bool IsPathInSize(char *path, int length);

#endif