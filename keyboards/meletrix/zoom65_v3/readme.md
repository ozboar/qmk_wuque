# Zoom65 v3 keyboard

The most customizable 65% keyboard.

* Keyboard Maintainer: @ozwaldorf
* Hardware Supported:
  * :warning: If you have a case with taillights, use the revision marked `-tl`
  * 1.2mm or 1.6mm ANSI-only: `ansi` revision
  * Zoom65 v3 1.6mm ISO/ANSI pcb: `iso` revision

* Hardware Availability: [meletrix](https://meletrix.com/)

Make example for this keyboard (after setting up your build environment):

    ```
    make meletrix/zoom65_v3/ansi:via
    make meletrix/zoom65_v3/iso:via
    make meletrix/zoom65_v3/ansi_tl:via
    make meletrix/zoom65_v3/iso_tl:via
    ```

Flashing example for this keyboard:

    ```
    make meletrix/zoom65_v3/ansi:via:flash
    make meletrix/zoom65_v3/iso:via:flash
    make meletrix/zoom65_v3/ansi_tl:via:flash
    make meletrix/zoom65_v3/iso_tl:via:flash
    ```

**Reset Key**: Hold down the key located at *K00*, which programmed as *Esc* while plugging in the keyboard.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
