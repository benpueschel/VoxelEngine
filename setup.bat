@echo off
echo "Setting up project"
python scripts/Setup.py

echo "Creating Premake projects"
call generate-projects.bat

PAUSE