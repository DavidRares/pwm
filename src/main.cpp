#include <iostream>
#include <exception>
#include <cstring>

#include "cli.h"

DatabaseService database;
UsersDatabase user_table(database);
VaultDatabase vault_table(database);
AuthService auth(user_table);
PasswordHasher hasher;
CLIService cli;

int main(const int argc, const char* argv[]){
    User user("username", "password");

    try{
        cli.validate_args(argc, argv);

        User new_user(argv[2],argv[3]);
        
        cli.handle_doorway(argv[1], new_user);
        user = new_user;

    }
    catch(const invalid_argument& e){
        cout <<"Error: "<< e.what() << endl;
        return 0;
    }

    string command;
    const string input_prompt = user.get_username() + " $ ";

    cout << input_prompt;
    while (getline(cin, command)){
        if (command == "exit")
            break;
            
        try{
            cli.handle_command(command, user);
        }
        catch (const invalid_argument& e){
            cout << "Error: " << e.what() << endl;
        }
        
        cout << input_prompt;
    }
    
    return 0;
}