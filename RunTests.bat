@echo off
REM CATastrophe Test Runner
REM This script runs automated tests for the Catastrophe project

echo ========================================
echo CATastrophe Automated Test Runner
echo ========================================
echo.

REM Set paths
set UE_EDITOR="C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
set PROJECT_PATH="%~dp0Catastrophe.uproject"
set REPORT_PATH="%~dp0TestResults"

REM Check if Unreal Editor exists
if not exist %UE_EDITOR% (
    echo ERROR: Unreal Editor not found at %UE_EDITOR%
    echo Please update the UE_EDITOR path in this script.
    pause
    exit /b 1
)

REM Create report directory if it doesn't exist
if not exist %REPORT_PATH% mkdir %REPORT_PATH%

echo Project: %PROJECT_PATH%
echo Report Output: %REPORT_PATH%
echo.

REM Prompt user for test type
echo Select test type to run:
echo   1. All Tests
echo   2. Unit Tests Only
echo   3. Integration Tests Only
echo   4. Performance Tests Only
echo   5. Quick Tests (Unit + Integration, Skip Performance)
echo.

set /p TEST_CHOICE="Enter choice (1-5): "

REM Set test filter based on choice
if "%TEST_CHOICE%"=="1" (
    set TEST_FILTER=Catastrophe
    set TEST_NAME=All Tests
)
if "%TEST_CHOICE%"=="2" (
    set TEST_FILTER=Catastrophe.Unit
    set TEST_NAME=Unit Tests
)
if "%TEST_CHOICE%"=="3" (
    set TEST_FILTER=Catastrophe.Integration
    set TEST_NAME=Integration Tests
)
if "%TEST_CHOICE%"=="4" (
    set TEST_FILTER=Catastrophe.Performance
    set TEST_NAME=Performance Tests
)
if "%TEST_CHOICE%"=="5" (
    set TEST_FILTER=Catastrophe.Unit+Catastrophe.Integration
    set TEST_NAME=Quick Tests
)

echo.
echo ========================================
echo Running: %TEST_NAME%
echo ========================================
echo.
echo Please wait... This may take a few minutes.
echo.

REM Run the tests
%UE_EDITOR% %PROJECT_PATH% -ExecCmds="Automation RunTests %TEST_FILTER%; Quit" -unattended -nopause -NullRHI -ReportOutputPath=%REPORT_PATH%

REM Check exit code
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Tests completed successfully!
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Tests completed with errors!
    echo ========================================
)

echo.
echo Test reports saved to: %REPORT_PATH%
echo.

REM Open report in browser
set REPORT_HTML=%REPORT_PATH%\index.html
if exist %REPORT_HTML% (
    echo Opening test report...
    start "" %REPORT_HTML%
) else (
    echo Note: Test report HTML not generated
)

echo.
pause
