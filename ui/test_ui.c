#include <ui.h>
#include <ui_window.h>
#include <ui_button.h>

#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#define STR(str) (str), (sizeof((str)) - 1)

void
on_button_clicked_event(UiButton* button, void* extra_data) {
    (void)button;
    (void)extra_data;

    puts("clicked");
    ui_button_destroy(&button);
}

void
on_activate_handler(Ui* self) {
    puts("activated");

    UiWindow* window = ui_window_create(self, STR("This is a title"), 800, 600);
    // (void)window;
    // ui_window_show(window, true);
    // ui_child_add(self, window);

    UiButton* button = ui_button_create(window, STR("button"));
    ui_button_event_clicked(button, on_button_clicked_event, NULL);
    // ui_window_child_add(window, button);

    // ui_window_show(window, true);
}

int main() {
    // if you register class if you are using vscode on flatpak,
    // it will crash as vscode doesn't have a permission to register to dbus
    // Ui* ui = ui_create(NULL, 0);
    Ui* ui = ui_create(STR("com.windows.ui"));
    // UiWindow* window = ui_window_create(ui, STR("This is a title"), 800, 600);
    // (void)window;

    ui_mainloop(ui, on_activate_handler);
    ui_destroy(&ui);
}


// #include <stdio.h>
// #include <string.h>
// #include <stddef.h>
// #include <assert.h>

// #include <windows.h>
// #include <windowsx.h>

// static LRESULT __stdcall windowProcedure(HWND, UINT, WPARAM, LPARAM);

// void
// create_app() {
//     //---- Get WinMain Parameters ----//

//     HINSTANCE hInstance = GetModuleHandle(NULL);
//     STARTUPINFO si;
//     GetStartupInfo(&si);  
//     int nCmdShow = si.wShowWindow;

//     // -------------------------//
//     OutputDebugString("Starting WinMain Application");
//     puts("Starting WinMain Application");
    
//     //Window class name must be unique 
//     const char wincClassName [] = "NameOfWindow";
    
//     // Win32 Window class structure
//     // WNDCLASSEX wc = {0};
//     WNDCLASSEX* wc = calloc(1, sizeof(WNDCLASSEX));
//     // Win32 message structure 
//     MSG Msg;        	
    
//     wc->lpszClassName = wincClassName;
//     wc->lpfnWndProc = &windowProcedure;
//     wc->cbSize = sizeof(WNDCLASSEX);
//     // wc->style  = 0;
//     // wc->cbClsExtra = 0;
//     // wc->cbWndExtra = 0;
//     wc->hInstance = hInstance;
//     // wc->hIcon = LoadIcon(NULL, IDI_APPLICATION);
//     wc->hCursor = LoadCursor(NULL, IDC_ARROW);
//     wc->hbrBackground = GetSysColorBrush(COLOR_WINDOW);
//     // wc->lpszMenuName = NULL;	
//     // wc->hIconSm = LoadIcon(NULL, IDI_APPLICATION);  

//     if(!RegisterClassEx(wc)) {
//         MessageBox(NULL,
//                    "Window Registration Failed!",
//                    "Error!",
//                    MB_ICONEXCLAMATION | MB_OK);
//         //Error status code 
//         return -1;
//     }

//     puts("Class Registered");
//     int width = 500, height = 400;
//     int pos_left = 400, pos_top = 100;

//     HWND hwnd = CreateWindowA(
//         wc->lpszClassName,
//         "Title of Window",
//         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//         pos_left,
//         pos_top,
//         width,
//         height,
//         NULL,
//         NULL,
//         hInstance,
//         NULL
//         );
    
//     const size_t buttonID = 1001;
    
//     OutputDebugString(" [INFO] Window created OK");
//     if(hwnd == NULL){
//         MessageBox(NULL,
//                    "Error: Failure to create Window",
//                    "Error Report",
//                    MB_ICONEXCLAMATION | MB_OK);
//         return -1;
//     }
//     ShowWindow(hwnd, nCmdShow);
//     UpdateWindow(hwnd);

//     //---- Message Loop ----------//
//     while(GetMessage(&Msg, NULL, 0, 0) > 0 ){
//         TranslateMessage(&Msg);
//         DispatchMessage(&Msg);
//     }
// }

// int main() {
//     create_app();

//     // Success status code 
//     return 0;
// }
// // ------------ End of Main ------------------------- //


