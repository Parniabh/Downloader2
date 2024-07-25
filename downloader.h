#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtCore>
#include <QtNetwork>
#include <QJsonDocument>
#include <cstdio>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

using namespace std;

class Downloader : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

public:
    Downloader();
    Downloader(const QString &url);
    void doDownload(const QUrl &url);
    void setUrl(const QString &url);
    static QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    QJsonDocument loadedJson;

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

signals:
    void download_finished_sgnl();

private:
    QString downloadUrl;
};

#endif // DOWNLOADER_H
