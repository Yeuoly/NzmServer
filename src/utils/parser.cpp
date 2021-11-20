#include "./parser.h"

bool IsPathInSize(char *path, int length) {
    int deep = 0, pos = 0;
    while(pos < length && path[pos] != 0){
        if(path[pos] == '.'){
            if(path[pos + 1] == '/'){
                pos += 2;
            }else if(path[pos + 1] == '.' && path[pos + 2] == '/'){
                deep--;
                pos += 3;
            }else{
                pos++;
            }
        }else if(path[pos] == '/'){
            pos++;
            deep++;
        }else{
            pos++;
        }
    }
    
    return deep >= 1;
}

