# Microsoft Developer Studio Project File - Name="BFCEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BFCEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BFCEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BFCEngine.mak" CFG="BFCEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BFCEngine - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BFCEngine - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BFCEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "BFC_EDITOR" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x100c /d "NDEBUG"
# ADD RSC /l 0x100c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3d8.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BFCEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "BFC_EDITOR" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x100c /d "_DEBUG"
# ADD RSC /l 0x100c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 D3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3d8.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BFCEngine - Win32 Release"
# Name "BFCEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BFCBase.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCEngineState.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCGraphicEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCLight.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCMath.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCPipeline.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCSaveLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BFCAssert.h
# End Source File
# Begin Source File

SOURCE=.\BFCBase.h
# End Source File
# Begin Source File

SOURCE=.\BFCDebug.h
# End Source File
# Begin Source File

SOURCE=.\BFCEngineState.h
# End Source File
# Begin Source File

SOURCE=.\BFCGraphicEngine.h
# End Source File
# Begin Source File

SOURCE=.\BFCLight.h
# End Source File
# Begin Source File

SOURCE=.\BFCManager.h
# End Source File
# Begin Source File

SOURCE=.\BFCMath.h
# End Source File
# Begin Source File

SOURCE=.\BFCMesh.h
# End Source File
# Begin Source File

SOURCE=.\BFCPipeline.h
# End Source File
# Begin Source File

SOURCE=.\BFCSaveLoad.h
# End Source File
# Begin Source File

SOURCE=.\BFCTexture.h
# End Source File
# Begin Source File

SOURCE=.\BFCUtility.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# End Group
# Begin Group "Editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BFCKeyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCKeyManager.h
# End Source File
# Begin Source File

SOURCE=.\EdFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\EdFunc.h
# End Source File
# Begin Source File

SOURCE=.\EditorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditorState.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorState.h
# End Source File
# Begin Source File

SOURCE=.\EdMain.cpp
# End Source File
# Begin Source File

SOURCE=.\EdMain.h
# End Source File
# Begin Source File

SOURCE=.\Primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\Primitives.h
# End Source File
# End Group
# Begin Group "DialogClass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BFCDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BFCDialog.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SimpleEdit.rc
# End Source File
# End Group
# End Target
# End Project
