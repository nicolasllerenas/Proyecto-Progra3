#include "DatabaseManager.h"

DatabaseManager* DatabaseManager::instance = nullptr;

int main(){
    DatabaseManager* instance1 = DatabaseManager::getInstance();
    instance1->handleCSV();
    instance1->runMenu();
    return 0;
}
