// Includes.h : header file
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPGRIDCONTROL_INCLUDES_H__)
#	define __XTPGRIDCONTROL_INCLUDES_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "GridControl/Resource.h"
#	include "GridControl/XTPGridDefines.h"
#	include "GridControl/XTPGridAllocators.h"
#	include "GridControl/XTPGridColumn.h"
#	include "GridControl/XTPGridColumns.h"
#	include "GridControl/XTPGridControl.h"
#	include "GridControl/XTPGridDragDrop.h"
#	include "GridControl/XTPGridFilterEditControl.h"
#	include "GridControl/XTPGridHeader.h"
#	include "GridControl/XTPGridHyperlink.h"
#	include "GridControl/XTPGridNavigator.h"
#	include "GridControl/XTPGridPaintManager.h"
#	include "GridControl/XTPGridRecord.h"
#	include "GridControl/XTPGridRecordItem.h"
#	include "GridControl/XTPGridRecordItemConstraint.h"
#	include "GridControl/XTPGridRecordItemRange.h"
#	include "GridControl/XTPGridRecordItemControls.h"
#	include "GridControl/XTPGridRecords.h"
#	include "GridControl/XTPGridRow.h"
#	include "GridControl/XTPGridRows.h"
#	include "GridControl/XTPGridSelectedRows.h"
#	include "GridControl/XTPGridGroupRow.h"
#	include "GridControl/XTPGridSubListControl.h"
#	include "GridControl/XTPGridTip.h"
#	include "GridControl/XTPGridInplaceControls.h"
#	include "GridControl/XTPGridView.h"
#	include "GridControl/XTPGridPageSetupDialog.h"
#	include "GridControl/Behavior/XTPGridBehavior.h"
#	include "GridControl/XTPGridSection.h"
#	include "GridControl/XTPGridSections.h"
#	include "GridControl/XTPGridHitTestInfo.h"
#	include "GridControl/XTPGridBorder.h"
#	include "GridControl/XTPGridADO.h"
#	include "GridControl/XTPGridDataManager.h"

#	include "GridControl/ItemTypes/XTPGridRecordItemText.h"
#	include "GridControl/ItemTypes/XTPGridRecordItemNumber.h"
#	include "GridControl/ItemTypes/XTPGridRecordItemDateTime.h"
#	include "GridControl/ItemTypes/XTPGridRecordItemPreview.h"
#	include "GridControl/ItemTypes/XTPGridRecordItemIcon.h"
#	include "GridControl/ItemTypes/XTPGridRecordItemVariant.h"

#	include "GridControl/Themes/XTPGridThemeDefault.h"
#	include "GridControl/Themes/XTPGridThemeExplorer.h"
#	include "GridControl/Themes/XTPGridThemeOffice2003.h"
#	include "GridControl/Themes/XTPGridThemeOffice2003Luna.h"
#	include "GridControl/Themes/XTPGridThemeOffice2013.h"
#	include "GridControl/Themes/XTPGridThemeOfficeXP.h"
#	include "GridControl/Themes/XTPGridThemeResource.h"
#	include "GridControl/Themes/XTPGridThemeNativeWindows10.h"
#	include "GridControl/Themes/XTPGridThemeVisualStudio2012.h"
#	include "GridControl/Themes/XTPGridThemeVisualStudio2015.h"
#	include "GridControl/Themes/XTPGridThemeVisualStudio2017.h"
#	include "GridControl/Themes/XTPGridThemeVisualStudio2019.h"
#	include "GridControl/Themes/XTPGridThemeVisualStudio2022.h"

#	include "GridControl/TrackControl/XTPTrackPaintManager.h"

#	include "GridControl/TrackControl/Themes/XTPTrackThemeDefault.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeExplorer.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeOffice2003.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeOffice2003Luna.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeOffice2013.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeOfficeXP.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeResource.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeVisualStudio2015.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeVisualStudio2017.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeVisualStudio2019.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeVisualStudio2022.h"
#	include "GridControl/TrackControl/Themes/XTPTrackThemeNativeWindows10.h"

#	include "GridControl/TrackControl/XTPTrackControlItem.h"
#	include "GridControl/TrackControl/XTPTrackControl.h"
#	include "GridControl/TrackControl/XTPTrackBlock.h"
#	include "GridControl/TrackControl/XTPTrackHeader.h"
#	include "GridControl/TrackControl/XTPTrackUndoManager.h"

#	include "GridControl/XTPGridDeprecated.h"

#endif //#if !defined(__XTPGRIDCONTROL_INCLUDES_H__)
//}}AFX_CODEJOCK_PRIVATE
