#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

enum HashAlgorithm {
    MD5, SHA256
};

namespace Core::Utils {

const QString hash(const QByteArray & data, HashAlgorithm algorithm = HashAlgorithm::SHA256);

} // namespace Core::Utils

#endif // UTILS_H
