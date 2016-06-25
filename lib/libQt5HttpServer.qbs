import qbs;

Product {
    name: "qt5-http-server-lib";
    type: (project.buildAsSharedLib ? "dynamiclibrary" : "staticlibrary");
    targetName: "Qt5HttpServer";
    cpp.includePaths: ".";
    cpp.cxxLanguageVersion: (Qt.core.versionMajor >= 5 && Qt.core.versionMinor >= 7 ? "c++11" : "c++98");
    cpp.cxxStandardLibrary: "libstdc++"; // NOTE : because there are issues with libc++

    Export {
        cpp.includePaths: ".";
        cpp.cxxLanguageVersion: (Qt.core.versionMajor >= 5 && Qt.core.versionMinor >= 7 ? "c++11" : "c++98");
        cpp.cxxStandardLibrary: "libstdc++"; // NOTE : because there are issues with libc++

        Depends {
            name: "cpp";
        }
        Depends {
            name: "Qt";
            submodules: ["core", "network"];
        }
    }
    Depends {
        name: "cpp";
    }
    Depends {
        name: "Qt";
        submodules: ["core", "network"];
    }
    Group {
        name: "C++ Sources";
        files: [
            "QtHttpClientWrapper.cpp",
            "QtHttpHeader.cpp",
            "QtHttpReply.cpp",
            "QtHttpRequest.cpp",
            "QtHttpServer.cpp",
        ]
    }
    Group {
        name: "C++ Headers";
        files: [
            "QtHttpClientWrapper.h",
            "QtHttpHeader.h",
            "QtHttpReply.h",
            "QtHttpRequest.h",
            "QtHttpServer.h",
        ]
    }
    Group {
        qbs.install: true;
        fileTagsFilter: product.type;
    }
}
