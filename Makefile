# File Name: Makefile
# Description: Makefile to build and clean up the wordle executable.
# Author: Amy Uccello
# Student Number: 251236036
# Assignment Number: 1
# Class: CS3307
# Date Created: September 30, 2023

# Set WT_BASE to the location of your Wt installation
ifdef WT_BASE
WT_BASE := $(WT_BASE)
else
WT_BASE := /usr/local/include/Wt
endif

# Set up the compiler and linker flags
CXX=g++
CXXFLAGS=--std=c++14 -I$(WT_BASE)/include 
LDFLAGS=-L$(WT_BASE)/lib -Wl,-rpath,$(WT_BASE)/lib -lwthttp -lwt -lboost_thread-mt -lboost_atomic-mt -lboost_filesystem

# Identify our code base to be building.  Headers are listed under DEPS, and objects under OBJS.
DEPS = Wordle.h
OBJS = main.o Wordle.o

# How we build objects
%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build the executable
wordle: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

# Clean Up
clean:
	rm -f wordle $(OBJS)
