// XTPSyntaxEditUndoManager.h
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITUNDOMANAGER_H__)
#	define __XTPSYNTAXEDITUNDOMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditBufferManager;
class CXTPSyntaxEditCtrl;

//===========================================================================
// Summary:
//     This class represents an abstract Edit Control command interface.
//     Its descendants must implement custom Execute and UnExecute
//     functionality depending on a command type.
//
//     This class is used internally by the library only.
// See Also:
//     CXTPSyntaxEditUndoRedoManager, CXTPSyntaxEditBatchCommand,
//     CXTPSyntaxEditBufferCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditCommand : public CObject
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object, handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     This is an interface function for operation execution.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the action.
	//     lcTo      - [out] End text position affected by the action.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the following edit actions:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See also:
	//     UnExecute
	//-----------------------------------------------------------------------
	virtual int Execute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						CXTPSyntaxEditCtrl* pEditCtrl) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This is an interface function for operation undoing.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the action.
	//     lcTo      - [out] End text position affected by the action.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the following edit actions:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See also:
	//     Execute
	//-----------------------------------------------------------------------
	virtual int UnExecute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						  CXTPSyntaxEditCtrl* pEditCtrl) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets command text.
	// Remarks:
	//     Each command has a string describing contained changes.
	//     (e.g. "Typing: something", "Delete", "Paste", etc.).
	//     This string is referred to as command text.
	//
	//     This function allows for a command to overwrite its
	//     own command text retrieval.
	// Returns:
	//     Command text.
	// See also:
	//     SetCommandText
	//-----------------------------------------------------------------------
	virtual LPCTSTR GetCommandText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets command text.
	// Parameters:
	//     szText - [in] Command text to be set.
	// Remarks:
	//     Each command has a string describing contained changes.
	//     (e.g. "Typing: something", "Delete", "Paste", etc.).
	//     This string is referred to as command text.
	//
	//     This function allows for a command to overwrite its
	//     own command text.
	// See also:
	//      GetCommandText
	//-----------------------------------------------------------------------
	virtual void SetCommandText(LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo command data size (in bytes).
	// Returns:
	//     The undo command data size (in bytes).
	//-----------------------------------------------------------------------
	virtual int GetDataSize() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor because this class could not be instantiated
	//     itself, only by its descendants.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the line and column position for the specified control using
	//     internal fast methods.
	// Parameters:
	//     pEditCtrl - [in] Pointer to the edit control.
	//     lcPos     - [in] Reference to the text position
	//                      to be set to the edit control.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetPositionInternally(CXTPSyntaxEditCtrl* pEditCtrl,
												const XTP_EDIT_LINECOL& lcPos);

	CString m_strCommandText; // Text description of the command.
};

AFX_INLINE void CXTPSyntaxEditCommand::SetCommandText(LPCTSTR szText)
{
	m_strCommandText = szText;
}
AFX_INLINE LPCTSTR CXTPSyntaxEditCommand::GetCommandText() const
{
	return m_strCommandText;
}
AFX_INLINE int CXTPSyntaxEditCommand::GetDataSize() const
{
	return 0;
}
//===========================================================================
// Summary:
//     This class represents a pack of commands in one.
//     It stores commands in the list, and implements standard command
//     execution interfaces on command (un)execution.
// See Also:
//     CXTPSyntaxEditCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditBatchCommand : public CXTPSyntaxEditCommand
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditBatchCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	CXTPSyntaxEditBatchCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object with all sub-commands,
	//     handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	~CXTPSyntaxEditBatchCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Executes the batch command by successively executing all stored commands.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command chain.
	//     lcTo      - [out] End text position affected by the command chain.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     UnExecute
	//-----------------------------------------------------------------------
	virtual int Execute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unexecutes the batch command by successively undoing all stored commands.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command chain.
	//     lcTo      - [out] End text position affected by the command chain.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     Execute
	//-----------------------------------------------------------------------
	virtual int UnExecute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						  CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a command to the batch command buffer.
	// Parameters:
	//     pCommand - [in] Pointer to the command to be added.
	// Returns:
	//     The position of the newly added command inside the internal list.
	// Example:
	//     pBatchCmd->AddCommand(new CXTPSyntaxEditDeleteStringCommand(...));
	//-----------------------------------------------------------------------
	POSITION AddCommand(CXTPSyntaxEditCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of individual command items inside
	//     the batch command buffer.
	// Returns:
	//     The number of individual command items inside
	//     the batch command buffer.
	//-----------------------------------------------------------------------
	int GetCommandsCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo command data size (in bytes).
	// Returns:
	//     The undo command data size (in bytes).
	//-----------------------------------------------------------------------
	virtual int GetDataSize() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Clears the batch command buffer and deletes all commands.
	//-----------------------------------------------------------------------
	void Clear();

	CPtrList m_CommandList; // Commands buffer storage.
};

