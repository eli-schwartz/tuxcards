/***************************************************************************
                          WhatsThis.h  -  description
                             -------------------
    begin                : Sun Aug 05 2007
    copyright            : (C) 2007 by Alexander Theel
    email                : alex.theel@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef WHATS_THIS_H
#define WHATS_THIS_H

/** ********************** WhatsThis texts for file actions ****************/
//  Q3WhatsThis::whatsThisButton(mpMainTools);
//
//
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("filenew", getIcon("filenew"));
//  Q3WhatsThis::add(clearTool,"<img source=\"filenew\">"
//                            "<b>Clear whole Tree</b><p>"
//                            "Click this button to remove all entries from "
//                            "the tree.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("fileopen", getIcon("fileopen"));
//  Q3WhatsThis::add(openTool, "<img source=\"fileopen\"><b>Open a new File</b><p>"
//                            "Click this button to open a new data file. "
//                            "You can also select the <i>Open</i> command "
//                            "from the <i>File</i> menu.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("filesave", getIcon("filesave"));
//  Q3WhatsThis::add(saveTool, "<img source=\"filesave\"><b>Save Data to File</b> (Ctrl+S)<p>"
//                            "Click this button to save all data to disk. "
//                            "If this is a new file, you will be prompted "
//                            "for a file name.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("fileprint", getIcon("fileprint"));
//  Q3WhatsThis::add(printTool,  "<img source=\"fileprint\"> <b>Print current Entry</b><p>"
//                            "Click this button to print the currently active "
//                            "entry.<p>"
//                            "<i>Please note: Only RTF-notes can be printed correctly. "
//                            "When printing an plain text note white spaces are "
//                            "ignored ");


/** ********************** WhatsThis texts for tree actions ****************/
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("addTreeElement", getIcon("addTreeElement"));
//  Q3WhatsThis::add(mpAddElementAction, "<img source=\"addTreeElement\"><b>Add Entry</b> (INSERT)<p>"
//                                      "You can add further child-entries using this "
//                                      "button. The same function is available through "
//                                      "the context menu (right click on an item).<p>"
//                                      "Clicking this button will open a dialog which "
//                                      "prompts you for a name of the new entry.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("changeProp", getIcon("changeProperty"));
//  Q3WhatsThis::add(mpChangeElementPropertyAction, "<img source=\"changeProp\"><b>Change Property</b><p>"
//                                      "Clicking this button, a dialog will appear. There, "
//                                      "you may enter a new name and select another icon "
//                                      "for the currently active entry.<p>"
//                                      "You can also use the context menu (right click on "
//                                      "an item) to call this function.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("delete", getIcon("delete"));
//  Q3WhatsThis::add(mpRemoveElementAction, "<img source=\"delete\"><b>Remove active Entry</b> (DELETE)<p>"
//                                    "This will remove the currently active entry <i>and "
//                                    "all</i> child-entries from the tree.<p>"
//                                    "You can also use the context menu (right click on "
//                                    "an item) for this function.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("lock", getIcon("lock"));
//  Q3WhatsThis::add(mpLockButton, "<img source=\"lock\"><b>Encrypt active Entry</b><p>"
//                                "Encryptes the currently active entry.<p>"
//                                "If it was not encrypted yet a dialog for "
//                                "entering a password will open." );
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("unlock", getIcon("unlock"));
//  Q3WhatsThis::add(mpRemoveLockButton, "<img source=\"unlock\"><b>Remove Encryption</b><p>"
//                                "Removes encryption from active entry.<p>"
//                                "For security reasons, this action can only be "
//                                "done if the entry is decrypted." );
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("upArrow", getIcon("upArrow"));
//  Q3WhatsThis::add(ieUpTool, "<img source=\"upArrow\"><b>Move Up</b><p>"
//                            "Moves the currently active entry one position upwards "
//                            "within the tree.<p>"
//                            "The entry is moved with all of its children.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("downArrow", getIcon("downArrow"));
//  Q3WhatsThis::add(ieDownTool, "<img source=\"downArrow\"> <b>Move Down</b><p>"
//                              "Moves the currently active entry "
//                              "one position downwards within the tree.<p>"
//                              "The entry is moved with all of its children.");
//
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("historyBack", getIcon("back"));
//  Q3WhatsThis::add(mpHistoryPrevAction, "<img source=\"historyBack\"><b>History, Back</b> (Alt+Left)<p>"
//                                "History function. If this button is clicked, then the "
//                                "last active entry is selected.<p>"
//                                "<i>This is similar to the back button within your "
//                                "browser.</i>" );
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("historyForward", getIcon("forward"));
//  Q3WhatsThis::add(mpHistoryNextAction, "<img source=\"historyForward\"><b>History, Forward</b> (Alt+Right)<p>"
//                                 "History function. If this button is clicked, then the "
//                                 "next entry is selected.<p>"
//                                 "<i>This is similar to the forward button within your "
//                                 "browser.</i>" );

