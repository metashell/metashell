/*

globals.h

is part of:

WinEditLine (formerly MinGWEditLine)
Copyright 2010-2014 Paolo Tosco <paolo.tosco@unito.it>
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

const char *rl_readline_name = NULL;
wchar_t *_el_print = NULL;
wchar_t *_el_temp_print = NULL;
size_t _el_temp_print_size = 0;
wchar_t *_el_next_compl = NULL;
wchar_t *_el_file_name = NULL;
wchar_t *_el_dir_name = NULL;
wchar_t *_el_old_arg = NULL;
wchar_t *_el_wide = NULL;
wchar_t *_el_text = NULL;
wchar_t *_el_line_buffer = NULL;
size_t _el_line_buffer_size = 0;
wchar_t *_el_basic_word_break_characters = NULL;
wchar_t *_el_completer_word_break_characters = NULL;
wchar_t _el_basic_file_break_characters[_EL_MAX_FILE_BREAK_CHARACTERS];
char *rl_line_buffer = NULL;
char *rl_prompt = NULL;
const char rl_basic_word_break_characters[] = " \t\n\\'`@$><=;|&{(";
const char *rl_completer_word_break_characters = NULL;
char *_el_text_mb = NULL;
wchar_t *_el_prompt = NULL;
wchar_t **_el_compl_array = NULL;
int _el_ctrl_c_pressed = FALSE;
int rl_point = 0;
int rl_attempted_completion_over = 0;
int rl_completion_append_character = ' ';
int _el_compl_index = 0;
int _el_n_compl = 0;
int _el_prompt_len = 0;
rl_completion_func_t *rl_attempted_completion_function = NULL;
rl_compentry_func_t *rl_completion_entry_function = NULL;
rl_compentryfree_func_t *rl_user_completion_entry_free_function = NULL;
BOOL _el_prev_in_cm_saved = FALSE;
BOOL _el_prev_out_cm_saved = FALSE;
BOOL _el_prev_sbInfo_saved = FALSE;
DWORD _el_prev_in_cm = 0;
DWORD _el_prev_out_cm = 0;
HANDLE _el_h_in = NULL;
HANDLE _el_h_out = NULL;
COORD _el_prev_size;
