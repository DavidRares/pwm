#include "database.h"

using namespace std;

DatabaseService::DatabaseService(){
    sqlite3* db;

    int ret = 0;
    try{
        ret = sqlite3_open("pwm.db", &db);

        if (ret) {
            throw runtime_error("Unable to open the database.");
        }
    }
    catch(const runtime_error& e){
        cout << "Error: " << e.what() << endl;
    }

    this->db = db;
}

DatabaseService::~DatabaseService(){
    cout << db;
    sqlite3_close(this->db);
}