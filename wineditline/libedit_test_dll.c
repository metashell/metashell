/*

libedit_test_dll.c

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


/*
This example shows how to optionally support WinEditLine
if it is available as a runtime DLL and to work without
editline support if it is unavailable
*/

/* includes public domain from tjh@cryptsoft.com */

#include <editline/readline.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif /* !WIN32 */

#ifdef WIN32
#define strdup(X) _strdup(X)
#endif

/* simple mapping for dlopen to WIN32 */
#ifdef WIN32
#define  dlopen(X,Y)    LoadLibrary((X))
#define  dlsym(X,Y)    GetProcAddress((HINSTANCE)(X),(Y))
#define  dlclose(X)    FreeLibrary((X))
#endif /* WIN32 */

#ifndef RTLD_NOW
#define RTLD_NOW 0
#endif
#ifndef RTLD_LOCAL
#define RTLD_LOCAL 0
#endif

#define DL_OPEN_FLAGS RTLD_NOW|RTLD_LOCAL

#define DO_EDIT_ERROR_NONE     0
#define DO_EDIT_ERROR_FAILED    1

/* libedit implements the same interface that readline does but as it 
 * is under a BSD license it is more commonly used and that is what
 * we by default attempt to use before falling back to an interface
 * which provides no command line editing
 *
 * note: we do not attempt to use any of the features line command
 *       completion as that requires interfacing a pile more knowledge
 *       of the available commands and that is well beyond the simple
 *       editable command line we want to offer here
 */
#ifdef WIN32
#ifdef _MSC_VER
#define DO_EDIT_DEFAULT_LIB  "edit.dll"
#else
#define DO_EDIT_DEFAULT_LIB  "libedit.dll"
#endif
#else
#define DO_EDIT_DEFAULT_LIB  "libedit.so"
#endif

typedef struct func_t_st {
  char *name;
  void (*func)(void);
} FUNC_T;

/* note: the order in this table is known by the functions that it wraps */
static FUNC_T do_edit_extern_funcs[] = {
/* [0] */  { "readline", NULL },
/* [1] */  { "add_history", NULL },
};
#define N_do_edit_extern_funcs \
  (sizeof(do_edit_extern_funcs)/sizeof(do_edit_extern_funcs[0]))

static int   do_edit_extern_funcs_init=0;
static int   do_edit_extern_funcs_loaded=0;
static void *do_edit_dl_handle=NULL;

/* load all the functions in the list from the shared library / dll */
int DO_EDIT_load(char *libname)
{
  int kret=DO_EDIT_ERROR_NONE;
  /* ISO C considers void pointer and void function pointer to be
   * distinct and this union works around that 'sanity' 
   */
  union { void *p; void (*fp)(void); } u;
  int i;

  if (!do_edit_extern_funcs_loaded) {
    do_edit_dl_handle=dlopen(libname,DL_OPEN_FLAGS);
    if (do_edit_dl_handle!=NULL) {
      for(i=0;i<N_do_edit_extern_funcs;i++) {
#ifdef WIN32
  u.fp=(void (*)(void))dlsym(do_edit_dl_handle,
        do_edit_extern_funcs[i].name);
#else /* !WIN32 */
  u.p=dlsym(do_edit_dl_handle,do_edit_extern_funcs[i].name);
#endif /* WIN32 */
  if (u.p==NULL) {
    kret=DO_EDIT_ERROR_FAILED;
  } else {
    do_edit_extern_funcs[i].func=u.fp;
  }
      }
    } else {
      kret=DO_EDIT_ERROR_FAILED;
    }

    /* only if successful to we mark this as loaded */
    if (kret==DO_EDIT_ERROR_NONE) {
      do_edit_extern_funcs_loaded++;
    } else {
      if (do_edit_dl_handle!=NULL) {
        dlclose(do_edit_dl_handle);
  do_edit_dl_handle=NULL;
      }
    }
  }

  return kret;
}

/* unload the shared library / dll */
int DO_EDIT_unload(char *name)
{
  int kret=DO_EDIT_ERROR_NONE;

  if (do_edit_extern_funcs_loaded) {
    if (do_edit_dl_handle!=NULL) {
      dlclose(do_edit_dl_handle);
      do_edit_dl_handle=NULL;
      do_edit_extern_funcs_loaded--;
    }
  }
  return kret;
}

/* prototypes for the function pointers so we can safely cast and ensure
 * that we are using things the expected way
 */
typedef char *(*f_readline_t)(char *prompt);
typedef int (*f_add_history_t)(char *line);

/* wrapper on readline */
static char *l_do_edit_readline(char *prompt)
{
  char *ret=NULL;

  if (do_edit_extern_funcs[0].func!=NULL)
    ret=((f_readline_t)(*do_edit_extern_funcs[0].func))(prompt);
  return ret;
}

/* wrapper on add_history */
static int l_do_edit_add_history(char *line)
{
  int ret=0;

  if (do_edit_extern_funcs[1].func!=NULL)
    ret=((f_add_history_t)(*do_edit_extern_funcs[1].func))(line);
  return ret;
}

/* the simple function to provide a prompt and read a line of input */
char *DO_EDIT_read(char *prompt)
{
  char *ret=NULL;
  char buf[BUFSIZ];
  size_t len;

  /* only load the default edit library first time calling this function
   * and only if it not already loaded
   */
  if (!do_edit_extern_funcs_init) {
    if (!do_edit_extern_funcs_loaded)
      (void)DO_EDIT_load(DO_EDIT_DEFAULT_LIB);
    do_edit_extern_funcs_init=1;
  }

  /* if we have an edit implementation loaded then we run with it otherwise
   * we implement a simple non-editing interface
   */
  if (do_edit_extern_funcs_loaded) {
    ret=l_do_edit_readline(prompt);
    if (ret!=NULL)
      l_do_edit_add_history(ret);
  } else {
    printf("%s",prompt);
    fflush(stdout);
    if (fgets(buf,sizeof(buf)-1,stdin)!=NULL) {
      /* drop any trailing newlines off the end of the buffer as that
       * is what 'readline' does 
       */
      len=strlen(buf);
      while(len>0) {
        if ((buf[len-1]=='\r')||(buf[len-1]=='\n')) {
          buf[len-1]='\0';
          len--;
        } else {
          break;
        }
      }
      ret=strdup(buf);
    }
  }
  return ret;
}


int main(int argc, char *argv[])
{
  char *line;

  printf("\nType exit to quit the test\n\n");
  while ((line = DO_EDIT_read("prompt>"))
    && (strncmp(line, "exit", 4))) {
    printf("string=%s\n", line);
    //free(line);
  }
  
  return 0;
}
