PROJECT("Dewarping library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Xml)

SET(
	sources
	Curve.cpp Curve.h
	DistortionModel.cpp DistortionModel.h
	DistortionModelBuilder.cpp DistortionModelBuilder.h
	DetectVertContentBounds.cpp DetectVertContentBounds.h
	TowardsLineTracer.cpp TowardsLineTracer.h
	TextLineTracer.cpp TextLineTracer.h
	TextLineRefiner.cpp TextLineRefiner.h
	TopBottomEdgeTracer.cpp TopBottomEdgeTracer.h
	CylindricalSurfaceDewarper.cpp CylindricalSurfaceDewarper.h
	DewarpingPointMapper.cpp DewarpingPointMapper.h
	RasterDewarper.cpp RasterDewarper.h
)
SOURCE_GROUP("Sources" FILES ${sources})

ADD_LIBRARY(dewarping STATIC ${sources})

target_link_libraries(dewarping PRIVATE Qt6::Core Qt6::Gui Qt6::Xml)
