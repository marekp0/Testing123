#include "pch.h"
#include "TheLibrary.h"
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

typedef std::function<void(HWND hwnd)> HwndCallback;

static int InternalWinMain(const std::string& title, HwndCallback callback = nullptr);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int THELIBRARY_API Add(int a, int b)
{
    return a + b;
}

void THELIBRARY_API RunWindow(const std::string& title)
{
    InternalWinMain(title);
}

HWND THELIBRARY_API RunWindowBackground(const std::string& title)
{
    typedef std::string(*FunctionFromExeType)();
    FunctionFromExeType FunctionFromExe = (FunctionFromExeType)GetProcAddress(GetModuleHandle(NULL), "FunctionFromExe");
    if (FunctionFromExe) {
        std::string msg = FunctionFromExe();
        MessageBoxA(NULL, msg.c_str(), "Testing", MB_OK);
    }
    else {
        MessageBoxA(NULL, "FunctionFromExe not found", "Testing", MB_ICONERROR);
    }
    

    HWND ret;
    bool ret_set = false;
    std::mutex mtx;
    std::condition_variable cv;
    auto callback = [&](HWND hwnd) {
        std::unique_lock<std::mutex> lock(mtx);
        ret = hwnd;
        ret_set = true;
        cv.notify_all();
    };
    std::thread bg_thread([&title, &callback]() {
        InternalWinMain(title, callback);
    });
    bg_thread.detach();

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&ret_set]() { return ret_set; });

    return ret;
}

HBRUSH g_hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

void THELIBRARY_API SetBackgroundColor(int r, int g, int b)
{
    g_hbrBackground = CreateSolidBrush(RGB(r, g, b));
    // memory leak, but whatever
}

static int InternalWinMain(const std::string& title, HwndCallback callback)
{
    auto hInstance = GetModuleHandle(NULL);
    int nCmdShow = SW_SHOWDEFAULT;

    // convert title to std::wstring
    std::wstring wtitle(title.begin(), title.end());

    // Register the window class.
    const TCHAR CLASS_NAME[] = TEXT("Sample Window Class");

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        wtitle.c_str(),                 // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (callback) {
        callback(hwnd);
    }

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.
        FillRect(hdc, &ps.rcPaint, g_hbrBackground);
        EndPaint(hwnd, &ps);
    }
    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
