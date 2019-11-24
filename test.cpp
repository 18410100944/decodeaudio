#include <stdio.h>
#include "decoder.h"
#include "callback.h"

FILE * fb;

int PcmCallback(char *data,int lenl,int ts){
    printf("data len=%d\n",lenl);
    fwrite(data,lenl,1,fb);
    return 0;
}

int main(int argc ,char ** argv){
    fb=fopen("./out.pcm","w+");
    decoder *tf=new decoder();
    tf->setUrl("./62bff14f534995249b89f1bf86e9ea68.ts");
    tf->setCallback(&PcmCallback);
    tf->init();
    tf->run();
    tf->clear();
    fclose(fb);
    return 0;
}