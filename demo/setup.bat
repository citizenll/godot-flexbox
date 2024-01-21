@echo off
IF NOT EXIST ".\addons\godot-flexbox" (
  xcopy /E /I "..\addons\godot-flexbox" ".\addons\godot-flexbox\"
)