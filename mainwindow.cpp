#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ProcessJson->setEnabled(false);
    downloader = new Downloader();
    connect(downloader, SIGNAL(download_finished_sgnl()), this, SLOT(enable_button()));
    connect(ui->ProcessJson, SIGNAL(clicked()),this, SLOT(show_json()));
    downloader->execute();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enable_button()
{
    ui->ProcessJson->setEnabled(true);
}

void MainWindow::show_json()
{
    QJsonObject jsonobject = downloader->loadedJson.object();
    qDebug()<<jsonobject;
    int j = 0;
    foreach(const QString& key, jsonobject.keys()) {
        QJsonValue value = jsonobject.value(key);
        qDebug() << "Key = " << key << ", Value = " << value;
        ui->listWidget->insertItem(j, key);
        j++;
    }
    QString forcast_url = jsonobject.value("properties").toObject().value("forecast").toString();
    qDebug()<<"url: " << forcast_url;

}

struct weather_data {
    QString name;
    double temperature;
    QString id;
    QString type;
    QString geometryType;
    QVector<double> coordinates;
    QString propertiesType;
    QString county;
    QString cwa;
    QString fireWeatherZone;
    QString forecast;
    QString forecastGridData;
    QString forecastHourly;
    QString forecastOffice;
    QString forecastZone;
    QString gridId;
    int gridX;
    int gridY;
    QString timeZone;
};

// the JSON-LD data
void WeatherData(const QByteArray& jsonData) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject root = doc.object();

    // Extract geometry
    QJsonObject geometry = root["geometry"].toObject();
    QString geometryType = geometry["type"].toString();
    QJsonArray coordinatesArray = geometry["coordinates"].toArray();
    QVector<double> coordinates;
    for (const QJsonValue& coordValue : coordinatesArray) {
        coordinates.append(coordValue.toDouble());
    }

    // Extract id and type
    QString id = root["id"].toString();
    QString type = root["type"].toString();

    // Extract properties
    QJsonObject properties = root["properties"].toObject();
    QString propertiesType = properties["@type"].toString(); // Assuming it's a string
    QString county = properties["county"].toString();
    QString cwa = properties["cwa"].toString();
    QString fireWeatherZone = properties["fireWeatherZone"].toString();
    QString forecast = properties["forecast"].toString();
    QString forecastGridData = properties["forecastGridData"].toString();
    QString forecastHourly = properties["forecastHourly"].toString();
    QString forecastOffice = properties["forecastOffice"].toString();
    QString forecastZone = properties["forecastZone"].toString();
    QString gridId = properties["gridId"].toString();
    int gridX = properties["gridX"].toInt();
    int gridY = properties["gridY"].toInt();



    // Extract timeZone
    QString timeZone = properties["timeZone"].toString();

    // Create a weather_data object
    weather_data weather;
    weather.geometryType = geometryType;
    weather.coordinates = coordinates;
    weather.id = id;
    weather.type = type;
    weather.propertiesType = propertiesType;
    weather.county = county;
    weather.cwa = cwa;
    weather.fireWeatherZone = fireWeatherZone;
    weather.forecast = forecast;
    weather.forecastGridData = forecastGridData;
    weather.forecastHourly = forecastHourly;
    weather.forecastOffice = forecastOffice;
    weather.forecastZone = forecastZone;
    weather.gridId = gridId;
    weather.gridX = gridX;
    weather.gridY = gridY;
   weather.timeZone = timeZone;

    // Example usage
    qDebug() << "Geometry Type: " << weather.geometryType;
    qDebug() << "Coordinates: " << weather.coordinates;
    qDebug() << "ID: " << weather.id;
    qDebug() << "Type: " << weather.type;
    qDebug() << "Properties Type: " << weather.propertiesType;
    qDebug() << "County: " << weather.county;
    qDebug() << "CWA: " << weather.cwa;
    qDebug() << "Fire Weather Zone: " << weather.fireWeatherZone;
    qDebug() << "Forecast: " << weather.forecast;
    qDebug() << "Forecast Grid Data: " << weather.forecastGridData;
    qDebug() << "Forecast Hourly: " << weather.forecastHourly;
    qDebug() << "Forecast Office: " << weather.forecastOffice;
    qDebug() << "Forecast Zone: " << weather.forecastZone;
    qDebug() << "Grid ID: " << weather.gridId;
    qDebug() << "Grid X: " << weather.gridX;
    qDebug() << "Grid Y: " << weather.gridY;
    qDebug() << "Time Zone: " << weather.timeZone;

    // Print or store other properties as required
}

// Example usage
int mainWindow() {
    QByteArray jsonData = R"(
        // Paste your JSON-LD data here
    )";

    WeatherData(jsonData);

    return 0;
}
