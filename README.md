VICE PSP
=========

Commodore 64 emulator

&copy; 2009-2011 Akop Karapetyan  
&copy; 1998-2009 VICE team

New Features
------------

#### Version 2.2.15 (January 16 2011)

*   Added <- to the list of mappable controls

Installation
------------

Unzip `vice.zip` into `/PSP/GAME/` folder on the memory stick.

Controls
--------

During emulation:

| PSP controls                    | Emulated controls           |
| ------------------------------- | --------------------------- |
| D-pad up/down/left/right        | Joystick up/down/left/right |
| Analog stick up/down/left/right | Joystick up/down/left/right |
| X (cross)                       | Joystick Fire               |
| O (circle)                      | Spacebar                    |
| Start                           | Run/Stop                    |
| [R]                             | Show virtual keyboard       |
| [L] + [R]                       | Return to the emulator menu |

When the virtual keyboard is on:

| PSP controls                    | Function                 |
| ------------------------------- | ------------------------ |
| Directional pad                 | select key               |
| [ ] (square)                    | press key                |
| O (circle)                      | hold down/release key    |
| ^ (triangle)                    | release all held keys    |

Only certain keys (e.g. SHIFT) can be held down.

Keyboard mappings can be modified for each game. Button configuration changes are automatically retained after a mapping is modified. To set a mapping as the default mapping, press [ ] (square).

Tape and Disk Image Loading, Autoloading
----------------------------------------

Anytime you attempt to load a game from the Game tab, VICE PSP will reset the system; this is intended to make most games easily loadable.

To load tapes/disks without resetting the system, go to the System tab, select the Tape or Drive 8 menu option (depending on whether you want to load a tape or disk image), then press X (cross) to load another image. If a tape or disk image is already loaded, it will be ejected, and another one will be loaded in its place. To eject a loaded image, select an option, and press ^ (triangle).

To _autoload_ a program (load a program from a mounted image directly), first load the image, then go to the System tab, highlight the Tape or Drive 8 menu option, and press \[right\]. Select a program from the list, and press X (cross) to autoload it. Note that autoloading _will_ reset the system.

Save State Autoloading
----------------------

VICE PSP can be configured to automatically load a saved game whenever a new game is loaded via the Game tab. To do this, specify one of the save state slots as the Autoload slot (Options tab). If a saved game exists in the specified slot, it will be automatically loaded.

Known Issues
------------

Counter will sometimes report frames-per-second information incorrectly – this has to do with the sound rendering routine, which is shaky at the moment. Exiting to the emulator menu and returning will usually correct this.

When frame skipping is in effect, sound may “crackle”. This is also, most likely, a result of the above bug.

Compiling
---------

To compile, ensure that [zlib](svn://svn.pspdev.org/psp/trunk/zlib) and [libpng](svn://svn.pspdev.org/psp/trunk/libpng) are installed, and run make:

`make -f Makefile_C64.psp`

Version History
---------------

#### 2.2.1 (January 02 2011)

*   Updated VICE to version 2.2
*   Includes SFX Sound Sampler and SFX Sound Expander emulation
*   Includes EasyFlash cartridge emulation

#### 2.1.21 (June 03 2009)

*   Bugfix: Saving state while in NTSC mode resulted in a crash
*   Bugfix: An extra line from the screen border appeared while in NTSC mode

#### 2.1.2 (June 01 2009)

*   Added 4:3 (fit screen height) stretch mode, visible in NTSC mode or when border is disabled
*   Added border toggle – shows/hides the border surrounding the main display area
*   Added PAL/NTSC/Old NTSC mode selection
*   Added vertical blanking (VSync) support, only available in NTSC mode
*   Bugfix: PRG loading now works
*   Bugfix: “fit screen” stretch mode aspect ratio was incorrect; now fixed
*   Minor fixes

#### 2.1.1 (May 23 2009)

*   Initial release

Credits
-------

VICE team (VICE emulator)  
