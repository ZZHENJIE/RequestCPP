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

  MemoryObj->Data = (char *)realloc(MemoryObj->Data,MemoryObj->Size + Realsize + 1);
  if (MemoryObj->Data == NULL){
    fprintf(stderr,"realloc() Failed!\n");
    return 0;
  }

  memcpy(&(MemoryObj->Data[MemoryObj->Size]),Ptr,Realsize);
  MemoryObj->Size += Realsize;
  MemoryObj->Data[MemoryObj->Size] = 0;

  return Realsize;
}

Instance::Instance(){
    this->CurlHandle = curl_easy_init();
    if(CurlHandle == NULL){
        perror("curl_easy_init() Failed!\n");
    }
    this->IsRequest = false;
    this->OptionsFunction = NULL;
}

Instance::~Instance(){
    curl_easy_cleanup(this->CurlHandle);
    if(this->IsRequest == true){
        free(this->MemoryObj.Data);
    }
}

void Instance::SetOptionsFunction(SetOptions Function){
    this->OptionsFunction = Function;
}

CURLcode Instance::Request(const char * Url,void * UserData){
    this->IsRequest = true;
    if(this->OptionsFunction != NULL){
        this->OptionsFunction(this->CurlHandle,UserData);
    }
    curl_easy_setopt(this->CurlHandle,CURLOPT_URL,Url);
    curl_easy_setopt(this->CurlHandle,CURLOPT_WRITEFUNCTION,Write_Callback);
    curl_easy_setopt(this->CurlHandle,CURLOPT_WRITEDATA,(void *)&this->MemoryObj);
    return curl_easy_perform(this->CurlHandle);
}