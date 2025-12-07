@echo off

set CUSTOM_VR_ENABLE=1
set RS_SOURCE_INDEX=0
set SLAM_CONFIG=.\d435i.toml
set VIT_SYSTEM_LIBRARY_PATH=.\bin\mondo.....
set XRT_DEBUG_GUI=1

echo Environment variables set:
echo   CUSTOM_VR_ENABLE=%CUSTOM_VR_ENABLE%
echo   RS_SOURCE_INDEX=%RS_SOURCE_INDEX%
echo   SLAM_CONFIG=%SLAM_CONFIG%
echo   VIT_SYSTEM_LIBRARY_PATH=%VIT_SYSTEM_LIBRARY_PATH%
echo   XRT_DEBUG_GUI=%XRT_DEBUG_GUI%
echo.


REM --- Start Monado service or executable ---
.\src\xrt\targets\service\monado-service.exe

pause