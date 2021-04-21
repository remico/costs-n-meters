#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDirIterator>
#include <QQmlContext>

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

#if defined Q_OS_ANDROID
    QDirIterator it("assets:/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << "@@" << it.next();
    }

    engine.rootContext()->setContextProperty("assets", "assets:");
    engine.addImportPath(QStringLiteral("assets:/qml"));
    const QUrl url(QStringLiteral("assets:/qml/main.qml"));
#else
    engine.rootContext()->setContextProperty("assets", "qrc:");
    engine.addImportPath(QStringLiteral("qrc:/qml"));
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
#endif

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
