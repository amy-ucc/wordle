Setting envvironment up
-----------------------------------
set WT_BASE in the environment to the location of the Wt installation
or edit the Makefile default location of WT_BASE 


Building the C++ Wordle Application
-----------------------------------

Building things is fairly straightforward using the provided Makefile. 

When ready, things can be built simply be executing:

  >  make

When done, you will have the wordle executable ready to run.

To clean up the build, you can execute:

  >  make clean

Running the Wordle Application
----------------------------------

To run things, you can do so by executing:

  > ./wordle --docroot . --http-listen 0.0.0.0:8080

This runs the hello executable in your current directory, giving it a couple
of parameters.  Specifying --docroot . tells the application to find files
it needs from the current directory.  (Specifically, it will search for 
things in the resources folder in the current directory.)  The remaining
parameters (--http-listen 0.0.0.0:8080) tells the server to listen for 
incoming HTTP requests on any network interface on the system at port number
8080.  

With the application running, you can then use any web browser to connect to 
http://127.0.0.1:8080 and access things from there.

If you would like to tailor things further, running:

  > ./wordle --help

will report a full set of command line options for the application.
