import qbs 1.0;

Project {
    name: "The Qt5 HTTP Server";

    property bool buildAsSharedLib : true;

    Product {
        name: "qt5-http-server-lib";
        type: (project.buildAsSharedLib ? "dynamiclibrary" : "staticlibrary");
        targetName: "Qt5HttpServer";
        cpp.includePaths: ".";

        Depends {
            name: "cpp";
        }
        Depends {
            name: "Qt";
            submodules: ["core", "network"];
        }
        Group {
            name: "C++ Sources";
            files: ["Qt*.cpp"];
        }
        Group {
            name: "C++ Headers";
            files: ["Qt*.h"]
        }
        Group {
            qbs.install: true;
            fileTagsFilter: (project.buildAsSharedLib ? "dynamiclibrary" : "staticlibrary");
        }
    }
    Application {
        name: "qt5-http-server-test-app";
        targetName: "Qt5HttpServerTestApp";

        Depends {
            name: "cpp";
        }
        Depends {
            name: "Qt";
            submodules: ["core", "network"];
        }
        Depends {
            name: "qt5-http-server-lib";
        }

        Group {
            name: "C++ Sources";
            files: ["Example*.cpp", "main.cpp"];
        }
        Group {
            name: "C++ Headers";
            files: ["Example*.h"]
        }
        Group {
            qbs.install: true;
            fileTagsFilter: "application";
        }
    }
}
