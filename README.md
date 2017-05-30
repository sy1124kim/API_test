# API_test

To build the API_test, the following build system dependencies are required:
 
 <ubuntu>
 
  $ sudo apt-get install libtool
  
  $ sudo apt-get install autoconf
  
  $ sudo apt-get install autumake
  
  $ git clone https://github.com/json-c/json-c.git
  
  $ cd json-c
  
  json-c$ sh autogen.sh
  
  json-c$ ./configure --prefix=/usr/local
  
  json-c$ make
  
  json-c$ make install
  
  
 <json-c test>
 
  json-c$ make check 
  
  
Build the API_test
 $ git clone https://github.com/sy1124kim/API_test.git
 
 $ cd API_test.git
 
 API_test$ make or make static
 
  
