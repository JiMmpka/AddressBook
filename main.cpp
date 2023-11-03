// KSIAZKA ADRESOWA - LOGOWANIE

#include <iostream>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

struct Addressee{
    int id{}, userId;
    string name{}, surname{}, telephone{}, email{}, address{};
};

struct User{
    int userId{};
    string name{}, password{};
};

enum SearchType{
    FIRST_NAME, LAST_NAME
};

string loadInputLine(){
    string term;

    getline(cin, term);

    return term ;
}

char readSign(){
    char sign;
    string input;

    while(true){
        input = loadInputLine();

        if (input.length() == 1){
            sign = input[0];
            break;
        }
        else cout << "This is not a single sign. Try again." << endl;
    }
    return sign;
}

char readInt(){
    int number;
    string input;

    while(true){
        input = loadInputLine();
        stringstream myStream(input);

        if (myStream >> number){
            break;
        }
        cout << "This is not a number. Try again." << endl;
    }
    return number;
}

void saveUserToTheFile(User tempUser, fstream &file){
    file << tempUser.userId << '|'
         << tempUser.name << '|'
         << tempUser.password << '|' << endl;
}

void registration(vector <User> &users){
    string regName, regPassword;
    int lastAddresseeId = users.empty() ? 0 : users.back().userId;
    User tempUser;

    cout << "Enter your username: ";
    regName = loadInputLine();

    for (auto person : users){
        if (person.name == regName){
            cout << "This user exists. Please enter a different username: ";
            regName = loadInputLine();
        }
    }

    cout << "Enter the password: ";
    regPassword = loadInputLine();

    tempUser.name = regName;
    tempUser.password = regPassword;
    tempUser.userId = ++lastAddresseeId;

    users.push_back(tempUser);

    fstream usersList("UsersList.txt", ios::out | ios::app);

    if(usersList.good()){
        saveUserToTheFile(tempUser, usersList);
        usersList.close();
        cout << endl << "Account has been created" << endl;
        Sleep(1500);
    }
    else {
        cout << "File doesn't exist. Adressee not added" << endl;
        Sleep(1500);
        users.pop_back();
    }
}

int login(vector <User> users){
    string logName, logPassword;
    int howManyMoreTimesCanYouEnterThePassword = 2;

    cout << "Enter your username: ";
    logName = loadInputLine();

    for (auto person : users){
        if (person.name == logName){
            for (int j = 0; j <= howManyMoreTimesCanYouEnterThePassword; j++){
                cout << "enter the password: ";
                logPassword = loadInputLine();

                if (person.password == logPassword){
                    cout << endl << "You logged in" << endl;
                    Sleep(1000);
                    return person.userId;
                }
                else{
                    cout << "Incorect passsword. Attempts left: " << (howManyMoreTimesCanYouEnterThePassword - j) << endl;
                }
            }
            cout << "Password was entered wrong three times, wait 3 seconds to try again." << endl;
            Sleep(3000);
            return(0);
        }
    }
    cout << "There is no such user.";
    Sleep(2000);
    return(0);
}

void changePassword(vector <User> &users, int idOfLoggedUser){
    string changedPassword;

    cout << "Enter new password: ";
    changedPassword = loadInputLine();

    for (auto &person : users){
        if (person.userId == idOfLoggedUser){
            person.password = changedPassword;
            cout << "Password has been changed.";
            Sleep(2000);
        }
    }
}

void viewOneAddressee(Addressee addressees){
    cout << setw(15) << left << "Id:" << addressees.id << endl;
    cout << setw(15) << left << "Name:" << addressees.name << endl;
    cout << setw(15) << left << "Surname:" << addressees.surname << endl;
    cout << setw(15) << left << "Telephone:" << addressees.telephone << endl;
    cout << setw(15) << left << "Email:" << addressees.email << endl;
    cout << setw(15) << left << "Address:" << addressees.address << endl << endl;
}

void viewAllAddressees(vector <Addressee> addressees){
    if (addressees.empty()){
        cout << "There are no addressees in the list." << endl << "To use any option first add addressee - option 1" << endl;
        system("pause");
        return;
    }

    for (auto person : addressees){
        viewOneAddressee(person);
    }
    system("pause");
}

bool checkingIfTheAddresseeWithTheSameDataAlreadyExist(vector <Addressee> addressees, Addressee tempAdressee){
    for (auto person : addressees){
        if ((person.name == tempAdressee.name) &&
            (person.surname == tempAdressee.surname) &&
            (person.telephone == tempAdressee.telephone) &&
            (person.email == tempAdressee.email) &&
            (person.address == tempAdressee.address))
            return true;
    }
    return false;
}

Addressee checkAddresseeAvailability(vector <Addressee> addressees, int id){
    for (auto person : addressees){
        if (person.id == id){
            return person;
        }
    }
    return {};
}

