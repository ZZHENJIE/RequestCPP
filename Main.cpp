#include "Request.h"

int main(int argc,char * argv[]){

    Request::RequestInit(CURL_GLOBAL_DEFAULT);

    Request::Instance Obj;
    
    Obj.Request("http://www.example.com",NULL);

    printf("%s\n",Obj.GetData().Data);

    Request::RequestQuit();

    return 0;
}