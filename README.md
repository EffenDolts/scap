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
