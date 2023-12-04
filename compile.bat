set APP_INSTALL_PATH=F:\Project_C++\qwer_code\install
set SOURCE_PATH=%~dp0
set VCPKG_INSTALLED_PATH=F:\vcpkg
set QT_INSTALLED_PATH=D:\Soft\Qt5.14.1\data\5.14.2\msvc2017_64



rd /S /Q %APP_INSTALL_PATH%
rd /S /Q %SOURCE_PATH%build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_INSTALLED_PATH%\scripts\buildsystems\vcpkg.cmake -Wno-dev -DQt5_DIR=%QT_INSTALLED_PATH% -DCMAKE_INSTALL_PREFIX=%APP_INSTALL_PATH%
cmake --build build --config Release
cmake --install build
rd /S /Q %SOURCE_PATH%build