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

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TestItem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    Ui::MainWindow widget;

private:
    QMap<QString, int> jobsHash;
    QMap<QString, int> testsHash;
    QList<TestItem> testsList;
    QMap<QString, QList<int> > allTests;
    QStringList jobNames;
    QString data;
    QString hudson;
    QString bug1_link;
    int bug1_max;
    QString bug2_link;
    int bug2_max;
    QString bug3_link;
    int bug3_max;
    QString template_title[10];
    QString template_regexp[10];

    void closeEvent(QCloseEvent *event);
    void readFile(const QString& fileName);
    void applyFilters();
    void displayTests();
    QString toHtml(QString& text);
    QString linkToBug(QString bugId);
    bool isRealName(QString& name);

private slots:
    void about();
    void refresh();
    void displayBugs();
    void cellChanged(int row, int /*column*/, int /*previousRow*/, int /*previousColumn*/);
    void linkActivated(const QUrl& link);

};

#endif	/* _MAINWINDOW_H */
