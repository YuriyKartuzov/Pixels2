# Pixels2
GPU project 

Authors: Alex Kong, Dmytro Sych, Yuriy Kartuzov

Turning and image into ASCII art happens in the following order:

1. Capture origin image
2. Turn it black and white
3. Cropping an image
4. Image reduction
5. Mapping pixels to ASCII char set
6. Writing to file


NOTE: Currently it averages 4 pixels wide and 7 pixels tall into a single Char (hardcoded). For this reason best results can be achieved with image that is 1000-1500 pixels wide and 800-1200 pixels tall. 


POSSIBLE IMPROVEMENTS:

a) Have a variable size of pixels -> char algorithm, allowing any picture size to be optimally processed.
b) Optimize code by changing vector access to "PASS BY REFERENCE"
b) Create an Image folder which will contain everything we wan to process. Create a queue where files are automatically fed by reading file from filesystem one by one.
