#include "utils.h"

#include <QCryptographicHash>

namespace Core::Utils {

const QString hash(const QByteArray & data, HashAlgorithm algorithm)
{
    QByteArray result;

    switch (algorithm) {
    case MD5:
        result = QCryptographicHash::hash(data, QCryptographicHash::Md5);
        break;
    case SHA256:
        result = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
        break;
    default:
        break;
    }

    return QString(result.toHex());
}

} // namespace Core::Utils
