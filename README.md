# scap
Screen capture class

Constructor takes screen coordinates (int x1, int y1, int x2, int y2) and creates a buffer for screen capture
CopyScreen() initiate the actual screen capture
write(const char * BmpName, int spec) writes a BMP format file with the contents of the capture buffer or a
secondary buffer that typically contains a modified version of the original screen capture
