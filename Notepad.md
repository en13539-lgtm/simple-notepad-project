Simple Notepad Project 
====================================

## Required Features

# 1. Exception Handling

In `notepad_exception.h` there is a class named `notepad_exception`. This class inherits from `std::runtime_error` class.
The base class provides support for storing and retrieving error messages through what() method.

From notepad_exception class I created the following derived classes:
1. `file_not_found_exception` - is thrown when the selected file does not exist
2. `file_read_exception` - is thrown when a file cannot be opened for reading.
3. `file_write_exception` - is thrown when saving to a file fails.

I used these exceptions inside the file operations implemented in open_file() and save_file().
For ex:
```c++
if (!file.exists()) {
            throw file_not_found_exception(path.toStdString());
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw file_read_exception(path.toStdString());
        }
```
All file operations are wrapped inside try-catch blocks:
```c++
try {
    ...
}
catch (const notepad_exception& ex) {
    QMessageBox::critical(this, "Error", ex.what());
}
```

# 2. Spell Checker
I was provided data/words.txt word list (one word per line). 
The list is words_alpha.txt from `dwyl/english-words`, released into the public domain under The Unlicense (370105 lowercase a-z entries).
So there are two clases `spell_checker` and `spell_checker_highlighter`. 
### Spell Checker Class
This class is used for checking the correctness of the word. It loads words from the `data/words.txt` and inserts them into the set dictionary.
```c++
std::set<std::string> dictionary;
```
The dictionary is loaded inside the constructor.
The constructor:
* opens data/words.txt using QFile,
* reads the file line by line using QTextStream,
* removes extra whitespace using trimmed(),
* converts each word from QString to std::string,
* inserts the word into the dictionary set.

To check whether the correctness of the word, I implemented `bool is_correct(const std::string& word) const`.
This function check whether the provided word is in the dictionary.

### Spell Checker Highlighter Class
While spell checker is used only for checking correctness of the word, `spell_checker_highlighter` is used for visually highlighting incorrect words. 
This class is inherited from the base class `QSyntaxHighlighter`. The main reason for that is that Qt automatically calls its highlighting functions whenever the document text changes.
The highlighter is connected to the editor document using:
```c++
highlighter =
    new spell_checker_highlighter(
    editor->document(),
    checker);
```

## Optional Features

# 1. Zoom
The zoom feature is added in the View menu with three actions:
* View -> Zoom In
* View -> Zoom Out
* View -> Reset Zoom
The actions are created inside setup_view_menu() using QAction
For zooming in and out, I used the built-in QTextEdit functions:
```c++
editor->zoomIn();
editor->zoomOut();
```
These functions increase or decrease the visual size of the text inside the editor.
I also added keyboard shortcuts:
* Ctrl++
* Ctrl+-
* Ctrl+0
Since there is a Reset action, I decided to add  `int zoom_level = 0;`. This variable tracks how many zoom steps were applied.
When the user zooms in, then it increases, if out it increases.
For reset, the program checks the current zoom_level.
If zoom_level is positive, the editor was zoomed in, so the program calls zoomOut() the same number of times.
If zoom_level is negative, the editor was zoomed out, so the program calls zoomIn() the same number of times. 
After resetting, zoom_level becomes 0.

# 2. Font Dialog
The feature is available from:
`Format -> Font...`
When the user clicks this action, the application opens a standard Qt font selection dialog using:
`QFontDialog::getFont(...)`
The feature works in two cases:
If the user selected some text, the chosen font is applied only to the selected text.
If no text is selected, the application selects the whole document, applies the chosen font, and then clears the selection.
This matches the requirement that the font dialog should apply the selected font to either the selection or the whole document.
The main idea is that QFontDialog handles the font selection UI, while QTextEdit applies the selected font to the editor content.

# 3. Color Picker
The feature is available from:
`Format -> Text Color...`
When the user selects this action, the application opens a standard Qt color selection dialog using:
`QColorDialog::getColor(...)`

The selected color is stored in a QColor object. If the user closes the dialog or cancels the selection, no changes are applied.
To apply the selected color, I used QTextCharFormat:
`QTextCharFormat fmt;`
`fmt.setForeground(color);`
QTextCharFormat stores formatting information for text characters, and setForeground() sets the text color.
The feature supports two cases:
If the user selected text, the color is applied only to the selected text using:
`cursor.mergeCharFormat(fmt);`
If no text is selected, the formatting is applied to future typed text using:
`editor->mergeCurrentCharFormat(fmt);`

# 4. Recent Files
The feature is available from:
`File -> Recent Files`
This feature is implemented not as QAction but as QMenu since Recent Files list must contain dynamically changing entries.
I stored recent file paths in a container inside the application:
`std::vector<QString> recent_files;`
When a file is successfully opened:
* its path is added to the recent files list,
* duplicate entries are removed,
* the list size is limited to a maximum number of files.
* The Recent Files submenu is rebuilt dynamically using QAction objects.

For each stored file path, the application creates a new action using:
`recent_menu->addAction(...)`
Each action is connected using connect():
`connect(action, &QAction::triggered, this, [this, path] {
open_recent_file(path);
});`
This allows the application to reopen the selected file directly from the Recent Files submenu.
The menu is updated every time the recent file list changes, ensuring that the displayed files always reflect the most recently opened documents.