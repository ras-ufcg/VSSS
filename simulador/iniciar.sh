#!/bin/bash
#
# Este script inicia a simulação
#
gnome-terminal -- ./VSS-Simulator/build/vss-simulator &
gnome-terminal -- ./VSS-Viewer/build/vss-viewer &
gnome-terminal -- ./VSS-SampleCpp/configure.sh &
gnome-terminal -- ./VSS-SampleCpp/build/vss-sample &
gnome-terminal -- ./VSS-SampleCpp/build/vss-sample -t blue &
