
#include <QCoreApplication>
#include <QDir>

#include "ExampleRequestParsing.h"
#include "ExampleStaticFileServing.h"

int main (int argc, char * argv []) {
    QCoreApplication app (argc, argv);
    //new ExampleRequestParsing (&app);
    new ExampleStaticFileServing (QDir::homePath (), &app);
    return app.exec ();
}

