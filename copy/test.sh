#!/bin/bash
# Author: Anas Aly

# Compile the program
gcc copy.c -o copy

# Test case 1 - Copy file
echo "Test 1 - Copy file"
touch file1.txt
./copy file1.txt file2.txt

# Test case 2 - Copy directory
echo "Test 2 - Copy directory" 
mkdir dir1
touch dir1/file1.txt
./copy dir1 dir2

# Test case 3 - Copy non-existent file 
echo "Test 3 - Copy non-existent file"
./copy nofile.txt newfile.txt

# Test case 4 - Copy non-existent directory
echo "Test 4 - Copy non-existent directory"  
./copy nodir/ dir2