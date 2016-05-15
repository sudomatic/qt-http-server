import qbs;

Project {
    name: "The Qt5 HTTP Server";
    references: [
        "lib/libQt5HttpServer.qbs",
        "test/static-files/testAppQtHttpStaticFiles.qbs",
        "test/request-parsing/testAppQtHttpRequestParsing.qbs",
    ]

    property bool buildAsSharedLib : true;
}
