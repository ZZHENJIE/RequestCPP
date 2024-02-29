#include "Request.h"

using namespace Request;

void Request::RequestInit(long flags){
    curl_global_init(flags);
}

void Request::RequestQuit(){
    curl_global_cleanup();
}

size_t Write_Callback(void * Ptr, size_t Size, size_t Nmemb, void * Userdata){
    size_t Realsize = Size * Nmemb;
    Memory * MemoryObj = (Memory *)Userdata;

    char * NewData = (char *)realloc(NULL,MemoryObj->Size + Realsize + 1);
    if (MemoryObj->Data == NULL){
        perror("realloc() Failed!\n");
        return 0;
    }else{
        memcpy(NewData,MemoryObj->Data,MemoryObj->Size);
        free(MemoryObj->Data);
        MemoryObj->Data = NewData;
    }

    memcpy(&(MemoryObj->Data[MemoryObj->Size]),Ptr,Realsize);
    MemoryObj->Size += Realsize;
    MemoryObj->Data[MemoryObj->Size] = 0;

    return Realsize;
}

Memory Instance::GetData(){
    return this->MemoryObj;
}

Instance::Instance(){
    this->CurlHandle = curl_easy_init();
    this->MemoryObj.Data = (char *)malloc(1);
    if(CurlHandle == NULL){
        perror("curl_easy_init() Failed!\n");
    }
    this->OptionsFunction = NULL;
}

Instance::~Instance(){
    curl_easy_cleanup(this->CurlHandle);
    free(this->MemoryObj.Data);
}

void Instance::SetOptionsFunction(SetOptions Function){
    this->OptionsFunction = Function;
}

CURLcode Instance::Request(const char * Url,void * UserData){
    if(this->OptionsFunction != NULL){
        this->OptionsFunction(this->CurlHandle,UserData);
    }
    free(this->MemoryObj.Data);
    this->MemoryObj.Data = (char *)malloc(1);
    this->MemoryObj.Size = 0;
    curl_easy_setopt(this->CurlHandle,CURLOPT_URL,Url);
    curl_easy_setopt(this->CurlHandle,CURLOPT_WRITEFUNCTION,Write_Callback);
    curl_easy_setopt(this->CurlHandle,CURLOPT_WRITEDATA,(void *)&this->MemoryObj);
    return curl_easy_perform(this->CurlHandle);
}