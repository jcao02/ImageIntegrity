Content based authentication signature for still images
=======================================================

Program written in C++ using opencv libraries to extract an image signature 
using the Edges of the image or the discrete cosine transform (DCT) in order to 
detect if the image was modified and if so, show the modified region as an output.


Requirements
------------

* g++ 4.7.2
* opencv 2.4.7
* Linux environment (Tested on Debian based distributions)


Content
-------

* imagesignature.cpp : Main program with functions
* Examples: folder with input/output examples
* images: folder with original images and subfolders with differents attacks
  * Compression: attacks by compression
  * Gauss: attacks by gauss 
  * Resized: attacks by resizing
  * Sharp: attack by sharping
  * others: other images.
* generate_ber.py: script for Generate bit error ratio from the image folder using the algorithm.
* Makefile: to build the app.

Usage
-----

* Build: make clean; make.
* Run:
	* Two image needed: Original image without modifications and the modified image.
	* For execution: ./imagesignature original_image modified_image signature 
    * where signature is 1 for Edges based signature or 2 for DCT based signature 
