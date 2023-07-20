// XTPFlowGraphUndoManager.h: interface for the CXTPFlowGraphUndoManager class.
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
#if !defined(__XTPFLOWGRAPHUNDOMANAGER_H__)
#	define __XTPFLOWGRAPHUNDOMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

const int XTP_ID_FLOWGRAPHUNDO_GROUP				 = -1;   // The undo group command.
const int XTP_ID_FLOWGRAPHUNDO_ADDNODE				 = 1003; // The undo add Node command.
const int XTP_ID_FLOWGRAPHUNDO_DELETENODE			 = 1004; // The undo delete Node command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODEPOSITION		 = 1005; // The undo set Node position command.
const int XTP_ID_FLOWGRAPHUNDO_DELETECONNECTION		 = 1006; // The undo delete connection command.
const int XTP_ID_FLOWGRAPHUNDO_SETCONNECTIONPOSITION = 1007; // The undo set connection position
															 // command.
const int XTP_ID_FLOWGRAPHUNDO_ADDCONNECTION		 = 1008; // The undo add connection command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODECAPTION		 = 1009; // The undo set Node caption command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODESIZE			 = 1010; // The undo set Node size command.
const int XTP_ID_FLOWGRAPHUNDO_DELETECONNECTIONPOINT = 1011; // The undo delete point command.
const int XTP_ID_FLOWGRAPHUNDO_ADDCONNECTIONPOINT	= 1012; // The undo add point command.

//===========================================================================
// Summary:
//     This class abstracts the undo command.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoCommand
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoCommand object.
	// Parameters:
	//     nID - The command ID.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoCommand(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoCommand object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoCommand();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the command ID associated with this object.
	// Returns:
	//     An int value denoting the command ID associated with this object.
	//-----------------------------------------------------------------------
	int GetID() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	const int m_nID; // The command ID.

	friend class CXTPFlowGraphUndoManager;
	friend class CXTPFlowGraphUndoGroupCommand;
};

//===========================================================================
// Summary:
//     This class abstracts the undo commands in a group.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoGroupCommand : public CXTPFlowGraphUndoCommand
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoGroupCommand object.
	// Parameters:
	//     pManager - Pointer to a CXTPFlowGraphUndoManager object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoGroupCommand(CXTPFlowGraphUndoManager* pManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoGroupCommand object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoGroupCommand();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the number of command objects in this
	//     group.
	// Returns:
	//     An int value denoting the number of command objects in this group.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a command to the command group.
	// Parameters:
	//     pCommand - Pointer to a CXTPFlowGraphUndoCommand object.
	//-----------------------------------------------------------------------
	void Add(CXTPFlowGraphUndoCommand* pCommand);

protected:
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrUndoCommands; // The command
																					// collection.
	CXTPFlowGraphUndoManager* m_pManager; // The undo manager.
	friend class CXTPFlowGraphUndoManager;
};

//===========================================================================
// Summary:
//     This class abstracts the undo delete Node command. This is a kind of
//     CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteNodeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoDeleteNodeCommand object.
	// Parameters:
	//     pNode - Pointer to a CXTPFlowGraphNode object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteNodeCommand(CXTPFlowGraphNode* pNode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoDeleteNodeCommand object, handles
	//     cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteNodeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode; // The FlowGraph Node object.
	CXTPFlowGraphPage* m_pPage; // The FlowGraph page object.
};

//===========================================================================
// Summary:
//     This class abstracts the undo delete connection point command. This
//     is a kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteConnectionPointCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoDeleteConnectionPointCommand object.
	// Parameters:
	//     pConnectionPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoDeleteConnectionPointCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteConnectionPointCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;						  // The FlowGraph Node object.
	CXTPFlowGraphConnectionPoint* m_pConnectionPoint; // The FlowGraph connection point object.
	int m_nIndex;
};

//===========================================================================
// Summary:
//     This class abstracts the undo delete connection command. This is a
//     kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteConnectionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoDeleteConnectionCommand object.
	// Parameters:
	//     pConnection - Pointer to a CXTPFlowGraphConnection object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteConnectionCommand(CXTPFlowGraphConnection* pConnection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoDeleteConnectionCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteConnectionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection; // The FlowGraph connection object.
	CXTPFlowGraphConnectionPoint* m_pInputPoint;
	CXTPFlowGraphConnectionPoint* m_pOutputPoint;
	CXTPFlowGraphPage* m_pPage; // The FlowGraph page object.
};

//===========================================================================
// Summary:
//     This class abstracts the undo add Node command. This is a kind of
//     CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddNodeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoAddNodeCommand object.
	// Parameters:
	//     pNode - Pointer to a CXTPFlowGraphNode object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddNodeCommand(CXTPFlowGraphNode* pNode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoAddNodeCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddNodeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode; // Pointer to the FlowGraph Node object.
};

//===========================================================================
// Summary:
//     This class abstracts the undo add connection point command. This is a
//     kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddConnectionPointCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoAddConnectionPointCommand object.
	// Parameters:
	//     pConnectionPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoAddConnectionPointCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddConnectionPointCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnectionPoint* m_pConnectionPoint; // Pointer to the FlowGraph point object.
};

