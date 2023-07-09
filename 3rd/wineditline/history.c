/*

history.c

is part of:

WinEditLine (formerly MinGWEditLine)
Copyright 2010-2020 Paolo Tosco <paolo.tosco.mail@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of WinEditLine (formerly MinGWEditLine) nor the
    name of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#define _UNICODE
#define UNICODE

#include <editline/readline.h>
#include <editline/wineditline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <tchar.h>


HISTORY_STATE _el_hs = { 0 };
extern size_t _el_line_buffer_size;


/*
remove excess spaces (tabs, linefeeds...)
at the end of line
*/
void _el_remove_tail_spaces(char *line)
{
  int len;
  
  
  len = (int)strlen(line);
  while (len && isspace(line[len - 1])) {
    --len;
  }
  line[len] = '\0';
}


/*
read the .editrc file
*/
void source_editrc()
{
  wchar_t *appdata = NULL;
  wchar_t *editrc = NULL;
  wchar_t string[_EL_ENV_BUF_LEN];
  char line[_EL_ENV_BUF_LEN];
  char s1[_EL_ENV_BUF_LEN];
  char s2[_EL_ENV_BUF_LEN];
  int d1 = -1;
  size_t n;
  FILE *handle = NULL;
  
  
  _wgetenv_s(&n, NULL, 0, _T("EDITRC"));
  if (n) {
    if (!(editrc = malloc((n + 1) * sizeof(wchar_t)))) {
      return;
    }
    _wgetenv_s(&n, editrc, n, _T("EDITRC"));
    if (!n) {
      free(editrc);
    }
  }
  if (!n) {
    /*
    if the EDITRC environment variable is not set
    look for %APPDATA%\.editrc
    */
    _wgetenv_s(&n, NULL, 0, _T("APPDATA"));
    if (n) {
      if (!(appdata = malloc((n + 1) * sizeof(wchar_t)))) {
        return;
      }
      _wgetenv_s(&n, appdata, n, _T("APPDATA"));
    }
    if (!n) {
      return;
    }
    n = wcslen(appdata) + _EL_ENV_BUF_LEN;
    if (!(editrc = malloc(n * sizeof(wchar_t)))) {
      return;
    }
    swprintf_s(editrc, n, _T("%s\\.editrc"), appdata);
    free(appdata);
  }
  if (_wfopen_s(&handle, editrc, _T("r"))) {
    free(editrc);
    return;
  }
  /*
  if .editrc could be opened,
  look for the "history size" line and
  read the value
  */
  while (fgets(line, _EL_ENV_BUF_LEN, handle)) {
    line[_EL_ENV_BUF_LEN - 1] = '\0';
    if (line[0]) {
      sscanf_s(line, "%16s %16s %d",
        s1, _EL_ENV_BUF_LEN, s2, _EL_ENV_BUF_LEN, &d1);
      if ((!_stricmp(s1, "history"))
        && (!_stricmp(s2, "size"))) {
        if (d1 < _EL_MIN_HIST_SIZE) {
          d1 = _EL_MIN_HIST_SIZE;
        }
      }
    }
  }
  fclose(handle);
  /*
  if a valid value has been found, set the
  MINGWEDITLINE_HISTORY_SIZE environment variable
  to this value
  */
  if (d1 != -1) {
    swprintf_s(string, _EL_ENV_BUF_LEN, _T("MINGWEDITLINE_HISTORY_SIZE=%d"), d1);
    _wputenv(string);
  }
  free(editrc);
}


/*
this function MUST be called before using history functions
returns 0 if successful, -1 if not
*/
int using_history()
{
  wchar_t *string_size;
  int size = DEFAULT_HISTORY_SIZE;
  int temp_size;
  size_t n;
  
  
  /*
  try to load .editrc; if it does not exist
  or is malformed, use the default history size
  */
  source_editrc();
  _wgetenv_s(&n, NULL, 0, _T("MINGWEDITLINE_HISTORY_SIZE"));
  if (n) {
    if ((string_size = malloc((n + 1) * sizeof(wchar_t)))) {
      _wgetenv_s(&n, string_size, n, _T("MINGWEDITLINE_HISTORY_SIZE"));
    }
    else {
      n = 0;
    }
  }
  if (n) {
    swscanf_s(string_size, _T("%d"), &temp_size);
    if (temp_size >= _EL_MIN_HIST_SIZE) {
      size = temp_size;
    }
  }
  /*
  initialize history
  */
  memset(&_el_hs, 0, sizeof(HISTORY_STATE));
  _el_hs.entries = (HIST_ENTRY **)_el_alloc_array
    (size + 1, sizeof(HIST_ENTRY));
  if (!_el_hs.entries) {
    return -1;
  }
  _el_hs.length = 1;
  _el_hs.size = size;
  
  return 0;
}


