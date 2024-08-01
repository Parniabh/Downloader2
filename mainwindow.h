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
    //QVector<weatherDatapoint> weatherforecast;
    QJsonArray forecastArray;

private:
    Ui::MainWindow *ui;
    Downloader *downloader;
    QMap<QString, QJsonValue> jsonValues;
    void showDatainList();
    void inigetForecastURL(const QString &url);

    struct WeatherDataPoint {
        QString startTime;
        QString endTime;
        double temperature;
        QString weather;
        double probabilityOfPrecipitation;
        QString relativeHumidity;
    };


    QJsonArray getForecastArray(const QString &url);
    QString forecastURL;
    QVector<weatherDatapoint> weatherData;
public slots:
    void enable_button();
    void show_json();
    void onlistchanged(QListWidgetItem *item);
    void process_forecast_data();
    void getForecastURL();
    void getWeatherPrediction();
;

};

#endif // MAINWINDOW_H