//  Q3MimeSourceFactory::defaultFactory()->setPixmap("find", getIcon("find"));
//  Q3WhatsThis::add(findTool, "<img source=\"find\"><b>Search</b> (Ctrl+F)<p>"
//                            "Use this button, if you want to search for words "
//                            "within your data.");
//
//
//  Q3WhatsThis::add(textFormatTool, "This button shows you the text format (ASCII / RTF) "
//                                  "of the currently active entry. It does also let "
//                                  "you convert between the two formats.<p>"
//                                  "<i>Usually, you do not need to worry about "
//                                  "converting text formats.</i>");
//

/** ********************** WhatsThis texts for editor actions **************/
/** Part 1*/
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("undo", getIcon("undo"));
//  mpEditUndoAction->setWhatsThis("<img source=\"undo\"><b>Undo</b> (Ctrl+Z)<p>"
//                              "This button undoes changes made within the editor.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("redo", getIcon("redo"));
//  mpEditRedoAction->setWhatsThis("<img source=\"redo\"><b>Redo</b> (Ctrl+Y)<p>"
//                               "This button provides redo functionality for the editor.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("editcut", getIcon("editcut"));
//  Q3WhatsThis::add(editCutTool, "<img source=\"editcut\"><b>Cut</b> (Ctrl+X)<p>"
//                            "Cut text within the editor.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("editcopy", getIcon("editcopy"));
//  mpEditCopyAction->setWhatsThis("<img source=\"editcopy\"><b>Copy</b> (Ctrl+C)<p>"
//                               "Copy text within the editor.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("editpaste", getIcon("editpaste"));
//  Q3WhatsThis::add(editPasteTool, "<img source=\"editpaste\"><b>Paste</b> (Ctrl+V)<p>"
//                            "Paste text within the editor.");
/** Part 2*/
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("textBold", getIcon("text_bold"));
//  Q3WhatsThis::add(textBoldTool, "<img source=\"textBold\"><b>Bold</b><p>"
//                                "Use this button, if you want to use bold text.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("textItalic", getIcon("text_italic"));
//  Q3WhatsThis::add(textItalicTool, "<img source=\"textItalic\"><b>Italic</b><p>"
//                                "Use this button, if you want to use italic text.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("textUnder", getIcon("text_under"));
//  Q3WhatsThis::add(textUnderTool, "<img source=\"textUnder\"><b>Underline</b><p>"
//                                "Use this button, if you want to use underlined text.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("textColor", getIcon("text_color"));
//  Q3WhatsThis::add(mpEditColorAction, "<img source=\"textColor\"><b>Text Color</b><p>"
//                                 "Changes the color of the selected text.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("alignLeft", getIcon("text_left"));
//  Q3WhatsThis::add(mpEditTextLeftAction, "<img source=\"alignLeft\"><b>Align Left</b><p>"
//                                "Aligns the currently edited paragraph on the left side.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("alignCenter", getIcon("text_center"));
//  Q3WhatsThis::add(mpEditTextCenterAction, "<img source=\"alignCenter\"><b>Center</b><p>"
//                                  "Centers the currently edited paragraph within the editor.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("alignRight", getIcon("text_left"));
//  Q3WhatsThis::add(mpEditTextRightAction, "<img source=\"alignRight\"><b>Align Right</b><p>"
//                                 "Aligns the currently edited paragraph on the right side.");
//  Q3MimeSourceFactory::defaultFactory()->setPixmap("alignBlock", getIcon("text_block"));
//  Q3WhatsThis::add(mpEditTextBlockAction, "<img source=\"alignBlock\"><b>Text Block</b><p>"
//                                   "Aligns the currently edited paragraph on both sides "
//                                   "within the editor.");

#endif