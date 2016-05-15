import qbs;

Project {
    name: "The Qt5 HTTP Server";
    references: [
        "libQt5HttpServer.qbs",
        "testAppQt5HttpServer.qbs",
    ]

    property bool buildAsSharedLib : true;
}
