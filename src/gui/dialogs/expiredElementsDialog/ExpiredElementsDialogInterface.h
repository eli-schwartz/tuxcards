#ifndef EXPIREDELEMENTSDIALOGINTERFACE_H
#define EXPIREDELEMENTSDIALOGINTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ExpiredElementsDialogInterface
{
public:
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QLabel *textLabel1;
    QHBoxLayout *hboxLayout;
    QTreeWidget *pExpiryList;
    QVBoxLayout *vboxLayout1;
    QPushButton *pRemoveFromListButton;
    QSpacerItem *spacerItem;
    QLabel *textLabel6_2_2_2_2_2_2;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout2;
    QLabel *pElementIconLabel;
    QLabel *pElementNameLabel;
    QSpacerItem *spacerItem1;
    QLabel *textLabel4;
    QLabel *pChildCountLabel;
    QTextEdit *pTextEdit;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *ExpiredElementsDialogInterface)
    {
    ExpiredElementsDialogInterface->setObjectName(QString::fromUtf8("ExpiredElementsDialogInterface"));
    ExpiredElementsDialogInterface->resize(QSize(449, 450).expandedTo(ExpiredElementsDialogInterface->minimumSizeHint()));
    ExpiredElementsDialogInterface->setSizeGripEnabled(true);
    widget = new QWidget(ExpiredElementsDialogInterface);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(9, 29, 431, 139));
    vboxLayout = new QVBoxLayout(ExpiredElementsDialogInterface);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(8);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    textLabel1 = new QLabel(ExpiredElementsDialogInterface);
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
    QFont font;
    font.setFamily(QString::fromUtf8(""));
    font.setPointSize(12);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setWeight(50);
    font.setStrikeOut(false);
    textLabel1->setFont(font);
    textLabel1->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(textLabel1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    pExpiryList = new QTreeWidget(ExpiredElementsDialogInterface);
    pExpiryList->setObjectName(QString::fromUtf8("pExpiryList"));
    pExpiryList->setMinimumSize(QSize(200, 0));

    hboxLayout->addWidget(pExpiryList);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(11);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    pRemoveFromListButton = new QPushButton(ExpiredElementsDialogInterface);
    pRemoveFromListButton->setObjectName(QString::fromUtf8("pRemoveFromListButton"));

    vboxLayout1->addWidget(pRemoveFromListButton);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem);

    textLabel6_2_2_2_2_2_2 = new QLabel(ExpiredElementsDialogInterface);
    textLabel6_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel6_2_2_2_2_2_2"));

    vboxLayout1->addWidget(textLabel6_2_2_2_2_2_2);


    hboxLayout->addLayout(vboxLayout1);


    vboxLayout->addLayout(hboxLayout);

    groupBox = new QGroupBox(ExpiredElementsDialogInterface);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    hboxLayout1 = new QHBoxLayout(groupBox);
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(8);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(11);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    pElementIconLabel = new QLabel(groupBox);
    pElementIconLabel->setObjectName(QString::fromUtf8("pElementIconLabel"));

    hboxLayout2->addWidget(pElementIconLabel);

    pElementNameLabel = new QLabel(groupBox);
    pElementNameLabel->setObjectName(QString::fromUtf8("pElementNameLabel"));

    hboxLayout2->addWidget(pElementNameLabel);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);

    textLabel4 = new QLabel(groupBox);
    textLabel4->setObjectName(QString::fromUtf8("textLabel4"));

    hboxLayout2->addWidget(textLabel4);

    pChildCountLabel = new QLabel(groupBox);
    pChildCountLabel->setObjectName(QString::fromUtf8("pChildCountLabel"));

    hboxLayout2->addWidget(pChildCountLabel);


    vboxLayout2->addLayout(hboxLayout2);

    pTextEdit = new QTextEdit(groupBox);
    pTextEdit->setObjectName(QString::fromUtf8("pTextEdit"));
    pTextEdit->setReadOnly(true);

    vboxLayout2->addWidget(pTextEdit);


    hboxLayout1->addLayout(vboxLayout2);


    vboxLayout->addWidget(groupBox);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    spacerItem2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem2);

    buttonOk = new QPushButton(ExpiredElementsDialogInterface);
    buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
    buttonOk->setAutoDefault(true);
    buttonOk->setDefault(true);

    hboxLayout3->addWidget(buttonOk);

    buttonCancel = new QPushButton(ExpiredElementsDialogInterface);
    buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
    buttonCancel->setAutoDefault(true);

    hboxLayout3->addWidget(buttonCancel);


    vboxLayout->addLayout(hboxLayout3);

    retranslateUi(ExpiredElementsDialogInterface);

    QMetaObject::connectSlotsByName(ExpiredElementsDialogInterface);
    } // setupUi

    void retranslateUi(QDialog *ExpiredElementsDialogInterface)
    {
    ExpiredElementsDialogInterface->setWindowTitle(QApplication::translate("ExpiredElementsDialogInterface", "TuxCards"));
    textLabel1->setText(QApplication::translate("ExpiredElementsDialogInterface", "TuxCards - Removing Expired Entries"));
    pRemoveFromListButton->setToolTip(QApplication::translate("ExpiredElementsDialogInterface", "Removes the selected elements from the list and removes their expiry date."));
    pRemoveFromListButton->setText(QApplication::translate("ExpiredElementsDialogInterface", "&Remove from List"));
    textLabel6_2_2_2_2_2_2->setText(QApplication::translate("ExpiredElementsDialogInterface", "<i>Please note: If an entry is removed\n"
"all children are removed as well.</i>"));
    groupBox->setTitle(QApplication::translate("ExpiredElementsDialogInterface", "Entry Preview"));
    pElementIconLabel->setText(QApplication::translate("ExpiredElementsDialogInterface", ""));
    pElementNameLabel->setText(QApplication::translate("ExpiredElementsDialogInterface", "Entry Name"));
    textLabel4->setText(QApplication::translate("ExpiredElementsDialogInterface", "Number of Children:"));
    pChildCountLabel->setText(QApplication::translate("ExpiredElementsDialogInterface", "0"));
    buttonOk->setToolTip(QApplication::translate("ExpiredElementsDialogInterface", "Deletes all elements that are still within the list above."));
    buttonOk->setText(QApplication::translate("ExpiredElementsDialogInterface", "&Delete All Expired Entries"));
    buttonOk->setShortcut(QApplication::translate("ExpiredElementsDialogInterface", "Alt+D"));
    buttonCancel->setToolTip(QApplication::translate("ExpiredElementsDialogInterface", "Leaves this dialog without removing any element."));
    buttonCancel->setText(QApplication::translate("ExpiredElementsDialogInterface", "&Close"));
    buttonCancel->setShortcut(QApplication::translate("ExpiredElementsDialogInterface", "Alt+C"));
    Q_UNUSED(ExpiredElementsDialogInterface);
    } // retranslateUi

};

namespace Ui {
    class ExpiredElementsDialogInterface: public Ui_ExpiredElementsDialogInterface {};
} // namespace Ui

#endif // EXPIREDELEMENTSDIALOGINTERFACE_H
