
#include <QCoreApplication>
#include <QDebug>

#include "QtHttpServer.h"

static QtHttpServer * server = NULL;

static void onServerStarted (quint16 port) {
    qDebug () << "QtHttpServer started on port" << port;
}

static void onServerStopped () {
    qDebug () << "QtHttpServer stopped";
}

static void onServerError (QString msg) {
    qDebug () << "QtHttpServer error :" << msg;
}

static void onClientConnected (QString guid) {
    qDebug () << "QtHttpServer client connected :" << guid;
}

static void onClientDisconnected (QString guid) {
    qDebug () << "QtHttpServer client disconnected :" << guid;
}

int main (int argc, char * argv []) {
    QCoreApplication app (argc, argv);

    server = new QtHttpServer (&app);

    QObject::connect (server, &QtHttpServer::started,            &onServerStarted);
    QObject::connect (server, &QtHttpServer::stopped,            &onServerStopped);
    QObject::connect (server, &QtHttpServer::error,              &onServerError);
    QObject::connect (server, &QtHttpServer::clientConnected,    &onClientConnected);
    QObject::connect (server, &QtHttpServer::clientDisconnected, &onClientDisconnected);

    server->start (1234);

    return app.exec ();
}

