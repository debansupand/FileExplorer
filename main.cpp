#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;
using namespace std;

// Function to list files and directories
void listFiles(const string& path) {
    cout << "\n📂 Listing files in: " << path << "\n";
    cout << "-------------------------------------------\n";
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_directory(entry.path()))
                cout << "[DIR]  " << entry.path().filename().string() << endl;
            else
                cout << "       " << entry.path().filename().string() << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Function to change directory
void changeDirectory(string& currentPath, const string& dir) {
    string newPath = (fs::path(currentPath) / dir).string();
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = fs::canonical(newPath).string();
        fs::current_path(currentPath);
    } else {
        cout << "❌ Directory not found.\n";
    }
}

// Function to create a file
void createFile(const string& filename) {
    ofstream file(filename);
    if (file)
        cout << "✅ File created: " << filename << endl;
    else
        cout << "❌ Failed to create file.\n";
}

// Function to delete a file
void deleteFile(const string& filename) {
    if (fs::exists(filename)) {
        fs::remove(filename);
        cout << "🗑️ File deleted: " << filename << endl;
    } else {
        cout << "❌ File not found.\n";
    }
}

// Function to copy a file
void copyFile(const string& src, const string& dest) {
    try {
        fs::copy(src, dest, fs::copy_options::overwrite_existing);
        cout << "📄 File copied from " << src << " → " << dest << endl;
    } catch (...) {
        cout << "❌ Failed to copy file.\n";
    }
}

// Function to move (rename) a file
void moveFile(const string& src, const string& dest) {
    try {
        fs::rename(src, dest);
        cout << "🚚 File moved from " << src << " → " << dest << endl;
    } catch (...) {
        cout << "❌ Failed to move file.\n";
    }
}

// Function to search for a file
void searchFile(const string& path, const string& filename) {
    bool found = false;
    cout << "🔍 Searching for: " << filename << " in " << path << "\n";
    for (auto& p : fs::recursive_directory_iterator(path)) {
        if (p.path().filename() == filename) {
            cout << "✅ Found: " << p.path() << endl;
            found = true;
        }
    }
    if (!found)
        cout << "❌ File not found.\n";
}

// Function to change file permissions
void changePermissions(const string& filename) {
    cout << "Enter permissions in octal (e.g., 755): ";
    int perms;
    cin >> oct >> perms;
    if (chmod(filename.c_str(), perms) == 0)
        cout << "✅ Permissions changed successfully.\n";
    else
        perror("chmod failed");
}

// Menu Display
void displayMenu() {
    cout << "\n=========== 🧭 FILE EXPLORER MENU ===========\n";
    cout << "1. List files\n";
    cout << "2. Change directory\n";
    cout << "3. Go back (..)\n";
    cout << "4. Create file\n";
    cout << "5. Delete file\n";
    cout << "6. Copy file\n";
    cout << "7. Move file\n";
    cout << "8. Search file\n";
    cout << "9. Change file permissions\n";
    cout << "0. Exit\n";
    cout << "============================================\n";
    cout << "Enter your choice: ";
}

int main() {
    string currentPath = fs::current_path().string();
    int choice;
    string filename, src, dest, dir;

    cout << "🚀 Welcome to the Linux File Explorer (C++) 🚀\n";

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                listFiles(currentPath);
                break;
            case 2:
                cout << "Enter directory name: ";
                getline(cin, dir);
                changeDirectory(currentPath, dir);
                break;
            case 3:
                changeDirectory(currentPath, "..");
                break;
            case 4:
                cout << "Enter filename to create: ";
                getline(cin, filename);
                createFile(filename);
                break;
            case 5:
                cout << "Enter filename to delete: ";
                getline(cin, filename);
                deleteFile(filename);
                break;
            case 6:
                cout << "Enter source file: ";
                getline(cin, src);
                cout << "Enter destination file: ";
                getline(cin, dest);
                copyFile(src, dest);
                break;
            case 7:
                cout << "Enter source file: ";
                getline(cin, src);
                cout << "Enter new location/name: ";
                getline(cin, dest);
                moveFile(src, dest);
                break;
            case 8:
                cout << "Enter filename to search: ";
                getline(cin, filename);
                searchFile(currentPath, filename);
                break;
            case 9:
                cout << "Enter filename to change permissions: ";
                getline(cin, filename);
                changePermissions(filename);
                break;
            case 0:
                cout << "👋 Exiting File Explorer. Goodbye!\n";
                return 0;
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }
    }
}

