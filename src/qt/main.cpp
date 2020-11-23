#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDirIterator>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("assets:/qml"));

    const QUrl url(QStringLiteral("assets:/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    QDirIterator it("assets:/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << "@@" << it.next();
    }


    return app.exec();
}
