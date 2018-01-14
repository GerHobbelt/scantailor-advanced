TEMPLATE = app
TARGET = name_of_the_app

QT = core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH = E:/Asepsis/ScanTailor/boost_1_66_0

HEADERS += \
	AbstractCommand.h \
	AbstractRelinker.h \
	AtomicFileOverwriter.h \
	CommandLine.h \
	DebugImages.h \
	Despeckle.h \
	Dpi.h \
	Dpm.h \
	filters\output\BlackWhiteOptions.h \
	filters\output\ColorGrayscaleOptions.h \
	filters\output\ColorParams.h \
	filters\output\DepthPerception.h \
	filters\output\DespeckleLevel.h \
	filters\output\DewarpingMode.h \
	filters\page_layout\Alignment.h \
	filters\page_layout\Params.h \
	filters\page_layout\Settings.h \
	filters\page_split\LayoutType.h \
	foundation\AutoRemovingFile.h \
	foundation\FastQueue.h \
	foundation\IntrusivePtr.h \
	foundation\NonCopyable.h \
	foundation\RefCountable.h \
	foundation\VirtualFunction.h \
	ImageFileInfo.h \
	ImageId.h \
	ImageLoader.h \
	ImageMetadata.h \
	ImageMetadataLoader.h \
	imageproc\BinaryImage.h \
	imageproc\BinaryThreshold.h \
	imageproc\BitOps.h \
	imageproc\BWColor.h \
	imageproc\ByteOrder.h \
	imageproc\Connectivity.h \
	imageproc\ConnectivityMap.h \
	imageproc\Constants.h \
	imageproc\GrayImage.h \
	imageproc\Grayscale.h \
	imageproc\InfluenceMap.h \
	imageproc\NonCopyable.h \
	imageproc\Scale.h \
	Margins.h \
	OrthogonalRotation.h \
	OutOfMemoryHandler.h \
	PageId.h \
	PageInfo.h \
	PageSequence.h \
	RelinkablePath.h \
	TaskStatus.h \
	ThumbnailLoadResult.h \
	ThumbnailPixmapCache.h \
	Utils.h \
	version.h \
	XmlMarshaller.h \
	XmlUnmarshaller.h

SOURCES += \
	AtomicFileOverwriter.cpp \
	CommandLine.cpp \
	DebugImages.cpp \
	Despeckle.cpp \
	Dpi.cpp \
	Dpm.cpp \
	filters\output\BlackWhiteOptions.cpp \
	filters\output\ColorGrayscaleOptions.cpp \
	filters\output\ColorParams.cpp \
	filters\output\DepthPerception.cpp \
	filters\output\DespeckleLevel.cpp \
	filters\output\DewarpingMode.cpp \
	filters\page_layout\Alignment.cpp \
	filters\page_layout\Params.cpp \
	filters\page_layout\Settings.cpp \
	filters\page_split\LayoutType.cpp \
	foundation\AutoRemovingFile.cpp \
	ImageFileInfo.cpp \
	ImageId.cpp \
	ImageLoader.cpp \
	ImageMetadata.cpp \
	ImageMetadataLoader.cpp \
	imageproc\BinaryImage.cpp \
	imageproc\BitOps.cpp \
	imageproc\ConnectivityMap.cpp \
	imageproc\Constants.cpp \
	imageproc\GrayImage.cpp \
	imageproc\Grayscale.cpp \
	imageproc\InfluenceMap.cpp \
	imageproc\Scale.cpp \
	main.cpp \
	OrthogonalRotation.cpp \
	OutOfMemoryHandler.cpp \
	PageId.cpp \
	PageInfo.cpp \
	PageSequence.cpp \
	RelinkablePath.cpp \
	ThumbnailPixmapCache.cpp \
	Utils.cpp \
	XmlMarshaller.cpp \
	XmlUnmarshaller.cpp
