set(CMAKE_AUTOUIC ON)

set(ui_files
    OtsuBinarizationOptionsWidget.ui
    ApplyColorsDialog.ui
    ChangeDewarpingDialog.ui
    ChangeDpiDialog.ui
    OptionsWidget.ui
    PictureZonePropDialog.ui
    SauvolaBinarizationOptionsWidget.ui
    SauvolaModBinarizationOptionsWidget.ui
    WolfBinarizationOptionsWidget.ui)

set(sources
    ApplyColorsDialog.cpp ApplyColorsDialog.h
    ChangeDpiDialog.cpp ChangeDpiDialog.h
    ImageView.cpp ImageView.h
    ImageViewTab.h
    TabbedImageView.cpp TabbedImageView.h
    Filter.cpp Filter.h
    OptionsWidget.cpp OptionsWidget.h
    Task.cpp Task.h
    CacheDrivenTask.cpp CacheDrivenTask.h
    OutputGenerator.cpp OutputGenerator.h
    OutputMargins.h
    Settings.cpp Settings.h
    Thumbnail.cpp Thumbnail.h
    Utils.cpp Utils.h
    Params.cpp Params.h
    BlackWhiteOptions.cpp BlackWhiteOptions.h
    ColorCommonOptions.cpp ColorCommonOptions.h
    RenderParams.cpp RenderParams.h
    ColorParams.cpp ColorParams.h
    OutputImageParams.cpp OutputImageParams.h
    OutputFileParams.cpp OutputFileParams.h
    OutputParams.cpp OutputParams.h
    PictureLayerProperty.cpp PictureLayerProperty.h
    ZoneCategoryProperty.cpp ZoneCategoryProperty.h
    PictureZonePropFactory.cpp PictureZonePropFactory.h
    PictureZonePropDialog.cpp PictureZonePropDialog.h
    PictureZoneComparator.cpp PictureZoneComparator.h
    PictureZoneEditor.cpp PictureZoneEditor.h
    FillColorProperty.cpp FillColorProperty.h
    FillZonePropFactory.cpp FillZonePropFactory.h
    FillZoneComparator.cpp FillZoneComparator.h
    FillZoneEditor.cpp FillZoneEditor.h
    ColorPickupInteraction.cpp ColorPickupInteraction.h
    DespeckleState.cpp DespeckleState.h
    DespeckleView.cpp DespeckleView.h
    DespeckleVisualization.cpp DespeckleVisualization.h
    DespeckleLevel.cpp DespeckleLevel.h
    DewarpingView.cpp DewarpingView.h
    DewarpingOptions.cpp DewarpingOptions.h
    ChangeDewarpingDialog.cpp ChangeDewarpingDialog.h
    DepthPerception.cpp DepthPerception.h
    SplittingOptions.cpp SplittingOptions.h
    OtsuBinarizationOptionsWidget.cpp OtsuBinarizationOptionsWidget.h
    SauvolaBinarizationOptionsWidget.cpp SauvolaBinarizationOptionsWidget.h
    SauvolaModBinarizationOptionsWidget.cpp SauvolaModBinarizationOptionsWidget.h
    WolfBinarizationOptionsWidget.cpp WolfBinarizationOptionsWidget.h 
    BinarizationOptionsWidget.h 
    PictureShapeOptions.cpp PictureShapeOptions.h 
    OutputProcessingParams.cpp OutputProcessingParams.h
    OutputImage.h
    OutputImageWithForeground.h
    OutputImageWithOriginalBackground.h
    OutputImagePlain.cpp OutputImagePlain.h
    OutputImageWithForegroundMask.cpp OutputImageWithForegroundMask.h
    OutputImageWithOriginalBackgroundMask.cpp OutputImageWithOriginalBackgroundMask.h
    OutputImageBuilder.cpp OutputImageBuilder.h ForegroundType.h)

add_library(output STATIC ${sources} ${ui_files})
target_link_libraries(output PUBLIC core PRIVATE dewarping)
target_include_directories(output PRIVATE "$<TARGET_PROPERTY:TIFF::TIFF,INTERFACE_INCLUDE_DIRECTORIES>")

translation_sources(scantailor ${sources} ${ui_files})
