// DiskSpaceWarning.cpp : Define el punto de entrada de la aplicación.
//

#include <windows.h>
#include <fstream>
#include <cmath>
#include <string>

void ShowMessageBox(const std::wstring& message, const std::wstring& title) {
    MessageBoxW(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONWARNING);
}

double ReadThresholdFromConfig(const std::string& configFilePath, double defaultThreshold) {
    std::ifstream configFile(configFilePath);
    double threshold = defaultThreshold;

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
                break;
            }
        }
        configFile.close();
    }
    else {
        // Create the file with the default threshold if it doesn't exist
        std::ofstream newConfigFile(configFilePath);
        if (newConfigFile.is_open()) {
            newConfigFile << "threshold=" << defaultThreshold << std::endl;
            newConfigFile.close();
        }
    }

    return threshold;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t* drive = L"C:";
    const std::string configFilePath = "config.ini";
    const double defaultThreshold = 20.0;

    double threshold = ReadThresholdFromConfig(configFilePath, defaultThreshold);

    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    if (GetDiskFreeSpaceExW(drive, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        double freeSpaceGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024 * 1024 * 1024);
        freeSpaceGB = std::round(freeSpaceGB * 100) / 100; // Round to 2 decimal places

        if (freeSpaceGB < threshold) {
            std::wstring message = L"Warning: Free disk space on " + std::wstring(drive) +
                L" is below " + std::to_wstring(threshold) + L" GB.\n" +
                L"Current free space: " + std::to_wstring(freeSpaceGB) + L" GB.";
            ShowMessageBox(message, L"Disk Space Alert");
        }
    }

    return 0;
}