
#include <QCoreApplication>
#include <QDir>

#include "ExampleRequestParsing.h"

int main (int argc, char * argv []) {
    QCoreApplication app (argc, argv);
    new ExampleRequestParsing (&app);
    return app.exec ();
}

