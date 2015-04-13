# **** FILE CREATION MASKS
SHELL = /bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

.SUFFIXES: .c .o
	
CC           = cc -g
BIN_PATH     = ../../bin
MAN_PATH     = ../../Man
BINSTALL     = cp
PAR          =
RANLIB	     = ranlib
AR	     = ar r

OTHER = nametable.o
OTHERSRC = nametable.c
#START
A2LOBJ=  \
	AddStamp.o \
	AddTimeStamp.o \
	AddToEquivCla.o \
	AddToNodeList.o \
	AdeAlloc.o \
	ArchiveStuff.o \
	AreConstsEqua.o \
	AreEdgesEqual.o \
	AreNodesDepen.o \
	AreNodesEqual.o \
	AreValuesEqua.o \
	AssignNewKpor.o \
	AssignNewLabe.o \
	AssignNewLoop.o \
	AssignNewLpor.o \
	AssignNewMpor.o \
	AssignNewRpor.o \
	AssignNewTpor.o \
	AssignSourceF.o \
	AssignTags.o \
	AssocListAllo.o \
	AttachEdge.o \
	BuildItems.o \
	CallAlloc.o \
	CallItems.o \
	CanPropagateC.o \
	CaseCmp.o \
	ChangeEdgeToC.o \
	ChangeExportP.o \
	ChangeExports.o \
	ChangeImportP.o \
	ChangeToConst.o \
	ClearTouchFla.o \
	CopyEdge.o \
	CopyEdgeAndLi.o \
	CopyEdgeAndTh.o \
	CopyString.o \
	Count.o \
	CountItems.o \
	CreateAndInse.o \
	CreateNewEqui.o \
	DontCombineFi.o \
	EdgeAlloc.o \
	ElapsedTime.o \
	EnterInSet.o \
	EnterScope.o \
	Error1.o \
	Error2.o \
	ExitScope.o \
	FastAreNodesE.o \
	FindExport.o \
	FindFunction.o \
	FindGraph.o \
	FindGraphNode.o \
	FindImport.o \
	FindInfo.o \
	FindLastGraph.o \
	FindLastImpor.o \
	FindLastNode.o \
	FindNode.o \
	FindSource.o \
	FixConstantIm.o \
	FixSignedCons.o \
	FoldItems.o \
	GatherOthers.o \
	GetNewCallNod.o \
	GetSourceLine.o \
	GetStampStrin.o \
	HasWriteExpor.o \
	If1Read.o \
	If1Write.o \
	If2Read.o \
	If2Write.o \
	ImportSwap.o \
	InfoAlloc.o \
	InitEquivClas.o \
	IntToAscii.o \
	IsABRecord.o \
	IsAdePresent.o \
	IsAggregate.o \
	IsCInterface.o \
	IsCalled.o \
	IsCaller.o \
	IsCandidate.o \
	IsDivByZero.o \
	IsEntryPoint.o \
	IsFortranInte.o \
	IsInnerLoop.o \
	IsIntrinsic.o \
	IsPMarked.o \
	IsPath.o \
	IsPathInit.o \
	IsReadOnly.o \
	IsStamp.o \
	LinkAdeExport.o \
	LinkAdeImport.o \
	LinkAssocList.o \
	LinkExport.o \
	LinkExportLis.o \
	LinkExportToE.o \
	LinkGraph.o \
	LinkImport.o \
	LinkImportLis.o \
	LinkNode.o \
	LinkNodeLists.o \
	LookupInfo.o \
	LowerCase.o