/*
this function may be called after calling
readline() for the last time
if you do, remember to call again
using_history() before calling readline()
*/
void free_history()
{
  int i;
  
  
  if (_el_hs.entries) {
    for (i = 0; i <= _el_hs.size; ++i) {
      if (_el_hs.entries[i]) {
        free(_el_hs.entries[i]->line);
        _el_hs.entries[i]->line = NULL;
      }
    }
    _el_free_array(_el_hs.entries);
  }
}


/*
this function reads history from a text
file; if filename is a NULL pointer, then
%APPDATA%\.history is searched
returns 0 if successful, -1 or errno if not
*/
int read_history(const char *filename)
{
  wchar_t *name = NULL;
  char *line = NULL;
  char *eof;
  int grow_line;
  int line_size = 0;
  int s = 0;
  int i;
  int line_len = 0;
  FILE *file;
  
  
  if (!_el_hs.entries) {
    if (using_history()) {
      return -1;
    }
  }
  errno = 0;
  if (_el_find_history_file(filename, &name)) {
    return -1;
  }
  if (_wfopen_s(&file, name, _T("r"))) {
    return errno;
  }
  i = 0;
  grow_line = 1;
  do {
    if (grow_line) {
      line_size += _EL_BUF_LEN;
      line = realloc(line, line_size);
      if (!line) {
        return -1;
      }
    }
    memset(&line[s], 0, _EL_BUF_LEN);
    eof = fgets(&line[s], _EL_BUF_LEN, file);
    line_len = (int)strlen(line);
    s = 0;
    grow_line = (eof && line_len && (line[line_len - 1] != '\n'));
    if (grow_line) {
      s = line_len;
    }
    else {
      _el_remove_tail_spaces(line);
      if (line[0]) {
        ++i;
      }
    }
  } while (eof);
  rewind(file);
  /*
  skip the older history lines until the
  remaining do not fit into _el_hs.size
  */
  while (i > _el_hs.size) {
    if (!fgets(line, line_size, file)) {
      break;
    }
    _el_remove_tail_spaces(line);
    if (line[0]) {
      --i;
    }
  }
  i = 0;
  while (fgets(line, line_size, file)) {
    _el_remove_tail_spaces(line);
    if (line[0]) {
      line_len = (int)strlen(line);
      _el_hs.entries[i]->line = realloc
        (_el_hs.entries[i]->line, line_len + 1);
      if (!_el_hs.entries[i]->line) {
        return -1;
      }
      strcpy_s(_el_hs.entries[i]->line, line_len + 1, line);
      ++i;
      ++_el_hs.length;
      ++_el_hs.offset;
    }
  }
  fclose(file);
  free(name);
  free(line);
  
  return 0;
}


/*
this function writes history to a text
file; if filename is a NULL pointer, then
%APPDATA%\.history is written
returns 0 if successful, -1 or errno if not
*/
int _el_write_history_helper(const char *filename, const wchar_t *mode, int nelements)
{
  wchar_t *name = NULL;
  int i;
  FILE *file = NULL;
  
  
  errno = 0;
  if (!_el_hs.entries
    || _el_find_history_file(filename, &name)) {
    return -1;
  }
  if (_wfopen_s(&file, name, mode)) {
    return errno;
  }
  for (i = ((nelements == -1) || ((nelements + 1) > _el_hs.length))
    ? 0 : _el_hs.length - (nelements + 1); i < (_el_hs.length - 1); ++i) {
    if (_el_hs.entries[i]->line[0]) {
      fprintf(file, "%s\n", _el_hs.entries[i]->line);
    }
  }
  fclose(file);
  free(name);

  return 0;
}


/*
this function writes history to a text file; if filename is
a NULL pointer, then %APPDATA%\.history is written
returns 0 if successful, -1 or errno if not
*/
int write_history(const char *filename)
{
  return _el_write_history_helper(filename, _T("w"), -1);
}


/*
this function appends the last nelements of history to a text file;
if filename is a NULL pointer, then %APPDATA%\.history is written
returns 0 if successful, -1 or errno if not
*/
int append_history(int nelements, const char *filename)
{
  return _el_write_history_helper(filename, _T("a"), nelements);
}


