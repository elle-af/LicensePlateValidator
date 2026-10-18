#include <windows.h>
#include <string>
#include <cctype>
using namespace std;

bool accepts(string x)
{
    bool valid = true;

    // Check Format A (ABC 1234)
    bool formatA = (
        x.length() == 8 &&
        isalpha((unsigned char)x[0]) &&
        isalpha((unsigned char)x[1]) &&
        isalpha((unsigned char)x[2]) &&
        x[3] == ' ' &&
        isdigit((unsigned char)x[4]) &&
        isdigit((unsigned char)x[5]) &&
        isdigit((unsigned char)x[6]) &&
        isdigit((unsigned char)x[7])
    );

    // Check Format B (123 ABC)
    bool formatB = (
        x.length() == 7 &&
        isdigit((unsigned char)x[0]) &&
        isdigit((unsigned char)x[1]) &&
        isdigit((unsigned char)x[2]) &&
        x[3] == ' ' &&
        isalpha((unsigned char)x[4]) &&
        isalpha((unsigned char)x[5]) &&
        isalpha((unsigned char)x[6])
    );

    // Must match either format
    if (!formatA && !formatB)
    {
        valid = false;
    }
        
    // Checks I, O, Q
    for (char c : x)
    {
        char upper = (char)toupper((unsigned char)c);

        if (upper == 'I' || upper == 'O' || upper == 'Q')
        {
            valid = false;
        }
    }

    // number cannot start with 000
    if (x.length() >= 3 &&
        x[0] == '0' &&
        x[1] == '0' &&
        x[2] == '0')
    {
        valid = false;
    }

    // number cannot start with 000
    if (x.length() >= 3 &&
        x[0] == '0' &&
        x[1] == '0' &&
        x[2] == '0')
    {
        valid = false;
    }

    // Check first and last alphanumeric characters
    char first = '\0';
    char last = '\0';

    for (char c : x)
    {
        if (isalnum((unsigned char)c))
        {
            if (first == '\0')
            {
                first = c;
            }

            last = c;
        }
    }

    string vowels = "AEIOUaeiou";
        
    bool firstVowel = vowels.find(first) != string::npos;
    bool lastVowel = vowels.find(last) != string::npos;

    if (firstVowel && lastVowel)
    {
        valid = false;
    }

    return valid;
}

// UI
enum { ID_EDIT = 101 };

static HWND  g_edit, g_title, g_result, g_hint;
static HFONT g_fTitle, g_fEdit, g_fResult, g_fHint;
static HBRUSH g_bg;
static COLORREF g_resultColor = RGB(140, 140, 140);

static const COLORREF C_TEXT  = RGB(33, 33, 33);
static const COLORREF C_MUTED = RGB(140, 140, 140);
static const COLORREF C_OK    = RGB(22, 163, 74);
static const COLORREF C_BAD   = RGB(220, 38, 38);

static HFONT MakeFont(int px, int weight, const char* face)
{
    return CreateFontA(-px, 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                       DEFAULT_PITCH | FF_DONTCARE, face);
}

static HWND MakeStatic(HWND parent, const char* text, int x, int y, int w, int h, HFONT f)
{
    HWND s = CreateWindowExA(0, "STATIC", text, WS_CHILD | WS_VISIBLE | SS_CENTER,
                             x, y, w, h, parent, nullptr, nullptr, nullptr);
    SendMessageA(s, WM_SETFONT, (WPARAM)f, TRUE);
    return s;
}

static void UpdateResult()
{
    char buf[64] = {0};
    GetWindowTextA(g_edit, buf, sizeof(buf));
    string plate = buf;

    if (plate.empty())
    {
        g_resultColor = C_MUTED;
        SetWindowTextA(g_result, "Enter a plate number");
    }
    else if (accepts(plate))
    {
        g_resultColor = C_OK;
        SetWindowTextA(g_result, "Valid");
    }
    else
    {
        g_resultColor = C_BAD;
        SetWindowTextA(g_result, "Invalid");
    }
    InvalidateRect(g_result, nullptr, TRUE);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_bg      = CreateSolidBrush(RGB(255, 255, 255));
        g_fTitle  = MakeFont(20, FW_SEMIBOLD, "Segoe UI");
        g_fEdit   = MakeFont(26, FW_NORMAL,   "Consolas");
        g_fResult = MakeFont(18, FW_SEMIBOLD, "Segoe UI");
        g_fHint   = MakeFont(13, FW_NORMAL,   "Segoe UI");

        g_title = MakeStatic(hwnd, "License Plate Validator", 0, 28, 360, 30, g_fTitle);

        g_edit = CreateWindowExA(0, "EDIT", "",
                                 WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE | ES_AUTOHSCROLL,
                                 40, 80, 280, 46, hwnd, (HMENU)ID_EDIT, nullptr, nullptr);
        SendMessageA(g_edit, WM_SETFONT, (WPARAM)g_fEdit, TRUE);
        SendMessageA(g_edit, EM_SETLIMITTEXT, 12, 0);
        // vertically nudge text inside the box
        SendMessageA(g_edit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(8, 8));

        g_result = MakeStatic(hwnd, "", 0, 146, 360, 28, g_fResult);
        g_hint   = MakeStatic(hwnd, "Formats:  ABC 1234   or   123 ABC", 0, 196, 360, 20, g_fHint);

        UpdateResult();
        return 0;
    }

    case WM_SETFOCUS:
        SetFocus(g_edit);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wp) == ID_EDIT && HIWORD(wp) == EN_CHANGE)
            UpdateResult();
        return 0;

    case WM_CTLCOLORSTATIC:
    {
        HDC dc = (HDC)wp;
        HWND ctl = (HWND)lp;
        SetBkColor(dc, RGB(255, 255, 255));
        if (ctl == g_result)      SetTextColor(dc, g_resultColor);
        else if (ctl == g_hint)   SetTextColor(dc, C_MUTED);
        else                      SetTextColor(dc, C_TEXT);
        return (LRESULT)g_bg;
    }

    case WM_ERASEBKGND:
    {
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wp, &rc, g_bg);
        return 1;
    }

    case WM_DESTROY:
        DeleteObject(g_fTitle); DeleteObject(g_fEdit);
        DeleteObject(g_fResult); DeleteObject(g_fHint);
        DeleteObject(g_bg);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{
    WNDCLASSA wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszClassName = "PlateValidatorWnd";
    RegisterClassA(&wc);

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT r = {0, 0, 360, 240};
    AdjustWindowRect(&r, style, FALSE);
    int w = r.right - r.left, h = r.bottom - r.top;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    HWND hwnd = CreateWindowExA(0, wc.lpszClassName, "License Plate Validator", style,
                                x, y, w, h, nullptr, nullptr, hInst, nullptr);
    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);
    SetFocus(g_edit);

    MSG m;
    while (GetMessageA(&m, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&m);
        DispatchMessageA(&m);
    }
    return (int)m.wParam;
}
