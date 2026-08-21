# SmartClipboard

A lightweight Windows clipboard manager built in C++ that maintains a history of up to 6 copied text items and allows quick access using global hotkeys.

## Features

- Maintains a history of up to 6 clipboard items.
- Automatically removes the oldest item when the history is full.
- Uses global hotkeys to interact with the clipboard.
- Allows restoring any stored clipboard item to the system clipboard.
- Uses Windows API for hotkey registration and clipboard operations.
- Built using Object-Oriented Programming principles.

## Hotkeys

| Shortcut | Action |
|----------|--------|
| `Ctrl + Shift + C` | Save current clipboard text to history |
| `Ctrl + Shift + 1` | Restore clipboard item 1 |
| `Ctrl + Shift + 2` | Restore clipboard item 2 |
| `Ctrl + Shift + 3` | Restore clipboard item 3 |
| `Ctrl + Shift + 4` | Restore clipboard item 4 |
| `Ctrl + Shift + 5` | Restore clipboard item 5 |
| `Ctrl + Shift + 6` | Restore clipboard item 6 |

After restoring an item, the normal `Ctrl + V` can be used to paste it.

## Architecture

SmartClipboard follows an object-oriented design with separate responsibilities for different components:

```text
                    SmartClipboard
                          |
          +---------------+---------------+
          |               |               |
          v               v               v
 ClipboardHistory    HotKeyManager   ClipboardManager
          |               |               |
        deque        Windows API      Windows API
```

## Classes
1. HotKeyManager
- Responsible for registering, unregistering, and processing global keyboard shortcuts using the Windows API.
2. ClipboardHistory
- Maintains a maximum of 6 clipboard items using std::deque.
3. ClipboardManager
- Handles reading from and writing to the Windows system clipboard.
4. SmartClipboard
- Acts as the main controller and coordinates the other components.

## How It Works

1. The user copies text normally using `Ctrl + C`.
2. Press `Ctrl + Shift + C` to store the current clipboard text in the application's history.
3. The history maintains up to 6 clipboard items.
4. Press `Ctrl + Shift + 1` to `Ctrl + Shift + 6` to select a stored item.
5. The selected item is restored to the Windows system clipboard.
6. The user can paste the restored item normally using `Ctrl + V`.

## Build and Run

### Requirements

- Windows
- C++ compiler such as MinGW/G++

### Compile

```bash
g++ main.cpp -o SmartClipboard
```

### Run
```bash
.\SmartClipboard.exe
```
