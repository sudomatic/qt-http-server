
#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpClientWrapper.h"

#include <QDebug>

const QString QtHttpServer::s_httpVersion = QStringLiteral ("HTTP/1.1");

QtHttpServer::QtHttpServer (QObject * parent)
    : QObject (parent)
{
    m_sockServer = new QTcpServer (this);
    connect (m_sockServer, &QTcpServer::newConnection, this, &QtHttpServer::onClientConnected);
}

QtHttpServer::~QtHttpServer () {
    stop ();
}

const QString QtHttpServer::getHttpVersion () {
    return s_httpVersion;
}

void QtHttpServer::start (quint16 port) {
    if (m_sockServer->listen (QHostAddress::Any, port)) {
        emit started (m_sockServer->serverPort ());
    }
    else {
        emit error   (m_sockServer->errorString ());
    }
}

void QtHttpServer::stop () {
    if (m_sockServer->isListening ()) {
        m_sockServer->close ();
        emit stopped ();
    }
}

void QtHttpServer::onClientConnected () {
    while (m_sockServer->hasPendingConnections ()) {
        QTcpSocket * sockClient = m_sockServer->nextPendingConnection ();
        QtHttpClientWrapper * wrapper = new QtHttpClientWrapper (sockClient, this);
        connect (sockClient, &QTcpSocket::disconnected, this, &QtHttpServer::onClientDisconnected);
        m_socksClientsHash.insert (sockClient, wrapper);
        emit clientConnected (wrapper->getGuid ());
    }
}

void QtHttpServer::onClientDisconnected () {
    QTcpSocket * sockClient = qobject_cast<QTcpSocket *> (sender ());
    if (sockClient) {
        QtHttpClientWrapper * wrapper = m_socksClientsHash.value (sockClient, NULL);
        if (wrapper) {
            emit clientDisconnected (wrapper->getGuid ());
            wrapper->deleteLater ();
            m_socksClientsHash.remove (sockClient);
        }
    }
}
