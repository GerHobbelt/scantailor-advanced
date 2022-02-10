PROJECT(zones)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml)

SET(
	sources
	SplineVertex.cpp SplineVertex.h
	SplineSegment.cpp SplineSegment.h
	EditableSpline.cpp EditableSpline.h
	SerializableSpline.cpp SerializableSpline.h
	Zone.cpp Zone.h
	ZoneSet.cpp ZoneSet.h
	EditableZoneSet.cpp EditableZoneSet.h
	BasicSplineVisualizer.cpp BasicSplineVisualizer.h
	ZoneInteractionContext.cpp ZoneInteractionContext.h
	ZoneDefaultInteraction.cpp ZoneDefaultInteraction.h
	ZoneCreationInteraction.cpp ZoneCreationInteraction.h
	ZoneVertexDragInteraction.cpp ZoneVertexDragInteraction.h
	ZoneContextMenuInteraction.cpp ZoneContextMenuInteraction.h
	ZoneContextMenuItem.h
)

SOURCE_GROUP(Sources FILES ${sources})

ADD_LIBRARY(zones STATIC ${sources})

target_link_libraries(zones PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml)

TRANSLATION_SOURCES(scantailor ${sources})
