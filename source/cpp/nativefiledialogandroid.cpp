#include "nativefiledialogandroid.h"
#include <qdebug.h>
#include <QStandardPaths>


void NativeFileDialogAndroid::open(QUrl initialFolder=QUrl()) {

    QAndroidJniObject pickAction = QAndroidJniObject::fromString("android.intent.action.PICK");

    // making sure the pick action is available
    if (pickAction.isValid()) {

        QAndroidJniObject intent("android/content/Intent");
        QString contentType;

        // querying the android version to make sure the multiple selection feature is available
        if (m_selectMultiple && QtAndroid::androidSdkVersion() >= 19) {
            intent.callObjectMethod("putExtra","(Ljava/lang/String;Z)Landroid/content/Intent;",
                QAndroidJniObject::getStaticObjectField("android/content/Intent", "EXTRA_ALLOW_MULTIPLE", "Ljava/lang/String;").object<jobject>(), true);
        }

        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", pickAction.object<jstring>());

        // pictures
        if (initialFolder == QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation))) {
            contentType = "image/*";
        }

        // pictures & videos
        // Using MoviesLocation to also let the user to pick images, the reason for this is
        // the fact that the current available shortcuts in Qt are too desktop centric
        else if (initialFolder == QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation))) {
            contentType = "image/*,video/*";
        }

        // ...
        else {
            contentType = "*/*";
        }

        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString(contentType).object<jstring>());

        // starting the action
        QtAndroid::startActivity(intent.object<jobject>(), PICK_FILE, this);
    }
    else {
        throw "Could not load pick action!";
    }
}

void NativeFileDialogAndroid::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) {

    QStringList result;

    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == PICK_FILE && resultCode == RESULT_OK) {

        bool singleFile = true;

        // multiple selection mode
        if (m_selectMultiple) {
            QAndroidJniObject clipData = data.callObjectMethod("getClipData","()Landroid/content/ClipData;");

            // making sure the multiple selection is really available on the current android,
            // this also depends on the gallery app that is used to pick the files, otherwise
            // it fallbacks to single file
            if (clipData.isValid()) {
                singleFile = false;

                int clipDataTotal = clipData.callMethod<jint>("getItemCount");

                // querying the selected uris
                for (int i=0; i < clipDataTotal; i++){
                    QAndroidJniObject itemSelected = clipData.callObjectMethod(
                                "getItemAt",
                                "(I)Landroid/content/ClipData$Item;", i);

                    QAndroidJniObject pathSelected = itemSelected.callObjectMethod("getUri", "()Landroid/net/Uri;");

                    QString path = NativeFileDialogAndroid::filePath(pathSelected);

                    // making sure the path is not empty (invalid)
                    if (!path.isEmpty()) {
                        result.append(path);
                    }
                }
            }
        }

        // single selection mode
        if (singleFile) {

            QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");

            QString path = NativeFileDialogAndroid::filePath(uri);

            // making sure the path is not empty (invalid)
            if (!path.isEmpty()){
                result.append(path);
            }
        }
    }

    emit selectedFilePaths(result);
}

QString NativeFileDialogAndroid::filePath(QAndroidJniObject & uri) {

    // in case the uri is already resolved
    if (uri.toString().startsWith("file:")) {

        return uri.callObjectMethod("getPath", "()Ljava/lang/String;").toString();
    }

    // otherwise query the file path through content resolver
    else {

        QAndroidJniEnvironment env;

        QAndroidJniObject mediaStore = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;");

        jobjectArray projection = (jobjectArray)env->NewObjectArray(1, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        env->SetObjectArrayElement(projection, 0, mediaStore.object<jstring>());

        QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

        QAndroidJniObject cursor = contentResolver.callObjectMethod("query",
            "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;",
            uri.object<jobject>(), projection, NULL, NULL, NULL);

        if (cursor.isValid()) {

            cursor.callMethod<jboolean>("moveToFirst");

            jint columnIndex = cursor.callMethod<jint>("getColumnIndexOrThrow","(Ljava/lang/String;)I", mediaStore.object<jstring>());

            QAndroidJniObject path = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);

            if (!cursor.callMethod<jboolean>("isClosed")) {

                cursor.callObjectMethod("close", "()Z");
            }

            // making sure the path has been resolved
            if (path.isValid()) {
                QString filePath = path.toString();

                if (!filePath.isEmpty()) {
                    filePath = "file://" + filePath;
                    return filePath;
                }
            }
        }

        return QString();
    }
}

NativeFileDialogAndroid::NativeFileDialogAndroid(bool selectMultiple, QObject *parent): QObject(parent) {

    m_selectMultiple = selectMultiple;
}