//===========================================================================
// Summary:
//     The base class for all edit commands related to CXTPSyntaxEditBufferManager.
//     It stores the handle of the buffer manager and provides implementation
//     of common buffer operations such as text insertion and deletion.
// See Also:
//     CXTPSyntaxEditCommand, CXTPSyntaxEditInsertStringCommand,
//     CXTPSyntaxEditDeleteStringCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditBufferCommand : public CXTPSyntaxEditCommand
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditBufferCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object, handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditBufferCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the buffer command object, initializes its members.
	// Parameters:
	//     pMgr   - [in] Pointer to the associated buffer manager object.
	//     szText - [in] Text which was changed during the buffer operation.
	//                   It will be used for insert/remove text operations.
	//     lcFrom - [in] Start text position affected by the command.
	//     lcTo   - [in] End text position affected by the command.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferCommand(CXTPSyntaxEditBufferManager* pMgr, LPCTSTR szText,
								const XTP_EDIT_LINECOL& lcFrom, const XTP_EDIT_LINECOL& lcTo);

	CXTPSyntaxEditBufferManager* m_pBufferMgr; // Pointer to the associated text buffer.

	//=======================================================================
	// Summary:
	//     Keeps the buffer overwrite flag for the specified buffer manager
	//     from class construction until destruction, and resets this flag to
	//     FALSE during class lifetime.
	// Example:
	//  void DoSomething()
	//  {
	//      CXTPSyntaxEditBufferKeepOverwriteSettings bufOwr(m_pBufferMgr);
	//      ...
	//  }
	//=======================================================================
	class CXTPSyntaxEditBufferKeepOverwriteSettings
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Creates the object and stores buffer overwrite settings.
		// Parameters:
		//     pBufferMgr - [in] Pointer to the associated buffer manager object.
		//-------------------------------------------------------------------
		CXTPSyntaxEditBufferKeepOverwriteSettings(CXTPSyntaxEditBufferManager* pBufferMgr);

		//-------------------------------------------------------------------
		// Summary:
		//     Destroys the object and restores buffer overwrite settings.
		//-------------------------------------------------------------------
		virtual ~CXTPSyntaxEditBufferKeepOverwriteSettings();

	private:
		CXTPSyntaxEditBufferManager* m_pBufMgr; // Associated buffer manager.
		BOOL m_bOldSettings;					// Buffer overwrite settings.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts the stored text at the stored positions and returns
	//     the start and final affected text positions.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     DoDeleteText
	//-----------------------------------------------------------------------
	int DoInsertText(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
					 CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes the text at the stored positions and returns
	//     the start and final affected text positions.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     DoInsertText
	//-----------------------------------------------------------------------
	int DoDeleteText(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
					 CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo command data size (in bytes).
	// Returns:
	//     The undo command data size (in bytes).
	//-----------------------------------------------------------------------
	virtual int GetDataSize() const;

protected:
	CString m_strText;		   // Changed text.
	XTP_EDIT_LINECOL m_lcFrom; // Start edit position.
	XTP_EDIT_LINECOL m_lcTo;   // End edit position.
};

//===========================================================================
// Summary:
//     This class represents the insert string command.
//     Command execution will insert specified text in the buffer
//     and un-execution will delete this text from the buffer.
// See Also:
//      CXTPSyntaxEditBufferCommand, CXTPSyntaxEditDeleteStringCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditInsertStringCommand : public CXTPSyntaxEditBufferCommand
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditInsertStringCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the insert string command object, initializes its members.
	// Parameters:
	//     pMgr   - [in] Pointer to the associated buffer manager object.
	//     szText - [in] Text which was changed during the buffer operation.
	//                   It will be used for insert/remove text operations.
	//     lcFrom - [in] Start text position affected by the command.
	//     lcTo   - [in] End text position affected by the command.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditInsertStringCommand(CXTPSyntaxEditBufferManager* pMgr, LPCTSTR szText,
									  const XTP_EDIT_LINECOL& lcFrom, const XTP_EDIT_LINECOL& lcTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object, handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditInsertStringCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Executes the insert string command.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     UnExecute
	//-----------------------------------------------------------------------
	virtual int Execute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unexecutes the insert string command (i.e. deletes the string).
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW
	// See Also:
	//     Execute
	//-----------------------------------------------------------------------
	virtual int UnExecute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						  CXTPSyntaxEditCtrl* pEditCtrl);
};

