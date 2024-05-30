#pragma once

#include "framework.h"
#include <string>

#ifdef THELIBRARY_EXPORTS
#define THELIBRARY_API __declspec(dllexport)
#else
#define THELIBRARY_API __declspec(dllimport)
#endif

/**
 * Adds two integers.
 * 
 * @param a First number
 * @param b Second number
 * @return Sum of a and b
 */
int THELIBRARY_API Add(int a, int b);

/**
 * Runs a window, blocking until the window is closed.
 * 
 * @param title Title of the window
 */
void THELIBRARY_API RunWindow(const std::string& title);

HWND THELIBRARY_API RunWindowBackground(const std::string& title);

void THELIBRARY_API SetBackgroundColor(int r, int g, int b);
