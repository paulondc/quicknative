#include "nativefiledialogext.h"
#include <QQmlInfo>

#ifdef Q_OS_ANDROID
    #include "nativefiledialogandroid.h"
#endif


NativeFileDialogExt::NativeFileDialogExt(QQuickItem * parent): m_selectMultiple(false), QQuickItem(parent) {

    // file dialog is not visible by default
    setVisible(false);

    // also, providing the same shortcuts found in the file dialog (for convenience)
    m_shortcuts.insert("desktop", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
    m_shortcuts.insert("documents", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)));
    m_shortcuts.insert("home", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));
    m_shortcuts.insert("music", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MusicLocation)));
    m_shortcuts.insert("movies", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)));
    m_shortcuts.insert("pictures", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)));
}

void NativeFileDialogExt::componentComplete() {

    // in case the component is set to visible by default
    onVisibleChanged();

    // there are two ways to trigger the picker, either by calling .open() or making it visible,
    // for this reason hooking the visibleChanged signal
    connect(this, SIGNAL(visibleChanged()), this, SLOT(onVisibleChanged()));
}

void NativeFileDialogExt::onVisibleChanged() {

    if (isVisible())
        open();
}

void NativeFileDialogExt::open() {

    #ifdef Q_OS_ANDROID
        NativeFileDialogAndroid * picker = new NativeFileDialogAndroid(m_selectMultiple);
    #endif

    connect(picker, SIGNAL(selectedFilePaths(QStringList)), this, SLOT(setFileUrls(QStringList)));

    // launching the native picker dialog
    try {
        picker->open(m_folder);
    }
    catch (const char* exceptionMessage) {
        qDebug() << exceptionMessage;
    }
}

QString NativeFileDialogExt::fileUrl() {

    return m_fileUrls.size() ? m_fileUrls[0] : QString();
}

QStringList NativeFileDialogExt::fileUrls() {

    return m_fileUrls;
}

bool NativeFileDialogExt::selectMultiple() {

    return m_selectMultiple;
}

QUrl NativeFileDialogExt::folder() {

    return m_folder;
}

QVariantMap NativeFileDialogExt::shortcuts() {

    return m_shortcuts;
}

void NativeFileDialogExt::setFileUrl(QString url) {

    if (url.isEmpty()) {
        setFileUrls(QStringList());
    }
    else{
        setFileUrls(QStringList(url));
    }
}

void NativeFileDialogExt::setFileUrls(QStringList urls) {

    if (m_selectMultiple) {
        m_fileUrls = urls;
        emit fileUrlsChanged();
    }
    else{
        m_fileUrls = urls.size() ? QStringList(urls[0]) : QStringList();
    }

    emit fileUrlChanged();
}

void NativeFileDialogExt::setSelectMultiple(bool multiple) {

    m_selectMultiple = multiple;

    emit selectMultipleChanged();
}

void NativeFileDialogExt::setFolder(QUrl folder) {

    m_folder = folder;

    emit folderChanged();
}

NativeFileDialogExt* NativeFileDialogExt::qmlAttachedProperties(QObject * object) {

    return new NativeFileDialogExt();
}

static void registerNativeFileDialogExt() {
    // overriding the qml version when this component is avaiable
    qmlRegisterType<NativeFileDialogExt>("QuickNative", 0, 1, "NativeFileDialog");
}

Q_COREAPP_STARTUP_FUNCTION(registerNativeFileDialogExt)