/*
this function truncates the history file "filename", leaving only
the last nlines lines; if filename is a NULL pointer, then
%APPDATA%\.history is used
returns 0 if successful, -1 or errno if not
*/
int history_truncate_file(const char *filename, int nlines)
{
  int ret = -1;
  HISTORY_STATE _el_hs_copy = _el_hs;
  
  memset(&_el_hs, 0, sizeof(HISTORY_STATE));
	if (!read_history(filename)) {
    ret = _el_write_history_helper(filename, _T("w"), nlines);
  }
	free_history();
  _el_hs = _el_hs_copy;

	return ret;
}


/*
this function frees a history entry
*/
void free_history_entry(HIST_ENTRY *entry)
{
  if (entry) {
    free(entry->line);
    free(entry);
  }
}


/*
this function clears current history
*/
void clear_history()
{
  int i;
  
  
  if (!_el_hs.entries) {
    return;
  }
  for (i = 0; i < _el_hs.length; ++i) {
    if (_el_hs.entries[i]) {
      free(_el_hs.entries[i]->line);
      _el_hs.entries[i]->line = NULL;
    }
  }
  _el_hs.length = 1;
  _el_hs.offset = 0;
}


/*
this function adds line to history;
returns a pointer to the newly added
string, or NULL in case of error
*/
char *add_history(char *line)
{
  int len;
  HIST_ENTRY *temp;
  
  
  if (!line) {
    return NULL;
  }
  len = (int)strlen(line);
  if (!_el_hs.entries) {
    if (using_history()) {
      return NULL;
    }
  }
  if (!_el_hs.entries) {
    return NULL;
  }
  if (_el_hs.length > _el_hs.size) {
    temp = _el_hs.entries[0];
    memmove(&_el_hs.entries[0], &_el_hs.entries[1],
      _el_hs.size * sizeof(HIST_ENTRY *));
    _el_hs.entries[_el_hs.size] = temp;
    --_el_hs.length;
  }
  _el_hs.entries[_el_hs.length - 1]->line = realloc
    (_el_hs.entries[_el_hs.length - 1]->line, len + 1);
  if (!_el_hs.entries[_el_hs.length - 1]->line) {
    return NULL;
  }
  strcpy_s(_el_hs.entries[_el_hs.length - 1]->line, len + 1, line);
  if (_el_hs.entries[_el_hs.length]->line) {
    _el_hs.entries[_el_hs.length]->line[0] = '\0';
  }
  _el_hs.offset = _el_hs.length;
  ++_el_hs.length;
  
  return _el_hs.entries[_el_hs.length - 2]->line;
}


/*
this function removes a history entry
identified by its progressive index
(starting from zero)
returns the old entry so the user can
free the memory allocated to the string
and to the HIST_ENTRY structure itself,
or NULL if not successful
*/
HIST_ENTRY *remove_history(int i)
{
  HIST_ENTRY *temp;
  
  
  if (!_el_hs.entries || !_el_hs.entries[i]
    || (i < 0) || (i >= _el_hs.length)) {
    return NULL;
  }
  temp = _el_hs.entries[i];
  if (i < (_el_hs.length - 1)) {
    memmove(&_el_hs.entries[i], &_el_hs.entries[i + 1],
      (_el_hs.length - i - 1) * sizeof(HIST_ENTRY *));
  }
  _el_hs.entries[_el_hs.length - 1] =
    (HIST_ENTRY *)malloc(sizeof(HIST_ENTRY));
  memset(_el_hs.entries[_el_hs.length - 1], 0, sizeof(HIST_ENTRY));
  
  return temp;
}


/*
this function replaces the string content
of a history entry identified by its progressive
index (starting from zero)
the histdata_t is ignored and kept only for
compatibility reasons
returns a pointer to the updated HIST_ENTRY
or NULL if not successful
*/
HIST_ENTRY *replace_history_entry(int i, char *line, histdata_t dummy)
{
  size_t len;
  
  
  len = strlen(line) + 1;
  if (!_el_hs.entries || !_el_hs.entries[i]
    || (i < 0) || (i >= _el_hs.length)) {
    return NULL;
  }
  _el_hs.entries[i]->line = realloc(_el_hs.entries[i]->line, len);
  if (!_el_hs.entries[i]->line) {
    return NULL;
  }
  strcpy_s(_el_hs.entries[i]->line, len, line);
  
  return _el_hs.entries[i];
}


/*
this function returns a pointer
to the HIST_ENTRY array
*/
HIST_ENTRY **history_list()
{
  return _el_hs.entries;
}


/*
this function returns the current
history index
*/
int where_history()
{
  return _el_hs.offset;
}


/*
this function returns the current
history length
*/
int history_length()
{
  return _el_hs.length - 1;
}


