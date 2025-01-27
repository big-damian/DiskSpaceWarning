// DiskSpaceWarning.cpp : Define el punto de entrada de la aplicación.
//

#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

void ShowMessageBox(const std::wstring& message, const std::wstring& title) {
    MessageBoxW(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONWARNING);
}

std::pair<double, std::vector<std::wstring>> ReadConfig(const std::string& configFilePath, double defaultThreshold, const std::wstring& defaultDrive) {
    std::ifstream configFile(configFilePath);
    double threshold = defaultThreshold;
    std::vector<std::wstring> drives;

    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("threshold=") == 0) {
                try {
                    threshold = std::stod(line.substr(10));
                }
                catch (...) {
                    threshold = defaultThreshold;
                }
            }
            else if (line.find("drive=") == 0) {
                std::string driveList = line.substr(6);
                std::stringstream ss(driveList);
                std::string drive;
                while (std::getline(ss, drive, ',')) {
                    drive.erase(std::remove(drive.begin(), drive.end(), ' '), drive.end()); // Remove any spaces
                    std::wstring wideDrive = std::wstring(drive.begin(), drive.end());
                    drives.push_back(wideDrive);
                }
            }
        }
        configFile.close();
    }
    else {
        // Create a default config file if it doesn't exist
        std::ofstream newConfigFile(configFilePath);
        if (newConfigFile.is_open()) {
            newConfigFile << "threshold=" << defaultThreshold << std::endl;
            newConfigFile << "drive=C,D" << std::endl;
            newConfigFile.close();
        }
        drives.push_back(defaultDrive);
    }

    return { threshold, drives };
}

void CheckDriveSpace(const std::vector<std::wstring>& drives, double threshold) {
    for (const auto& drive : drives) {
        std::wstring formattedDrive = drive + L":\\";
        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
        if (GetDiskFreeSpaceExW(formattedDrive.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
            double freeSpaceGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024 * 1024 * 1024);
            freeSpaceGB = std::round(freeSpaceGB * 100) / 100; // Round to 2 decimal places

            if (freeSpaceGB < threshold) {
                std::wstring message = L"Warning: Free disk space on " + drive +
                    L" is below " + std::to_wstring(threshold) + L" GB.\n" +
                    L"Current free space: " + std::to_wstring(freeSpaceGB) + L" GB.";
                ShowMessageBox(message, L"Disk Space Alert");
            }
        }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const std::string configFilePath = "config.ini";
    const double defaultThreshold = 50.0;
    const std::wstring defaultDrive = L"C";

    auto [threshold, drives] = ReadConfig(configFilePath, defaultThreshold, defaultDrive); // This line uses structured bindings

    // Debug output to check the parsed drives and threshold
    std::wcout << L"Threshold: " << threshold << L" GB" << std::endl;
    std::wcout << L"Drives: ";
    for (const auto& drive : drives) {
        std::wcout << drive << L" ";
    }
    std::wcout << std::endl;

    CheckDriveSpace(drives, threshold);

    return 0;
}
