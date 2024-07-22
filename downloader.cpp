#include "downloader.h"

Downloader::Downloader()
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(downloadFinished(QNetworkReply*)));


}

void Downloader::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors,
            this, &Downloader::sslErrors);
#endif

    currentDownloads.append(reply);
}

QString Downloader::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool Downloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void Downloader::execute()
{
    QStringList args;
    args.append("https://api.weather.gov/points/39.7456,-97.0892");
    args.append("https://api.weather.gov/points/39.7456,-97.0892");
    args.takeFirst();           // skip the first argument, which is the program's name
    if (args.isEmpty()) {
        printf("Qt Download example - downloads all URLs in parallel\n"
               "Usage: download url1 [url2... urlN]\n"
               "\n"
               "Downloads the URLs passed in the command-line to the local directory\n"
               "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
               "differentiate.\n");
        QCoreApplication::instance()->quit();
        return;
    }

    for (const QString &arg : qAsConst(args)) {
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}

void Downloader::loadtoJson(const QUrl &url)
{

}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {
            QString filename = "/home/parnia/Documents/TestWeather.json";
            loadedJson = QJsonDocument::fromJson(reply->readAll());
            qDebug()<<loadedJson;
            QFile jsonFile(filename);
            jsonFile.open(QFile::WriteOnly);
            jsonFile.write(loadedJson.toJson());
            jsonFile.close();
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();
    emit(download_finished_sgnl());

}
