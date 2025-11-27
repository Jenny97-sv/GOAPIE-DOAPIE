@ECHO OFF
@PUSHD %~dp0
@CALL "Premake\premake5" vs2022 || PAUSE
@REM CALL "Premake/premake5" --version && @PAUSE

@POPD
@ECHO ON
PAUSE