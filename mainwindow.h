#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloader.h"
#include "QListWidgetItem"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct weatherDatapoint
{
    QString startTime;
    QString endTime;
    double probability;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<weatherDatapoint> weatherforecast;
    QJsonArray forecastArray;
private:
    Ui::MainWindow *ui;
    Downloader *downloader;
    QMap<QString, QJsonValue> jsonValues;
    void showDatainList();
    QString getForecastURL(const QString &url);
    QJsonArray getForecastArray(const QString &url);
public slots:
    void enable_button();
    void show_json();
    void onlistchanged(QListWidgetItem *item);
    void process_forecast_data();
};

#endif // MAINWINDOW_H
