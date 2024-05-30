// Testing123.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <TheLibrary.h>

using std::cout, std::endl;

int main()
{
    cout << Add(1, 2) << endl;
    cout << "Window title: ";
    std::string title;
    std::getline(std::cin, title);
    auto hwnd = RunWindowBackground(title);
    cout << "Press any key to turn the window blue." << endl;
    std::cin.get();
    SetBackgroundColor(0, 64, 200);
    InvalidateRect(hwnd, NULL, TRUE);
    cout << "Press any key to close the window." << endl;
    std::cin.get();
    CloseWindow(hwnd);
    cout << "Done" << endl;
    std::cin.get();
}

extern "C" {
    __declspec(dllexport) std::string FunctionFromExe() {
        return "Hello from exe!";
    }
}
