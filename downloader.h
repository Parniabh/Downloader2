#include <QtCore>
#include <QtNetwork>
#include <QJsonDocument>
#include <cstdio>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

using namespace std;

class Downloader: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;
    void loadtoJson(const QUrl &url);
public:
    Downloader();
    void doDownload(const QUrl &url);
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
};
