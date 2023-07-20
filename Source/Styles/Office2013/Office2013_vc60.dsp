# Microsoft Developer Studio Project File - Name="Office2013" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Office2013 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Office2013.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Office2013.mak" CFG="Office2013 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Office2013 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release/vc60"
# PROP BASE Intermediate_Dir "Release/vc60"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/vc60"
# PROP Intermediate_Dir "Release/vc60"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCEARSA_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCE_EXPORTS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../" /d "NDEBUG" /d "_AFXDLL" /d "_XTP_INCLUDE_VERSION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Office2013.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build
TargetPath=\Source\ToolkitPro\Source\Styles\Office2013.dll
TargetName=Office2013
InputPath=\Source\ToolkitPro\Source\Styles\Office2013.dll
SOURCE="$(InputPath)"

BuildCmds= \
	if exist "$(TargetPath)" if exist "../../../Workspace/*" copy "$(TargetPath)" "../../../bin/vc60/Styles" \
	if exist "$(TargetPath)" if exist "../../../Workspace/*" copy "$(TargetPath)" "../../../Samples/ActiveX/Styles" \
	if not exist "../../../Workspace/*" if exist "../../../bin/vc60/Styles" rmdir /q "../../../bin/vc60/Styles">nul \
	if not exist "../../../Workspace/*" if exist "../../../Samples/ActiveX/Styles" rmdir /q "../../../Samples/ActiveX/Styles">nul && rmdir /q "../../../Samples/ActiveX">nul \
	

"../../../bin/vc60/Styles/$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"../../../Samples/ActiveX/Styles/$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build
# Begin Target

# Name "Office2013 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Office2013.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ".png;.bmp"
# Begin Group "png"

# PROP Default_Filter "*.png"

# Begin Source File

SOURCE=".\res\backstage-back-button-small-rtl.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-medium-rtl.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-large-rtl.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-extralarge-rtl.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-small.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-medium.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-large.png"
# End Source File

# Begin Source File

SOURCE=".\res\backstage-back-button-extralarge.png"
# End Source File

# Begin Source File

SOURCE=".\res\dark-gray-large-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-large-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-medium-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\light-gray-small-underwater.png"
# End Source File
# Begin Source File

SOURCE=.\res\TimeLineViewAppointmentIcon.png
# End Source File
# Begin Source File

SOURCE=".\res\white-large-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-underwater.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-circles-and-stripes.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-doodle-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-doodle-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-lunchbox.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-school-supplies.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-tree-rings.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-underwater.png"
# End Source File
# End Group
# Begin Group "bmp"

# PROP Default_Filter "*.bmp"
# Begin Source File

SOURCE=.\res\CalendarEventGlyphs.bmp
# End Source File
# Begin Source File

SOURCE=.\Styles\Office2013\res\CalendarEventGlyphs.bmp
# End Source File
# End Group
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ".ini"
# Begin Source File

SOURCE=.\Office2013Access.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013AccessGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013AccessGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Excel.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013ExcelGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013ExcelGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNote.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNoteGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNoteGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Outlook.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OutlookGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OutlookGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPoint.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPointGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPointGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Publisher.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PublisherGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PublisherGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Word.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013WordGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013WordGrayLight.ini
# End Source File
# End Group
# Begin Group "Zip Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Styles\Office2013\XAMLStyle.zip
# End Source File
# End Group
# End Target
# End Project
