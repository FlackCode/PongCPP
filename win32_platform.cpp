#include "utils.cpp"
#include <Windows.h>

global_variable bool running = true;

//cleanup time

struct Render_State {
    int height, width;
    void* memory;

    BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "platform_common.cpp"
#include "renderer.cpp"

//void* memory;
//int width;
//int height;
//BITMAPINFO bitmap_info;

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
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            //creating the buffer size
            int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);
            
            // if the buffer memory is already allocated but we resize the window, 
            // free the memory and reallocate the memory depending on the window size
            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            //allocating memory to the buffer, allowing to read and write to it.
            render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;


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

    Input input = {};

    while (running) {
        //Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

                    switch (vk_code) {
                        case VK_UP: {
                            input.buttons[BUTTON_UP].is_down = is_down;
                            input.buttons[BUTTON_UP].changed = true;
                        } break;
                    }
                } break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }
                
            }
        }
        //Simulate
        

        clear_screen(0xffffff);
        if(input.buttons[BUTTON_UP].is_down)
            draw_rect(0, 0, 10, 10, 0xff0000);
        draw_rect(30, 30, 5, 5, 0xff0000);
        draw_rect(-20, 20, 8, 3, 0xff0000);

        //Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, 
            render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }

}