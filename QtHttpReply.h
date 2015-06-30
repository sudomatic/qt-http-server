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
    bool              useChunked      () const;
    StatusCode        getStatusCode   () const;
    QByteArray        getRawData      () const;
    QList<QByteArray> getHeadersList  () const;

    QByteArray getHeader (QByteArray header) const;

    static const QByteArray getStatusTextForCode (StatusCode statusCode);

public slots:
    void setUseChunked    (bool chunked = false);
    void setStatusCode    (StatusCode statusCode);
    void appendRawData    (QByteArray data);
    void addHeader        (QByteArray header, QByteArray value);
    void resetRawData     (void);

signals:
    void requestSendHeaders ();
    void requestSendData    ();

private:
    bool                          m_useChunked;
    StatusCode                    m_statusCode;
    QByteArray                    m_data;
    QtHttpServer                * m_serverHandle;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREPLY_H
