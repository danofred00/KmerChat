#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QObject>
#include <QVariantMap>

namespace Server {

class ConfigLoader : public QObject
{
    Q_OBJECT
public:

    ConfigLoader(QString path, QObject *parent = nullptr);

    const QString path() const { return mPath; }
    void setPath(const QString & path) { mPath = path; }

    void load();
    QVariantMap config() const;

    bool isLoaded() const { return mIsLoaded; }

private:
    QString mPath;
    bool mIsLoaded;
    QVariantMap mConfig;
};

} // namespace Server;

#endif // CONFIGLOADER_H
