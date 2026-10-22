#ifndef CLI
#define CLI

#include "auth.h"
using namespace std;

class CLIService{
    public:
        void validate_args(const int argc, const char* argv[]);
        void handle_doorway(const string command, User& user);
}extern input_handler;

#endif