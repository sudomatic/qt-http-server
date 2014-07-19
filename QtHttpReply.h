#ifndef QTHTTPREPLY_H
#define QTHTTPREPLY_H

#include <QObject>
#include <QHash>

class QtHttpServer;
class QtHttpReply : public QObject {
    Q_OBJECT
    Q_ENUMS (StatusCode)

public:
    explicit QtHttpReply (QtHttpServer * parent);

    enum StatusCode {
        Ok         = 200,
        BadRequest = 400,
        Forbidden  = 403,
        NotFound   = 404
    };

    int               getRawDataSize  () const;
    StatusCode        getStatusCode   () const;
    QByteArray        getResponseData () const;
    QList<QByteArray> getHeadersList  () const;

    QByteArray getHeader (QByteArray header) const;

    static const QByteArray getStatusTextForCode (StatusCode statusCode);

public slots:
    void setStatusCode    (StatusCode statusCode);
    void appendRawData    (QByteArray data);
    void addHeader        (QByteArray header, QByteArray value);

private:
    StatusCode                    m_statusCode;
    QByteArray                    m_data;
    QtHttpServer                * m_serverHandle;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREPLY_H
