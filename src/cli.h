#ifndef CLI
#define CLI

#include "auth.h"
#include <sstream>
#include <memory>

using namespace std;

class Command{
    public:
        Command();
        virtual ~Command();
        virtual void exec() = 0;
    private:
        User* user;
};

class addEntry : public Command{
    public:
        addEntry();
        ~addEntry() override;
        void exec() override;
    private:
        string domain;
        string title_hash;
        string password_hash;
};

class deleteEntry : public Command{
    public:
        deleteEntry();
        ~deleteEntry() override;
        void exec() override;
    private:
        string domain;
        int index;
};

class updateEntry : public Command{
    public:
        updateEntry();
        ~updateEntry() override;
        void exec() override;
    private:
        string domain;
        string title_hash;
        string password_hash;
        int index;
};

class getEntry : public Command{
    public:
        getEntry();
        ~getEntry() override;
        void exec() override;
    private:
        string domain;
};

class changePassword : public Command{
    public:
        changePassword();
        ~changePassword() override;
        void exec() override;
};

class CLIService{
    public:
        void validate_args(const int argc, const char* argv[]);
        void handle_doorway(const string command, User& user);
        unique_ptr<Command> handle_command(const string input_str);
}extern input_handler;
#endif