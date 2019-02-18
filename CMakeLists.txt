cmake_minimum_required (VERSION 2.6)
project (NewYearGarlands)

set (wxWidgets_ROOT_DIR "$ENV{wxwin}")
set (wxWidgets_LIB_DIR "$ENV{wxwin}\\lib\\vc141_x64_dll")
find_package (wxWidgets COMPONENTS core base REQUIRED)

add_executable (GarlandsServer WIN32 "./service/Main.cpp")
add_executable (GarlandsClient WIN32 "./client/Main.cpp")
target_link_libraries (GarlandsClient ${wxWidgets_LIBRARIES})