void saveAddresseDataToTheFileInOneLine(Addressee tempAdressee, fstream &file){
    file << tempAdressee.id << '|'
         << tempAdressee.userId << '|'
         << tempAdressee.name << '|'
         << tempAdressee.surname << '|'
         << tempAdressee.telephone << '|'
         << tempAdressee.email << '|'
         << tempAdressee.address << '|' << '\n';
}

bool saveAddreesseToFile(Addressee tempAdressee){
    fstream file("AddresseesList.txt", ios::out | ios::app);

    if(file.good()){
        saveAddresseDataToTheFileInOneLine(tempAdressee, file);
        file.close();
        cout << endl << "Addresse was added to the list.";
        Sleep(1500);
        return true;
    }
    else {
        cout << "File doesn't exist. Adressee not added" << endl;
        Sleep(1500);
        return false;
    }
}

int addAddressee(vector <Addressee> &addressees, int idOfLoggedUser, int lastAddresseeId){
    bool whetherTheAddreesseHasBeenSaved;
    Addressee tempAdressee;
    tempAdressee.userId = idOfLoggedUser;

    cout << "Please enter the addressee's name: "; tempAdressee.name = loadInputLine();
    cout << "Please enter the addressee's last name: "; tempAdressee.surname = loadInputLine();
    cout << "Please enter the addressee's telephone number: "; tempAdressee.telephone = loadInputLine();
    cout << "Please enter the addressee's email: "; tempAdressee.email = loadInputLine();
    cout << "Please enter the addressee's full address: "; tempAdressee.address = loadInputLine();

    if (!addressees.empty() && checkingIfTheAddresseeWithTheSameDataAlreadyExist(addressees, tempAdressee)){
        cout << "\nThees addressee already exist. Back to main menu. \n" ;
        Sleep(2000);
        return lastAddresseeId;
    }
    else {
        tempAdressee.id = ++lastAddresseeId;
        whetherTheAddreesseHasBeenSaved = saveAddreesseToFile(tempAdressee);

        if (whetherTheAddreesseHasBeenSaved) addressees.push_back(tempAdressee);
        else {
            addressees.pop_back();
            return --lastAddresseeId;
        }
        return lastAddresseeId;
    }
}

void loadUsersListFormTxtFile(vector <User> &users){
    User tempUser;
    ifstream file("UsersList.txt", ios::in | ios::app);
    string line{}, separatedStreamElements{};

    if(file.good()){
        while(getline(file, line)){
            stringstream myStream(line);

            getline(myStream, separatedStreamElements,'|');
            tempUser.userId = stoi(separatedStreamElements);

            getline(myStream,tempUser.name,'|');
            getline(myStream,tempUser.password,'|');
            users.push_back(tempUser);
        }
        file.close();
    }
    else{
        cout << "File ""UsersList.txt"" doesn't exist." << endl << endl;
        system("pause");
    }
}

int loadAddresseesListFormTxtFile(vector <Addressee> &addressees, int idOfLoggedUser){
    size_t positionOfSign{};
    int userIdFromAdressesList{}, lastAddresseeId{};
    string line{}, lastline{}, separatedStreamElements{};
    Addressee tempAddressee;

    ifstream file("AddresseesList.txt", ios::in | ios::app);

    if(file.good()){
        while(getline(file, line)){
            if (!line.empty()){
                lastline = line;
            }
            lastAddresseeId = stoi(lastline.substr(0));

            positionOfSign = line.find('|');
            userIdFromAdressesList = stoi(line.substr(positionOfSign + 1));

            if(userIdFromAdressesList != idOfLoggedUser){
                continue;
            }
            else {
                stringstream myStream(line);

                getline(myStream, separatedStreamElements,'|');
                tempAddressee.id = stoi(separatedStreamElements);

                getline(myStream, separatedStreamElements,'|');
                tempAddressee.userId = stoi(separatedStreamElements);

                getline(myStream,tempAddressee.name,'|');
                getline(myStream,tempAddressee.surname,'|');
                getline(myStream,tempAddressee.telephone,'|');
                getline(myStream,tempAddressee.email,'|');
                getline(myStream,tempAddressee.address,'|');
                addressees.push_back(tempAddressee);
            }
        }
        file.close();
        return lastAddresseeId;
    }
    else{
        cout << "File ""AddresseesList.txt"" doesn't exist." << endl << endl;
        system("pause");
        return 0;
    }
}

