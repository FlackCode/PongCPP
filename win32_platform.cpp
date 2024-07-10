#include <Windows.h>

bool running = true;

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Window Class
    WNDCLASSA windowClass = {};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpszClassName = "Game Window Class";
    windowClass.lpfnWndProc = windowCallback;

    // Register
    // Pointer points to the location of a certain thing in order for it to 
    RegisterClassA(&windowClass);

    // Create window
    HWND window = CreateWindowA(
        windowClass.lpszClassName,
        "My First Game",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        0, 0,
        hInstance,
        0);

    while (running) {
        //Input
        MSG message;
        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        //Simulate

        //Render
    }

}