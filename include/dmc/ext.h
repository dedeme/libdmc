// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Functions using external programs

#ifndef DM_EXT_H
  #define DM_EXT_H

typedef struct achar_Achar Achar;

/// ext_wget calls "wget -q -O - <i>url</i>" and returns the text read in an
/// Arr[char].<br>
/// If the reading fails 'r' is an empty Arr.
Achar *ext_wget(char *url);

/// ext_zenity_entry reads a text using GUI. It calls:
///   zenity --entry --title=<i>title</i> --text=<i>prompt</i>
/// 'r' removes starting and trailing spaces.<br>
/// If user clicks on cancel, 'r' is an empty string.<br>
/// It is posible set a default text adding in promp:
///   \" --entry-text \"<i>text_to_add</i>
char *ext_zenity_entry(char *title, char *prompt);

/// ext_zenity_msg shows a message box. It calls:
///   zenity --notification --window-icon=<i>type</i> --text=<i>text</i>
/// 'icon' is one of gnome icon stock. For example: info, dialog-warning,
/// dialog-error, dialog-information, face-wink, etc
void ext_zenity_msg(char *icon, char *text);

/// ext_pdf generates a pdf file from a html text. It calls:
///   pdfPrinter -s %s -t %s options 2>&1
/// Parameters:
///   tx_source  : Text html
///   file_target: Path of the new pdf file
///   options    : Options for pdfPrinter
/// Throws exception.
void ext_pdf(char *tx_source, char *file_target, char *options);

/// ext_zip compress source in target. It calls:
///   zip -q <i>target</i> <i>source</i> 2>&1
/// if 'target' already exists, source will be added to it. If you require a
/// fresh target file, you have to delete it previously.<br>
/// Parameters:
///   source: can be a file or directory,
///   target: Zip file. If it is a relative path, it hangs on source parent.
/// Throws exception.
void ext_zip(char *source, char *target);

/// ext_unzip uncompress source in target, It calls:
///   unzip -q <i>source</i> -d <i>target</i> 2>&1
/// Parameters:
///   source: Zip file.
///   target: A directory. It it does not exist, it is created.
/// Throw exception.
void ext_unzip(char *source, char *target);

#endif