void searchByNameOrLastName(vector <Addressee> addressees, SearchType searchType){
    string nameOrLastName{};
    bool flag = true;

    if (addressees.empty()){
        cout << "There are no addressees in the list." << endl << "To use any option first add addressee - option 1" << endl;
        system("pause");
        return;
    }

    cout << ((searchType == FIRST_NAME) ? "Enter name: ": "Enter last name: ");
    nameOrLastName = loadInputLine();

    for (auto person : addressees){
        if ((person.name == nameOrLastName) && (searchType == FIRST_NAME)){
            flag = false;
            viewOneAddressee(person);
        }
        else if ((person.surname == nameOrLastName) && (searchType == LAST_NAME)){
            flag = false;
            viewOneAddressee(person);
        }
    }

    if (flag){
        cout << "There is no such person on the list of addressees. Back to main menu";
        Sleep(2000);
    }
    else system("pause");
}

void saveModifyedAddresseToVectorOfStruct(vector <Addressee> &addressees, Addressee wantedAaddressee){
    for (auto &person : addressees){
        if (person.id == wantedAaddressee.id){
            person = wantedAaddressee;
        }
    }
}

int deleteAddreesseFromTxtFile(int deletedAddresseeId, int lastAddresseeId){
    int addreesseIdFromTxtFile, currentAddresseeId;
    string line;
    ifstream file("AddresseesList.txt");
    ofstream out("outfile.txt");

    if (file.good()){
        while (getline(file, line)){
            addreesseIdFromTxtFile = stoi(line.substr(0));

            if (addreesseIdFromTxtFile != deletedAddresseeId){
                currentAddresseeId = addreesseIdFromTxtFile;
                out << line << '\n';
            }
            else lastAddresseeId = (deletedAddresseeId == lastAddresseeId) ? currentAddresseeId : lastAddresseeId;
        }
        file.close();
        out.close();
        remove("AddresseesList.txt");
        rename("outfile.txt","AddresseesList.txt");

        cout << endl << "Addressee deleted" << endl;
        Sleep(1500);
    }
    else {
        cout << "File doesn't exist. Back to main menu" << endl;
        Sleep(1500);
    }
    return lastAddresseeId;
}

int deleteAddressee(vector <Addressee> &addressees, int lastAddresseeId){
    int id;
    char yesNo;
    Addressee wantedAaddressee;

    if (addressees.empty()){
        cout << "There are no addressees in the list." << endl << "To use any option first add addressee - option 1" << endl;
        system("pause");
        return lastAddresseeId;
    }

    cout << "Please enter the addressee's ID: ";
    id = readInt();

    wantedAaddressee = checkAddresseeAvailability(addressees, id);

    if (wantedAaddressee.id != 0){
        cout << "\nDo you want to delete " <<  wantedAaddressee.name  << " " << wantedAaddressee.surname  << " from addressess list? (type Y or N): ";

        while(1){
            yesNo = readSign();

            if (toupper(yesNo) == 'Y'){
                for (auto it = addressees.begin(); it != addressees.end();){
                    if (it->id == id){
                        it = addressees.erase(it);
                    }
                    else ++it;
                }
                lastAddresseeId = deleteAddreesseFromTxtFile(id, lastAddresseeId);

                return lastAddresseeId;
            }
            else if (toupper(yesNo) == 'N'){
                cout << "Taped ""N"". Back to main menu" << endl;
                Sleep(2500);
                return lastAddresseeId;
            }
            else cout << "You can tape ""Y"" or ""N"". Try again" << endl;
        }
    }
    else {
        cout << "There is no such person on the list of addressees. Back to main menu";
        Sleep(2500);
        return lastAddresseeId;
    }
}

void viewEditMenu(){
    cout << "Which data do you want to change?" << endl << endl
         << "1. Name" << endl
         << "2. Surname" << endl
         << "3. Telephone number" << endl
         << "4. Email" << endl
         << "5. Address" << endl
         << "9. Save changes and go to user menu" << endl << endl;
}

bool editAdresseeInVectorOfStruct(Addressee &addressee, bool &changes){
    char choise;
    bool flag = true;

    choise = readSign();
    cout << endl;

    switch(choise){
        case '1': cout << "Enter a new name: "; addressee.name = loadInputLine(); changes = true; break;
        case '2': cout << "Enter a new surname: "; addressee.surname = loadInputLine(); changes = true; break;
        case '3': cout << "Enter a new telephone number: "; addressee.telephone = loadInputLine(); changes = true; break;
        case '4': cout << "Enter a new Email: "; addressee.email = loadInputLine(); changes = true; break;
        case '5': cout << "Enter a new Address: "; addressee.address = loadInputLine(); changes = true; break;
        case '9': flag = false; break;
        default: cout << "Please choose one of the available options from 1 to 5 or 9 to exit to main menu." << endl; Sleep(1500);
    }
    return flag;
}

