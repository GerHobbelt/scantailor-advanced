PROJECT(interaction)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

SET(
	sources
	DraggableObject.h
	DraggablePoint.cpp DraggablePoint.h
	DraggableLineSegment.cpp DraggableLineSegment.h
	ObjectDragHandler.cpp ObjectDragHandler.h
	InteractionHandler.cpp InteractionHandler.h
	InteractionState.cpp InteractionState.h
	DragHandler.cpp DragHandler.h
	DragWatcher.cpp DragWatcher.h
	ZoomHandler.cpp ZoomHandler.h
	InteractiveXSpline.cpp InteractiveXSpline.h
)

SOURCE_GROUP(Sources FILES ${sources})

ADD_LIBRARY(interaction STATIC ${sources})

target_link_libraries(interaction PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

TRANSLATION_SOURCES(scantailor ${sources})
