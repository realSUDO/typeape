#!/bin/bash
set -e
echo "Uninstalling TypeApe..."
sudo rm -f /usr/local/bin/typeape
sudo rm -rf /usr/local/share/typeape
sudo rm -f /usr/share/applications/typeape.desktop
echo "Done."
