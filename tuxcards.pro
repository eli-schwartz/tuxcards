# TEMPLATE = app #lib
QT += xml

CONFIG += qt debug  #warn_on release #release  #staticlib
#exists( $(QTDIR)/lib/libqt-mt* ) {
#   message( "Configuring for multi-threaded Qt..." )
#   CONFIG += thread
#}

TARGET = tuxcards

OBJECTS_DIR = ./output/bin
MOC_DIR     = ./output/moc
UI_DIR      = ./output/ui

ICON = macOS_tuxcards.icns

HEADERS += src/global.h \
           src/version.h \
           src/CCommandLineOptions.h \
           src/utilities/crypt/BlowFish.h \
           src/utilities/crypt/BlowFish2.h \
           src/utilities/crypt/MD5.h \
           src/utilities/crypt/StringCrypter.h \
           src/utilities/strings.h \
           src/utilities/base64/CBase64Coder.h \
           src/utilities/CIconManager.h \
           src/fontsettings.h \
           src/CTuxCardsConfiguration.h \
           src/information/IRemoveElementListener.h \
           src/information/IParent.h \
           src/information/CInformationElement.h \
           src/information/CTreeInformationElement.h \
           src/information/Path.h \
           src/information/IView.h \
           src/information/CInformationCollection.h \
           src/information/xmlpersister.h \
           src/information/htmlwriter.h \
           src/information/IHistoryListener.h \
           src/information/CInformationElementHistory.h \
           src/informationExt/CCollectionModel.h \
           src/gui/colorbar/CColorBar.h \
           src/gui/cactusbar/cactus.h \
           src/gui/cactusbar/cactusshoot.h \
           src/gui/cactusbar/cactustools.h \
           src/gui/cactusbar/imagemanager.h \
           src/gui/cactusbar/CCactusBar.h \
           src/gui/CTree.h \
           src/gui/editor/CEditor.h \
           src/gui/editor/CSingleEntryView.h \
           src/gui/recentFileList/CRecentFileList.h \
           src/gui/CMainWindow.h \
           src/gui/dialogs/dateInputDialog/CDateInputDialog.h \
           src/gui/dialogs/iconselectordialog/CIconSelectorItem.h \
           src/gui/dialogs/iconselectordialog/CIconSelectorDialog.h \
           src/gui/dialogs/propertyDialog/CPropertyDialog.h \
           src/gui/dialogs/configurationDialog/CConfigurationDialog.h \
           src/gui/dialogs/searchDialog/CSearchDialog.h \
           src/gui/dialogs/searchDialog/CSearchPosition.h \
           src/gui/dialogs/searchDialog/CSearchListItem.h \
           src/gui/dialogs/searchDialog/CSearchListItemDelegate.h \
           src/gui/dialogs/passwdDialog/CPasswdDialog.h \
           src/gui/editor/CEncryptedEntryView.h

SOURCES += src/main.cpp \
           src/CCommandLineOptions.cpp \
           src/utilities/crypt/BlowFish.cpp \
           src/utilities/crypt/MD5.cpp \
           src/utilities/crypt/StringCrypter.cpp \
           src/utilities/strings.cpp \
           src/utilities/base64/CBase64Coder.cpp \
           src/utilities/CIconManager.cpp \
           src/fontsettings.cpp \
           src/CTuxCardsConfiguration.cpp \
           src/information/informationformat.cpp \
           src/information/CInformationElement.cpp \
           src/information/CTreeInformationElement.cpp \
           src/information/Path.cpp \
           src/information/CInformationCollection.cpp \
           src/information/xmlpersister.cpp \
           src/information/htmlwriter.cpp \
           src/information/CInformationElementHistory.cpp \
           src/informationExt/CCollectionModel.cpp \
           src/gui/colorbar/CColorBar.cpp \
           src/gui/cactusbar/cactus.cpp \
           src/gui/cactusbar/cactusshoot.cpp \
           src/gui/cactusbar/cactustools.cpp \
           src/gui/cactusbar/imagemanager.cpp \
           src/gui/cactusbar/CCactusBar.cpp \
           src/gui/CTree.cpp \
           src/gui/editor/CEditor.cpp \
           src/gui/editor/CSingleEntryView.cpp \
           src/gui/recentFileList/CRecentFileList.cpp \
           src/gui/CMainWindow.cpp \
           src/gui/dialogs/dateInputDialog/CDateInputDialog.cpp \
           src/gui/dialogs/iconselectordialog/CIconSelectorItem.cpp \
           src/gui/dialogs/iconselectordialog/CIconSelectorDialog.cpp \
           src/gui/dialogs/propertyDialog/CPropertyDialog.cpp \
           src/gui/dialogs/configurationDialog/CConfigurationDialog.cpp \
           src/gui/dialogs/searchDialog/CSearchDialog.cpp \
           src/gui/dialogs/searchDialog/CSearchPosition.cpp \
           src/gui/dialogs/searchDialog/CSearchListItem.cpp \
           src/gui/dialogs/searchDialog/CSearchListItemDelegate.cpp \
           src/gui/dialogs/passwdDialog/CPasswdDialog.cpp \
           src/gui/editor/CEncryptedEntryView.cpp

#FORMS  =  src/gui/dialogs/expiredElementsDialog/ExpiredElementsDialogInterface.ui \

FORMS  =   src/gui/dialogs/propertyDialog/IPropertyDialog.ui \
           src/gui/dialogs/dateInputDialog/IDateInputDialog.ui \
           src/gui/dialogs/iconselectordialog/ISelectorDialog.ui \
           src/gui/dialogs/configurationDialog/IConfigurationDialog.ui \
           src/gui/dialogs/searchDialog/ISearchDialog.ui \
           src/gui/dialogs/passwdDialog/IPasswdDialog.ui


RESOURCES += src/icons/icons.qrc
RESOURCES += src/translations/translations.qrc

TRANSLATIONS = src/translations/tuxcards_de.ts

#target.path = /usr/local/bin/
#INSTALLS += target

#documentation.path  = /usr/local/doc/tuxcards/
#documentation.files = docs/*
#documentation.files = src/gui/cactusbar/flowers/
#INSTALLS += documentation
