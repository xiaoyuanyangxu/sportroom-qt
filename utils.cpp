#include "utils.h"
#include <QColor>
#include <QDebug>
#include <QFileInfo>

QString Utils::localizeFile(QString remoteFile,
                                  const QString remoteAssetDir,
                                  const QString localAssetDir)
{
    qDebug() << Q_FUNC_INFO << remoteFile <<":"<< remoteAssetDir << ":"<<localAssetDir;
    if (QFileInfo::exists(remoteFile)){
        return remoteFile;
    }
    if (!localAssetDir.isEmpty() && !remoteAssetDir.isEmpty()){
        if (remoteFile.startsWith(remoteAssetDir)){
            QString resultFile = localAssetDir + remoteFile.right(remoteFile.length() - remoteAssetDir.length());
            qDebug() << Q_FUNC_INFO <<  remoteFile << "->" << resultFile;
            return resultFile;
        }
    }
    return remoteFile;
}

