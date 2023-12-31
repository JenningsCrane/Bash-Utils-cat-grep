# Simple Bash Utils

Development of Bash text utilities: cat, grep.

### cat Usage

`cat [OPTION] [FILE]...`

### cat Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### grep Usage

`grep [options] template [file_name]`

### grep Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -i | Ignore uppercase vs. lowercase.  |
| 2 | -v | Invert match. |
| 3 | -c | Output count of matching lines only. |
| 4 | -l | Output matching files only.  |
| 5 | -n | Precede each matching line with a line number. |
| 6 | -h | Output matching lines without preceding them by file names. |
| 7 | -s | Suppress error messages about nonexistent or unreadable files. |
| 8 | -o | Output the matched parts of a matching line. |
