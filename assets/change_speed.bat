@echo off
setlocal

echo Enter input file:
set /p input_file=

echo Enter total speed factor (e.g. 3 for 3x faster):
set /p total_speed=

echo Enter output file name:
set /p output_file=

echo If audio only? (y/n):
set /p is_audio=

if "%is_audio%"=="y" (
    if %total_speed% LEQ 2 (
        ffmpeg -i "%input_file%" -filter:a "atempo=%total_speed%" "%output_file%"
    ) else (
        echo For speeds >2, you must chain atempo filters.
        echo Example for 3x: atempo=1.5,atempo=2.0
        echo Enter atempo chain (e.g. atempo=1.5,atempo=2.0):
        set /p atempo_chain=
        ffmpeg -i "%input_file%" -filter:a "%atempo_chain%" "%output_file%"
    )
) else (
    ffmpeg -i "%input_file%" -filter_complex "[0:v]setpts=(1/%total_speed%)*PTS[v];[0:a]atempo=%total_speed%[a]" -map "[v]" -map "[a]" "%output_file%"
)

echo Done!
pause
