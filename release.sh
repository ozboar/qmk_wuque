#!/usr/bin/env bash
set -e

echo ">> Fetching firmware revision"
VERSION=$(nix run nixpkgs\#jq -- -r .usb.device_version keyboards/meletrix/zoom65_v3/info.json)
DIR="firmware-v$VERSION"

echo ">> Building firmware releases"
make meletrix/zoom65_v3/ansi:via meletrix/zoom65_v3/iso:via

echo ">> Bundling release directory"
[[ -f "$DIR" ]] && rm $DIR -r
mkdir -p $DIR

mv meletrix_zoom65_v3_ansi_via.bin $DIR/meletrix_zoom65_v3_ansi_via_$VERSION.bin
cp keyboards/meletrix/zoom65_v3/ansi/via.json $DIR/meletrix_zoom65_v3_ansi_via_$VERSION.json

mv meletrix_zoom65_v3_iso_via.bin $DIR/meletrix_zoom65_v3_iso_via_$VERSION.bin
cp keyboards/meletrix/zoom65_v3/iso/via.json $DIR/meletrix_zoom65_v3_iso_via_$VERSION.json

echo ">> Done"
