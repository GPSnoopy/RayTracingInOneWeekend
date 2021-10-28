mkdir build
cd build || goto :error
mkdir windows || goto :error
cd windows || goto :error
cmake -D ISPC_EXECUTABLE="%ProgramFiles%/ISPC/ispc-v1.16.1-windows/bin/ispc.exe" -G "Visual Studio 16 2019" -A "x64" ../.. || goto :error
msbuild RayTracingInOneWeekend.sln /t:Rebuild /p:Configuration=Release || goto :error
cd ..
cd ..

exit /b


:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
