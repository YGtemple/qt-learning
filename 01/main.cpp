#include "myproject.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Qt 标准的多语言装配流程：
    // 遍历系统 UI 语言列表，找到匹配的 .qm 翻译文件就装上。
    // 翻译文件在编译时通过 lrelease 从 01_zh_CN.ts 生成，
    // embed_translations 会把它编进二进制，所以用 ":/i18n/" 资源路径加载。
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "01_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    myproject w;
    w.show();
    return a.exec();
}
