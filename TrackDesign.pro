QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -execution-charset:utf-8

VERSION = 21.02.25.001_Beta

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:\GDAL\GDAL232\include
INCLUDEPATH += D:\VTK9_0_1/include/vtk-9.0

DESTDIR = ../bin

SOURCES += \
    Parser/dataparser.cpp \
    Public/designtrackstyle.cpp \
    Reader/rasterfilereader.cpp \
    Utils/coordinatetransform.cpp \
    Utils/dataexport.cpp \
    Utils/topologyutil.cpp \
    Widget/widgetopengl.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Base/defines.h \
    Parser/dataparser.h \
    Public/designtrackstyle.h \
    Public/threadpool.h \
    Reader/rasterfilereader.h \
    Utils/coordinatetransform.h \
    Utils/dataexport.h \
    Utils/topologyutil.h \
    Widget/widgetopengl.h \
    mainwindow.h

FORMS += \
    Widget/widgetopengl.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -LD:\GDAL\GDAL232\lib \
-lgdal_i

LIBS += -LD:/VTK9_0_1/lib_release \
-lvtkChartsCore-9.0 \
-lvtkCommonColor-9.0 \
-lvtkCommonComputationalGeometry-9.0 \
-lvtkCommonCore-9.0 \
-lvtkCommonDataModel-9.0 \
-lvtkCommonExecutionModel-9.0 \
-lvtkCommonMath-9.0 \
-lvtkCommonMisc-9.0 \
-lvtkCommonSystem-9.0 \
-lvtkCommonTransforms-9.0 \
-lvtkDICOMParser-9.0 \
-lvtkDomainsChemistry-9.0 \
-lvtkdoubleconversion-9.0 \
-lvtkexodusII-9.0 \
-lvtkexpat-9.0 \
-lvtkFiltersAMR-9.0 \
-lvtkFiltersCore-9.0 \
-lvtkFiltersExtraction-9.0 \
-lvtkFiltersFlowPaths-9.0 \
-lvtkFiltersGeneral-9.0 \
-lvtkFiltersGeneric-9.0 \
-lvtkFiltersGeometry-9.0 \
-lvtkFiltersHybrid-9.0 \
-lvtkFiltersHyperTree-9.0 \
-lvtkFiltersImaging-9.0 \
-lvtkFiltersModeling-9.0 \
-lvtkFiltersParallel-9.0 \
-lvtkFiltersParallelImaging-9.0 \
-lvtkFiltersPoints-9.0 \
-lvtkFiltersProgrammable-9.0 \
-lvtkFiltersSelection-9.0 \
-lvtkFiltersSMP-9.0 \
-lvtkFiltersSources-9.0 \
-lvtkFiltersStatistics-9.0 \
-lvtkFiltersTexture-9.0 \
-lvtkFiltersTopology-9.0 \
-lvtkFiltersVerdict-9.0 \
-lvtkfreetype-9.0 \
-lvtkGeovisCore-9.0 \
-lvtkgl2ps-9.0 \
-lvtkglew-9.0 \
-lvtkGUISupportQt-9.0 \
-lvtkGUISupportQtSQL-9.0 \
-lvtkhdf5-9.0 \
-lvtkhdf5_hl-9.0 \
-lvtkImagingColor-9.0 \
-lvtkImagingCore-9.0 \
-lvtkImagingFourier-9.0 \
-lvtkImagingGeneral-9.0 \
-lvtkImagingHybrid-9.0 \
-lvtkImagingMath-9.0 \
-lvtkImagingMorphological-9.0 \
-lvtkImagingSources-9.0 \
-lvtkImagingStatistics-9.0 \
-lvtkImagingStencil-9.0 \
-lvtkInfovisCore-9.0 \
-lvtkInfovisLayout-9.0 \
-lvtkInteractionImage-9.0 \
-lvtkInteractionStyle-9.0 \
-lvtkInteractionWidgets-9.0 \
-lvtkIOAMR-9.0 \
-lvtkIOAsynchronous-9.0 \
-lvtkIOCityGML-9.0 \
-lvtkIOCore-9.0 \
-lvtkIOEnSight-9.0 \
-lvtkIOExodus-9.0 \
-lvtkIOExport-9.0 \
-lvtkIOExportGL2PS-9.0 \
-lvtkIOExportPDF-9.0 \
-lvtkIOGeometry-9.0 \
-lvtkIOImage-9.0 \
-lvtkIOImport-9.0 \
-lvtkIOInfovis-9.0 \
-lvtkIOLegacy-9.0 \
-lvtkIOLSDyna-9.0 \
-lvtkIOMINC-9.0 \
-lvtkIOMotionFX-9.0 \
-lvtkIOMovie-9.0 \
-lvtkIONetCDF-9.0 \
-lvtkIOOggTheora-9.0 \
-lvtkIOParallel-9.0 \
-lvtkIOParallelXML-9.0 \
-lvtkIOPLY-9.0 \
-lvtkIOSegY-9.0 \
-lvtkIOSQL-9.0 \
-lvtkIOTecplotTable-9.0 \
-lvtkIOVeraOut-9.0 \
-lvtkIOVideo-9.0 \
-lvtkIOXML-9.0 \
-lvtkIOXMLParser-9.0 \
-lvtkjpeg-9.0 \
-lvtkjsoncpp-9.0 \
-lvtklibharu-9.0 \
-lvtklibproj-9.0 \
-lvtklibxml2-9.0 \
-lvtkloguru-9.0 \
-lvtklz4-9.0 \
-lvtklzma-9.0 \
-lvtkmetaio-9.0 \
-lvtknetcdf-9.0 \
-lvtkogg-9.0 \
-lvtkParallelCore-9.0 \
-lvtkParallelDIY-9.0 \
-lvtkpng-9.0 \
-lvtkpugixml-9.0 \
-lvtkRenderingAnnotation-9.0 \
-lvtkRenderingContext2D-9.0 \
-lvtkRenderingContextOpenGL2-9.0 \
-lvtkRenderingCore-9.0 \
-lvtkRenderingFreeType-9.0 \
-lvtkRenderingGL2PSOpenGL2-9.0 \
-lvtkRenderingImage-9.0 \
-lvtkRenderingLabel-9.0 \
-lvtkRenderingLOD-9.0 \
-lvtkRenderingOpenGL2-9.0 \
-lvtkRenderingQt-9.0 \
-lvtkRenderingSceneGraph-9.0 \
-lvtkRenderingUI-9.0 \
-lvtkRenderingVolume-9.0 \
-lvtkRenderingVolumeOpenGL2-9.0 \
-lvtkRenderingVtkJS-9.0 \
-lvtksqlite-9.0 \
-lvtksys-9.0 \
-lvtkTestingRendering-9.0 \
-lvtktheora-9.0 \
-lvtktiff-9.0 \
-lvtkverdict-9.0 \
-lvtkViewsContext2D-9.0 \
-lvtkViewsCore-9.0 \
-lvtkViewsInfovis-9.0 \
-lvtkViewsQt-9.0 \
-lvtkWrappingTools-9.0 \
-lvtkzlib-9.0

