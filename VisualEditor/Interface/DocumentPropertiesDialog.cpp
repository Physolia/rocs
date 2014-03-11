/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DocumentPropertiesDialog.h"

#include "DataTypePage.h"
#include "PointerTypePage.h"
#include <Data.h>
#include "Scene/DataItem.h"

#include <QWidget>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <KLocalizedString>
#include <KGuiItem>
#include <KStandardGuiItem>

DocumentPropertiesDialog::DocumentPropertiesDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Document Properties"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget(this);
    _dataTypePage = new DataTypePage(this);
    _pointerTypePage = new PointerTypePage(this);
    tabWidget->addTab(_dataTypePage, i18nc("@title:tab", "Data Types"));
    tabWidget->addTab(_pointerTypePage, i18nc("@title:tab", "Pointer Types"));
    mainLayout->addWidget(tabWidget);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QPushButton *closeButton = new QPushButton;
    KGuiItem::assign(closeButton, KStandardGuiItem::close());
    closeButton->setShortcut(Qt::CTRL | Qt::Key_Return);

    buttons->addButton(closeButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    setAttribute(Qt::WA_DeleteOnClose);
}


void DocumentPropertiesDialog::setDocument(Document* document)
{
    Q_ASSERT(document);
    _dataTypePage->setDocument(document);
    _pointerTypePage->setDocument(document);
}


void DocumentPropertiesDialog::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10,  screenPosition.y() + 10);
}
