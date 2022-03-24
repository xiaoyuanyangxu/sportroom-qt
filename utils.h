#ifndef UTILS_H
#define UTILS_H

#include <QString>



class Utils
{
public:
    static QString localizeFile(QString remoteFile,
                                const QString remoteAssetDir,
                                const QString localAssetDir);
};

#endif // UTILS_H
