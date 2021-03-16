# Generate shared library by default.
SET(BUILD_SHARED_LIBS ON)

# Set default install dir
IF(NOT CMAKE_INSTALL_LIBDIR)
	SET(CMAKE_INSTALL_LIBDIR lib)
ENDIF()

MESSAGE(STATUS "Installation path prefix: " ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Library installation directory: " ${CMAKE_INSTALL_LIBDIR})
MESSAGE(STATUS "PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})

# Where header files can be found.
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# Include sub directory adapters and utils.
#ADD_SUBDIRECTORY(utils)

# Collect all source files.
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c)
#AUX_SOURCE_DIRECTORY(. SRC_LIST)
#AUX_SOURCE_DIRECTORY(./utils SRC_LIST_UTILS)

# Not compile test files into library
IF(NOT ENABLE_TEST)
	SET (TEST_CPP_LIST
#		utils/MTestUtil.cpp
		stub.cpp
	)

	list(REMOVE_ITEM SOURCES ${TEST_CPP_LIST})
ENDIF()


# Generate shared library lib${PROJECT_NAME}.
#ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST} ${SRC_LIST_ADAPTERS}
#		${SRC_LIST_UTILS} ${SRC_LIST_ADAPTERS_REDIS})
ADD_LIBRARY (${PROJECT_NAME} SHARED ${SOURCES})

# Set library version
SET_TARGET_PROPERTIES(
		${PROJECT_NAME}
		PROPERTIES
		SOVERSION ${M_VERSION_MAJOR}
		VERSION ${M_VERSION}
)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
IF(NOT ENABLE_TEST)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
ELSE()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
ENDIF()

# Install lib to /usr/local/lib by default
INSTALL(
		TARGETS ${PROJECT_NAME}
		LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files to /usr/local/include/${PROJECT_NAME}
INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/include/
		DESTINATION include/${PROJECT_NAME}
)

# Install COPYRIGHT, README, bench
#INSTALL(FILES COPYRIGHT README.md DESTINATION share/doc/${PROJECT_NAME})


# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "jsonParser")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "jsonParser based on rapid json and static json")
set(CPACK_PACKAGE_VENDOR "Max")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RPM_PACKAGE_GROUP "Max")
set(CPACK_RPM_PACKAGE_URL "https://github.com/maxcong001/jsonParser")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Max ")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "MIT Licence")
 
# 设置默认生成器，RPM生成器会构建RPM安装包，其它还有TGZ/ZIP等
set(CPACK_GENERATOR "RPM")
 
# 安装前和安装后执行的shell脚本, 会打包到RPM中，安装时执行。这里可扩展性很强, 放在源码目录下即可
# set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pre_script.sh)
# set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/post_script.sh)
 
# 引入CPack模块，必须的
include(CPack)