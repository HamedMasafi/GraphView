set(GRAPHVIEW_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc

    ${CMAKE_CURRENT_SOURCE_DIR}/core/jsonserialize.h ${CMAKE_CURRENT_SOURCE_DIR}/core/jsonserialize.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/abstractwidget.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/abstractwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/textnode.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/textnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/relation.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/relation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/abstractrectwidget.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/abstractrectwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/arrowhead.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/arrowhead.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/crossconnection.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/crossconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/multiline.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/multiline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/polylineitem.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/polylineitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/polygonitem.h ${CMAKE_CURRENT_SOURCE_DIR}/widgets/polygonitem.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/commands/addwidget.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/addwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/widgetsresizecommand.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/widgetsresizecommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/movewidgets.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/movewidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/addrelation.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/addrelation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/removewidget.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/removewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/splitrelation.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/splitrelation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/changerelation.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/changerelation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/removerelation.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/removerelation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/movehandlecommand.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/movehandlecommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commands/changewidgetsgeometry.h ${CMAKE_CURRENT_SOURCE_DIR}/commands/changewidgetsgeometry.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/data/abstractdata.h ${CMAKE_CURRENT_SOURCE_DIR}/data/abstractdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data/functiondata.h ${CMAKE_CURRENT_SOURCE_DIR}/data/functiondata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data/variabledata.h ${CMAKE_CURRENT_SOURCE_DIR}/data/variabledata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data/firmwaredata.h ${CMAKE_CURRENT_SOURCE_DIR}/data/firmwaredata.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/tools/abstracttool.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/abstracttool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/createwidgettool.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/createwidgettool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/selecttool.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/selecttool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/connectwidgets.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/connectwidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/widgetmove.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/widgetmove.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/editrelation.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/editrelation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/resizewidget.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/resizewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/resizehandlesprovider.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/resizehandlesprovider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/zoom.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/zoom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/itemremover.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/itemremover.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/magnifier.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/magnifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/abstracttoolscontainer.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/abstracttoolscontainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/crosspointer.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/crosspointer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/createpolyline.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/createpolyline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/colorpicker.h ${CMAKE_CURRENT_SOURCE_DIR}/tools/colorpicker.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/core/pointshelper.h ${CMAKE_CURRENT_SOURCE_DIR}/core/pointshelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/painterhelper.h ${CMAKE_CURRENT_SOURCE_DIR}/core/painterhelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/recthelper.h ${CMAKE_CURRENT_SOURCE_DIR}/core/recthelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/abstractserializable.h ${CMAKE_CURRENT_SOURCE_DIR}/core/abstractserializable.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/scene.h ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/itemresizer.h ${CMAKE_CURRENT_SOURCE_DIR}/itemresizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphview.h ${CMAKE_CURRENT_SOURCE_DIR}/graphview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphviewglobal.h ${CMAKE_CURRENT_SOURCE_DIR}/graphviewglobal.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/handles/polylinehandle.h ${CMAKE_CURRENT_SOURCE_DIR}/handles/polylinehandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/handles/resizehandle.h ${CMAKE_CURRENT_SOURCE_DIR}/handles/resizehandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/handles/abstracthandle.h ${CMAKE_CURRENT_SOURCE_DIR}/handles/abstracthandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/handles/connectionhandle.h ${CMAKE_CURRENT_SOURCE_DIR}/handles/connectionhandle.cpp
)

set(GRAPHVIEW_SOURCE ${GRAPHVIEW_SOURCE} PARENT_SCOPE)

add_library(GraphView STATIC ${GRAPHVIEW_SOURCE}
    tools/nodestool.h tools/nodestool.cpp
    commands/removenodescommand.h commands/removenodescommand.cpp
    commands/nodescommands.h commands/nodescommands.cpp
    widgets/abstractconnectablewidget.h widgets/abstractconnectablewidget.cpp
    widgets/gridview.h widgets/gridview.cpp
    widgets/widgetviewproxy.h widgets/widgetviewproxy.cpp
    tools/movebymodiftool.h tools/movebymodiftool.cpp
    commands/changepropertycommand.h commands/changepropertycommand.cpp
    core/graphview_global.h
    thumbnailview.h thumbnailview.cpp
    tools/movewidgetsbyarrowkeys.h tools/movewidgetsbyarrowkeys.cpp
    commands/clonewidgets.h commands/clonewidgets.cpp
    scene_p.h
    commands/addremovewidgets.h commands/addremovewidgets.cpp
    batchcreator.h batchcreator.cpp
)
target_include_directories(GraphView PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GraphView PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
