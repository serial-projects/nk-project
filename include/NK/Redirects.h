/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKRedirects_h
#define NKRedirects_h

#include "NK/Types/Basics.h"
#include <memory.h>
#include <stdio.h>

/** OS File Operations: */

#define NK_OSFileEmptyHandler   NULL
#define NK_OSFile               FILE*
#define NK_OSFileOpen           fopen
#define NK_OSFileClose          fclose
#define NK_OSFileGetCharacter   fgetc
#define NK_OSFileSeek           fseek
#define NK_OSFileTell           ftell
#define NK_OSFileSeekCurrent    SEEK_SET

/** Memory Operations: */

#define NK_RedirectMalloc       malloc
#define NK_RedirectCalloc       calloc
#define NK_RedirectRealloc      realloc
#define NK_RedirectFree         free
#define NK_RedirectMemcpy       memcpy
#define NK_RedirectMemset       memset

/** String Operations: */

#define NK_RedirectStrlen       strlen
#define NK_RedirectStrcmp       strcmp
#define NK_RedirectStrncmp      strncmp

#endif