M2ZOBJ = \
	MakeAde.o \
	MakeCompound.o \
	MakeConst.o \
	MakeEdge.o \
	MakeGraph.o \
	MakeInfo.o \
	MakeNode.o \
	MakePtrTypes.o \
	MonoIf1Read.o \
	MonoIf1Write.o \
	MonoMakeCompo.o \
	MonoMakeConst.o \
	MonoMakeEdge.o \
	MonoMakeGraph.o \
	MonoMakeNode.o \
	MonoReadAssoc.o \
	MonoWriteCons.o \
	MonoWriteInfo.o \
	MonoWriteNode.o \
	MyAlloc.o \
	MyBBlockAlloc.o \
	NameAlloc.o \
	NodeAlloc.o \
	NormalizeNode.o \
	PPrintConst.o \
	PPrintIndenta.o \
	Patches.o \
	PlaceGraph.o \
	PlaceNode.o \
	PointToHead.o \
	PopGraphList.o \
	PopNodeList.o \
	PrepareForNex.o \
	PrintLInfo.o \
	PrintWarning.o \
	PropagateCons.o \
	PushGraphList.o \
	PushNodeList.o \
	ReadAssocList.o \
	ReadError.o \
	ReadInteger.o \
	ReadItems.o \
	ReadLineId.o \
	ReadLiteral.o \
	ReadPragmas.o \
	ReadStamp.o \
	ReadString.o \
	RemoveFromEqu.o \
	RemoveStamp.o \
	SameEquivClas.o \
	SetAlloc.o \
	SmashItems.o \
	StartProfiler.o \
	Stop.o \
	StopProfiler.o \
	TSECND.o \
	TempAlloc.o \
	ThreadToUse.o \
	TimeItems.o \
	TouchTheUnion.o \
	UnlinkAdeExpo.o \
	UnlinkAdeImpo.o \
	UnlinkExport.o \
	UnlinkGraph.o \
	UnlinkImport.o \
	UnlinkNode.o \
	UpperCase.o \
	UsageCount.o \
	Warning1.o \
	WriteConst.o \
	WriteGlobals.o \
	WriteImports.o \
	WriteInfo.o \
	WriteLoopMap.o \
	WriteNodes.o \
	WriteStamps.o \
	WriteTheLMap.o
SRC=  \
	AddStamp.c \
	AddTimeStamp.c \
	AddToEquivCla.c \
	AddToNodeList.c \
	AdeAlloc.c \
	ArchiveStuff.c \
	AreConstsEqua.c \
	AreEdgesEqual.c \
	AreNodesDepen.c \
	AreNodesEqual.c \
	AreValuesEqua.c \
	AssignNewKpor.c \
	AssignNewLabe.c \
	AssignNewLoop.c \
	AssignNewLpor.c \
	AssignNewMpor.c \
	AssignNewRpor.c \
	AssignNewTpor.c \
	AssignSourceF.c \
	AssignTags.c \
	AssocListAllo.c \
	AttachEdge.c \
	BuildItems.c \
	CallAlloc.c \
	CallItems.c \
	CaseCmp.c \
	CanPropagateC.c \
	ChangeEdgeToC.c \
	ChangeExportP.c \
	ChangeExports.c \
	ChangeImportP.c \
	ChangeToConst.c \
	ClearTouchFla.c \
	CopyEdge.c \
	CopyEdgeAndLi.c \
	CopyEdgeAndTh.c \
	CopyString.c \
	Count.c \
	CountItems.c \
	CreateAndInse.c \
	CreateNewEqui.c \
	DontCombineFi.c \
	EdgeAlloc.c \
	ElapsedTime.c \
	EnterInSet.c \
	EnterScope.c \
	Error1.c \
	Error2.c \
	ExitScope.c \
	FastAreNodesE.c \
	FindExport.c \
	FindFunction.c \
	FindGraph.c \
	FindGraphNode.c \
	FindImport.c \
	FindInfo.c \
	FindLastGraph.c \
	FindLastImpor.c \
	FindLastNode.c \
	FindNode.c \
	FindSource.c \
	FixConstantIm.c \
	FixSignedCons.c \
	FoldItems.c \
	GatherOthers.c \
	GetNewCallNod.c \
	GetSourceLine.c \
	GetStampStrin.c \
	HasWriteExpor.c \
	If1Read.c \
	If1Write.c \
	If2Read.c \
	If2Write.c \
	ImportSwap.c \
	InfoAlloc.c \
	InitEquivClas.c \
	IntToAscii.c \
	IsABRecord.c \
	IsAdePresent.c \
	IsAggregate.c \
	IsCInterface.c \
	IsCalled.c \
	IsCaller.c \
	IsCandidate.c \
	IsDivByZero.c \
	IsEntryPoint.c \
	IsFortranInte.c \
	IsInnerLoop.c \
	IsIntrinsic.c \
	IsPMarked.c \
	IsPath.c \
	IsPathInit.c \
	IsReadOnly.c \
	IsStamp.c \
	LinkAdeExport.c \
	LinkAdeImport.c \
	LinkAssocList.c \
	LinkExport.c \
	LinkExportLis.c \
	LinkExportToE.c \
	LinkGraph.c \
	LinkImport.c \
	LinkImportLis.c \
	LinkNode.c \
	LinkNodeLists.c \
	LookupInfo.c \
	LowerCase.c \
	MakeAde.c \
	MakeCompound.c \
	MakeConst.c \
	MakeEdge.c \
	MakeGraph.c \
	MakeInfo.c \
	MakeNode.c \
	MakePtrTypes.c \
	MonoIf1Read.c \
	MonoIf1Write.c \
	MonoMakeCompo.c \
	MonoMakeConst.c \
	MonoMakeEdge.c \
	MonoMakeGraph.c \
	MonoMakeNode.c \
	MonoReadAssoc.c \
	MonoWriteCons.c \
	MonoWriteInfo.c \
	MonoWriteNode.c \
	MyAlloc.c \
	MyBBlockAlloc.c \
	NameAlloc.c \
	NodeAlloc.c \
	NormalizeNode.c \
	PPrintConst.c \
	PPrintIndenta.c \
	Patches.c \
	PlaceGraph.c \
	PlaceNode.c \
	PointToHead.c \
	PopGraphList.c \
	PopNodeList.c \
	PrepareForNex.c \
	PrintLInfo.c \
	PrintWarning.c \
	PropagateCons.c \
	PushGraphList.c \
	PushNodeList.c \
	ReadAssocList.c \
	ReadError.c \
	ReadInteger.c \
	ReadItems.c \
	ReadLineId.c \
	ReadLiteral.c \
	ReadPragmas.c \
	ReadStamp.c \
	ReadString.c \
	RemoveFromEqu.c \
	RemoveStamp.c \
	SameEquivClas.c \
	SetAlloc.c \
	SmashItems.c \
	StartProfiler.c \
	Stop.c \
	StopProfiler.c \
	TSECND.c \
	TempAlloc.c \
	ThreadToUse.c \
	TimeItems.c \
	TouchTheUnion.c \
	UnlinkAdeExpo.c \
	UnlinkAdeImpo.c \
	UnlinkExport.c \
	UnlinkGraph.c \
	UnlinkImport.c \
	UnlinkNode.c \
	UpperCase.c \
	UsageCount.c \
	Warning1.c \
	WriteConst.c \
	WriteGlobals.c \
	WriteImports.c \
	WriteInfo.c \
	WriteLoopMap.c \
	WriteNodes.c \
	WriteStamps.c \
	WriteTheLMap.c
