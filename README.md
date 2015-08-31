# CAEMCommSIS3100
adapted code to use the CAENComm library with a Stuck SIS3100 VME interface

please download the original CAENComm code from http://www.caen.it/csite/CaenProd.jsp?parent=38&idmod=684

you can use the heade file together with the Makefile and the C code in this repository to compile CAEM software like the Updater or wavedump to be used with the Struck SIS3100 VME interface instead of the CAEN interface.

This requires the Struck library and driver for the SIS3100 (http://www.struck.de/pcievme.html)

please add a directory called 'include' and copy the original CAENComm.h header file into it.
