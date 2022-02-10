INCLUDE_DIRECTORIES(BEFORE ..)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

SET(
	sources
	main.cpp
	TestBinaryImage.cpp TestReduceThreshold.cpp
	TestSlicedHistogram.cpp
	TestConnCompEraser.cpp TestConnCompEraserExt.cpp
	TestGrayscale.cpp
	TestRasterOp.cpp TestShear.cpp
	TestOrthogonalRotation.cpp
	TestSkewFinder.cpp
	TestScale.cpp
	TestTransform.cpp
	TestMorphology.cpp
	TestDentFinder.cpp
	TestBinarize.cpp
	TestPolygonRasterizer.cpp
	TestSeedFill.cpp
	TestSEDM.cpp
	TestRastLineFinder.cpp
	Utils.cpp Utils.h
)
SOURCE_GROUP("Sources" FILES ${sources})

SET(
	libs
	imageproc math foundation ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${Boost_PRG_EXECUTION_MONITOR_LIBRARY} ${EXTRA_LIBS}
)

ADD_EXECUTABLE(imageproc_tests ${sources})
TARGET_LINK_LIBRARIES(imageproc_tests ${libs} Qt6::Core Qt6::Gui Qt6::Widgets)
