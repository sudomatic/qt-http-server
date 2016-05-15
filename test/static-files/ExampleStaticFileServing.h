#ifndef EXAMPLESTATICFILESERVING_H
#define EXAMPLESTATICFILESERVING_H

#include <QObject>
#include <QMimeDatabase>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

class ExampleStaticFileServing : public QObject {
    Q_OBJECT

public:
    explicit ExampleStaticFileServing (QString baseUrl, QObject * parent = NULL);
    virtual ~ExampleStaticFileServing (void);

public slots:
    void onServerStopped      (void);
    void onServerStarted      (quint16 port);
    void onServerError        (QString msg);
    void onRequestNeedsReply  (QtHttpRequest * request, QtHttpReply * reply);

private:
    QString         m_baseUrl;
    QtHttpServer  * m_server;
    QMimeDatabase * m_mimeDb;
};

#endif // EXAMPLESTATICFILESERVING_H
