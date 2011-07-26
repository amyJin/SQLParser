#
#  Test/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for tests
#

ADD_EXECUTABLE (sqltest
    sqltest.cpp
    )

TARGET_LINK_LIBRARIES (sqltest
    sql-parser
    )

INSTALL (FILES
    CMakeLists.txt
    sqltest.cpp
    DESTINATION src/test
    )