/*
this function returns a pointer to
the current HIST_ENTRY structure
*/
HIST_ENTRY *current_history()
{
  return ((_el_hs.entries && _el_hs.offset)
    ? _el_hs.entries[_el_hs.offset - 1] : NULL);
}


/*
this function returns a pointer to
the HIST_ENTRY structure identified
by its progressive index
(starting from zero) or NULL if the
index i does not exist
*/
HIST_ENTRY *history_get(int i)
{
  return (!_el_hs.entries || ((i < 0)
    || (i > _el_hs.length)) ? NULL : _el_hs.entries[i]);
}


/*
this function sets the current history
offset based on the history entry
progressive index (starting from zero)
returns 1 if succesful, 0 if not
*/
int _el_history_set_pos(int i)
{
  if ((i < 1) || (i > (_el_hs.length + 1))) {
    return 0;
  }
  _el_hs.offset = i - 1;
  
  return 1;
}


/*
this function sets the current history
offset based on the history entry
progressive index (starting from zero)
returns 1 if succesful, 0 if not
*/
int history_set_pos(int i)
{
  if ((i < 1) || (i > _el_hs.length)) {
    return 0;
  }
  _el_hs.offset = i - 1;
  
  return 1;
}


/*
this function returns a pointer to
the previous HIST_ENTRY, or NULL
if the current offset is already at
the beginning of history
*/
HIST_ENTRY *_el_previous_history()
{
  return ((_el_hs.entries && _el_hs.offset)
    ? _el_hs.entries[--_el_hs.offset] : NULL);
}


/*
this function returns a pointer to
the previous HIST_ENTRY, or NULL
if the current offset is already at
the beginning of history
*/
HIST_ENTRY *previous_history()
{
  return ((_el_hs.entries && (_el_hs.offset > 1))
    ? _el_hs.entries[--_el_hs.offset] : NULL);
}


/*
this function returns a pointer to the next HIST_ENTRY, or NULL
if the current offset is already at the end of history
*/
HIST_ENTRY *_el_next_history()
{
  return ((((_el_hs.offset == 0) && (_el_hs.length == 1))
    || (_el_hs.offset == _el_hs.length)
    || !_el_hs.entries || !_el_hs.entries[_el_hs.offset + 1])
    ? NULL : _el_hs.entries[++_el_hs.offset]);
}


/*
this function returns a pointer to the next HIST_ENTRY, or NULL
if the current offset is already at the end of history
*/
HIST_ENTRY *next_history()
{
  return (((_el_hs.offset == _el_hs.length - 1)
    || !_el_hs.entries || !_el_hs.entries[_el_hs.offset + 1])
    ? NULL : _el_hs.entries[++_el_hs.offset]);
}


/*
this function displays the current history entry
*/
void _el_display_history()
{
  int line_len;
  int h_len;
  
  
  _el_set_cursor(-rl_point);
  if (!_el_hs.entries || (!_el_mb2w
    (_el_hs.entries[_el_hs.offset]->line, &_el_wide))) {
    return;
  }
  h_len = (int)wcslen(_el_wide);
  line_len = (int)wcslen(_el_line_buffer);
  _el_grow_buffers(h_len);
  wcscpy_s(_el_line_buffer, _el_line_buffer_size, _el_wide);
  wcscpy_s(_el_print, _el_line_buffer_size, _el_wide);
  rl_point = (int)h_len;
  if (h_len < line_len) {
    _el_add_char(_el_print, _T(' '), line_len - h_len);
  }
  _el_print_string(_el_print);
  _el_set_cursor(h_len);
}


/*
this function copies the "filename" string to "name"
if filename is a NULL pointer, %APPDATA%\.history
is copied to "name"
*/
int _el_find_history_file(const char *filename, wchar_t **name)
{
  wchar_t *appdata = NULL;
  size_t n;
  
  
  if (!filename) {
    _wgetenv_s(&n, NULL, 0, _T("APPDATA"));
    if (n) {
      if ((appdata = malloc((n + 1) * sizeof(wchar_t)))) {
        _wgetenv_s(&n, appdata, n, _T("APPDATA"));
      }
      else {
        n = 0;
      }
    }
    if (n) {
      n = (wcslen(appdata) + _EL_ENV_BUF_LEN);
      if (!(*name = malloc(n * sizeof(wchar_t)))) {
        return -1;
      }
      swprintf_s(*name, n, _T("%s\\"), appdata);
    }
    wcscat_s(*name, n, _T(".history"));
  }
  else {
    if (!_el_mb2w((char *)filename, name)) {
      return -1;
    }
  }
  free(appdata);
  
  return 0;
}
