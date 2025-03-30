# Install script for directory: /home/xsh/catkin_ws/src/Firmware/platforms/posix

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/xsh/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4" TYPE EXECUTABLE FILES "/home/xsh/catkin_ws/devel/lib/px4/px4")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4"
         OLD_RPATH "/home/xsh/catkin_ws/build/Firmware/external/Install/lib:/home/xsh/catkin_ws/devel/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/px4/px4")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4" TYPE DIRECTORY FILES
    "/home/xsh/catkin_ws/src/Firmware/integrationtests"
    "/home/xsh/catkin_ws/src/Firmware/launch"
    "/home/xsh/catkin_ws/src/Firmware/posix-configs"
    "/home/xsh/catkin_ws/src/Firmware/ROMFS"
    "/home/xsh/catkin_ws/src/Firmware/test"
    USE_SOURCE_PERMISSIONS)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4" TYPE FILE FILES
    "/home/xsh/catkin_ws/src/Firmware/CMakeLists.txt"
    "/home/xsh/catkin_ws/src/Firmware/package.xml"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4/Tools" TYPE DIRECTORY FILES "/home/xsh/catkin_ws/src/Firmware/Tools/ecl_ekf" USE_SOURCE_PERMISSIONS)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4/Tools" TYPE PROGRAM FILES
    "/home/xsh/catkin_ws/src/Firmware/Tools/setup_gazebo.bash"
    "/home/xsh/catkin_ws/src/Firmware/Tools/upload_log.py"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4/build/posix_sitl_default" TYPE DIRECTORY FILES "/home/xsh/catkin_ws/src/Firmware/build/posix_sitl_default/build_gazebo" FILES_MATCHING REGEX "/CMakeFiles$" EXCLUDE REGEX "/[^/]*\\.so$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4/Tools/sitl_gazebo" TYPE DIRECTORY FILES
    "/home/xsh/catkin_ws/src/Firmware/Tools/sitl_gazebo/models"
    "/home/xsh/catkin_ws/src/Firmware/Tools/sitl_gazebo/worlds"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/px4/Tools/sitl_gazebo" TYPE FILE FILES
    "/home/xsh/catkin_ws/src/Firmware/Tools/sitl_gazebo/CMakeLists.txt"
    "/home/xsh/catkin_ws/src/Firmware/Tools/sitl_gazebo/package.xml"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/xsh/catkin_ws/build/Firmware/platforms/posix/src/cmake_install.cmake")

endif()

