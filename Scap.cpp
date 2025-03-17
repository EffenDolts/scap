/** @author Mental Blok */

// See:
// https://cplusplus.com/forum/windows/105644/

#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "Scap.h"

const int NB = 4;
const int BC = 32;
using namespace std;


int Scap::clickMouse(float targetX, float targetY)
{
  POINT cursorPos;
  float returnX = 1000, returnY = 500;

  // Calculate absolute coordinates (0 to 65535)
  // Example: Move mouse to (100, 200)
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);
  targetX /= screenWidth;
  targetY /= screenHeight;

  // Get the cursor position
  if (GetCursorPos(&cursorPos)) {
    returnX = cursorPos.x;
    returnY = cursorPos.y;
    if (0) cout << "X: " << returnX << ", Y: " << returnY << endl;
  }
  returnX /= screenWidth;
  returnY /= screenHeight;
  if (targetX < 0 || targetX >= 1.0 || targetY < 0 || targetY >= 1.0) {
    cout << "Range Target X: " << targetX << ", Y: " << targetY << endl;
    exit(0);
  }
  if (returnX < 0 || returnX >= 1.0 || returnY < 0 || returnY >= 1.0) {
    cout << "Range Return X: " << returnX << ", Y: " << returnY << endl;
    exit(0);
  }
#if 0
  INPUT inputs[4] = {};
  ZeroMemory(inputs, sizeof(inputs));

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  inputs[0].mi.dx = (targetX * 65535) / screenWidth;
  inputs[0].mi.dy = (targetY * 65535) / screenHeight;

  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  inputs[2].type = INPUT_MOUSE;
  inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  inputs[3].type = INPUT_MOUSE;
  inputs[3].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  inputs[3].mi.dx = (returnX * 65535) / screenWidth;
  inputs[3].mi.dy = (returnY * 65535) / screenHeight;

  return SendInput(4, inputs, sizeof(INPUT));
#else 
  INPUT input;
  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  input.mi.dx = targetX * 65535;
  input.mi.dy = targetY * 65535;
  SendInput(1, &input, sizeof(INPUT));

  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &input, sizeof(INPUT));

  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, &input, sizeof(INPUT));

  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  input.mi.dx = returnX * 65535;
  input.mi.dy = returnY * 65535;
  return SendInput(1, &input, sizeof(INPUT));
#endif
}

Scap::Scap(int x1, int y1, int x2, int y2)
    : m_x1(x1) , m_y1(y1) , m_x2(x2) , m_y2(y2)
{
  HWND DesktopHwnd = GetDesktopWindow();
  RECT DesktopParams;
  GetWindowRect(DesktopHwnd, &DesktopParams);
  m_width = m_x2 - m_x1;
  m_height = m_y2 - m_y1;
  m_imgSize = m_width * m_height * NB;
  m_bfSize = sizeof(BITMAPFILEHEADER);
  m_biSize = + sizeof(BITMAPINFOHEADER);
  m_fileSize = m_bfSize + m_biSize  + m_imgSize;
  m_bmpData = (char*)GlobalAlloc(0x0040, m_fileSize);
  m_image = (RGBQUAD*)&m_bmpData[m_bfSize + m_biSize];
  m_buf = (RGBQUAD *)(char*)GlobalAlloc(0x0040, m_imgSize);
 
  // clickMouse(1700, 900); exit(0);
}

Scap::~Scap()
{
  if (m_bmpData) GlobalFree(m_bmpData); 
  if (m_buf) GlobalFree(m_buf); 
}

RGBQUAD * Scap::ptr(int x, int y)
{
  return (RGBQUAD*)&m_image[x + (y * m_width)];
}

void Scap::buf(int x, int y, uint32_t v)
{
  uint32_t * p = (uint32_t*)&m_buf[x + (y * m_width)];
  *p = v;
}

uint32_t Scap::uval(int x, int y)
{
  uint32_t * p = (uint32_t*)&m_image[x + (y * m_width)];
  return *p;
}

RGBQUAD Scap::v(int x, int y)
{
  RGBQUAD * p = (RGBQUAD*)&m_image[x + (y * m_width)];
  return *p;
}

BYTE Scap::r(int x, int y)
{
  RGBQUAD * p = (RGBQUAD*)&m_image[x + (y * m_width)];
  return (*p).rgbRed;
}

BYTE Scap::g(int x, int y)
{
  RGBQUAD * p = (RGBQUAD*)&m_image[x + (y * m_width)];
  return (*p).rgbGreen;
}

BYTE Scap::b(int x, int y)
{
  RGBQUAD * p = (RGBQUAD*)&m_image[x + (y * m_width)];
  return (*p).rgbBlue;
}

void Scap::print()
{
  cout << m_x1 << ", " << m_y1 << ", " << m_x2 << ", " << m_y2
    << " (" << m_width << " x " << m_height << ") "
    << m_imgSize <<  " " << sizeof(RGBQUAD) << endl;
}

