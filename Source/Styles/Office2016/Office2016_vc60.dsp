# Microsoft Developer Studio Project File - Name="Office2016" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Office2016 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Office2016.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Office2016.mak" CFG="Office2016 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Office2016 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Office2016.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build
TargetPath=\Projects\Codejock\Source\Styles\Office2016.dll
TargetName=Office2016
InputPath=\Projects\Codejock\Source\Styles\Office2016.dll
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

# Name "Office2016 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Office2016.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ".png;.bmp"
# Begin Group "png"

# PROP Default_Filter "*.png"
# Begin Source File

SOURCE=".\res\backstage-back-button-extralarge-rtl.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-extralarge.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-large-rtl.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-large.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-medium-rtl.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-medium.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-small-rtl.png"
# End Source File
# Begin Source File

SOURCE=".\res\backstage-back-button-small.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\Styles\Office2016\res\black-large-calligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-large-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-signature.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-medium-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\black-small-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-large-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-medium-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-school.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-spring.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-stars.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-straws.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\colorful-small-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-school.png"
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

SOURCE=".\res\dark-gray-large-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-large-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-school.png"
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

SOURCE=".\res\dark-gray-medium-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-medium-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-school.png"
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

SOURCE=".\res\dark-gray-small-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\dark-gray-small-under.png"
# End Source File
# Begin Source File

SOURCE=.\res\TimeLineViewAppointmentIcon.png
# End Source File
# Begin Source File

SOURCE=".\res\white-large-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-school.png"
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

SOURCE=".\res\white-large-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-large-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-school.png"
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

SOURCE=".\res\white-medium-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-medium-under.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-caligraphy.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-circles.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-circuit.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-clouds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-diamonds.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-doodle.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-geometry.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-lunch.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-school.png"
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

SOURCE=".\res\white-small-tree.png"
# End Source File
# Begin Source File

SOURCE=".\res\white-small-under.png"
# End Source File
# End Group
# Begin Group "bmp"

# PROP Default_Filter "*.bmp"
# Begin Source File

SOURCE=.\res\CalendarEventGlyphs.bmp
# End Source File
# End Group
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ".ini"
# Begin Source File

SOURCE=.\Office2016AccessBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016AccessColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016AccessDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016AccessWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016ExcelBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016ExcelColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016ExcelDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016ExcelWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OneNoteBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OneNoteColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OneNoteDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OneNoteWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OutlookBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OutlookColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OutlookDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016OutlookWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PowerPointBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PowerPointColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PowerPointDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PowerPointWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PublisherBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PublisherColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PublisherDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016PublisherWhite.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016WordBlack.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016WordColorful.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016WordDarkGray.ini
# End Source File
# Begin Source File

SOURCE=.\Office2016WordWhite.ini
# End Source File
# End Group
# Begin Group "Zip Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Styles\Office2016\XAMLStyle.zip
# End Source File
# End Group
# End Target
# End Project
