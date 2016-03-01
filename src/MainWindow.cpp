/*
Copyright (c) 2016, Valeriy Soldatov
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the football.mojgorod.ru nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFile>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <QTextStream>

#include "MainWindow.h"
#include "TestItem.h"

#include "ui_MainWindow.h"
#include "HtmlItemDelegate.h"

using namespace std;

MainWindow::MainWindow() {

    widget.setupUi(this);

    connect(widget.actionExit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(widget.actionAbout, SIGNAL(triggered()),
            this, SLOT(about()));
    connect(widget.btnRefresh, SIGNAL(clicked()),
            this, SLOT(refresh()));
    connect(widget.table, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(cellChanged(int, int, int, int)));
    connect(widget.text, SIGNAL(anchorClicked(const QUrl &)),
            this, SLOT(linkActivated(const QUrl &)));

    QRect rectangle = frameGeometry();
    rectangle.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rectangle.topLeft());

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
            "QHudsonResults", "QHudsonResults");
    data = settings.value("data").toString();
    if (data == "") {
        QString applicationDir = QCoreApplication::applicationDirPath();
        if (applicationDir != "") {
            if (QFile::exists(applicationDir + "/dump.txt")) {
                data = applicationDir + "/dump.txt";
            }
        }
    }
    hudson = settings.value("hudson").toString();
    bug1_link = settings.value("bug1_link").toString();
    bug1_max = settings.value("bug1_max").toInt();
    bug2_link = settings.value("bug2_link").toString();
    bug2_max = settings.value("bug2_max").toInt();
    bug3_link = settings.value("bug3_link").toString();
    bug3_max = settings.value("bug3_max").toInt();

    widget.table->setItemDelegate(new HtmlItemDelegate(widget.table));
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
            "QHudsonResults", "QHudsonResults");
    settings.setValue("data", data);
    settings.setValue("hudson", hudson);
    settings.setValue("bug1_link", bug1_link);
    settings.setValue("bug1_max", bug1_max);
    settings.setValue("bug2_link", bug2_link);
    settings.setValue("bug2_max", bug2_max);
    settings.setValue("bug3_link", bug3_link);
    settings.setValue("bug3_max", bug3_max);
    settings.sync();
    event->accept();
}

void MainWindow::about() {
    QMessageBox::about(this, QString::fromUtf8("О программе QHudsonResults"), QString::fromUtf8("<h2>QHudsonResults</h2><p>Анализ хадсоновских результатов тестов</p>"));
}

void MainWindow::refresh() {
    widget.table->setDisabled(true);
    widget.table->clear();
    widget.table->setRowCount(0);
    widget.table->setSortingEnabled(false);
    readFile(data);
    widget.table->setSortingEnabled(true);
    widget.table->sortByColumn(0, Qt::AscendingOrder);
    widget.table->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    widget.table->setEnabled(true);
}

void MainWindow::cellChanged(int row, int column, int previousRow, int previousColumn) {
    QModelIndex index = widget.table->model()->index(row, 0);
    QString name = widget.table->model()->data(index).toString();
    QString text = QString("<h1>%1</h1>").arg(name);
    QString methodName = name.section(".", -1, -1);
    QString fileName = name.section(".", -2, -2);
    QString packageName = name.section(".", 0, -3);

    foreach (QString jobName, jobNames) {
        QString key = QString(QString("%1|%2").arg(name).arg(jobName));
        if (allTests.contains(key)) {
            text += QString("<h2>%1</h2>").arg(jobName);
            QList<int> testItems = allTests.value(key);
            text += "<table>";
            foreach (int testIndex, testItems) {
                text += "<tr>";
                TestItem item = testsList[testIndex];
                text += "<td>&nbsp;&nbsp;" + item.getBuild() + "</td>";
                text += "<td>&nbsp;&nbsp;" + item.getStatusImage() + "</td>";
                QString message = item.getMessage();
                if (message == "null") {
                    message = QString("<a href=\"%1%2/%3/testReport/%4/%5/%6\">OK</a>")
                            .arg(hudson)
                            .arg(jobName)
                            .arg(item.getBuild())
                            .arg(packageName)
                            .arg(fileName)
                            .arg(methodName);
                } else {
                    message = QString("<a href=\"%1%2/%3/testReport/%4/%5/%6\">%7</a>")
                            .arg(hudson)
                            .arg(jobName)
                            .arg(item.getBuild())
                            .arg(packageName)
                            .arg(fileName)
                            .arg(methodName)
                            .arg(message);
                }
//                text += "<td>&nbsp;&nbsp;" + item.getBugId() + "</td>";
                text += "<td>&nbsp;&nbsp;" + message + "</td>";
                text += "</tr>";
            }
            text += "</table>";
        }
    }
    widget.text->setText(text);
}

void MainWindow::linkActivated(const QUrl& link) {
    QDesktopServices::openUrl(link);
    return;
}

void MainWindow::readFile(const QString& fileName) {
    QString testNameFilter = widget.editTestName->text();

    jobsHash.clear();
    testsHash.clear();
    testsList.clear();
    allTests.clear();
    jobNames.clear();

    int counter = 0;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream  in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains(testNameFilter, Qt::CaseInsensitive)) {
            QStringList fields = line.split(",");
            QString job = fields.at(0).trimmed();
            QString build = fields.at(2).trimmed();
            QString name = fields.at(3).trimmed();
            QString status = fields.at(4).trimmed();
            QString bugId = fields.at(5).trimmed();
            QString message = fields.at(6).trimmed();

            if (name.contains(testNameFilter, Qt::CaseInsensitive)) {
                testsList.push_back(TestItem(job, build, name, status, bugId, message));
                jobsHash[job] = 0;
                testsHash[name] = 0;
                QString key = QString(QString("%1|%2").arg(name).arg(job)); 
                if (!allTests.contains(key)) {
                    allTests[key] = QList<int>();
                }
                allTests[key].append(counter);
                counter++;
            }
        }
    }
    file.close();

    int sizeJobs = jobsHash.size();
    widget.table->setRowCount(testsHash.size());
    widget.table->setColumnCount(sizeJobs + 1);
    widget.table->setColumnWidth(0, 400);
    widget.table->horizontalHeader()->setFixedHeight(40);
    for (int i = 1; i < sizeJobs; i++) {
        widget.table->setColumnWidth(i, 120);
    }

    QStringList headers = QStringList();
    headers.append(QString::fromUtf8("Имя теста"));
    counter = 0;
    foreach (QString job, jobsHash.keys()) {
        headers.append(job);
        jobNames.append(job);
        jobsHash[job] = counter;
        counter++;
    }
    widget.table->setHorizontalHeaderLabels(headers);

    counter = 0;
    foreach (QString test, testsHash.keys()) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(test);
        widget.table->setItem(counter, 0, item1);

        int column = 0;
        foreach (QString jobName, jobNames) {
            QString key = QString(QString("%1|%2").arg(test).arg(jobName));
            QString cellValue("");
            if (allTests.contains(key)) {
                QList<int> testItems = allTests.value(key);
                foreach (int testIndex, testItems) {
                    cellValue += testsList[testIndex].getStatusImage();
                }
            }
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(cellValue);
            widget.table->setItem(counter, column + 1, item);
            column++;
        }

        counter++;
    }
}
