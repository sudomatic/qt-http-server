#ifndef EXAMPLEREQUESTPARSING_H
#define EXAMPLEREQUESTPARSING_H

#include <QObject>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

class ExampleRequestParsing : public QObject {
    Q_OBJECT

public:
    explicit ExampleRequestParsing (QObject * parent = NULL);
    virtual ~ExampleRequestParsing (void);

public slots:
    void onServerStopped      (void);
    void onServerStarted      (quint16 port);
    void onServerError        (QString msg);
    void onClientConnected    (QString guid);
    void onClientDisconnected (QString guid);
    void onRequestNeedsReply  (QtHttpRequest * request, QtHttpReply * reply);

private:
    QtHttpServer * server;
};

#endif // EXAMPLEREQUESTPARSING_H