// // Window Procedure - Process window messages or events 
// LRESULT __stdcall windowProcedure (
//      HWND   hwnd    // Window Handle (Window object)
//     ,UINT   msg     // Window Message  
//     ,WPARAM wParam  // Additional message information
//     ,LPARAM lParam  // Additional message information
// ) {

//     // Process messages 
//     switch(msg)
//     {
//     case WM_CREATE:
//         LONG units = GetDialogBaseUnits();
//         assert(CreateWindow( 
//             "BUTTON",  // Predefined class; Unicode assumed 
//             "OK",      // Button text 
//             // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
//             BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
//             CW_USEDEFAULT,         // x position 
//             CW_USEDEFAULT,         // y position 
//             MulDiv(LOWORD(units), 50, 4),        // Button width with default size
//             MulDiv(HIWORD(units), 14, 8),        // Button height with default size
//             hwnd,     // Parent window
//             NULL,       // No menu.
//             (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
//             // NULL,
//             NULL // Pointer not needed.
//         ));

//         assert(CreateWindow( 
//             "EDIT",  // Predefined class; Unicode assumed 
//             NULL,      // Button text 
//             // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
//             WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
//             100,         // x position 
//             100,         // y position 
//             MulDiv(LOWORD(units), 50, 4),         // Button width with default size
//             MulDiv(HIWORD(units), 14, 8),         // Button height with default size
//             hwnd,      // Parent window
//             NULL,       // No menu.
//             (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
//             // NULL,
//             NULL // Pointer not needed.
//         ));

//         HWND combobox = CreateWindow( 
//             "COMBOBOX",  // Predefined class; Unicode assumed 
//             NULL,      // Button text 
//             // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
//             CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
//             0,         // x position 
//             200,         // y position 
//             MulDiv(LOWORD(units), 50, 4),         // Button width with default size
//             MulDiv(HIWORD(units), 14, 8),         // Button height with default size
//             hwnd,      // Parent window
//             NULL,       // No menu.
//             (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
//             // NULL,
//             NULL // Pointer not needed.
//         );

//         TCHAR Planets[9][10] =  
//         {
//             TEXT("Mercury"), TEXT("Venus"), TEXT("Terra"), TEXT("Mars"), 
//             TEXT("Jupiter"), TEXT("Saturn"), TEXT("Uranus"), TEXT("Neptune"), 
//             TEXT("Pluto??") 
//         };
            
//         TCHAR A[16]; 
//         int  k = 0; 

//         memset(&A,0,sizeof(A));       
//         for (k = 0; k <= 8; k += 1)
//         {
//             strcpy_s(A, sizeof(A)/sizeof(TCHAR),  (TCHAR*)Planets[k]);

//             // Add string to combobox.
//             SendMessage(combobox,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
//         }
//         ComboBox_SetCurSel(combobox, 0);
//         break;
//     case WM_CLOSE:		
//         DestroyWindow(hwnd);
//         break;
//     case WM_DESTROY:
//         PostQuitMessage(0);
//         break;
//     case WM_MOVE:
//         break;
//     case WM_COMMAND:
//         char buf[2000];

//         switch(HIWORD(wParam)) {
//             case BN_CLICKED:
//                 puts("clicked");
//                 break;
//             case EN_UPDATE:
//                 Edit_GetText((HWND)lParam, buf, 2000);
//                 puts(buf);
//                 break;
//             case CBN_SELCHANGE:
//                 ComboBox_GetText((HWND)lParam, buf, 2000);
//                 puts(buf);
//                 break;
//             default: 
//                 return DefWindowProc(hwnd, msg, wParam, lParam);
//         }
//         break;
//     case WM_PAINT:
//     // {
//     // 	// GDI - Graphics Devices Interface Here
//     // 	//--------------------------------------------
//     // 	PAINTSTRUCT ps;
//     // 	HDC hdc;
//     // 	// std::cerr << " [INFO] Windown painting" << std::endl;
//     // 	hdc = BeginPaint(hwnd, &ps);
//     // 	const char* text = "Hello world Window!";		
//     // 	TextOutA(hdc, 125, 200, text, sizeof(text) - 1);
//     // 	Ellipse(hdc, 100, 100, 160, 160);
//     // 	Rectangle(hdc, 100, 100, 160, 160);
//     // 	EndPaint(hwnd, &ps);	
//     // }
//     break;
//     default:
//         return DefWindowProc(hwnd, msg, wParam, lParam);
//     }

//     return 0;
// }