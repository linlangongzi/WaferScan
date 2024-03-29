/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "utils_global.h"

#include <QTemporaryFile>

#include <memory>

namespace Utils {

class QTCREATOR_UTILS_EXPORT SaveFile : public QFile
{
    Q_OBJECT

public:
    explicit SaveFile(const QString &filename);
    ~SaveFile() override;

    bool open(OpenMode flags = QIODevice::WriteOnly) override;

    void rollback();
    bool commit();

    static void initializeUmask();

private:
    const QString m_finalFileName;
    std::unique_ptr<QTemporaryFile> m_tempFile;
    bool m_finalized;
    static QFile::Permissions m_umask;
};

} // namespace Utils
