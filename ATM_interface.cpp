#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

using namespace std;

class ATM{
private:
    string data; // <- for storing information of user;
    void updateDatabase(string data){
    // <- for updating database after transactions;
        string line;
        ofstream outputData("newATM.txt");
        ifstream inputData("ATM.txt");
        while(getline(inputData, line)){
            if(line.find(data.substr(0,5)) != 0){
                outputData << line << endl;
            }
        }
        outputData << data << endl;
        outputData.close();
        inputData.close();
        remove("ATM.txt");
        rename("newATM.txt", "ATM.txt");
    }
    bool validateUser(string card){
    // <- validating card and user;
        bool found = false;
        ifstream inputData("ATM.txt");
        if (inputData.is_open()) {
            while (getline(inputData, data)){
                if(!data.find(card)){
                    found = true;
                    break;
                }
            }
            inputData.close();
        }else{
            cout << "Restart.\n";
        }
        if(found){
            int count = 0;
            while(count < 3){
                // <- max 3 trials to enter PIN;
                string pin;
                cout << "Enter PIN: "; cin >> pin;
                if(data.substr(5,4) != pin) cout << "Wrong PIN\n";
                else return true;
                count++;
            }
            return false;
        }else{
            cout << "Card is invalid or expired.\n";
            return false;
        }
        return false;
    }
    void withdraw(double amount){
        if(stod(data.substr(10)) < amount){
            cout << "Low balance.\n";
            return;
        }
        double remainingBalance = stod(data.substr(10)) - amount;
        data = data.substr(0,10);
        data += to_string(remainingBalance);
        cout << "Your balance: ₹" << balance() << "\n";
        cout << "Collect your cash.\n";
        updateDatabase(data);
        return;
    }
    void deposit(double amount){
        double remainingBalance = stod(data.substr(10)) + amount;
        data = data.substr(0,10);
        data += to_string(remainingBalance);
        cout << "Your balance: ₹" << balance() << endl;
        updateDatabase(data);
    }
    string currentBalance(){
        string money = data.substr(10);
        money = money.substr(0, money.find('.') + 3);// <- upto 2 digits for displaying;
        return money;
    }
    void changePin(){
        int count = 0;
        string newPin, newPinConfirm;
        cout << "The new PIN must be of 4 digits.\n";
        while(count < 3){
            count++;
            cout << "Enter the new PIN: "; cin >> newPin;
            cout << "Enter the PIN again to confirm: "; cin >> newPinConfirm;
            if(newPin.length() != 4){
                cout << "The new PIN must be of 4 digits.\n";
                continue;
            }
            if(newPin != newPinConfirm){
                cout << "Wrong PINs.\n";
            }else break;
        }
        if(count == 2){
            cout << "You cant change the PIN.\n";
            return;
        }
        string card = data.substr(0, 5);
        string money = data.substr(10);
        data = card + newPin + " " + money;
        updateDatabase(data);
    }
public:
    //calling private function using public functions;
    bool user(string card){
        return validateUser(card);
    }
    void cashWithdrawal(double amount){
        return withdraw(amount);
    }
    void cashDeposit(double amount){
        return deposit(amount);
    }
    void pinChange(){
        return changePin();
    }
    string balance(){
        return currentBalance();
    }

};

int main(){
    cout << "Insert your ATM card\n";
    string card;
    double amount;
    cout << "Enter last 4 digits of card: "; cin >> card;
    ATM obj;
    bool user = obj.user(card);
    if(!user){
        cout << "You cannot proceed. Kindly collect your card.\n";
        return 0;
    }
    cout << "\nFeatures: \n\t1)Cash Withdrawal\n\t2)Check Balance\n\t3)PIN Change\n\t4)Cash Deposit\n\t5)Exit\n";
    int choice;
    while(true){
        cout << "\nEnter your choice: "; cin >> choice;
        switch(choice){
            case 1:
                cout << "Enter amount: "; cin >> amount;
                obj.cashWithdrawal(amount);
                break;
            case 2:
                cout << "Your current balance: ₹" << obj.balance() << "\n";
                break;
            case 3:
                obj.pinChange();
                break;
            case 4:
                cout << "Enter amount: "; cin >> amount;
                obj.cashDeposit(amount);
                break;
            case 5:
                cout << "\nKindly collect your card.\nVisit again.\n\n";
                return 0;
            default:
                cout << "Wrong choice.\n";
        }
    }

    return 0;
}