//===========================================================================
// Summary:
//     This class abstracts the undo add connection command. This is a kind
//     of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddConnectionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoAddConnectionCommand object.
	// Parameters:
	//     pConnection - Pointer to a CXTPFlowGraphConnection object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddConnectionCommand(CXTPFlowGraphConnection* pConnection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoAddConnectionCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddConnectionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection; // Pointer to the FlowGraph connection object.
};

//===========================================================================
// Summary:
//     This class abstracts the undo set Node position command. This is a
//     kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodePositionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoSetNodePositionCommand object.
	// Parameters:
	//     pNode         - Pointer to a CXTPFlowGraphNode object.
	//     ptOldPosition - The old position of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodePositionCommand(CXTPFlowGraphNode* pNode, CPoint ptOldPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoSetNodePositionCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodePositionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode; // Pointer to the FlowGraph Node object.
	CPoint m_ptOldPosition;		// Old position of the Node.
};

//===========================================================================
// Summary:
//     This class abstracts the undo set Node size command. This is a
//     kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodeSizeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoSetNodeSizeCommand object.
	// Parameters:
	//     pNode        - Pointer to a CXTPFlowGraphNode object.
	//     szOldSize    - The old size of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodeSizeCommand(CXTPFlowGraphNode* pNode, CSize szOldSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoSetNodeSizeCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodeSizeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode; // Pointer to the FlowGraph Node object.
	CPoint m_szOldSize;			// Old size of the Node.
};

//===========================================================================
// Summary:
//     This class abstracts the undo set Node caption command. This is a
//     kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodeCaptionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoSetNodeCaptionCommand object.
	// Parameters:
	//     pNode         - Pointer to a CXTPFlowGraphNode object.
	//     strOldCaption - The old caption of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodeCaptionCommand(CXTPFlowGraphElement* pNode, CString strOldCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoSetNodeCaptionCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodeCaptionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphElement* m_pNode; // Pointer to the FlowGraph Node object.
	CString m_strOldCaption;	   // Old caption of the Node.
};

//===========================================================================
// Summary:
//     This class abstracts the undo set connection position command. This is
//     a kind of CXTPFlowGraphUndoCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetConnectionPositionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoSetConnectionPositionCommand object.
	// Parameters:
	//     pConnection   - Pointer to a CXTPFlowGraphConnection object.
	//     ptOldPosition - The old position of the connection.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetConnectionPositionCommand(CXTPFlowGraphConnection* pConnection,
												  CPoint ptOldPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoSetConnectionPositionCommand object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetConnectionPositionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     This is a virtual function, so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection; // Pointer to the FlowGraph connection object.
	CPoint m_ptOldPosition;					// Old position of the connection.
};

//===========================================================================
// Summary:
//     This class abstracts an undo manager. It manages all the undo
//     operations of the FlowGraph control. This is a kind of CXTPCmdTarget.
// Remarks:
//     The FlowGraphUndoManager allows for certain edit operations to be
//     undone and re-done in the flow graph. Supported operations are
//     Add/Remove/Move/Resize Nodes/Move Connector Lines/Rename Nodes/Grouping.
//     If any of these operations are performed, then they can easily
//     be undone or re-done using the methods of the FlowGraphUndoManager.
//
//     StartGroup and EndGroup are used to "group" a set of "undoable"
//     operations together. Grouping the operations together is useful as
//     grouped operations can be undone or re-done with a single call
//     to Undo or Redo. If the operations are not grouped, then you would
//     have to call Undo or Redo for each operation performed.
//
//     Any "undoable" operations performed between StartGroup and EndGroup
//     will be undone or re-done with a single call to Undo or Redo.
//===========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphUndoManager : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphUndoManager object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphUndoManager object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	//-----------------------------------------------------------------------
	virtual void Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the redo operation.
	//-----------------------------------------------------------------------
	virtual void Redo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine if the undo operation is possible
	//     in the current context.
	// Returns:
	//     TRUE if the undo operation is possible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL CanUndo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine if the redo operation is possible
	//     in the current context.
	// Returns:
	//     TRUE if the redo operation is possible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL CanRedo() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to clear the list of undo and redo operations.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to add an undo command to the collection of undo
	//     commands.
	// Parameters:
	//     pCommand - Pointer to a CXTPFlowGraphUndoCommand object.
	//-----------------------------------------------------------------------
	void AddUndoCommand(CXTPFlowGraphUndoCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to start an undo group command.
	//-----------------------------------------------------------------------
	virtual void StartGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to end an undo group command.
	//-----------------------------------------------------------------------
	virtual void EndGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to enable/disable the undo manager.
	// Parameters:
	//     bEnabled - TRUE to enable, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableUndoManager(BOOL bEnabled);

protected:
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrUndoCommands; // The undo
																					// commands
																					// array.
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrRedoCommands; // The redo
																					// command
																					// array.

	CArray<CXTPFlowGraphUndoGroupCommand*, CXTPFlowGraphUndoGroupCommand*>
		m_arrGroupStack; // The groups stack.

	int m_nUndoContext; // The undo context.
	BOOL m_bEnabled;	// Specifies whether the undo manager is enabled or disabled.
	CXTPFlowGraphUndoGroupCommand* m_pUndoGroup; // The undo group command.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPFlowGraphUndoManager);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPFlowGraphUndoCommand::GetID() const
{
	return m_nID;
}
AFX_INLINE void CXTPFlowGraphUndoManager::EnableUndoManager(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHUNDOMANAGER_H__)
