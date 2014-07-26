/**
 * @file shlguid.h
 * Copyright 2012, 2013 MinGW.org project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef _SHLGUID_H
#define _SHLGUID_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFINE_SHLGUID(n,l,w1,w2) DEFINE_GUID(n,l,w1,w2,0xC0,0,0,0,0,0,0,0x46)
#define SID_SShellBrowser IID_IShellBrowser
extern const GUID CLSID_ShellDesktop;
extern const GUID CLSID_ShellLink;
extern const GUID FMTID_Intshcut;
extern const GUID FMTID_InternetSite;
extern const GUID CGID_Explorer;
extern const GUID CGID_ShellDocView;
extern const GUID CGID_ShellServiceObject;
extern const GUID IID_INewShortcutHookA;
extern const GUID IID_IShellBrowser;
extern const GUID IID_IShellView;
extern const GUID IID_IContextMenu;
extern const GUID IID_IColumnProvider;
extern const GUID IID_IQueryInfo;
extern const GUID IID_IShellIcon;
extern const GUID IID_IShellIconOverlayIdentifier;
extern const GUID IID_IShellFolder;
extern const GUID IID_IShellExtInit;
extern const GUID IID_IShellPropSheetExt;
extern const GUID IID_IPersistFolder;
extern const GUID IID_IExtractIconA;
extern const GUID IID_IShellLinkA;
extern const GUID IID_IShellCopyHookA;
extern const GUID IID_IFileViewerA;
extern const GUID IID_ICommDlgBrowser;
extern const GUID IID_IEnumIDList;
extern const GUID IID_IFileViewerSite;
extern const GUID IID_IContextMenu2;
extern const GUID IID_IContextMenu3;
extern const GUID IID_IShellExecuteHookA;
extern const GUID IID_IPropSheetPage;
extern const GUID IID_INewShortcutHookW;
extern const GUID IID_IFileViewerW;
extern const GUID IID_IShellLinkW;
extern const GUID IID_IExtractIconW;
extern const GUID IID_IShellExecuteHookW;
extern const GUID IID_IShellCopyHookW;
extern const GUID IID_IShellView2;
extern const GUID LIBID_SHDocVw;
extern const GUID IID_IShellExplorer;
extern const GUID DIID_DShellExplorerEvents;
extern const GUID CLSID_ShellExplorer;
extern const GUID IID_ISHItemOC;
extern const GUID DIID_DSHItemOCEvents;
extern const GUID CLSID_SHItemOC;
extern const GUID IID_DHyperLink;
extern const GUID IID_DIExplorer;
extern const GUID DIID_DExplorerEvents;
extern const GUID CLSID_InternetExplorer;
extern const GUID CLSID_StdHyperLink;
extern const GUID CLSID_FileTypes;
extern const GUID CLSID_InternetShortcut;
extern const GUID IID_IUniformResourceLocator;
extern const GUID CLSID_DragDropHelper;
extern const GUID IID_IDropTargetHelper;
extern const GUID IID_IDragSourceHelper;
extern const GUID CLSID_AutoComplete;
extern const GUID IID_IAutoComplete;
extern const GUID IID_IAutoComplete2;
extern const GUID CLSID_ACLMulti;
extern const GUID IID_IObjMgr;
extern const GUID CLSID_ACListISF;
extern const GUID IID_IACList;

#define IID_IFileViewer	__AW(IID_IFileViewer)
#define IID_IShellLink	__AW(IID_IShellLink)
#define IID_IExtractIcon	__AW(IID_IExtractIcon)
#define IID_IShellCopyHook	__AW(IID_IShellCopyHook)
#define IID_IShellExecuteHook	__AW(IID_IShellExecuteHook)
#define IID_INewShortcutHook	__AW(IID_INewShortcutHook)

#if (_WIN32_IE >= 0x400 || _WIN32_WINNT >= _WIN32_WINNT_WIN2K)
extern const GUID IID_IPersistFolder2;
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
extern const GUID IID_IPersistFolder3;
extern const GUID IID_IShellFolder2;
extern const GUID IID_IFileSystemBindData;
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
extern const GUID IID_IFolderView;
#endif

#ifdef __cplusplus
}
#endif

#endif
