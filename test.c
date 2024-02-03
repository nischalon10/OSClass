#include <sys/types.h> 
#include <stdio.h>
#include <unistd.h>


int main(){
    
    printf("Main Process:\t%d\n", getpid());
    int result = access("/usr/bin/ls", X_OK);
    printf("Access Result:\t%d\n", result);

}