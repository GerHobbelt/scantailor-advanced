PROJECT("Foundation library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Xml)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

SET(
	sources
	NonCopyable.h IntrusivePtr.h RefCountable.h
	AlignedArray.h
	FastQueue.h
	SafeDeletingQObjectPtr.h
	ScopedIncDec.h ScopedDecInc.h
	Span.h VirtualFunction.h FlagOps.h
	AutoRemovingFile.cpp AutoRemovingFile.h
	Proximity.cpp Proximity.h
	Property.h
	PropertyFactory.cpp PropertyFactory.h
	PropertySet.cpp PropertySet.h
	PerformanceTimer.cpp PerformanceTimer.h
	QtSignalForwarder.cpp QtSignalForwarder.h
	GridLineTraverser.cpp GridLineTraverser.h
	StaticPool.h
	DynamicPool.h
	NumericTraits.h
	VecNT.h
	VecT.h
	MatMNT.h
	MatT.h
	PriorityQueue.h
	Grid.h
	ValueConv.h
)
SOURCE_GROUP("Sources" FILES ${sources})

ADD_LIBRARY(foundation STATIC ${sources})

target_link_libraries(foundation PRIVATE Qt6::Core Qt6::Xml)
