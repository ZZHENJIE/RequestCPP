#pragma once

#include "curl/curl.h"

namespace Request{
    
    void RequestInit(long flags);
    void RequestQuit();

    struct Memory final{
        char * Data;
        size_t Size;
    };

    typedef void(SetOptions)(CURL * CurlHandle,void * Userdata);

    class Instance{
        public:
            Instance();
            ~Instance();
            void SetOptionsFunction(SetOptions Function);
            Memory GetData();
            CURLcode Request(const char * Url,void * UserData);
        private:
            CURL * CurlHandle;
            SetOptions * OptionsFunction;
            Memory MemoryObj;
    };

};