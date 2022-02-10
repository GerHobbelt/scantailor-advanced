INCLUDE_DIRECTORIES(BEFORE ..)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
find_package(Qt6 REQUIRED COMPONENTS Core)

SET(
	sources
	main.cpp TestContentSpanFinder.cpp
	TestSmartFilenameOrdering.cpp
	TestMatrixCalc.cpp
	../ContentSpanFinder.cpp ../ContentSpanFinder.h
	../SmartFilenameOrdering.cpp ../SmartFilenameOrdering.h
)

SOURCE_GROUP("Sources" FILES ${sources})

SET(
	libs
	imageproc math ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${Boost_PRG_EXECUTION_MONITOR_LIBRARY}
	${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${EXTRA_LIBS}
)

ADD_EXECUTABLE(tests ${sources})
TARGET_LINK_LIBRARIES(tests ${libs} Qt6::Core)
