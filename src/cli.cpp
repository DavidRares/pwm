#include "cli.h"

extern AuthService auth;

void CLIService::validate_args(const int argc, const char* argv[]){
    if(argc != 4)
        throw invalid_argument("Not enough arguments!");

    if (strcmp(argv[1], "register") != 0 && strcmp(argv[1], "login") != 0)
        throw invalid_argument("You can either register or login.");

    if (strpbrk(argv[2], UNALLOWED_CHARS) != NULL)
        throw invalid_argument("Username contains unallowed characters.");
}

void CLIService::handle_doorway(const string command, User& user){
    if(command == "register")
        auth.register_user(user);
    else if (command == "login")
        auth.login_user(user);
    else 
        throw invalid_argument("Invalid command.");
}

unique_ptr<Command> CLIService::handle_command(const string& input_str, User& user){
    stringstream ss(input_str);
    vector<string> argv;

    string word;
    while (ss >> word) 
        argv.push_back(word);

    if (argv.empty())
        return nullptr;

    int argc = argv.size();
    unique_ptr<Command> ret = nullptr;

    if(argv.at(0) == "add" && argc == 4)
        ret = make_unique<addEntry>(move(argv), user);
    // else if(argv.at(0) == "delete")
    //     ret = make_unique<deleteEntry>();
    // else if(argv.at(0) == "update")
    //     ret = make_unique<updateEntry>();
    // else if(argv.at(0) == "get")
    //     ret = make_unique<getEntry>();
    // else if(argv.at(0) == "chpwd")
    //     ret = make_unique<changePassword>();
    else 
        throw invalid_argument("Invalid command."); 

    return ret;
}

Command::Command(vector<string> argv, User& user):argv(move(argv)),user(&user){}

void addEntry::exec(){
}