#END
INCL   = world.h ../Library/IFX.h

MINIMAL=0
.c.o:
	${CC} -c $*.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

local: libifx.a
optional:
all: local optional

#------------------------------------------------------------
makenames: makenames.c
	${CC} -o makenames makenames.c
nametable.c: ../Library/IFX.h makenames
	makenames < ../Library/IFX.h > nametable.c

#------------------------------------------------------------
libifx.a: $(OTHER) $(A2LOBJ) $(M2ZOBJ)
	/bin/rm -f ./libifx.a
	$(AR) libifx.a $(OTHER) $(A2LOBJ)
	$(AR) libifx.a $(M2ZOBJ)
	$(RANLIB) libifx.a

#------------------------------------------------------------
lint: llib-lifx.ln

llib-lifx.ln: $(SRC) $(OTHERSRC) $(INCL)
	lint -u -v -x -o ifx -woff 91,302,307 \
		`echo -DLINT $(CFLAGS) $(CC) | tr ' ' '\012' | egrep "^-D"` \
		$(SRC) $(OTHERSRC) \
		| tee lint.log \
		| egrep -v '( )(free|malloc|strlen|lseek|read|isspace|isdigit)(.*)llib-lc'

#------------------------------------------------------------
install:
	@echo Nothing to install

clean:
	/bin/rm -f ${A2LOBJ} ${M2ZOBJ} ${OTHER} nametable.c
	/bin/rm -f libifx.a lint.log llib-lifx.ln makenames

${A2LOBJ} ${M2ZOBJ}: ${INCL}

#
