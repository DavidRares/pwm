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

unique_ptr<Command> CLIService::handle_command(const string input_str){
    stringstream ss(input_str);
    vector<string> argv;

    string word;
    while (ss >> word) 
        argv.push_back(word);

    if (argv.empty())
        return nullptr;

    unique_ptr<Command> ret = nullptr;

    //if(argv.at(0) == "add")
    //    ret = make_unique<addEntry>();
    // else if(argv.at(0) == "delete")
    //     ret = make_unique<deleteEntry>();
    // else if(argv.at(0) == "update")
    //     ret = make_unique<updateEntry>();
    // else if(argv.at(0) == "get")
    //     ret = make_unique<getEntry>();
    // else if(argv.at(0) == "changepwd")
    //     ret = make_unique<changePassword>();
    //else {
    //    throw runtime_error("Invalid command."); 
    //    return nullptr;
    //}

    return ret;
}