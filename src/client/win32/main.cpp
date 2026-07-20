
#include <Windows.h>
#include <fstream>
#include <iostream>

#include "engine/framework/log/log.hpp"
#include "engine/graphics/d3d12/renderer.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int nCmdShow) {
  using namespace isoeng::log;

  std::ofstream outfile("rebound.log");
  if (!outfile.is_open()) {
    std::cerr << "Failed to open file for writing.\n";
    return 1;
  }

  [[maybe_unused]] const auto &outstream = Ostream::Init(&outfile);
  constexpr Log log(log_lvl<Trace::All>, string<"main">);

  log.info(isoeng::log::string<"Rebound game is here!">);

  const auto renderer = isoeng::graphics::d3d12::Renderer::Create();

  // Register the window class.
  const wchar_t CLASS_NAME[] = L"Sample Window Class";

  WNDCLASS wc = {};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);

  // Create the window.
  HWND hwnd = CreateWindowEx(0,                           // Optional window styles.
                             CLASS_NAME,                  // Window class
                             L"Learn to Program Windows", // Window text
                             WS_OVERLAPPEDWINDOW,         // Window style

                             // Size and position
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                             nullptr,   // Parent window
                             nullptr,   // Menu
                             hInstance, // Instance handle
                             nullptr    // Additional application data
  );

  if (hwnd == nullptr) {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // Run the message loop.

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // All painting occurs here, between BeginPaint and EndPaint.

    FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));

    EndPaint(hwnd, &ps);
    return 0;
  }

  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
