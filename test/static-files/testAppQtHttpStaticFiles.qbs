import qbs;

Application {
    name: "app-test-qt5-http-static-files";
    targetName: "testQt5HttpStaticFiles";

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
            "ExampleStaticFileServing.cpp",
            "main_staticFiles.cpp",
        ]
    }
    Group {
        name: "C++ Headers";
        files: [
            "ExampleStaticFileServing.h",
        ]
    }
    Group {
        qbs.install: true;
        fileTagsFilter: "application";
    }
}
