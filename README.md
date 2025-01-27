# Disk Space Warning

**Disk Space Monitor** is a small silent and standalone application designed to be run with task scheduler that checks the available disk space on a specified drive. If the available space falls below a user-defined threshold, the app displays a warning message. The threshold can be configured via a simple `config.ini` file.

---

## Features

- The app alerts free disk space on a specified drive (default: `C:`).
- Displays an alert if the available space falls below the configured threshold.
- Automatically creates a configuration file (`config.ini`) if it does not exist.
- Runs silently in the background without opening window (except for the message window when needed).

---

## Configuration

The application uses a configuration file named `config.ini` located in the same directory as the executable. The configuration file contains a single parameter:

```ini
threshold=20
