IF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
ENDIF()

# it is probably incredibly obvious why this doesn't work
# but i'll have to look into it later.
#INCLUDE_DIRECTORIES(${CXXCAM_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/deps/cxxcam/include)

ADD_EXECUTABLE(${PROJECT_NAME} jscam.cpp jsmachine.cpp jsnef.cpp js.cpp platform.cpp)

# Addition of boost_system is required for an unknown reason
# on my ubuntu system.
TARGET_LINK_LIBRARIES(${PROJECT_NAME} v8 cxxcam boost_system)
