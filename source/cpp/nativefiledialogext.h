#ifndef NATIVEFILEDIALOGEXT_H
#define NATIVEFILEDIALOGEXT_H

#include <QObject>
#include <QQuickItem>
#include <QStringList>
#include <QVariantMap>
#include <QStandardPaths>
#include <QtQuick/QQuickItem>


// Component that works as wrapper to call the native file dialogue. This class intentionally
// mimics the basic interface of FileDialog, such as support for multiple selection,
// folder, shortcuts, fileUrls and how the dialog is triggered
class NativeFileDialogExt : public QQuickItem {

    Q_OBJECT

    Q_PROPERTY(QString fileUrl READ fileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QStringList fileUrls READ fileUrls NOTIFY fileUrlsChanged)
    Q_PROPERTY(bool selectMultiple READ selectMultiple WRITE setSelectMultiple NOTIFY selectMultipleChanged)
    Q_PROPERTY(QUrl folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QVariantMap shortcuts READ shortcuts CONSTANT FINAL)

public:
    explicit NativeFileDialogExt(QQuickItem * parent = 0);
    static NativeFileDialogExt * qmlAttachedProperties(QObject *object);
    Q_INVOKABLE void open();
    QString fileUrl();
    QStringList fileUrls();
    bool selectMultiple();
    QUrl folder();
    QVariantMap shortcuts();
    void componentComplete();

public slots:
    void setFileUrl(QString url);
    void setFileUrls(QStringList urls);
    void setSelectMultiple(bool multiple);
    void setFolder(QUrl folder);

private slots:
    void onVisibleChanged();

private:
    QStringList m_fileUrls;
    bool m_selectMultiple;
    QUrl m_folder;
    QVariantMap m_shortcuts;

signals:
    void fileUrlChanged();
    void fileUrlsChanged();
    void folderChanged();
    void selectMultipleChanged();
};

QML_DECLARE_TYPEINFO(NativeFileDialogExt, QML_HAS_ATTACHED_PROPERTIES)

#endif // NATIVEFILEDIALOGEXT_H
