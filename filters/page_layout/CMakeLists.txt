PROJECT("Page Layout Filter")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml)


INCLUDE_DIRECTORIES(BEFORE "${CMAKE_CURRENT_BINARY_DIR}")

FILE(GLOB ui_files "ui/*.ui")
SET_SOURCE_FILES_PROPERTIES(${ui_sources} PROPERTIES GENERATED TRUE)
SOURCE_GROUP("UI Files" FILES ${ui_files})
SOURCE_GROUP("Generated" FILES ${ui_sources})

SET(
	sources
	Utils.cpp Utils.h
	ImageView.cpp ImageView.h
	Filter.cpp Filter.h
	OptionsWidget.cpp OptionsWidget.h
	Task.cpp Task.h
	CacheDrivenTask.cpp CacheDrivenTask.h
	Params.cpp Params.h
	Settings.cpp Settings.h
	Thumbnail.cpp Thumbnail.h
	ApplyDialog.cpp ApplyDialog.h
	Alignment.cpp Alignment.h
	OrderByWidthProvider.cpp OrderByWidthProvider.h
	OrderByHeightProvider.cpp OrderByHeightProvider.h
)
SOURCE_GROUP("Sources" FILES ${sources})

ADD_LIBRARY(page_layout STATIC ${sources} ${ui_sources})

target_link_libraries(page_layout PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml)

TRANSLATION_SOURCES(scantailor ${sources} ${ui_files})
