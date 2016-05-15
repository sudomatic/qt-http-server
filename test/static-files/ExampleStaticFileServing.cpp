
#include "ExampleStaticFileServing.h"

#include <QStringBuilder>
#include <QUrlQuery>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QFile>

ExampleStaticFileServing::ExampleStaticFileServing (QString baseUrl, QObject * parent)
    : QObject   (parent)
    , m_baseUrl (baseUrl)
{
    m_mimeDb = new QMimeDatabase;

    m_server = new QtHttpServer (this);
    m_server->setServerName (QStringLiteral ("Qt Static HTTP File Server"));

    connect (m_server, &QtHttpServer::started,           this, &ExampleStaticFileServing::onServerStarted);
    connect (m_server, &QtHttpServer::stopped,           this, &ExampleStaticFileServing::onServerStopped);
    connect (m_server, &QtHttpServer::error,             this, &ExampleStaticFileServing::onServerError);
    connect (m_server, &QtHttpServer::requestNeedsReply, this, &ExampleStaticFileServing::onRequestNeedsReply);

    m_server->start (8765);
}

ExampleStaticFileServing::~ExampleStaticFileServing () {
    m_server->stop ();
}

void ExampleStaticFileServing::onServerStarted (quint16 port) {
    qDebug () << "QtHttpServer started on port" << port << m_server->getServerName ();
}

void ExampleStaticFileServing::onServerStopped () {
    qDebug () << "QtHttpServer stopped" << m_server->getServerName ();
}

void ExampleStaticFileServing::onServerError (QString msg) {
    qDebug () << "QtHttpServer error :" << msg;
}

static inline void printErrorToReply (QtHttpReply * reply, QString errorMessage) {
    reply->addHeader ("Content-Type", QByteArrayLiteral ("text/plain"));
    reply->appendRawData (errorMessage.toLocal8Bit ());
}

void ExampleStaticFileServing::onRequestNeedsReply (QtHttpRequest * request, QtHttpReply * reply) {
    QString command = request->getCommand ();
    if (command == QStringLiteral ("GET")) {
        QString path = request->getUrl ().path ();
        QFile file (m_baseUrl % "/" % path);
        if (file.exists ()) {
            QMimeType mime = m_mimeDb->mimeTypeForFile (file.fileName ());
            if (file.open (QFile::ReadOnly)) {
                QByteArray data = file.readAll ();
                reply->addHeader ("Content-Type", mime.name ().toLocal8Bit ());
                reply->appendRawData (data);
                file.close ();
            }
            else {
                printErrorToReply (reply, "Requested file " % path % " couldn't be open for reading !");
            }
        }
        else {
            printErrorToReply (reply, "Requested file " % path % " couldn't be found !");
        }
    }
    else {
        printErrorToReply (reply, "Unhandled HTTP/1.1 method " % command % " on static file server !");
    }
}

