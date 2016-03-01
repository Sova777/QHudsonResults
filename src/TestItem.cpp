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

#include <QString>
#include "TestItem.h"

TestItem::TestItem(QString _job, QString _time, QString _build, QString _name,
        QString _status, QString _bugId, QString _message) {
    job = _job;
    time = _time;
    build = _build;
    name = _name;
    status = _status;
    bugId = _bugId;
    message = _message;
}

QString TestItem::getBugId() const {
    return bugId;
}

int TestItem::getBugIdInt() const {
    return bugId.toInt();
}

QString TestItem::getTime() const {
    return time;
}

QString TestItem::getBuild() const {
    return build;
}

QString TestItem::getJob() const {
    return job;
}

QString TestItem::getMessage() const {
    return message;
}

QString TestItem::getName() const {
    return name;
}

QString TestItem::getStatus() const {
    return status;
}

QString TestItem::getStatusChar() const {
    if (status == "passed") {
        return "V";
    }
    if (status == "known") {
        return "=";
    }
    if (status == "failed") {
        return "X";
    }
    if (status == "absent") {
        return "!";
    }
    return status;
}

QString TestItem::getStatusImage() const {
    if (status == "passed") {
        return "<img src=':/icon/images/passed.png'>";
    }
    if (status == "known") {
        return "<img src=':/icon/images/known.png'>";
    }
    if (status == "failed") {
        return "<img src=':/icon/images/failed.png'>";
    }
    if (status == "absent") {
        return "<img src=':/icon/images/absent.png'>";
    }
    return status;
}
