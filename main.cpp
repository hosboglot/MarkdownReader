#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif  //Q_OS_ANDROID

#include "src/repositoryhandler.h"

#ifdef Q_OS_ANDROID
bool checkPermissions()
{
    bool success = true;
    if(QtAndroid::androidSdkVersion() >= 23)
    {
        static const QVector<QString> permissions({
            "android.permission.READ_EXTERNAL_STORAGE"
        });

        for(const QString &permission : permissions)
        {
            // check if permission is granded
            auto result = QtAndroid::checkPermission(permission);
            if(result != QtAndroid::PermissionResult::Granted)
            {
                // request permission
                auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
                if(resultHash[permission] != QtAndroid::PermissionResult::Granted)
                {
                    qDebug() << "Fail to get permission" << permission;
                    success = false;
                }
                else
                {
                    qDebug() << "Permission" << permission << "granted!";
                }
            }
            else
            {
                qDebug() << "Permission" << permission << "already granted!";
            }
        }
    }
    return success;
}
#endif  //Q_OS_ANDROID

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<RepositoryHandler>("repositoryHandler", 1, 0, "RepositoryHandler");

#ifdef Q_OS_ANDROID
    checkPermissions();
#endif  //Q_OS_ANDROID

    app.setOrganizationDomain("org");
    app.setOrganizationName("qtproject");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
