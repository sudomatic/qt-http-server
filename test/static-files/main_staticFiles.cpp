
#include <QCoreApplication>
#include <QDir>

#include "ExampleStaticFileServing.h"

int main (int argc, char * argv []) {
    QCoreApplication app (argc, argv);
    new ExampleStaticFileServing (QDir::homePath (), &app);
    return app.exec ();
}

