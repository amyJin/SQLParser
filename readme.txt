The program sqlparser.cpp takes a sql query as input and parses it to decide if the query is malicious or not to protect the database in the back-end.

We are using boost_1_44_0.

To use this SQL parser you will need to have the g++ and boost_1_44_0 installed on your system

Building system:

mkdir build   /** create a build directory */
cd build
cmake ..      /** prepration for make */ 
make          /** build the system */
make install  /** create install directory */

The source code is at install/src/sql-parser/src.

The test code is at install/src/test.


Sample command of compilation:

g++ yourFile.cc -I/(location of install dir)/install/include/ -L/(location of install dir)/install/lib/ -lsql-parser
