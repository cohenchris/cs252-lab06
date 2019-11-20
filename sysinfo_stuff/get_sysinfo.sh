#!/bin/bash

##### RELEASE #####
OS_INFO_PATH="/etc/os-release"
RELEASE_NAME=$(cat $OS_INFO_PATH | grep NAME | head -n1 | cut -d '"' -f2)
RELEASE_VERSION=$(cat $OS_INFO_PATH | grep VERSION | head -n1 | cut -d '"' -f2)
KERNEL_INFO_PATH="/proc/version_signature"
KERNEL_VERSION=$(cat $KERNEL_INFO_PATH | cut -d ' ' -f2-)


##### HARDWARE #####
# MEMORY
MEMINFO_PATH="/proc/meminfo"
TOTAL_RAM=$(cat $MEMINFO_PATH | grep MemTotal | awk '{print $2}')
TOTAL_RAM_GB=$(awk "BEGIN {printf \"%.2f\",${TOTAL_RAM}/1048576}")
# PROCESSOR
CPUINFO_PATH="/proc/cpuinfo"
CPU_INFO=$(cat $CPUINFO_PATH | grep "model name" | cut -d ':' -f2 | head -n1)
CPU_CORES=$(cat $CPUINFO_PATH | grep "cpu cores" | awk '{print $4}' | head -n1)

##### SYSTEM STATUS #####
# DISK SPACE
DISK_SPACE=$(df -h --total | grep total | awk '{print $4}')


########## PRINT INFORMATION ##########
# Release
echo $(echo $RELEASE_NAME | tr '[:lower:]' '[:upper:]')
echo Release $RELEASE_VERSION
echo Kernel $KERNEL_VERSION
# DESKTOP_SESSION should already be set
echo Desktop Environment $DESKTOP_SESSION

echo
echo

# Hardware
echo HARDWARE
echo Memory:    $TOTAL_RAM_GB GB
echo Processor: $CPU_INFO x $CPU_CORES 

echo
echo

# System Status
echo SYSTEM STATUS
echo Available Disk Space: $DISK_SPACE
