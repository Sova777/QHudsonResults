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
#include <QDate>
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
    template_title[0] = QString::fromUtf8("Все тесты");
    template_regexp[0] = ".*";
    template_title[1] = settings.value("template1_title").toString();
    template_regexp[1] = settings.value("template1_regexp").toString();
    template_title[2] = settings.value("template2_title").toString();
    template_regexp[2] = settings.value("template2_regexp").toString();
    template_title[3] = settings.value("template3_title").toString();
    template_regexp[3] = settings.value("template3_regexp").toString();
    template_title[4] = settings.value("template4_title").toString();
    template_regexp[4] = settings.value("template4_regexp").toString();
    template_title[5] = settings.value("template5_title").toString();
    template_regexp[5] = settings.value("template5_regexp").toString();
    template_title[6] = settings.value("template6_title").toString();
    template_regexp[6] = settings.value("template6_regexp").toString();
    template_title[7] = settings.value("template7_title").toString();
    template_regexp[7] = settings.value("template7_regexp").toString();
    template_title[8] = settings.value("template8_title").toString();
    template_regexp[8] = settings.value("template8_regexp").toString();
    template_title[9] = settings.value("template9_title").toString();
    template_regexp[9] = settings.value("template9_regexp").toString();

    int h = settings.value("height").toInt();
    int w = settings.value("width").toInt();
    if ((h > 200) && (w > 200)) {
        QSize nSize(w, h);
        setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                nSize, qApp->desktop()->availableGeometry()));
    }

    for (int i = 0; i < 10; i++) {
        widget.cbMode->addItem(template_title[i]);
    }
    widget.table->setItemDelegate(new HtmlItemDelegate(widget.table));
    widget.table->setItemDelegateForColumn(0, new QItemDelegate(widget.table));
    widget.calendarWidget->setSelectedDate(widget.calendarWidget->selectedDate().addDays(-10));
    widget.spinBuilds->setValue(10);
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
            "QHudsonResults", "QHudsonResults");
    settings.setValue("width", width());
    settings.setValue("height", height());
    settings.setValue("data", data);
    settings.setValue("hudson", hudson);
    settings.setValue("bug1_link", bug1_link);
    settings.setValue("bug1_max", bug1_max);
    settings.setValue("bug2_link", bug2_link);
    settings.setValue("bug2_max", bug2_max);
    settings.setValue("bug3_link", bug3_link);
    settings.setValue("bug3_max", bug3_max);
    settings.setValue("template1_title", template_title[1]);
    settings.setValue("template1_regexp", template_regexp[1]);
    settings.setValue("template2_title", template_title[2]);
    settings.setValue("template2_regexp", template_regexp[2]);
    settings.setValue("template3_title", template_title[3]);
    settings.setValue("template3_regexp", template_regexp[3]);
    settings.setValue("template4_title", template_title[4]);
    settings.setValue("template4_regexp", template_regexp[4]);
    settings.setValue("template5_title", template_title[5]);
    settings.setValue("template5_regexp", template_regexp[5]);
    settings.setValue("template6_title", template_title[6]);
    settings.setValue("template6_regexp", template_regexp[6]);
    settings.setValue("template7_title", template_title[7]);
    settings.setValue("template7_regexp", template_regexp[7]);
    settings.setValue("template8_title", template_title[8]);
    settings.setValue("template8_regexp", template_regexp[8]);
    settings.setValue("template9_title", template_title[9]);
    settings.setValue("template9_regexp", template_regexp[9]);
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

void MainWindow::cellChanged(int row, int /*column*/, int /*previousRow*/, int /*previousColumn*/) {
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
                text += "<td>&nbsp;&nbsp;" + item.getTime().replace('_', ' ') + "</td>";
                text += "<td>&nbsp;&nbsp;" + item.getStatusImage() + "</td>";
                if (item.getBugId() == "null") {
                    text += "<td>&nbsp;&nbsp;</td>";
                } else {
                    int bug = item.getBugIdInt();
                    QString link;
                    if (bug <= bug1_max) {
                        link = QString(bug1_link).arg(bug);
                    } else if (bug <= bug2_max) {
                        link = QString(bug2_link).arg(bug);
                    } else {
                        link = QString(bug3_link).arg(bug);
                    }
                    text += QString("<td>&nbsp;&nbsp;%1</td>").arg(link);                        
                }
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
                            .arg(toHtml(message));
                }
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
    QString dateMin = widget.calendarWidget->selectedDate().toString("yyyy-MM-dd");
    QRegExp jobsRegExp(template_regexp[widget.cbMode->currentIndex()]);
    int builds = widget.spinBuilds->value();
    bool realNamesOnly = widget.cbRealName->isChecked();

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
            QString time = fields.at(1).trimmed();
            QString build = fields.at(2).trimmed();
            QString name = fields.at(3).trimmed();
            QString status = fields.at(4).trimmed();
            QString bugId = fields.at(5).trimmed();
            QString message = fields.at(6).trimmed();

            if (name.contains(testNameFilter, Qt::CaseInsensitive)) {
                if (!realNamesOnly || isRealName(name)) {
                    if (time > dateMin) {
                        testsList.push_back(TestItem(job, time, build, name, status, bugId, message));
                        QString key = QString(QString("%1|%2").arg(name).arg(job));
                        if (!allTests.contains(key)) {
                            allTests[key] = QList<int>();
                        }
                        allTests[key].append(counter);
                        if (allTests[key].size() > builds) {
                            allTests[key].removeFirst();
                        }
                        counter++;
                    }
                }
            }
        }
    }
    file.close();

    foreach (QString key, allTests.keys()) {
        int testTotal = 0;
        int testFailed = 0;
        foreach (int id, allTests[key]) {
            if (testsList[id].isFailed()) {
                testFailed++;
            }
            testTotal++;
        }
        int minPercents = widget.spinPercents->value();
        if ((100 * testFailed / testTotal) < minPercents) {
            allTests.remove(key);
        } else {
            foreach (int id, allTests[key]) {
                if (jobsRegExp.exactMatch(testsList[id].getJob())) {
                    jobsHash[testsList[id].getJob()] = 0;
                    testsHash[testsList[id].getName()] = 0;
                }
            }
        }
    }

    foreach (QString job, jobsHash.keys()) {
        if (!jobsRegExp.exactMatch(job)) {
            jobsHash.remove(job);
        }
    }

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

QString MainWindow::toHtml(QString& text) {
    return text.replace('&', "&amp;").replace('\"', "&quot;").replace('<', "&lt;").replace('>', "&gt;");
}

bool MainWindow::isRealName(QString& name) {
    if (name.endsWith(".BeforeFirstTest")) {
        return false;
    }
    if (name.endsWith(".unknown")) {
        return false;
    }
    return QRegExp("[_.0-9a-zA-Z]*").exactMatch(name);
}
