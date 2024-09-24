#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QtTextToSpeech/qtexttospeech.h>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),speech(new QTextToSpeech(this))
{
    ui->setupUi(this);

    // 初始为不可重复点名模式
    allowRepeats = false;
    // 初始化已选名单为空
    selectedStudents.clear();

    setupTextToSpeech();
    loadStudentNames();

    // 连接两个按钮的信号到对应槽函数
    connect(ui->pushButtonPick, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->pushButtonMode, &QPushButton::clicked, this, &MainWindow::toggleMode);


}
void MainWindow::setupTextToSpeech() {
    // 配置语音属性
    speech->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    speech->setVolume(1.0);  // 设置音量
    speech->setPitch(0.5);   // 设置音调
    speech->setRate(0.3);    // 设置语速

    // 可选：列出所有可用的语音
    foreach (const QVoice &voice, speech->availableVoices()) {
        qDebug() << "Voice name:" << voice.name();
        qDebug() << "Language:" << voice.locale();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStudentNames() {
    QFile file("D:\\qtproject\\choose\\名单.txt");  // 打开文本文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->label->setText("无法打开文件");
        qDebug() << "无法打开文件:" << file.errorString();

        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();  // 读取每一行并去掉空格
        if (!line.isEmpty()) {
            ui->listWidget->addItem(line);  // 将名字添加到 QListWidget
        }
    }

    file.close();  // 关闭文件
}

void MainWindow::onButtonClicked()
{
    int itemCount = ui->listWidget->count();
    if (itemCount == 0) {
        ui->label->setText("名单为空");
        return;
    }

    // 如果是不可重复模式，收集未被选中的学生
    QList<int> availableIndexes;
    if (!allowRepeats) {
        for (int i = 0; i < itemCount; ++i) {
            if (!selectedStudents.contains(i)) {  // 检查是否已被选中
                availableIndexes.append(i);
            }
        }

        // 如果所有学生都已被选过
        if (availableIndexes.isEmpty()) {
            ui->label->setText("所有学生都已点过！");
            return;
        }
    }

    // 随机选择一个学生（根据当前模式选择从完整列表或未选名单）
    std::uniform_int_distribution<std::size_t> dist(0, allowRepeats ? itemCount - 1 : availableIndexes.size() - 1);
    int randomIndex = allowRepeats ? dist(rng) : availableIndexes[dist(rng)];

    // 获取并显示随机选中的学生姓名
    QListWidgetItem *item = ui->listWidget->item(randomIndex);
    QString randomStudent = item->text();
    ui->label->setText(randomStudent);
    // 将选中的学生添加到 "点到的人" 列表中
    ui->listWidgetPicked->addItem(randomStudent);
    speech->say(randomStudent);

    // 如果不可重复模式，将该学生添加到已选名单中
    if (!allowRepeats) {
        selectedStudents.insert(randomIndex);
    }
}

void MainWindow::toggleMode()
{
    // 切换模式
    allowRepeats = !allowRepeats;

    // 清空已选名单（如果从不可重复模式切换回来需要重置）
    if (allowRepeats) {
        ui->listWidgetPicked->clear();
        ui->labelMode->setText("当前模式：可重复点名");
    } else {
        ui->listWidgetPicked->clear();
        selectedStudents.clear();
        ui->labelMode->setText("当前模式：不可重复点名");
    }
}



