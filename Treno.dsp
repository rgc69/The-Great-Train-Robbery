# Microsoft Developer Studio Project File - Name="Steering" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Steering - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Treno.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Treno.mak" CFG="Steering - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Steering - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Steering - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Steering - Win32 Release"

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
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Steering - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Steering - Win32 Release"
# Name "Steering - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Arma_Leggera.cpp
# End Source File
# Begin Source File

SOURCE=.\Armi.cpp
# End Source File
# Begin Source File

SOURCE=.\AttackTargetGoal_Evaluator.cpp
# End Source File
# Begin Source File

SOURCE=.\CrudeTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\EntitaBase.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ExploreGoal_Evaluator.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzyModule.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzyOperators.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_LeftShoulder.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_RightShoulder.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_Singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\FuzzyVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\GetHealthGoal_Evaluator.cpp
# End Source File
# Begin Source File

SOURCE=.\GetWeaponGoal_Evaluator.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_AdjustRange.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_AttackTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_Explore.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_FollowPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_GetItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_HuntTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_MoveToPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_SeekToPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_Think.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_TraverseEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal_Wander.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\MondoDiGioco.cpp
# End Source File
# Begin Source File

SOURCE=.\PrecisionTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Proiettile_Leggero.cpp
# End Source File
# Begin Source File

SOURCE=.\Proiettili.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile_Pellet.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile_Rocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile_Slug.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_Feature.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_Goal_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_PathPlanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_Scriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_SensoryMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_SteeringBehaviors.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_TargetingSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Raven_UserOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\SistemaDiArma.cpp
# End Source File
# Begin Source File

SOURCE=.\SistemaMovimento.cpp
# End Source File
# Begin Source File

SOURCE=.\SpotIncidente.cpp
# End Source File
# Begin Source File

SOURCE=.\Trigger_HealthGiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Trigger_SoundNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\Trigger_WeaponGiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Veicolo.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon_Blaster.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon_RailGun.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon_RocketLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon_ShotGun.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Arma_Leggera.h
# End Source File
# Begin Source File

SOURCE=.\Armi.h
# End Source File
# Begin Source File

SOURCE=.\AStarHeuristicPolicies.h
# End Source File
# Begin Source File

SOURCE=.\AttackTargetGoal_Evaluator.h
# End Source File
# Begin Source File

SOURCE=.\Binario2D.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\CellSpacePartition.h
# End Source File
# Begin Source File

SOURCE=.\constants.h
# End Source File
# Begin Source File

SOURCE=.\CrudeTimer.h
# End Source File
# Begin Source File

SOURCE=.\DebugConsole.h
# End Source File
# Begin Source File

SOURCE=.\demo3_1res.h
# End Source File
# Begin Source File

SOURCE=.\EntitaBase.h
# End Source File
# Begin Source File

SOURCE=.\EntityManager.h
# End Source File
# Begin Source File

SOURCE=.\EnumerazioneOggetti.h
# End Source File
# Begin Source File

SOURCE=.\ExploreGoal_Evaluator.h
# End Source File
# Begin Source File

SOURCE=.\FrameCounter.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyHedges.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyModule.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyOperators.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyRule.h
# End Source File
# Begin Source File

SOURCE=.\FuzzySet.h
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_LeftShoulder.h
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_RightShoulder.h
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_Singleton.h
# End Source File
# Begin Source File

SOURCE=.\FuzzySet_Triangle.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyTerm.h
# End Source File
# Begin Source File

SOURCE=.\FuzzyVariable.h
# End Source File
# Begin Source File

SOURCE=.\FzSet.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\GetHealthGoal_Evaluator.h
# End Source File
# Begin Source File

SOURCE=.\GetWeaponGoal_Evaluator.h
# End Source File
# Begin Source File

SOURCE=.\Goal.h
# End Source File
# Begin Source File

SOURCE=.\Goal_AdjustRange.h
# End Source File
# Begin Source File

SOURCE=.\Goal_AttackTarget.h
# End Source File
# Begin Source File

SOURCE=.\Goal_Composite.h
# End Source File
# Begin Source File

SOURCE=.\Goal_Evaluator.h
# End Source File
# Begin Source File

SOURCE=.\Goal_Explore.h
# End Source File
# Begin Source File

SOURCE=.\Goal_FollowPath.h
# End Source File
# Begin Source File

SOURCE=.\Goal_GetItem.h
# End Source File
# Begin Source File

SOURCE=.\Goal_HuntTarget.h
# End Source File
# Begin Source File

SOURCE=.\Goal_MoveToPosition.h
# End Source File
# Begin Source File

SOURCE=.\Goal_SeekToPosition.h
# End Source File
# Begin Source File

SOURCE=.\Goal_Think.h
# End Source File
# Begin Source File

SOURCE=.\Goal_TraverseEdge.h
# End Source File
# Begin Source File

SOURCE=.\Goal_Wander.h
# End Source File
# Begin Source File

SOURCE=.\GraphAlgorithms.h
# End Source File
# Begin Source File

SOURCE=.\GraphEdgeTypes.h
# End Source File
# Begin Source File

SOURCE=.\GraphNodeTypes.h
# End Source File
# Begin Source File

SOURCE=.\HandyGraphFunctions.h
# End Source File
# Begin Source File

SOURCE=.\InvertedAABBox2D.h
# End Source File
# Begin Source File