void Scap::ScreenShot(const char * BmpName)
{
  HWND DesktopHwnd = GetDesktopWindow();
  RECT DesktopParams;
  HDC DevC = GetDC(DesktopHwnd);
  GetWindowRect(DesktopHwnd, &DesktopParams);
  DWORD Width = DesktopParams.right - DesktopParams.left;

  DWORD Height = DesktopParams.bottom - DesktopParams.top;
  DWORD fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
      + ((Width*Height * NB));
  char *bmpData = (char*)GlobalAlloc(0x0040, fileSize);

  PBITMAPFILEHEADER BFileHeader = (PBITMAPFILEHEADER)bmpData;
  PBITMAPINFOHEADER BInfoHeader
      = (PBITMAPINFOHEADER)&bmpData[sizeof(BITMAPFILEHEADER)];

  BFileHeader->bfType = 0x4D42; // BM
  BFileHeader->bfSize = sizeof(BITMAPFILEHEADER);
  BFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

  BInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
  BInfoHeader->biPlanes = 1;
  BInfoHeader->biBitCount = BC;
  BInfoHeader->biCompression = BI_RGB;
  BInfoHeader->biHeight = -Height;
  BInfoHeader->biWidth = Width;

  RGBQUAD *Image = (RGBQUAD*)&bmpData[sizeof(BITMAPFILEHEADER)
    + sizeof(BITMAPINFOHEADER)];
  
  HDC CaptureDC = CreateCompatibleDC(DevC);
  HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, Width, Height);
  SelectObject(CaptureDC, CaptureBitmap);
  BitBlt(CaptureDC, 0, 0, Width, Height, DevC, 0, 0, SRCCOPY|CAPTUREBLT);
  GetDIBits(CaptureDC, CaptureBitmap, 0, Height, Image
      , (LPBITMAPINFO)BInfoHeader, DIB_RGB_COLORS);

  DWORD Junk;
  HANDLE FH = CreateFileA(BmpName, GENERIC_WRITE, FILE_SHARE_WRITE, 0
      , CREATE_ALWAYS, 0, 0);
  WriteFile(FH, bmpData, fileSize, &Junk, 0);
  CloseHandle(FH);
  GlobalFree(bmpData); 
}

void Scap::CopyScreen()
{
  HWND DesktopHwnd = GetDesktopWindow();
  HDC DevC = GetDC(DesktopHwnd);
  if (0) cout << m_bfSize << " + " << m_biSize << " + " << m_imgSize << " = "
    << " " << m_fileSize << endl;
  
  // m_bmpData = (char*)GlobalAlloc(0x0040, m_fileSize);

  PBITMAPFILEHEADER BFileHeader = (PBITMAPFILEHEADER)m_bmpData;
  PBITMAPINFOHEADER BInfoHeader
      = (PBITMAPINFOHEADER)&m_bmpData[m_bfSize];

  BFileHeader->bfType = 0x4D42; // BM
  BFileHeader->bfSize = m_bfSize;
  BFileHeader->bfOffBits = m_bfSize + m_biSize;

  BInfoHeader->biSize = m_biSize;
  BInfoHeader->biPlanes = 1;
  BInfoHeader->biBitCount = BC;
  BInfoHeader->biCompression = BI_RGB;
  BInfoHeader->biHeight = -m_height;
  BInfoHeader->biWidth = m_width;

  RGBQUAD *m_image = (RGBQUAD*)&m_bmpData[m_bfSize + m_biSize];
  
  HDC CaptureDC = CreateCompatibleDC(DevC);
  HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, m_width, m_height);
  SelectObject(CaptureDC, CaptureBitmap);
  BitBlt(CaptureDC, 0, 0, m_width, m_height, DevC, m_x1, m_y1
      , SRCCOPY|CAPTUREBLT);
  GetDIBits(CaptureDC, CaptureBitmap, 0, m_height, m_image
      , (LPBITMAPINFO)BInfoHeader, DIB_RGB_COLORS);
}

void Scap::write(const char * BmpName, int spec)
{
  RGBQUAD * img = spec ? m_buf : m_image;
  DWORD Junk;
  HANDLE FH = CreateFileA(BmpName, GENERIC_WRITE, FILE_SHARE_WRITE, 0
      , CREATE_ALWAYS, 0, 0);
  WriteFile(FH, m_bmpData, (m_bfSize + m_biSize), &Junk, 0);
  WriteFile(FH, img, m_imgSize, &Junk, 0);
  CloseHandle(FH);
}

void Scap::write(const char * BmpName)
{
  DWORD Junk;
  HANDLE FH = CreateFileA(BmpName, GENERIC_WRITE, FILE_SHARE_WRITE, 0
      , CREATE_ALWAYS, 0, 0);
  WriteFile(FH, m_bmpData, m_fileSize, &Junk, 0);
  CloseHandle(FH);
}

void Scap::toDst()
{
  int W = m_width;
  int H = m_height;
  uint32_t * src = (uint32_t *)m_image;
  uint32_t * dst = (uint32_t *)m_buf;
  for (int y = 0; y < H; y++) {
    int x = 0;
    int z = x + (y * m_width);
    for (/* */; x < W; x++,z++) {
      dst[z] = src[z];
    }
  }
}

void Scap::blotDst(int x0, int y0, uint32_t c)
{
  uint32_t * dst = (uint32_t *)m_buf;
  for (int y = y0-1; y < y0+2; y++) {
    int x = x0-1;
    int z = x + (y * m_width);
    for (/* */; x < x0+2; x++,z++) {
      dst[z] = c;
    }
  }
}

void Scap::copySrcDst()
{
  int A = m_width * m_height;
  uint32_t * src = (uint32_t *)m_image;
  uint32_t * dst = (uint32_t *)m_buf;
  for (int z = 0; z < A; z++) {
    dst[z] = src[z];
  }
}

#if 0
#endif
