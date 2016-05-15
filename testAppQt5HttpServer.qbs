import qbs;

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
