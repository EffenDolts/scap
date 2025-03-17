/** @author Mental Blok */

#ifndef H_SCAP
#define H_SCAP

#include <Windows.h>
#include <stdint.h> 

// https://cplusplus.com/forum/windows/105644/
class Scap {
private:
  DWORD m_fileSize;
  int m_bfSize; // = sizeof(BITMAPFILEHEADER);
  int m_biSize; // = sizeof(BITMAPINFOHEADER);
  int m_imgSize;
  char * m_bmpData;
protected:
  RGBQUAD * m_image;
  RGBQUAD * m_buf;
  DWORD m_width;
  DWORD m_height;
  int m_x1;
  int m_y1;
  int m_x2;
  int m_y2;
public:
  Scap(int x1, int y1, int x2, int y2);
  virtual ~Scap();
  void ScreenShot(const char * BmpName);
  void CopyScreen();
  void write(const char * BmpName);
  void write(const char * BmpName, int spec);
  void print();
  void toDst();
  RGBQUAD * ptr(int x, int y);
  RGBQUAD v(int x, int y);
  uint32_t uval(int x, int y);
  void buf(int x, int y, uint32_t v);
  BYTE r(int x, int y);
  BYTE g(int x, int y);
  BYTE b(int x, int y);
  uint32_t red(uint32_t p) { return (p>>16) & 0x000000ff; }
  uint32_t green(uint32_t p) { return (p>>8) & 0x000000ff; }
  uint32_t blue(uint32_t p) { return p & 0x000000ff; }
  void blotDst(int x0, int y0, uint32_t c);
  void copySrcDst();
  int clickMouse(float targetX, float targetY);
  /////////////////////////////////////////////////////
  virtual uint32_t scanBoard() = 0;
  virtual void daubExec(uint newBits, int doit, Scap * cntrPtr) = 0;
  virtual void endExec(int doit) = 0;
  virtual void scanSpin(char * spinCode) = 0;
  virtual int findAnchors() = 0;
  virtual int scanFree() = 0;
  virtual int countBalls(int & nFree) = 0;
  virtual int readCounter(int & nFree, Scap * globePtr, int verb = 0) = 0;
  virtual void resetFree() { }
  virtual int startExec(Scap * anchor, int doit) = 0;
  virtual int clickFast(int row, int col, int doit) = 0;
  virtual void writeCounterArray(int cntrNum) = 0;
  virtual float globe() = 0;
  virtual float gLeft() = 0;
  virtual float gTop() = 0;
};

#endif /* !H_SCAP */

