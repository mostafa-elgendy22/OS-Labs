#!/bin/bash
# Examples from the doc and https://skillrack2k17.wordpress.com/2017/10/26/bonus-practice-problem/#:~:text=Kangaroo%20refers%20to%20a%20word,run%2C%20gen%2C%20gone%20etc, http://www.cs.mun.ca/~dchurchill/pc/archive/2016_09/A/A.pdf
# Make sure to build your exe with every change (Ctrl+shift+B on VSCode)
Executable="a.out" # Change this to your .exe name (without the exe)
echo "All of these should output 1"
# Lab doc
./$Executable chicken HEN
./$Executable lighted lit
./$Executable instructor tutor
./$Executable enjoyment joy
./$Executable supremacist racist
./$Executable bAd suBstandard

# Links 
./$Executable precipitation rain
./$Executable alone one
./$Executable alone lone
./$Executable fabrication fiction
./$Executable prosecute sue
./$Executable encourage courage
./$Executable encourage cog
./$Executable encourage cur
./$Executable encourage urge
./$Executable encourage core
./$Executable encourage cure
./$Executable encourage nag
./$Executable encourage rag
./$Executable encourage age
./$Executable encourage nor


echo "These should output 0"
# Lab doc
./$Executable Happy glad
./$Executable sad sad
./$Executable guy lad

# Links 
./$Executable encourage run
./$Executable encourage gen
./$Executable encourage gone
./$Executable precipitation drizzle
./$Executable encapsulate enclose
./$Executable encapsulate cover
./$Executable prosecute arraign
./$Executable nojoeys hello
./$Executable nojoeys world

echo "This should trigger the error condition"
./$Executable Too Many Inputs
./$Executable few
