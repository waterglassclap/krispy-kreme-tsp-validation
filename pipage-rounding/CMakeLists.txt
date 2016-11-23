project( gcut )
#ADD_DEFINITIONS(
#    -std=c++11 # Or -std=c++0x
#    # Other flags
#)
#find_package( OpenCV REQUIRED )

#aux_source_directory( ../common/utils/max-flow-min-cut-graph-cut/src/. SUB_SOURCES )

set(SOURCE
  ${SOURCE}
  ../common/utils/common_util.cpp
  ../common/utils/push_relabel.cpp
  ../common/utils/mincut_util.cpp
#	../common/utils/max-flow-min-cut-graph-cut/src/mincut_util.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/abs_ppr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/randomized_ppr.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/sample.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/sample2.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/sample_testall.cpp
)

set(SOURCE
  ${SOURCE}
  ${SUB_SOURCES}
)

add_executable( rand_ppr ${SOURCE} )
#target_link_libraries (rand_ppr ${OpenCV_LIBS})