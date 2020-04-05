#!/bin/bash
# Script to produce frames and collect them and create movie
# Prereiquirements: 
# -> imagemagick (for mogrify)
# -> ffmpeg (to create a video)

SECONDS=0
STEP=100
# test value
# STEP = 1

# Directory for pictures and movie
mkdir -p Pic

# Compile program
g++ Fraktal.cpp -o Fraktal.o

# Create pictures in groups of $STEP 
for i in {0..9}; do
# for test
# for i in {0..10}; do
	
	echo "Create frames from "$(($i*$STEP))" till "$((($i+1)*$STEP))"..."
	# Arguments in order - 
	# 1) Number of first frame
	# 2) Number of second frame
	./Fraktal.o $(($i*$STEP)) $((($i+1)*$STEP)) 
	
	echo "Converting .ppm to .jpg..."
	mogrify -format jpg Pic/*.ppm
	
	echo "Remove files .ppm..."
	rm Pic/*.ppm
	
	ALL=$(($ALL + $SECONDS))
	duration=$SECONDS
	echo -e "\e[31mThis part took me "$(($duration / 60))" minutes and "$(($duration % 60))" seconds.\e[0m"
	SECONDS=0
	
	done

echo "Making movie..."
ffmpeg -r 30 -i Pic/Fraktal_%4d.jpg -qscale 1 Pic/Mandelbrot.mp4

echo -e "\e[32mWhole thing took me "$(($ALL / 60))" minutes and "$(($ALL % 60))" seconds.\e[0m"
