// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Functions using external programs.

#ifndef DMC_EXT_H
  #define DMC_EXT_H

/// Calls "wget -q -O - 'url'" and returns the text read.
/// If the reading fails, it returns an empty string.
char *ext_wget(char *url);

/// Reads a text using GUI. It calls:
///   zenity --entry --title='title' --text='prompt'
/// The return removes starting and trailing spaces.
/// If user clicks on cancel, it returns an empty string.
/// It is posible set a default text adding in promp:
///   \" --entry-text \"[text_to_add]
char *ext_zenity_entry(char *title, char *prompt);

/// ext_zenity_msg shows a message box. It calls:
///   zenity --notification --window-icon='icon' --text='text'
/// 'icon' is one of gnome icon stock. For example: info, dialog-warning,
/// dialog-error, dialog-information, face-wink, etc
void ext_zenity_msg(char *icon, char *text);

/// ext_pdf generates a pdf file from a html text. It calls:
///   pdfPrinter -s %s -t %s 'options' 2>&1
///
///   tx_source  : Text html
///   file_target: Path of the new pdf file
///   options    : Options for pdfPrinter
void ext_pdf(
  char *tx_source,
  char *file_target,
  char *options
);

/// ext_zip compress source in target. It calls:
///   zip -q 'target' 'source' 2>&1
/// if 'target' already exists, source will be added to it. If you require a
/// fresh target file, you have to delete it previously.
///   source: can be a file or directory,
///   target: Zip file. If it is a relative path, it hangs on source parent.
void ext_zip(char *source, char *target);

/// ext_unzip uncompress source in target, It calls:
///   unzip -q 'source' -d 'target' 2>&1
///
///   source: Zip file.
///   target: A directory. It it does not exist, it is created.
void ext_unzip(char *source, char *target);

#endif
