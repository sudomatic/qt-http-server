
#include <QCoreApplication>
#include <QDebug>
#include <QStringBuilder>
#include <QUrlQuery>
#include <QList>
#include <QPair>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

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

    reply->appendRawData (QByteArrayLiteral ("<html>"));
    reply->appendRawData (QByteArrayLiteral ("<head>"));
    reply->appendRawData (QByteArrayLiteral ("<title>Testing Qt5 HTTP Server</title>"));
    reply->appendRawData (QByteArrayLiteral ("<style>pre { border: 1px solid darkgray; background: lightgray; padding: 10px; } td,th { border: 1px solid gray; text-align: center; }</style>"));
    reply->appendRawData (QByteArrayLiteral ("</head>"));
    reply->appendRawData (QByteArrayLiteral ("<body style='margin: 50px'>"));
    reply->appendRawData (QByteArrayLiteral ("<h1>It Works !</h1>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<hr />\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h2>HTTP command: ")       % request->getCommand ().toUtf8 ()             % QByteArrayLiteral ("</h2>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h3>Requested URL: ")      % request->getUrl ().toString ().toUtf8 ()     % QByteArrayLiteral ("</h3>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<table style='border: 1px solid gray'>"));
    reply->appendRawData (QByteArrayLiteral ("<tr>"));
    reply->appendRawData (QByteArrayLiteral ("<th colspan='2'>URL parameters</th>"));
    reply->appendRawData (QByteArrayLiteral ("</tr>"));
    reply->appendRawData (QByteArrayLiteral ("<tr>"));
    reply->appendRawData (QByteArrayLiteral ("<th>Key</th>"));
    reply->appendRawData (QByteArrayLiteral ("<th>Value</th>"));
    reply->appendRawData (QByteArrayLiteral ("</tr>"));
    QUrlQuery params (request->getUrl ());
    QList<QPair<QString, QString> > pairsList = params.queryItems ();
    for (int idx = 0; idx < pairsList.size (); idx++) {
        QPair<QString, QString> pair = pairsList.at (idx);
        reply->appendRawData (QByteArrayLiteral ("<tr>"));
        reply->appendRawData (QByteArrayLiteral ("<td>") % pair.first.toUtf8 ()  % QByteArrayLiteral ("</td>"));
        reply->appendRawData (QByteArrayLiteral ("<td>") % pair.second.toUtf8 () % QByteArrayLiteral ("</td>"));
        reply->appendRawData (QByteArrayLiteral ("</tr>"));
    }
    reply->appendRawData (QByteArrayLiteral ("</table>"));
    reply->appendRawData (QByteArrayLiteral ("<h4>Virtual host: ")       % request->getHeader (QtHttpHeader::Host)      % QByteArrayLiteral ("</h4>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h5>Your User-Agent: ")    % request->getHeader (QtHttpHeader::UserAgent) % QByteArrayLiteral ("</h5>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h6>Request raw data:\r\n</h6>"));
    reply->appendRawData (QByteArrayLiteral ("<pre>")                    % request->getRawData ()                       % QByteArrayLiteral ("</pre>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<br />\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h2>Server name : ")       % reply->getHeader (QtHttpHeader::Server)      % QByteArrayLiteral ("</h2>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<h3>Reply creation : ")    % reply->getHeader (QtHttpHeader::Date)        % QByteArrayLiteral ("</h3>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<pre>")                    % QByteArrayLiteral ("OK.")                    % QByteArrayLiteral ("</pre>\r\n"));
    reply->appendRawData (QByteArrayLiteral ("<br />\r\n"));
    reply->appendRawData (QByteArrayLiteral ("</body>"));
    reply->appendRawData (QByteArrayLiteral ("</html>"));
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

    server->setServerName (QStringLiteral ("My Test Qt HTTP Server"));
    server->start         (0);

    return app.exec ();
}

