#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <fstream>
#include <cstdio>

using namespace std;

class Crypto {
    string str;
    string cipherStr;
    int key;
public:
    void setStr(const string& str) {
        this->str = str;
    }

    void setCipherStr(const string& cipherStr) {
        this->cipherStr = cipherStr;
    }

    void setKey(int key) {
        this->key = key;
    }

    int getKey() { return key; }

    string getStr() {
        return str;
    }

    string getCipherStr() {
        return cipherStr;
    }

    string encrypt(const string& text, int key) {
        string cypherText = "";
        for (char c : text) {
            if (isalpha(c)) {
                char offset = isupper(c) ? 'A' : 'a';
                c = (c - offset + key) % 26 + offset;
            }
            cypherText += c;
        }
        return cypherText;
    }

    string decrypt(const string& text, int key) {
        return encrypt(text, 26 - key);
    }

    void bruteForce(const string& cypherText) {
        cout << "## Brute Force Attack ##" << endl;
        for (int i = 1; i < 26; i++) {
            cout << "Key: " << i << "  " << decrypt(cypherText, i) << endl;
        }
    }

    string freqAnalysis(const string& cipherStr) {
        map<char, int> freq;
        for (char c : cipherStr) {
            if (isalpha(c)) {
                freq[tolower(c)]++;
            }
        }

        vector<pair<char, int>> sortFrequency(freq.begin(), freq.end());
        sort(sortFrequency.begin(), sortFrequency.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
            return a.second > b.second;
        });

        string frequencyOrder = "etaoinshrdlcumwfgypbvkjxqz";

        for (char frequentChar : frequencyOrder) {
            for (pair<char, int> p : sortFrequency) {
                char mostFrequentChar = p.first;
                int predictedKey = (mostFrequentChar - frequentChar + 26) % 26;
                return decrypt(cipherStr, predictedKey);
            }
        }
        return "";
    }
} C;

class File {
    string inputFileName;
    string outputFileName;
public:
    File(const string& inputFileName, const string& outputFileName) {
        this->inputFileName = inputFileName;
        this->outputFileName = outputFileName;
        createInputFile();
        createOutputFile();
    }

    void createFile(const string& fileName) {
        ofstream onFile(fileName);
        if (onFile.is_open()) {
            cout << "File " << fileName << " created successfully.\n";
            onFile.close();
        } else {
            cout << "Error creating file " << fileName << ".\n";
        }
    }

    void createInputFile() {
        createFile(inputFileName);
    }

    void createOutputFile() {
        createFile(outputFileName);
    }

    void deleteFile() {
        if (remove(inputFileName.c_str()) == 0 && remove(outputFileName.c_str()) == 0) {
            cout << "Files " << inputFileName << " and " << outputFileName << " deleted successfully.\n";
        } else {
            cout << "Error deleting files.\n";
        }
    }

    string readFile(const string& fileName) {
        ifstream inFile(fileName);
        string content;
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                content += line + "\n";
            }
            inFile.close();
            cout << "File content read successfully!..." << endl;
        } else {
            cout << "! Unable to read file " << fileName << endl;
        }
        return content;
    }

    void writeFile(const string& fileName, const string& content) {
        ofstream outFile(fileName);
        if (outFile.is_open()) {
            outFile << content;
            outFile.close();
            cout << "! File written successfully..." << endl;
        } else {
            cout << "Unable to open file!..." << fileName << endl;
        }
    }
};

int main() {
    string text, cipherText, plainText;
    int ch, key;
    File fileHandler("input.txt", "output.txt");

    do {
        cout << "\n1. Encrypt\n2. Decrypt\n3. Brute Force Attack\n4. Set text\n5. Set ciphertext\n6. Frequency Analysis\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> ch;
        cin.ignore();  // To ignore the newline left by cin

        switch (ch) {
            case 1:
                cout << "Enter the text to encrypt: ";
                getline(cin, text);
                cout << "Enter the key: ";
                cin >> key;
                cin.ignore(); // To ignore the newline left by cin
                cipherText = C.encrypt(text, key);
                cout << "Encrypted Cypher Text Is: " << cipherText << endl;
                fileHandler.writeFile("output.txt", cipherText);
                break;
            case 2:
                cout << "Enter the ciphertext to decrypt: ";
                getline(cin, cipherText);
                cout << "Enter the key: ";
                cin >> key;
                cin.ignore(); // To ignore the newline left by cin
                text = C.decrypt(cipherText, key);
                cout << "Decrypted text: " << text << endl;
                fileHandler.writeFile("output.txt", text);
                break;
            case 3:
                cout << "Enter the ciphertext for brute force: ";
                getline(cin, cipherText);
                C.bruteForce(cipherText);
                break;
            case 4:
                cout << "Enter the text: ";
                getline(cin, text);
                cout << "Enter the key: ";
                cin >> key;
                C.setStr(text);
                C.setKey(key);
                break;
            case 5:
                cout << "Enter the ciphertext: ";
                getline(cin, cipherText);
                cout << "Enter the key: ";
                cin >> key;
                C.setCipherStr(cipherText);
                C.setKey(key);
                break;
            case 6:
                cout << "Reading cipher text from input file for frequency analysis..." << endl;
                cipherText = fileHandler.readFile("input.txt");
                plainText = C.freqAnalysis(cipherText);
                fileHandler.writeFile("output.txt", plainText) ;
                cout << "Frequency analysis written to output file." << endl;
                break;
            case 0:
                fileHandler.deleteFile();
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice, please try again.\n";
                break;
        }
    } while (ch != 0);

    return 0;
}
