
#include <QCoreApplication>
#include <QDebug>
#include <QStringBuilder>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"

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

static void onRequestNeedsReply (QtHttpRequest * request, QtHttpReply * reply) {
    qDebug () << "QtHttpServer client request :" << request;

    reply->appendRawData (QByteArrayLiteral ("<h1>Hello !</h1>"));
    reply->appendRawData (QByteArrayLiteral ("<h4>Requested URL: ") % request->getUrl ().toString ().toUtf8 () % QByteArrayLiteral ("</h4>"));
    reply->appendRawData (QByteArrayLiteral ("<h4>Your User-Agent: ") % request->getHeader (QByteArrayLiteral ("User-Agent")) % QByteArrayLiteral ("</h4>"));
    reply->appendRawData (QByteArrayLiteral ("<span>OK.</span>"));
}

int main (int argc, char * argv []) {
    QCoreApplication app (argc, argv);

    server = new QtHttpServer (&app);

    QObject::connect (server, &QtHttpServer::started,            &onServerStarted);
    QObject::connect (server, &QtHttpServer::stopped,            &onServerStopped);
    QObject::connect (server, &QtHttpServer::error,              &onServerError);
    QObject::connect (server, &QtHttpServer::clientConnected,    &onClientConnected);
    QObject::connect (server, &QtHttpServer::clientDisconnected, &onClientDisconnected);
    QObject::connect (server, &QtHttpServer::requestNeedsReply,  &onRequestNeedsReply);

    server->start (1234);

    return app.exec ();
}

