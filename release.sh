#!/usr/bin/env bash
set -e

VERSION=$(nix run nixpkgs\#jq -- -r .usb.device_version keyboards/meletrix/zoom65_v3/info.json)
DIR="firmware-v$VERSION"

echo ">> Building zoom65v3 firmware"

# clear and init dir
[[ -f "$DIR" ]] && rm $DIR -r; mkdir -p $DIR

make meletrix/zoom65_v3/ansi:via
mv meletrix_zoom65_v3_ansi_via.bin $DIR/meletrix_zoom65_v3_ansi_via_$VERSION.bin

make meletrix/zoom65_v3/ansi_tl:via
mv meletrix_zoom65_v3_ansi_tl_via.bin $DIR/meletrix_zoom65_v3_ansi_taillight_via_$VERSION.bin

make meletrix/zoom65_v3/iso:via
mv meletrix_zoom65_v3_iso_via.bin $DIR/meletrix_zoom65_v3_iso_via_$VERSION.bin

make meletrix/zoom65_v3/ansi_tl:via
mv meletrix_zoom65_v3_iso_tl_via.bin $DIR/meletrix_zoom65_v3_iso_taillight_via_$VERSION.bin

# copy via definitions
cp keyboards/meletrix/zoom65_v3/iso/via.json $DIR/meletrix_zoom65_v3_iso_via_$VERSION.json
cp keyboards/meletrix/zoom65_v3/ansi/via.json $DIR/meletrix_zoom65_v3_ansi_via_$VERSION.json

echo ">> Done"
