cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

set(source_dir "${CMAKE_BINARY_DIR}/liblogger-src")
set(build_dir "${CMAKE_BINARY_DIR}/liblogger-build")
set(depends_dir "${CMAKE_BINARY_DIR}/depends-inc")

EXTERNALPROJECT_ADD(
  liblogger
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-logger.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            liblogger-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)
      
if(NOT EXISTS ${depends_dir}/log)
    execute_process(
        COMMAND mkdir ${depends_dir}
        COMMAND ln -s ${source_dir}/inc/log ${depends_dir}
    )
endif()

# include_directories(${source_dir}/inc)
include_directories(${depends_dir})
link_directories(${build_dir}/build)
