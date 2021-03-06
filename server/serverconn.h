#ifndef SERVERCONN_H
#define SERVERCONN_H

#include "../cih/globalConf.h"

#include <QObject>
#include <QMap>
#include <QThread>
#include <QMessageBox>

class ServerConn : public QObject {
    Q_OBJECT

    friend class ServerConnBroadcaster;
public:
    ServerConn();
    explicit ServerConn(QObject *parent);

    void start(QString name);
    void beginAcceptConnection();
    void endAcceptConnection();
    void close();

    void sendData(Conn dest, byteseq data, int length);

    void broadcast(byteseq data, int length);

signals:
    void onClientConnected(Conn remote);
    void onClientDisconnected(Conn remote);
    void onClientData(Conn remote, byteseq data, int length);

    void _onReqSend();

private slots:
    void _send();

public slots:
    void socketReady();
    void socketDisconnected();

    void newConn();

private:
    QString name;

    enum class serverStatus {
        Closed,
        Started,
        Listening,
    };

    serverStatus status;

    void sendDataBySocket(QTcpSocket *sock, QByteArray data);

    void emitOnClientConnected(Conn remote);
    void emitOnClientDisconnected(Conn remote);
    void emitOnClientData(Conn remote, byteseq data, int length);

    QHostAddress broadcastAddress;

    ServerConnBroadcaster *threadBroadcaster;
    QTcpServer *serv;
    int counter;

    QMap<int, QTcpSocket*> clients;
    QMap<QTcpSocket*, int> ids;
    QMap<int, Conn> conns;
    QSet<QTcpSocket*> removing;

    QMap<QTcpSocket*, QByteArray> cache;

    struct sendCacheFr {
        QTcpSocket *remote;
        QByteArray data;

        sendCacheFr(QTcpSocket *r, QByteArray d);
    };

    QQueue<sendCacheFr> sendCache;
    QMutex sendCacheLock;

    void PkgHandler(QTcpSocket* sock);
};

// Sub-class definition

class ServerConnBroadcaster : public QThread {
    Q_OBJECT
public:
    ServerConnBroadcaster() = delete;
    explicit ServerConnBroadcaster(QObject *parent = 0, ServerConn *conn = 0);
private :
    ServerConn *conn;
protected:
    void run();
};

#endif // SERVERCONN_H
