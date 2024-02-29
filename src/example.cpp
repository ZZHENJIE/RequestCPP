#include <stdio.h>
#include "Request.h"

void Option(CURL * CurlHandle,void * Userdata){
    curl_easy_setopt(CurlHandle,CURLOPT_SSL_VERIFYPEER,0L);
}

int main(int argc,char * argv[]){
    
    Request::RequestInit(CURL_GLOBAL_DEFAULT);

    Request::Instance Obj;

    Obj.SetOptionsFunction(Option);

    Obj.Request("https://example.com/",NULL);

    printf("%s\n",Obj.GetData().Data);

    Request::RequestQuit();

    return 0;
}