ImageWatermarking
=================

Program written in C++ using opencv libraries to extract an image signature 
using the Edges of the image or the discrete cosine transform (DCT) in order to 
detect if the image was modified and if so, show the modified region as an output.


Requirements
------------

* g++ 4.7.2
* opencv 2.4.7
* Linux environment (Tested on Debian based distributions)



Usage
-----

* Build: make clean; make.
* Run:
	* Two image needed: Original image without modifications and the modified image.
	* ./watermarking path_to_original path_to_modified.
	* This version only takes NxN square images.