SOURCE=.\LuaHelperFunctions.h
# End Source File
# Begin Source File

SOURCE=.\MessageDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\MondoDiGioco.h
# End Source File
# Begin Source File

SOURCE=.\MovingEntity.h
# End Source File
# Begin Source File

SOURCE=.\NodeTypeEnumerations.h
# End Source File
# Begin Source File

SOURCE=.\PathEdge.h
# End Source File
# Begin Source File

SOURCE=.\PathManager.h
# End Source File
# Begin Source File

SOURCE=.\PathTarget.h
# End Source File
# Begin Source File

SOURCE=.\PrecisionTimer.h
# End Source File
# Begin Source File

SOURCE=.\PriorityQueue.h
# End Source File
# Begin Source File

SOURCE=.\Proiettile_Leggero.h
# End Source File
# Begin Source File

SOURCE=.\Proiettili.h
# End Source File
# Begin Source File

SOURCE=.\Projectile_Pellet.h
# End Source File
# Begin Source File

SOURCE=.\Projectile_Rocket.h
# End Source File
# Begin Source File

SOURCE=.\Projectile_Slug.h
# End Source File
# Begin Source File

SOURCE=.\Raven_Feature.h
# End Source File
# Begin Source File

SOURCE=.\Raven_Goal_Types.h
# End Source File
# Begin Source File

SOURCE=.\Raven_Map.h
# End Source File
# Begin Source File

SOURCE=.\Raven_Messages.h
# End Source File
# Begin Source File

SOURCE=.\Raven_PathPlanner.h
# End Source File
# Begin Source File

SOURCE=.\Raven_Scriptor.h
# End Source File
# Begin Source File

SOURCE=.\Raven_SensoryMemory.h
# End Source File
# Begin Source File

SOURCE=.\Raven_SteeringBehaviors.h
# End Source File
# Begin Source File

SOURCE=.\Raven_TargetingSystem.h
# End Source File
# Begin Source File

SOURCE=.\Raven_UserOptions.h
# End Source File
# Begin Source File

SOURCE=.\Regulator.h
# End Source File
# Begin Source File

SOURCE=.\Rimessa.h
# End Source File
# Begin Source File

SOURCE=.\Scriptor.h
# End Source File
# Begin Source File

SOURCE=.\SearchTerminationPolicies.h
# End Source File
# Begin Source File

SOURCE=.\Semaforo.h
# End Source File
# Begin Source File

SOURCE=.\SistemaDiArma.h
# End Source File
# Begin Source File

SOURCE=.\SistemaMovimento.h
# End Source File
# Begin Source File

SOURCE=.\SparseGraph.h
# End Source File
# Begin Source File

SOURCE=.\SpotIncidente.h
# End Source File
# Begin Source File

SOURCE=.\Stream_Utility_Functions.h
# End Source File
# Begin Source File

SOURCE=.\Telegram.h
# End Source File
# Begin Source File

SOURCE=.\TimeSlicedGraphAlgorithms.h
# End Source File
# Begin Source File

SOURCE=.\Transformations.h
# End Source File
# Begin Source File

SOURCE=.\Trigger.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_HealthGiver.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_LimitedLifeTime.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_OnButtonSendMsg.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_Respawning.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_SoundNotify.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_WeaponGiver.h
# End Source File
# Begin Source File

SOURCE=.\TriggerRegion.h
# End Source File
# Begin Source File

SOURCE=.\TriggerSystem.h
# End Source File
# Begin Source File

SOURCE=.\TypeToString.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# Begin Source File

SOURCE=.\Veicolo.h
# End Source File
# Begin Source File

SOURCE=.\Wall2D.h
# End Source File
# Begin Source File

SOURCE=.\WallIntersectionTests.h
# End Source File
# Begin Source File

SOURCE=.\Weapon_Blaster.h
# End Source File
# Begin Source File

SOURCE=.\Weapon_RailGun.h
# End Source File
# Begin Source File

SOURCE=.\Weapon_RocketLauncher.h
# End Source File
# Begin Source File

SOURCE=.\Weapon_ShotGun.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\crosshair.cur
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\2D\C2DMatrix.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\Cgdi.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\misc\Cgdi.h
# End Source File
# Begin Source File

SOURCE=.\EntityFunctionTemplates.h
# End Source File
# Begin Source File

SOURCE=..\Common\2D\geometry.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\iniFileLoaderBase.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\misc\iniFileLoaderBase.h
# End Source File
# Begin Source File

SOURCE=..\Common\2D\InvertedAABBox2D.h
# End Source File
# Begin Source File

SOURCE=.\Obstacle.cpp
# End Source File
# Begin Source File

SOURCE=.\Obstacle.h
# End Source File
# Begin Source File

SOURCE=.\ParamLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamLoader.h
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\Smoother.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\Stream_Utility_Functions.h
# End Source File
# Begin Source File

SOURCE=..\Common\2D\Transformations.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\utils.h
# End Source File
# Begin Source File

SOURCE=..\Common\2D\Vector2d.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\2D\Vector2D.h
# End Source File
# Begin Source File

SOURCE=..\Common\2D\Wall2D.h
# End Source File
# Begin Source File

SOURCE=..\Common\misc\WindowUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\misc\WindowUtils.h
# End Source File
# End Group
# End Target
# End Project
