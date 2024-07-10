#include <Windows.h>

bool running = true;

void* buffer_memory;
int buffer_width;
int buffer_height;
BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            buffer_width = rect.right - rect.left;
            buffer_height = rect.bottom - rect.top;
            //creating the buffer size
            int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);
            
            // if the buffer memory is already allocated but we resize the window, 
            // free the memory and reallocate the memory depending on the window size
            if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
            //allocating memory to the buffer, allowing to read and write to it.
            buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
            buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
            buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
            buffer_bitmap_info.bmiHeader.biPlanes = 1;
            buffer_bitmap_info.bmiHeader.biBitCount = 32;
            buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;


        }
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
    HDC hdc = GetDC(window);

    while (running) {
        //Input
        MSG message;
        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        //Simulate
        // we got black as our default color so now we iterate through every w/h pixel.
        // create a pixel pointer and then paint it to orange
        unsigned int* pixel = (unsigned int*)buffer_memory;
        for (int y = 0; y < buffer_height; y++) {
            for (int x = 0; x < buffer_width; x++) {
                *pixel++ = 0xff00ff*x + 0x00ff00*y;
            }
        }

        //Render
        StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, 
            buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }

}