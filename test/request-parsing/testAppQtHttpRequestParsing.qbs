import qbs;

Application {
    name: "app-test-qt5-http-request-parsing";
    targetName: "testQt5HttpRequestParsing";

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
        files: [
            "ExampleRequestParsing.cpp",
            "main_requestParsing.cpp",
        ]
    }
    Group {
        name: "C++ Headers";
        files: [
            "ExampleRequestParsing.h",
        ]
    }
    Group {
        qbs.install: true;
        fileTagsFilter: "application";
    }
}
