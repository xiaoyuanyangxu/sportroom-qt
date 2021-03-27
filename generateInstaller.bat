@echo off

echo This script generate the Installer

echo

echo Let start copying all the files
copy .\release\sportroom-qt.exe .\packages\sportroom\data\. /Y
copy sportroom-qt_ca_ES.qm .\packages\sportroom\data\. /Y
copy sportroom-qt_es_ES.qm .\packages\sportroom\data\. /Y

copy C:\Qt\5.15.2\mingw81_32\bin\icudt52.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\icuin52.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\icuuc52.dll .\packages\sportroom\data\. /Y
copy "C:\Qt\5.15.2\mingw81_32\bin\libgcc_s_dw2-1.dll" .\packages\sportroom\data\. /Y
copy "C:\Qt\5.15.2\mingw81_32\bin\libstdc++-6.dll" .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\libwinpthread-1.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\qt5printsupport.dll .\packages\sportroom\d"ata\. /Y

mkdir .\packages\sportroom\data\platforms
mkdir .\packages\sportroom\data\imageformats

copy C:\Qt\5.15.2\mingw81_32\plugins\platforms\qwindows.dll .\packages\sportroom\data\platforms\. /Y

copy "C:\Qt\5.15.2\mingw81_32\plugins\imageformats\*.dll" .\packages\sportroom\data\imageformats\. /Y

copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Core.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Gui.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Network.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Sql.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Widgets.dll .\packages\sportroom\data\. /Y
copy C:\Qt\Tools\mingw482_32\opt\bin\libeay32.dll .\packages\sportroom\data\. /Y
copy C:\Qt\Tools\\mingw810_32\opt\bin\ssleay32.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5Multimedia.dll .\packages\sportroom\data\. /Y
copy C:\Qt\5.15.2\mingw81_32\bin\Qt5WebSockets.dll .\packages\sportroom\data\. /Y

REM C:\Qt\Tools\QtInstallerFramework\2.0\bin\binarycreator.exe -c config\config.xml -p packages SportRoomQtInstaller_v0.5.0.exe

"c:\Program Files (x86)\NSIS\Bin\makensis.exe" installer.nsi
