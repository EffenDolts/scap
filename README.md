# scap
Screen capture class

Purpose of This Repository

The purpose of the scap repository is to provide a screen capture class. This class is designed to capture a specified rectangular area of the screen using the Windows API and save the captured content as a BMP image file. The README provides an overview of the class's functionality, including its constructor and methods for capturing and writing screen data.

Features and Technologies Used

The repository contains the implementation of a screen capture class. The key features include:

A constructor that initializes the screen capture area using specified coordinates.

A CopyScreen() method to perform the actual screen capture by reading the screen data within the specified coordinates.

A write() method to save the captured screen content to a BMP file, either from the primary capture buffer or a secondary buffer with potentially modified content.

Technologies used include the Windows API for screen capturing and C or C++ for handling image processing and file writing.

The clickMouse function in the Scap.cpp file is designed to move the mouse cursor to a specified target position on the screen and simulate a mouse click (left button down and up) at that position. Additionally, it returns the cursor to its original position after the click. Here is a detailed breakdown of its operations:

Getting Screen Metrics:

The function retrieves the screen width and height using GetSystemMetrics(SM_CXSCREEN) and GetSystemMetrics(SM_CYSCREEN).
Normalizing Target Coordinates:

It converts the target X and Y coordinates to a normalized range (0 to 1) based on the screen dimensions.
Getting Current Cursor Position:

The current cursor position is retrieved using GetCursorPos(&cursorPos).
The cursor position is also normalized to the screen dimensions.
Boundary Checks:

It checks if the target and current cursor positions are within the valid range (0 to 1). If not, it prints an error message and exits.
Simulating Mouse Events:

The function creates an INPUT structure and sets it up for mouse events.
It moves the mouse to the target position using MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE.
It simulates a left mouse button down (MOUSEEVENTF_LEFTDOWN) and up (MOUSEEVENTF_LEFTUP).
It moves the mouse back to the original position.
Sending Input Events:

The SendInput function is used to send the mouse events to the system.
Overall, the clickMouse function effectively automates mouse movement and clicking at specified screen coordinates and ensures the cursor returns to its starting position.
