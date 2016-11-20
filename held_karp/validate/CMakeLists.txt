project( validate )
#ADD_DEFINITIONS(
#    -std=c++11 # Or -std=c++0x
#    # Other flags
#)
find_package( OpenCV REQUIRED )

aux_source_directory( ../../common/utils/max-flow-min-cut-graph-cut/src/. SUB_SOURCES )

set(SOURCE
  ${SOURCE}
  ../../common/utils/common_util.cpp
  ../../common/utils/push_relabel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sample.cpp
)

set(SOURCE
  ${SOURCE}
  ${SUB_SOURCES}
)

add_executable( sample ${SOURCE} )
target_link_libraries (sample ${OpenCV_LIBS})