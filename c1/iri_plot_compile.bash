#!/bin/bash

f77 -c -fpic iritest.for irisub.for irifun.for iritec.for iridreg.for igrf.for cira.for iriflip.for

f77 -shared -o libiri.so iritest.o irisub.o irifun.o iritec.o iridreg.o igrf.o cira.o iriflip.o

gcc -L/home/frame/share/iri/answers/c1 -g -Wl,-rpath=/home/frame/share/iri/answers/c1 -o iri_plot iri_plot.c -liri -lgfortran -lm

