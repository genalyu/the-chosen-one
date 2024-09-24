#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QtTextToSpeech/qtexttospeech.h>
#include <random>
#include <QListWidget>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();  // 点名按钮的槽函数
    void toggleMode();       // 模式切换按钮的槽函数

private:
    QTextToSpeech *speech;

    void setupTextToSpeech();
    void loadStudentNames();
    Ui::MainWindow *ui;

    bool allowRepeats;  // 用于切换可重复/不可重复点名模式
    QSet<int> selectedStudents;  // 已被选过的学生索引
    std::mt19937 rng;  // 随机数生成器
};

#endif // MAINWINDOW_H
