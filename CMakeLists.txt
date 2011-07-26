
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (sql-parser)

# Define the boost envrionment so we don't get system libraries
set ( BOOST_ROOT       $ENV{BOOST_DIR} )

FIND_PACKAGE ( Boost 1.44 REQUIRED )

# Project include directory
INCLUDE_DIRECTORIES (
    ${sql-parser_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
    )

ADD_LIBRARY (sql-parser SHARED 
    src/sqlparser.cpp
    )

SET_TARGET_PROPERTIES (sql-parser
    PROPERTIES OUTPUT_NAME sql-parser
    )

# Check if the install prefix is set
IF (NOT CMAKE_INSTALL_PREFIX)
    SET ( CMAKE_INSTALL_PREFIX
        ${sql-parser_SOURCE_DIR}/install
        )
ENDIF(NOT CMAKE_INSTALL_PREFIX)


# Install files
SET ( CMAKE_INSTALL_PREFIX 
    ${sql-parser_SOURCE_DIR}/install
    )

FILE (GLOB includeFiles
    ${sql-parser_SOURCE_DIR}/include/*.h
    )

FILE (GLOB srcFiles
    ${sql-parser_SOURCE_DIR}/src/*.cpp
    )

#LIST (APPEND srcFiles
#    src/CMakeLists.txt
#    )

FILE (GLOB cmakeFiles
    ${sql-parser_SOURCE_DIR}/CMakeLists.txt
    )

INSTALL (FILES ${includeFiles}
    DESTINATION include/
    )

INSTALL (FILES ${includeFiles}
    DESTINATION src/sql-parser/include
    )

INSTALL (FILES ${srcFiles}
    DESTINATION src/sql-parser/src
    )

INSTALL (FILES ${cmakeFiles}
    DESTINATION src/sql-parser
    )

INSTALL (TARGETS sql-parser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )


ADD_SUBDIRECTORY(test)

