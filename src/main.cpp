#include <iostream>
#include <exception>
#include <cstring>

#include "auth.h"
#include "database.h"
#include "crypto.h"

using namespace std;

AuthService auth_service;

class InputHandler{
    public:
        void validate_args(const int argc, const char* argv[]){
            try{
                if(argc != 4)
                    throw runtime_error("Not enough arguments!");

                if (strcmp(argv[1],"register") != 0 && strcmp(argv[1],"login") != 0)
                    throw runtime_error("You can either register or login.");
                if (strpbrk(argv[2],UNALLOWED_CHARS) != NULL)
                    throw runtime_error("Username contains unallowed characters");
            }
            catch(const runtime_error& e){
                cout <<"Error: "<< e.what() << endl;
            }
        }
}input_handler;

int main(const int argc, const char* argv[]){
    input_handler.validate_args(argc, argv);
    
    return 0;
}