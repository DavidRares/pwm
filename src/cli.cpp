#include "cli.h"

extern AuthService auth;

void CLIService::validate_args(const int argc, const char* argv[]){
    if(argc != 4)
        throw runtime_error("Not enough arguments!");

    if (strcmp(argv[1], "register") != 0 && strcmp(argv[1], "login") != 0)
        throw runtime_error("You can either register or login.");

    if (strpbrk(argv[2], UNALLOWED_CHARS) != NULL)
        throw runtime_error("Username contains unallowed characters.");
}

void CLIService::handle_doorway(const string command, User& user){
    if(command == "register")
        auth.register_user(user);
    else if (command == "login")
        auth.login_user(user);
    else 
        throw runtime_error("Invalid command.");
}