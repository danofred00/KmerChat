#include "configloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <exception>


using namespace Server;

ConfigLoader::ConfigLoader(QString path, QObject *parent)
    : QObject{parent}, mPath{path}, mIsLoaded{ false }
{

}

void ConfigLoader::load()
{
    QJsonDocument jsonDoc;
    QJsonParseError error;

    QFile configFile = QFile(mPath);
    if(!configFile.exists())
        throw std::runtime_error(QString("File %1 do not exists.").arg(mPath).toStdString());

    if(configFile.open(QFile::ReadOnly)) {

        jsonDoc = QJsonDocument::fromJson(configFile.readAll(), &error);

        if(error.error != QJsonParseError::NoError)
            return;

        mConfig = jsonDoc.object().toVariantMap();
        mIsLoaded = true;
        configFile.close();
    }
}

QVariantMap ConfigLoader::config() const
{
    if(mIsLoaded)
        return mConfig;
    throw std::runtime_error("Config not loaded");
}


