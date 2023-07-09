/*

readline.h

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


#ifndef _READLINE_H_
#define _READLINE_H_

#include <windows.h>

/*
these defines may be changed
*/
#define DEFAULT_HISTORY_SIZE    200  /* default number of history entries */

#ifdef __cplusplus
extern "C" {
#endif

typedef char **rl_completion_func_t(const char *, int, int);
typedef char *rl_compentry_func_t(const char *, int);
typedef void rl_compentryfree_func_t(void *);
/*
this is ignored by WinEditLine
*/
typedef void *histdata_t;

typedef struct _hist_entry {
  char *line;
  char *timestamp;
  histdata_t data;
} HIST_ENTRY;

typedef struct _hist_state {
  HIST_ENTRY **entries;
  int offset;
  int length;
  int size;
  int flags;
} HISTORY_STATE;


/*
prototypes of functions which may be
called by the user
*/
void source_editrc();
char *readline(const char *prompt);
char **rl_completion_matches(const char *text, char *entry_func(const char *, int));
char *rl_filename_completion_function(const char *text, int state);
void rl_free(void *mem);
int using_history();
void free_history();
void free_history_entry(HIST_ENTRY *entry);
void clear_history();
char *add_history(char *line);
HIST_ENTRY *remove_history(int i);
HIST_ENTRY *replace_history_entry(int i, char *line, histdata_t dummy);
HIST_ENTRY **history_list();
int where_history();
int history_length();
HIST_ENTRY *current_history();
HIST_ENTRY *history_get(int offset);
int history_set_pos(int i);
HIST_ENTRY *previous_history();
HIST_ENTRY *next_history();
int read_history(const char *filename);
int write_history(const char *filename);
int append_history(int nelements, const char *filename);
int history_truncate_file(const char *filename, int nlines);

/*
extern variables
*/
extern char *rl_line_buffer;
extern char *rl_prompt;
extern int rl_point;
extern int rl_attempted_completion_over;
extern int rl_completion_append_character;
extern const char rl_basic_word_break_characters[];
extern const char *rl_completer_word_break_characters;
extern const char *rl_readline_name;
extern rl_completion_func_t *rl_attempted_completion_function;
extern rl_compentry_func_t *rl_completion_entry_function;
extern rl_compentryfree_func_t *rl_user_completion_entry_free_function;

#ifdef __cplusplus
}
#endif

#endif /* _READLINE_H_ */
