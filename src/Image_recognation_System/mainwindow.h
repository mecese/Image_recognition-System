#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsView>
#include<QPrinter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:

signals:
    void mySignal(bool x);

private:
    Ui::MainWindow *ui;
    QImage *image;
    QGraphicsView * graphicview;
    QString saveFileName;
    QGraphicsScene *scene;
    QStringList* qstrl;
    QList<QAction*> qlqa;
private:
    bool outputFile();
    void saveRecentFile(const QStringList &qsl);
    void readFromRecentFile( QStringList & qsl);
    QImage * open(QString opFileName);
    void recentFileChanged();
private slots:
    void openFileSlot();
    void saveFileSlot();
    bool saveAsFileSlot();
    void printSlot();
    void printPreviewSlot();
    void printSettingSlot();
    void recentFileSlot();
    void exitSlot();
    void paintRequestedSlot(QPrinter * printer);
//    void manageActionWithRctFile();
    void activeRctFileSlot(bool x);
};

#endif // MAINWINDOW_H
