#ifndef NATIVEFILEDIALOGANDROID_H
#define NATIVEFILEDIALOGANDROID_H

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include <QStringList>
#include <QUrl>
#include <QAndroidActivityResultReceiver>


// Android's file picker action through JNI
class NativeFileDialogAndroid: public QObject, QAndroidActivityResultReceiver {

    Q_OBJECT
    static const int PICK_FILE = 0xFFF;

private:
    static QString filePath(QAndroidJniObject & uri);
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    bool m_selectMultiple;

public:
    NativeFileDialogAndroid(bool selectMultiple, QObject *parent = 0);
    void open(QUrl initialFolder);

signals:
    void selectedFilePaths(QStringList result);
};

#endif // NATIVEFILEDIALOGANDROID_H
