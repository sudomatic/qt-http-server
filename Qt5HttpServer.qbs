import qbs 1.0;

Project {
    name: "The Qt5 HTTP Server";

    StaticLibrary {
        name: "qt5-http-server-lib";
        targetName: "Qt5HttpServer";
        cpp.includePaths: ["."];

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
    }
    Application {
        name: "qt5-http-server-test-app";
        targetName: "Qt5HttpServerTestApp";
        files: [
            "ExampleRequestParsing.cpp",
            "ExampleRequestParsing.h",
            "ExampleStaticFileServing.cpp",
            "ExampleStaticFileServing.h",
            "main.cpp",
        ]

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
    }
}