void editAddreesseinTxtFile(Addressee tempAdressee){
    int addreesseIdFromTxtFile;
    string line;
    ifstream file("AddresseesList.txt");
    fstream out("outfile.txt", ios::out);

    if (file.good()){
        while (getline(file, line)){
            addreesseIdFromTxtFile = stoi(line.substr(0));

            if(addreesseIdFromTxtFile != tempAdressee.id){
                out << line << '\n';
            }
            else {
                saveAddresseDataToTheFileInOneLine(tempAdressee, out);
            }
        }
    }
    else {
        cout << "File doesn't exist." << endl;
        Sleep(1500);
    }
    file.close();
    out.close();

    remove("AddresseesList.txt");
    rename("outfile.txt","AddresseesList.txt");
}

void editAddressee(vector <Addressee> &addressees){
    int id;
    char yesNo;
    bool flag = true, isThereAnyChangesToUpdate = false;
    Addressee wantedAaddressee;

    if (addressees.empty()){
        cout << "There are no addressees in the list." << endl << "To use any option first add addressee - option 1" << endl;
        system("pause");
        return;
    }

    cout << "Please enter the addressee's ID: ";
    id = readInt();

    wantedAaddressee = checkAddresseeAvailability(addressees, id);

    if (wantedAaddressee.id != 0){
        cout << "\nDo you want to edit " << wantedAaddressee.name << " " << wantedAaddressee.surname << "? (type Y or N): ";

        while(flag){
            yesNo = readSign();

            switch(toupper(yesNo)){
                case 'Y':{
                    while(flag){
                        system("cls");
                        viewOneAddressee(wantedAaddressee);
                        viewEditMenu();
                        flag = editAdresseeInVectorOfStruct(wantedAaddressee, isThereAnyChangesToUpdate);
                    }

                    if (isThereAnyChangesToUpdate){
                        saveModifyedAddresseToVectorOfStruct(addressees, wantedAaddressee);
                        editAddreesseinTxtFile(wantedAaddressee);
                        cout << "Addresse's details have been updated."; Sleep(1500);
                    }
                    else {
                        cout << "No changes have been made. Back to main menu" << endl; Sleep(2500);
                    }
                    break;
                }
                case 'N':{
                    cout << "Taped ""N"". Back to main menu" << endl;
                    Sleep(2500);
                    flag = false;
                    break;
                }
                default: cout << "You can tape ""Y"" or ""N"". Try again" << endl;
            }
        }
    }
    else {
        cout << "There is no such person on the list of addressees. Back to main menu";
        Sleep(2500);
    }
}

int main(){
    int idOfLoggedUser{}, lastAddresseeId{};
    char choise{};
    vector <User> users;
    vector <Addressee> addressees;

    loadUsersListFormTxtFile(users);

    while(1){
        if (idOfLoggedUser == 0){
            system("cls");
            cout << ">>> MAIN MENU <<<" << endl
                 << "--------------------------" << endl
                 << "1. Login" << endl
                 << "2. Registration" << endl
                 << "9. End the program" << endl
                 << "--------------------------" << endl;

            choise = readSign();
            cout << endl;

            switch(choise){
                case '1':{
                   if((idOfLoggedUser = login(users))){
                        lastAddresseeId = loadAddresseesListFormTxtFile(addressees, idOfLoggedUser);
                    }
                    break;
                }
                case '2': registration(users); break;
                case '9': cout << "End of the program." << endl; exit(0);
            }
        }
        else {
            system("cls");
            cout << ">>> USER MENU <<<" << endl
                 << "--------------------------" << endl
                 << "1. Add addressee" << endl
                 << "2. Search by name" << endl
                 << "3. Search by last name" << endl
                 << "4. Display all addressees" << endl
                 << "5. Delete addressee" << endl
                 << "6. Edit addressee" << endl
                 << "--------------------------" << endl
                 << "7. Change password" << endl
                 << "9. Logout" << endl
                 << "--------------------------" << endl << endl;

            choise = readSign();
            cout << endl;

            switch(choise){
                case '1': lastAddresseeId = addAddressee(addressees, idOfLoggedUser, lastAddresseeId); break;
                case '2': searchByNameOrLastName(addressees, FIRST_NAME); break;
                case '3': searchByNameOrLastName(addressees, LAST_NAME); break;
                case '4': viewAllAddressees(addressees); break;
                case '5': lastAddresseeId = deleteAddressee(addressees, lastAddresseeId); break;
                case '6': editAddressee(addressees); break;
                case '7': changePassword(users, idOfLoggedUser); break;
                case '9': cout << "Logged out." << endl; idOfLoggedUser = 0; addressees.clear(); Sleep(2000); break;
                default: cout << "Please choose one of the available options from 1 to 6 or exit."; Sleep(2000);
            }
        }
        cout << endl;
    }
    return 0;
}
