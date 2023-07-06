#include <windows.h>
#include <cmath>
#include <string>
#include <stdexcept>
#include <iostream>


// Control const
#define ID_EDIT_S      1001
#define ID_EDIT_K      1002
#define ID_EDIT_R      1003
#define ID_EDIT_D      1004
#define ID_EDIT_T      1005
#define ID_EDIT_SIGMA  1006
#define ID_CALCULATE   1007

// Funct
double calculateCallOptionPrice(double S, double K, double r, double d, double T, double sigma);
double calculatePutOptionPrice(double S, double K, double r, double d, double T, double sigma);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Window
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"DerivativesCalculatorClass";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        L"DerivativesCalculatorClass",
        L"Option Call/Pull Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Msg
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// draw
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hEditS, hEditK, hEditR, hEditD, hEditT, hEditSigma, hButton;
    static HWND hResultLabel;

    switch (msg)
    {
    case WM_CREATE:
        // fields
        hEditS = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 10, 200, 20, hWnd, (HMENU)ID_EDIT_S, NULL, NULL);
        hEditK = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 40, 200, 20, hWnd, (HMENU)ID_EDIT_K, NULL, NULL);
        hEditR = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 70, 200, 20, hWnd, (HMENU)ID_EDIT_R, NULL, NULL);
        hEditD = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 100, 200, 20, hWnd, (HMENU)ID_EDIT_D, NULL, NULL);
        hEditT = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 130, 200, 20, hWnd, (HMENU)ID_EDIT_T, NULL, NULL);
        hEditSigma = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 160, 200, 20, hWnd, (HMENU)ID_EDIT_SIGMA, NULL, NULL);

        CreateWindow(L"STATIC", L"Current Price (€):", WS_CHILD | WS_VISIBLE, 10, 10, 150, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"Strike Price (€):", WS_CHILD | WS_VISIBLE, 10, 40, 150, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"Risk-free Rate (%):", WS_CHILD | WS_VISIBLE, 10, 70, 150, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"Dividend Yield (%):", WS_CHILD | WS_VISIBLE, 10, 100, 150, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"Time to Expiration (y):", WS_CHILD | WS_VISIBLE, 10, 130, 150, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"Volatility (%):", WS_CHILD | WS_VISIBLE, 10, 160, 150, 20, hWnd, NULL, NULL, NULL);

        //btn
        hButton = CreateWindow(L"BUTTON", L"Calculate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 190, 200, 30, hWnd, (HMENU)ID_CALCULATE, NULL, NULL);

        //result
        hResultLabel = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 230, 380, 80, hWnd, NULL, NULL, NULL);
        break;

        //convert from str2double
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_CALCULATE && HIWORD(wParam) == BN_CLICKED)
        {
            WCHAR buffer[256];
            std::wstring strS, strK, strR, strD, strT, strSigma;

            GetWindowText(hEditS, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strS = buffer;

            GetWindowText(hEditK, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strK = buffer;

            GetWindowText(hEditR, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strR = buffer;

            GetWindowText(hEditD, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strD = buffer;

            GetWindowText(hEditT, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strT = buffer;

            GetWindowText(hEditSigma, buffer, sizeof(buffer) / sizeof(buffer[0]));
            strSigma = buffer;

            try
            {
                double S = std::stod(strS);
                double K = std::stod(strK);
                double r = std::stod(strR) / 100.0;
                double d = std::stod(strD) / 100.0;
                double T = std::stod(strT);
                double sigma = std::stod(strSigma) / 100.0;

                double callPrice = calculateCallOptionPrice(S, K, r, d, T, sigma);
                double putPrice = calculatePutOptionPrice(S, K, r, d, T, sigma);

                std::wstring resultMessage = L"Call Price: " + std::to_wstring(callPrice) + L"\nPut Price: " + std::to_wstring(putPrice);
                SetWindowText(hResultLabel, resultMessage.c_str());
            }
            catch (const std::exception&)
            {
                SetWindowText(hResultLabel, L"Invalid input. Please enter valid values.");
            }
        }
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

// calc func
double calculateCallOptionPrice(double S, double K, double r, double d, double T, double sigma)
{
    double d1 = (log(S / K) + (r - d + (sigma * sigma / 2)) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    double N_d1 = 0.5 * (1 + erf(d1 / sqrt(2)));
    double N_d2 = 0.5 * (1 + erf(d2 / sqrt(2)));

    double callPrice = S * exp(-d * T) * N_d1 - K * exp(-r * T) * N_d2;
    return callPrice;
}

double calculatePutOptionPrice(double S, double K, double r, double d, double T, double sigma)
{
    double d1 = (log(S / K) + (r - d + (sigma * sigma / 2)) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    double N_minus_d1 = 0.5 * (1 - erf(d1 / sqrt(2)));
    double N_minus_d2 = 0.5 * (1 - erf(d2 / sqrt(2)));

    double putPrice = K * exp(-r * T) * N_minus_d2 - S * exp(-d * T) * N_minus_d1;
    return putPrice;
}