//===========================================================================
// Summary:
//     This class represents the delete string command.
//     Command execution will delete the text from the buffer between the specified
//     text positions and un-execution will insert this text into the buffer.
// See Also:
//     CXTPSyntaxEditBufferCommand, CXTPSyntaxEditInsertStringCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditDeleteStringCommand : public CXTPSyntaxEditBufferCommand
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditDeleteStringCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the delete string command object, initializes its members.
	// Parameters:
	//     pMgr   - [in] Pointer to the associated buffer manager object.
	//     szText - [in] Text which was changed during the buffer operation.
	//                   It will be used for insert/remove text operations.
	//     lcFrom - [in] Start text position affected by the command.
	//     lcTo   - [in] End text position affected by the command.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDeleteStringCommand(CXTPSyntaxEditBufferManager* pMgr, LPCTSTR szText,
									  const XTP_EDIT_LINECOL& lcFrom, const XTP_EDIT_LINECOL& lcTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object, handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditDeleteStringCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Executes the delete string command.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW
	// See Also:
	//     UnExecute
	//-----------------------------------------------------------------------
	virtual int Execute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unexecutes the delete string command (i.e. inserts the string).
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     Execute
	//-----------------------------------------------------------------------
	virtual int UnExecute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						  CXTPSyntaxEditCtrl* pEditCtrl);
};

//===========================================================================
// Summary:
//     This class represents the replace string command.
//     Command execution will replace the text from the buffer between the specified
//     text positions and un-execution will do the contrary replacement.
// See Also:
//     CXTPSyntaxEditBufferCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditReplaceStringCommand : public CXTPSyntaxEditBufferCommand
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditReplaceStringCommand)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the replace string command object, initializes its members.
	// Parameters:
	//     pMgr           - [in] Pointer to the associated buffer manager object.
	//     szText         - [in] Text which was inserted during the buffer operation.
	//     szReplacedText - [in] Text which was changed during the buffer operation.
	//     lcFrom         - [in] Start text position affected by the command.
	//     lcTo           - [in] End text position affected by the command.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditReplaceStringCommand(CXTPSyntaxEditBufferManager* pMgr, LPCTSTR szText,
									   LPCTSTR szReplacedText, const XTP_EDIT_LINECOL& lcFrom,
									   const XTP_EDIT_LINECOL& lcTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     A default command destructor.
	//     Destroys the command object, handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditReplaceStringCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Executes the delete string command.
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW
	// See Also:
	//     UnExecute
	//-----------------------------------------------------------------------
	virtual int Execute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unexecutes the delete string command (i.e. inserts the string).
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     Execute
	//-----------------------------------------------------------------------
	virtual int UnExecute(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
						  CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo command data size (in bytes).
	// Returns:
	//     The undo command data size (in bytes).
	//-----------------------------------------------------------------------
	virtual int GetDataSize() const;

protected:
	CString m_strReplacedText; // Buffer which holds the replaced text.

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs the replace text operation.
	// Parameters:
	//     szText    - [in] Text to replace to.
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_INSERTROW
	//-----------------------------------------------------------------------
	int DoReplaceText(LPCTSTR szText, XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo,
					  CXTPSyntaxEditCtrl* pEditCtrl);
};

