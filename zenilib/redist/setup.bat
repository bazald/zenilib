"%~dp0visualc\vcredist_x86_9.0.30729.17.exe" /quiet /passive /C /T:%TEMP%
"%~dp0visualc\vcredist_x64_9.0.30729.17.exe" /quiet /passive /C /T:%TEMP%
"%~dp0visualc\vcredist_x86_10.0.30319.01.exe" /q
"%~dp0visualc\vcredist_x64_10.0.30319.01.exe" /q
"%~dp0directx\DXSETUP.exe" /silent
"%~dp0openal\oalinst_2.0.7.0.exe" /silent
