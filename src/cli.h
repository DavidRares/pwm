#ifndef CLI_H
#define CLI_H

#include "auth.h"
#include <sstream>
#include <memory>

using namespace std;

class Command{
public:
    Command(vector<string> argv, User& user);
    virtual void exec() = 0;

private:
    vector<string> argv;
    User* user;
};

class addEntry : public Command{
public:
    using Command::Command;
    void exec() override;
};

class deleteEntry : public Command{
public:
    using Command::Command;
    void exec() override;
};

class updateEntry : public Command{
public:
    using Command::Command;
    void exec() override;
};

class getEntry : public Command{
public:
    using Command::Command;
    void exec() override;
};

class changePassword : public Command{
public:
    using Command::Command;
    void exec() override;
};

class CLIService{
public:
    void validate_args(const int argc, const char* argv[]);
    void handle_doorway(const string command, User& user);
    unique_ptr<Command> handle_command(const string& input_str, User& user);
}extern input_handler;
#endif