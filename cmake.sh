#!/bin/bash

# Setzen Sie den absoluten Pfad zum Build-Verzeichnis
BUILD_DIR=/SHARE/groups/np/Geant4/TU1CSeibt250924/Simulation/build

# Löschen Sie zuerst das Build-Verzeichnis, um die CMake-Cache zu löschen
rm -rf $BUILD_DIR

# Erstellen Sie das Build-Verzeichnis, wenn es nicht vorhanden ist
mkdir -p $BUILD_DIR

# Wechseln Sie in das Build-Verzeichnis
cd $BUILD_DIR

# Führen Sie den cmake-Befehl aus, um das Projekt zu konfigurieren
cmake -DCMAKE_BUILD_TYPE=RELEASE /SHARE/groups/np/Geant4/TU1CSeibt250924/Simulation/source