//===========================================================================
// Summary:
//     This class maintains the list of undo/redo commands.
//     It is managed by the CEditBufferManager class.
//
//     This class allows for adding new commands in the buffer (see AddCommand),
//     undoing the last added operation (see DoUndo), and redoing operations
//     following the latest unexecuted operation (see DoRedo).
//
//     This class also allows for checking if it is possible to perform undo/redo
//     operations (see CanUndo / CanRedo).
//
//     This class also allows for marking the current position of the undo/redo
//     operation queue as saved, and returns the "modified" flag if the
//     queue position will be changed (see MarkSaved / IsModified).
//
//     Another piece of the functionality is commands grouping. The following
//     methods are related to this: SetGroupInsertMode and ChainLastCommand.
//     ChainLastCommand forces the latest added undo command to be merged into
//     a group with the previous undo command. This method could be useful
//     for group operations like moving a selected text by mouse. In this
//     case, there would be 2 different commands added: remove selected text
//     from its original position and insert it into the new position.
//     By using ChainLastCommand, both of these operations would be merged into
//     a single ooperation, which will be further done/undone together.
//     SetGroupInsertMode forces the undo/redo manager to merge all added
//     operations into a single chain. For example, this could be useful when
//     the user is typing some text, which could be later done/undone as
//     a single word, instead of individually by each character.
//
//     Also, one more piece of functionality is related to the command
//     description text management. Related functions allow setting the
//     new text description for the last undo command (SetLastCommandText).
//     As an example, it could be used after the ChainLastCommand method
//     for setting the new description for the merged command (e.g. 'Move'
//     instead of 'Delete'+'Insert').
//
//     Another two functions allow for the retrieval of string description lists
//     from the list of undo/redo commands (GetUndoTextList / GetRedoTextList).
//
//     However, this class is used internally by the library only.
// See Also:
//     CXTPSyntaxEditCommand
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditUndoRedoManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the undo/redo manager object, initializes its members.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditUndoRedoManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the undo/redo manager object,
	//     handles its cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditUndoRedoManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs a single undo operation (unexecutes the latest command).
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     DoRedo
	//-----------------------------------------------------------------------
	int DoUndo(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs a single redo operation (executes the next command).
	// Parameters:
	//     lcFrom    - [out] Start text position affected by the command.
	//     lcTo      - [out] End text position affected by the command.
	//     pEditCtrl - [in] Pointer to the edit control.
	// Returns:
	//     A bitwise combination of the edit actions that occurred:
	//     XTP_EDIT_EDITACTION_MODIFYROW, XTP_EDIT_EDITACTION_DELETEROW,
	//     XTP_EDIT_EDITACTION_INSERTROW
	// See Also:
	//     DoUndo
	//-----------------------------------------------------------------------
	int DoRedo(XTP_EDIT_LINECOL& lcFrom, XTP_EDIT_LINECOL& lcTo, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a command to the undo buffer.
	//     Deletes all redo commands if they exist.
	// Parameters:
	//     pCommand - [in] Pointer to the undo command to be added.
	//-----------------------------------------------------------------------
	void AddCommand(CXTPSyntaxEditCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the undo manager buffer and deletes all commands.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if it is possible to perform the undo action.
	// Returns:
	//     TRUE if it is possible to perform the undo action, otherwise FALSE.
	// See Also:
	//     CanRedo
	//-----------------------------------------------------------------------
	BOOL CanUndo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if it is possible to perform the redo action.
	// Returns:
	//     TRUE if it is possible to perform the redo action, otherwise FALSE.
	// See Also:
	//     CanUndo
	//-----------------------------------------------------------------------
	BOOL CanRedo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Marks the current command buffer position as last saved.
	// Remarks:
	//     This method is used for the further calculating of the modified flag.
	// See Also:
	//     IsModified
	//-----------------------------------------------------------------------
	void MarkSaved();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the document was modified by any command from
	//     the buffer since the document was last saved/loaded.
	// Returns:
	//     TRUE if the document was modified by any command from the buffer
	//     since the document was last saved/loaded, otherwise FALSE.
	// See Also:
	//     MarkSaved
	//-----------------------------------------------------------------------
	BOOL IsModified() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables group insert mode.
	// Parameters:
	//     bInsertInGroup - [in] TRUE to enable group insert mode,
	//                           FALSE to disable group insert mode.
	// Remarks:
	//     Group insert mode is required when a user is typing certain text in
	//     normal INS mode. When an undo/redo command occurs, a group of text
	//     will be undone/redone in a single shot. This setting is cleared when
	//     in overwrite mode or while the user presses SPACE, TAB, ENTER, etc.
	// See Also:
	//     ChainLastCommand
	//-----------------------------------------------------------------------
	void SetGroupInsertMode(BOOL bInsertInGroup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Merges the latest undo command into a batch command with
	//     the previous undo command.
	// See Also:
	//     SetGroupInsertMode
	//-----------------------------------------------------------------------
	void ChainLastCommand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text for the last undo command in the stack.
	// Parameters:
	//     szText  - [in] Text to be set.
	//     nTextId - [in] Resource text identifier of the text string to be set.
	// See Also:
	//     GetUndoTextList, GetRedoTextList
	//-----------------------------------------------------------------------
	void SetLastCommandText(LPCTSTR szText);
	void SetLastCommandText(
		UINT nTextId); // <combine CXTPSyntaxEditUndoRedoManager::SetLastCommandText@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of text for undo operations.
	// Returns:
	//     A reference to the list of text for undo operations.
	// See Also:
	//     GetRedoTextList, SetLastCommandText
	//-----------------------------------------------------------------------
	const CStringList& GetUndoTextList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of text for redo operations.
	// Returns:
	//     A reference to the list of text for redo operations.
	// See Also:
	//     GetUndoTextList, SetLastCommandText
	//-----------------------------------------------------------------------
	const CStringList& GetRedoTextList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo buffer data size (in bytes).
	// Returns:
	//     The undo buffer data size (in bytes).
	//-----------------------------------------------------------------------
	virtual int GetDataSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks the undo buffer data size and removes old stored commands
	//     to clear memory if the data size limit has been reached.
	// Parameters:
	//     nNewCommandData - Size (in bytes) of the undo command to be added.
	//     nDataSizeLimit  - Data size limit of the undo buffer.
	//-----------------------------------------------------------------------
	virtual void LimitDataSize(int nNewCommandData, int nDataSizeLimit);

private:
	//-----------------------------------------------------------------------
	// Summary:
	//      Removes the command queue tail after the current element.
	//-----------------------------------------------------------------------
	void RemoveTail();

protected:
	CPtrList m_CommandList;  // Commands buffer.
	POSITION m_posFirstUndo; // Actual command position in the buffer.
	POSITION m_posSavedMark; // Last saved command position in the buffer.

	BOOL m_bGroupInsertMode; // TRUE if group insert mode is on, FALSE otherwise.
	int m_nDataSizeLimit;	// Memory limit for undo data.

private:
	CStringList m_lstUndoText; // Temporary storage for undo text strings.
	CStringList m_lstRedoText; // Temporary storage for redo text strings.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITUNDOMANAGER_H__)
