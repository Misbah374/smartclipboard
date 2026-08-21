#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <deque>
#include <windows.h>

using namespace std;

class HotKeyManager {
private:
    int copyHotKeyId = 1;
    int pasteHotKeyIds[6] = {2, 3, 4, 5, 6, 7};

public:
    void registerHotKey() {
        if (!RegisterHotKey(nullptr, copyHotKeyId,
                            MOD_CONTROL | MOD_SHIFT, 'C')) {
            cout << "Copy hotkey registration failed\n";
        }
        for (int i = 0; i < 6; i++) {
            if (!RegisterHotKey(nullptr, pasteHotKeyIds[i],
                                MOD_CONTROL | MOD_SHIFT, '1' + i)) {
                cout << "Paste hotkey " << i + 1
                    << " registration failed\n";
            }
        }
    }
    void unregisterHotKey(){
        UnregisterHotKey(nullptr, copyHotKeyId);          // window api ka function hai, use for copy
        for (int i = 0; i < 6; i++) {
            UnregisterHotKey(nullptr, pasteHotKeyIds[i]);          // use for paste
        }
    }
    int process(){
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_HOTKEY) {
                return static_cast<int>(msg.wParam);
            }
        }
        return -1;
    }
};

class ClipboardHistory {
private:
    static constexpr int MAX_SIZE = 6;
    deque<string> items;

public:
    void add(const string& item) {
        if (items.size() < MAX_SIZE)
            items.push_front(item);
        else {
            items.pop_back();
            items.push_front(item);
        }
    }
    string get(int index) {
        if(index <= 0 || index > items.size()) throw out_of_range("Invalid clipboard index");
        else{
            return items[items.size() - index];
        }
    }
    int size() {
        return items.size();
    }
    bool isEmpty(){
        return items.empty();
    }
};

class ClipboardManager {
public:
    string readText(){
        if (!OpenClipboard(nullptr)) return "";
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == nullptr) {
            CloseClipboard();
            return "";
        }
        char* text = static_cast<char*>(GlobalLock(hData));
        if (text == nullptr) {
            CloseClipboard();
            return "";
        }
        string result(text);
        GlobalUnlock(hData);
        CloseClipboard();
        return result;
        /*
            OpenClipboard() -> GetClipboardData() -> GlobalLock() -> data ko string mein copy -> GlobalUnlock() -> CloseClipboard() -> return string
        */
    }
    bool writeText(const string& text){
        if(!OpenClipboard(nullptr)) return false;
        if (!EmptyClipboard()) {
            CloseClipboard();
            return false;
        }  
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);          // Windows memory allocate karo
        if (hMem == nullptr) {
            CloseClipboard();
            return false;
        }
        char* buffer = static_cast<char*>(GlobalLock(hMem));          // memory ko access karo
        if (buffer == nullptr) {
            GlobalFree(hMem);
            CloseClipboard();
            return false;
        }
        memcpy(buffer, text.c_str(), text.size() + 1);          // Apni string Windows memory mein copy karo
        GlobalUnlock(hMem);
        if (SetClipboardData(CF_TEXT, hMem) == nullptr) {
            GlobalFree(hMem);
            CloseClipboard();
            return false;
        }
        CloseClipboard();
        return true;
    }
};

class SmartClipboard {
private:
    ClipboardHistory history;
    HotKeyManager hotKeyManager;
    ClipboardManager clipboardManager;

public:
    void run(){
        hotKeyManager.registerHotKey();
        while (handleHotKey()) {}
        hotKeyManager.unregisterHotKey();
    }
    bool handleHotKey(){
        int id = hotKeyManager.process();
        if (id == -1) return false;
        if (id == 1) {
            smartCopy();
        }
        else if (id >= 2 && id <= 7) {
            smartPaste(id - 1);
        }
        return true;
    }
    void smartCopy() {
        string text = clipboardManager.readText();
        if (!text.empty()) {
            history.add(text);
        }
    }
    void smartPaste(int index) {
        string text = history.get(index);
        clipboardManager.writeText(text);
    }
};

int main(){
    SmartClipboard app;
    app.run();
    return 0;
}