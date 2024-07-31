#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <unordered_map>
#include <thread> // Include the header for sleep_for
#include <chrono> // Include the header for chrono

using namespace std;

// Base class CreatepassandEncrypt
class CreatepassandEncrypt {
protected:
    string choose;
    string pass;

public:
    virtual void setChoice(string c1) {
        choose = c1;
    }

    virtual void getChoice() {
        cout << "You have chosen to " << choose << endl;
    }

    virtual string getPassword(string password) {
        pass = password;
        return pass;
    }

    virtual void displayPassword() {
        cout << "Password is: " << pass << endl;
    }
};

// Derived class Passwordgenerator, inherits from CreatepassandEncrypt
class Passwordgenerator : public CreatepassandEncrypt {
protected:
    string pass;
    string man;

public:
    string generateRandomPassword() {
        const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        srand(time(0));
        pass.clear();
        for (int i = 0; i < 8; ++i) {  // Ensure the password is at least 8 characters long
            pass += characters[rand() % characters.length()];
        }
        cout<<"Generating random password .";
        for (int i=0;i<3;i++){
                this_thread::sleep_for(chrono::seconds(1));
                cout<<".";
            }
        cout<<endl<< "Generated random password: " << pass << endl;
        return pass;
    }

    string generateManualPassword() {
        cout << "Enter your password: " << endl;
        cin >> man;
        pass = man;
        cout << "Your password is: " << pass << endl;
        return pass;
    }
};

// Class User, inherits from CreatepassandEncrypt
class User : public CreatepassandEncrypt {
protected:
    string username;
    string password;

public:
    User(const string& username, const string& password) : username(username), password(password) {}

    virtual void displayPassword() override {
        cout << "Password for " << username << ": " << password << endl;
    }
};

// Derived class Faculty, inherits from User
class Faculty : public User {
public:
    Faculty(const string& username, const string& password) : User(username, password) {}
};

// Derived class Admin, inherits from User
class Admin : public User {
private:
    unordered_map<string, string> passwordFile; // Store passwords

public:
    Admin(const string& username, const string& password) : User(username, password) {
        // Mock data: Populate password file
        passwordFile = {
            {"faculty1", "password1"},
            {"faculty2", "password2"},
            {"admin", "adminpassword"}
        };
    }

    void displayPassword(const string& username) {
        if (passwordFile.find(username) != passwordFile.end()) {
            cout << "Password for " << username << ": " << passwordFile[username] << endl;
        } 
        else {
            cout << "Access Denied: No password found for " << username << endl;
        }
    }

    void changePassword(const string& username, const string& newPassword) {
        passwordFile[username] = newPassword;
        cout << "Password changed successfully for " << username << endl;
    }

    bool isAdmin() const {
        return username == "admin";
    }

    void displayAllPasswords() {
        if (!isAdmin()) {
            cout << "Access Denied: Only admin can access all passwords." << endl;
            return;
        }

        cout << "All passwords in the file:" << endl;
        for (auto& entry : passwordFile) {
            cout << entry.first << ": " << entry.second << endl;
        }
    }
};

// PasswordEncryptor class for encrypting and decrypting passwords
class PasswordEncryptor {
private:
    string key;

public:
    PasswordEncryptor(string key) {
        this->key = key;
    }

    string encrypt(string password) {
        string encrypted = password;
        for (size_t i = 0; i < password.length(); ++i) {
            encrypted[i] ^= key[i % key.length()]; // XOR encryption
        }
        return encrypted;
    }

    string decrypt(string encryptedPassword) {
        string decrypted = encryptedPassword;
        for (size_t i = 0; i < encryptedPassword.length(); ++i) {
            decrypted[i] ^= key[i % key.length()]; // XOR decryption (same operation as encryption)
        }
        return decrypted;
    }
};

// Function to save password to file
void savePasswordToFile(const string& password) {
    ofstream outfile("sample.txt", ios::app);
    if (outfile.is_open()) {
        outfile << password << endl;
        outfile.close();
        cout << "Password saved to file 'sample.txt'." << endl;
    } else {
        cout << "Unable to open file for saving password." << endl;
    }
}


int main() {
    string option, password, en, key1, key2, genpass, encrypted, decrypted;
    Passwordgenerator g1;
    Admin admin("admin", "adminpassword");
    Faculty faculty1("faculty1", "password1");

    while (true) {
        cout << "Are you Admin or Faculty? Enter 'A' for Admin or 'F' for Faculty or for exiting program [e/E]: ";
        cin >> option;
        if(option=="e" ||option=="E" ||option=="exit" || option=="Exit"){
            cout<<" Thank you.";
            for (int i=0;i<3;i++){
                this_thread::sleep_for(chrono::seconds(1));
                cout<<".";
            }
            exit(0);
        }
        if (option == "A" || option == "a") {
            string adminPassword;
            cout << "Enter admin password: ";
            cin >> adminPassword;
            cout<<"Checking your password .";
            for (int i=0;i<3;i++){
                this_thread::sleep_for(chrono::seconds(1));
                cout<<".";
            }
            if (adminPassword == "adminpassword") {
                cout<<endl<< "Access granted as Admin." << endl;
                admin.displayAllPasswords();

                cout << "Do you want to change any password? Enter 'Y' for Yes or 'N' for No: ";
                cin >> option;
                if (option == "Y" || option == "y") {
                    string username, newPassword;
                    cout << "Enter username to change password: ";
                    cin >> username;
                    cout << "Enter new password: ";
                    cin >> newPassword;
                    admin.changePassword(username, newPassword);
                }
            } else {
                cout << "Access denied. Incorrect admin password." << endl;
            }
        } else if (option == "F" || option == "f") {
            while (true) {
                cout << "Enter your choice - generate a password manually[m/M] or randomly[r/R] or not to enter password again[b/B]: ";
                cin >> option;
                g1.setChoice(option);
                g1.getChoice();

                if (option == "Manually" || option == "M" || option == "manually" || option == "m") {
                    password = g1.generateManualPassword();
                    savePasswordToFile(password);
                    cout << "***************" << endl;
                } else if (option == "Random" || option == "R" || option == "random" || option == "r") {
                    password = g1.generateRandomPassword();
                    savePasswordToFile(password);
                    cout << "***************" << endl;
                } else if (option == "Break" || option == "break" || option == "b" || option == "B") {
                    cout << "***************" << endl;
                    break;
                }
            }

            cout << "Do you want to encrypt your Password? Enter 'Y' for Yes or 'N' for No: ";
            cin >> en;
            if (en == "Y" || en == "y") {
                cout << "Enter your secret key: ";
                cin >> key1;
                PasswordEncryptor encryptor(key1);
                genpass = g1.getPassword(password);
                encrypted = encryptor.encrypt(genpass);
                cout << "Your Encrypted Password: " << encrypted << endl;

                cout << "Enter the secret key for decryption of password: ";
                cin >> key2;
                cout<<"wait checking your secret keys .";
                for(int i=0;i<4;i++){
                    this_thread::sleep_for(chrono::seconds(1));
                    cout<<".";
                }
                if (key2 == key1) {
                    decrypted = encryptor.decrypt(encrypted);
                    cout<<endl << "Your Decrypted Password: " << decrypted << endl;
                } else {
                    cout << "Wrong Key" << endl;
                }
            }
        } else {
            cout << "Invalid option. Please enter 'A' for Admin or 'F' for Faculty." << endl;
        }
    }

    return 0;
}