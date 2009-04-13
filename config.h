/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.in by autoheader.  */

/* Should we enable Amiga Amithlon support. */
/* #undef AMIGA_AMITHLON */

/* Should we enable AROS support. */
/* #undef AMIGA_AROS */

/* Should we enable M68K AmigaOS support. */
/* #undef AMIGA_M68K */

/* Should we enable Amiga Morphos support. */
/* #undef AMIGA_MORPHOS */

/* Should we enable AmigaOS4 support. */
/* #undef AMIGA_OS4 */

/* Enable alternate OS4 includes. */
/* #undef AMIGA_OS4_ALT */

/* Should we enable PPC Amiga PowerUP support. */
/* #undef AMIGA_POWERUP */

/* Should we enable Amigaos support. */
/* #undef AMIGA_SUPPORT */

/* Should we enable PPC Amiga WarpOS support. */
/* #undef AMIGA_WARPOS */

/* Enable support for BSD style joysticks. */
/* #undef BSD_JOYSTICK */

/* NLS datadirname. */
#define DATADIRNAME ""

/* Use debugging of the zfile */
/* #undef DEBUG_ZFILE */

/* Can we use the dos WATTCP library? */
/* #undef DOS_TCP */

/* Can we use the dos PCAP library? */
/* #undef DOS_TFE */

/* Is DWORD defined as long or int in the Windows header files? */
/* #undef DWORD_IS_LONG */

/* Define if NLS support is enabled. */
/* #define ENABLE_NLS */

/* Enable support for the TextField widget. */
#define ENABLE_TEXTFIELD 

/* Use the memmap feature. */
/* #undef FEATURE_CPUMEMHISTORY */

/* Enable GP2X compilation */
/* #undef GP2X */

/* Enable emulation for digital joysticks. */
/* #undef HAS_DIGITAL_JOYSTICK */

/* Enable Joystick emulation. */
#define HAS_JOYSTICK 

/* Support 64bit integer for Win32 performance counter */
/* #undef HAS_LONGLONG_INTEGER */

/* Is only one canvas supported? */
/* #undef HAS_SINGLE_CANVAS */

/* Enable internationalization support */
/* #undef HAS_TRANSLATION */

/* Do we have UnlockResource()? */
/* #undef HAS_UNLOCKRESOURCE */

/* Enable emulation for USB joysticks. */
/* #undef HAS_USB_JOYSTICK */

/* Define to 1 if you have the `accept' function. */
#define HAVE_ACCEPT 1

/* Define to 1 if you have the <allegro.h> header file. */
/* #undef HAVE_ALLEGRO_H */

/* Define to 1 if you have the <alsa/asoundlib.h> header file. */
/* #undef HAVE_ALSA_ASOUNDLIB_H */

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <artsc.h> header file. */
/* #undef HAVE_ARTSC_H */

/* Define to 1 if you have the `atexit' function. */
/* #define HAVE_ATEXIT 1 */

/* Define to 1 if you have the `bind' function. */
/* #define HAVE_BIND 1 */

/* Define to 1 if you have the <byteorder.h> header file. */
/* #undef HAVE_BYTEORDER_H */

/* Support for Catweasel MKIII. */
/* #undef HAVE_CATWEASELMKIII */

/* Define to 1 if you have the <commctrl.h> header file. */
/* #undef HAVE_COMMCTRL_H */

/* Define to 1 if you have the `connect' function. */
/* #define HAVE_CONNECT 1 */

/* Define to 1 if you have the <cwsid.h> header file. */
/* #undef HAVE_CWSID_H */

/* Define to 1 if you have the `dcgettext' function. */
/* #undef HAVE_DCGETTEXT */

/* Define to 1 if you have the declaration of `sys_siglist', and to 0 if you
   don't. */
//#define HAVE_DECL_SYS_SIGLIST 1
#define HAVE_DECL_SYS_SIGLIST 0

/* Define to 1 if you have the <devices/ahi.h> header file. */
/* #undef HAVE_DEVICES_AHI_H */

/* Define to 1 if you have the <direct.h> header file. */
/* #undef HAVE_DIRECT_H */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dir.h> header file. */
/* #undef HAVE_DIR_H */

/* Define to 1 if you have the <dmedia/audio.h> header file. */
/* #undef HAVE_DMEDIA_AUDIO_H */

/* Can we use the dos NET library? */
/* #undef HAVE_DOS_LIBNET */

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <esd.h> header file. */
/* #undef HAVE_ESD_H */

/* Define to 1 if you have the <fcntl.h> header file. */
/* #define HAVE_FCNTL_H 1 */

/* Enable FFMPEG library support */
/* #undef HAVE_FFMPEG */

/* Define to 1 if you have the <ffmpeg/avformat.h> header file. */
/* #undef HAVE_FFMPEG_AVFORMAT_H */

/* Define to 1 if you have the `fork' function. */
/* #define HAVE_FORK 1 */

/* Enable Fullscreen support. */
/* #undef HAVE_FULLSCREEN */

/* Define to 1 if you have the `getcwd' function. */
/* #define HAVE_GETCWD 1 */

/* Define to 1 if you have the `getdtablesize' function. */
/* #define HAVE_GETDTABLESIZE 1 */

/* Define to 1 if you have the `gethostbyname' function. */
/* #define HAVE_GETHOSTBYNAME 1 */

/* Define if gethostbyname2 can be used */
/* #define HAVE_GETHOSTBYNAME2 */

/* Define if getipnodebyname can be used */
/* #undef HAVE_GETIPNODEBYNAME */

/* Define to 1 if you have the `getrlimit' function. */
/* #define HAVE_GETRLIMIT 1 */

/* Define if gettext if available. */
/* #define HAVE_GETTEXT  */

/* Define to 1 if you have the `gettimeofday' function. */
/* #define HAVE_GETTIMEOFDAY 1 */

/* Can we use the GIF or UNGIF library? */
/* #undef HAVE_GIF */

/* Is the GUID lib of DX SDK present? */
/* #undef HAVE_GUIDLIB */

/* Support for HardSID. */
/* #undef HAVE_HARDSID */

/* Define to 1 if you have the `htonl' function. */
/* #define HAVE_HTONL 1 */

/* Define to 1 if you have the `htons' function. */
/* #define HAVE_HTONS 1 */

/* Enable arbitrary window scaling */
/* #undef HAVE_HWSCALE */

/* Define to 1 if you have the <hw/inout.h> header file. */
/* #undef HAVE_HW_INOUT_H */

/* Define to 1 if you have the `i386_set_ioperm' function. */
/* #undef HAVE_I386_SET_IOPERM */

/* Define to 1 if you have the <ieee1284.h> header file. */
/* #undef HAVE_IEEE1284_H */

/* Define to 1 if you have the `in8' function. */
/* #undef HAVE_IN8 */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `ioperm' function. */
/* #undef HAVE_IOPERM */

/* Define to 1 if you have the <io.h> header file. */
/* #undef HAVE_IO_H */

/* Define if ipv6 can be used */
/* #define HAVE_IPV6  */

/* Can we use the JPEG library? */
/* #undef HAVE_JPEG */

/* Define to 1 if you have the <lame/lame.h> header file. */
/* #undef HAVE_LAME_LAME_H */

/* Define to 1 if you have the `amd64' library (-lamd64). */
/* #undef HAVE_LIBAMD64 */

/* Define to 1 if you have the `bsd' library (-lbsd). */
/* #undef HAVE_LIBBSD */

/* Define to 1 if you have the `ieee1284' library (-lieee1284). */
/* #undef HAVE_LIBIEEE1284 */

/* use libintl for NLS. */
/* #define HAVE_LIBINTL_H */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `ossaudio' library (-lossaudio). */
/* #undef HAVE_LIBOSSAUDIO */

/* Define to 1 if you have the `resid' library (-lresid). */
/* #undef HAVE_LIBRESID */

/* Define to 1 if you have the <libusbhid.h> header file. */
/* #undef HAVE_LIBUSBHID_H */

/* Define to 1 if you have the <libusb.h> header file. */
/* #undef HAVE_LIBUSB_H */

/* Is libXpm available? */
/* #undef HAVE_LIBXPM */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <linux/soundcard.h> header file. */
/* #define HAVE_LINUX_SOUNDCARD_H 1 */

/* Define to 1 if you have the `listen' function. */
/* #define HAVE_LISTEN 1 */

/* Define to 1 if you have the <machine/cpufunc.h> header file. */
/* #undef HAVE_MACHINE_CPUFUNC_H */

/* Define to 1 if you have the <machine/joystick.h> header file. */
/* #undef HAVE_MACHINE_JOYSTICK_H */

/* Define to 1 if you have the <machine/pio.h> header file. */
/* #undef HAVE_MACHINE_PIO_H */

/* Define to 1 if you have the <machine/soundcard.h> header file. */
/* #undef HAVE_MACHINE_SOUNDCARD_H */

/* Define to 1 if you have the <machine/sysarch.h> header file. */
/* #undef HAVE_MACHINE_SYSARCH_H */

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
/* #define HAVE_MEMORY_H 1 */

/* Define to 1 if you have the <midasdll.h> header file. */
/* #undef HAVE_MIDASDLL_H */

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* Define to 1 if you have the `mmap_device_io' function. */
/* #undef HAVE_MMAP_DEVICE_IO */

/* Enable 1351 mouse support */
/* #define HAVE_MOUSE  */

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <netdb.h> header file. */
/* #define HAVE_NETDB_H 1 */

/* Define to 1 if you have the <netinet/in.h> header file. */
/* #define HAVE_NETINET_IN_H 1 */

/* Enable netplay support */
#undef HAVE_NETWORK

/* Support for OpenCBM (former CBM4Linux). */
/* #undef HAVE_OPENCBM */

/* OpenCBM header file is available. */
/* #undef HAVE_OPENCBM_H */

/* Enable openGL synchronization */
/* #undef HAVE_OPENGL_SYNC */

/* Define to 1 if you have the `out8' function. */
/* #undef HAVE_OUT8 */

/* Support for ParSID. */
/* #undef HAVE_PARSID */

/* Can we use the PNG library? */
/* #define HAVE_PNG */

/* Define to 1 if you have the <process.h> header file. */
/* #undef HAVE_PROCESS_H */

/* Define to 1 if you have the <proto/cybergraphics.h> header file. */
/* #undef HAVE_PROTO_CYBERGRAPHICS_H */

/* Define to 1 if you have the <proto/Picasso96API.h> header file. */
/* #undef HAVE_PROTO_PICASSO96API_H */

/* Define to 1 if you have the <proto/Picasso96.h> header file. */
/* #undef HAVE_PROTO_PICASSO96_H */

/* Support for block device disk image access. */
/* #define HAVE_RAWDRIVE */

/* Are we using the readline library replacement? */
/* #define HAVE_READLINE  */

/* Define to 1 if you have the `recv' function. */
/* #define HAVE_RECV 1 */

/* Define to 1 if you have the <regex.h> header file. */
/* #define HAVE_REGEX_H 1 */

/* This version provides ReSID support. */
#define HAVE_RESID 

/* Define to 1 if you have the `rewinddir' function. */
/* #define HAVE_REWINDDIR 1 */

/* Does the `readline' library support `rl_readline_name'? */
/* #define HAVE_RLNAME */

/* Enable RS232 emulation. */
/* #define HAVE_RS232 */

/* Define to 1 if you have the <SDL/SDL_audio.h> header file. */
/* #undef HAVE_SDL_SDL_AUDIO_H */

/* Define to 1 if you have the `seekdir' function. */
/* #define HAVE_SEEKDIR 1 */

/* Define to 1 if you have the `send' function. */
/* #define HAVE_SEND 1 */

/* Define to 1 if you have the <shlobj.h> header file. */
/* #undef HAVE_SHLOBJ_H */

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define to 1 if you have the `socket' function. */
/* #define HAVE_SOCKET 1 */

/* Define to 1 if you have the <socket.h> header file. */
/* #undef HAVE_SOCKET_H */

/* Define to 1 if you have the <soundcard.h> header file. */
/* #undef HAVE_SOUNDCARD_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
/* #define HAVE_STRINGS_H 1 */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `swab' function. */
#define HAVE_SWAB 1

/* Define to 1 if you have the <sys/audioio.h> header file. */
/* #undef HAVE_SYS_AUDIOIO_H */

/* Define to 1 if you have the <sys/audio.h> header file. */
/* #undef HAVE_SYS_AUDIO_H */

/* Define to 1 if you have the <sys/dirent.h> header file. */
/* #undef HAVE_SYS_DIRENT_H */

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/io.h> header file. */
/* #undef HAVE_SYS_IO_H */

/* Define to 1 if you have the <sys/joystick.h> header file. */
/* #undef HAVE_SYS_JOYSTICK_H */

/* Define to 1 if you have the <sys/mman.h> header file. */
/* #undef HAVE_SYS_MMAN_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/soundcard.h> header file. */
/* #define HAVE_SYS_SOUNDCARD_H 1 */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
/* #define HAVE_SYS_TYPES_H 1 */

/* Define to 1 if you have the `telldir' function. */
#define HAVE_TELLDIR 1

/* Support for The Final Ethernet */
/* #undef HAVE_TFE */

/* Define to 1 if you have the <UMS/UMSAudioDevice.h> header file. */
/* #undef HAVE_UMS_UMSAUDIODEVICE_H */

/* Define to 1 if you have the <UMS/UMSBAUDDevice.h> header file. */
/* #undef HAVE_UMS_UMSBAUDDEVICE_H */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <usbhid.h> header file. */
/* #undef HAVE_USBHID_H */

/* Define to 1 if you have the <usb.h> header file. */
/* #undef HAVE_USB_H */

/* Define to 1 if you have the `usleep' function. */
/* #undef HAVE_USLEEP */

/* Define to 1 if the system has the type `u_short'. */
#define HAVE_U_SHORT 1

/* Define to 1 if you have the `vfork' function. */
/* #define HAVE_VFORK 1 */

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the <winioctl.h> header file. */
/* #undef HAVE_WINIOCTL_H */

/* Define to 1 if `fork' works. */
/* #define HAVE_WORKING_FORK 1 */

/* Define to 1 if `vfork' works. */
/* #define HAVE_WORKING_VFORK 1 */

/* Define to 1 if you have the <X11/Sunkeysym.h> header file. */
/* #undef HAVE_X11_SUNKEYSYM_H */

/* Define to 1 if you have the <X11/xpm.h> header file. */
/* #undef HAVE_X11_XPM_H */

/* Define to 1 if you have the <xpm.h> header file. */
/* #undef HAVE_XPM_H */

/* Enable XRandR extension. */
/* #undef HAVE_XRANDR */

/* Enable XVideo support. */
/* #undef HAVE_XVIDEO */

/* Can we use the ZLIB compression library? */
#define HAVE_ZLIB 

/* Enable support for Linux style joysticks. */
/* #define LINUX_JOYSTICK  */

/* Enable Mac OS X application bundles. */
/* #undef MACOSX_BUNDLE */

/* Use Cocoa on Macs. */
/* #undef MACOSX_COCOA */

/* Enable Mac OS X specific code. */
/* #undef MACOSX_SUPPORT */

/* Enable Mac Joystick support. */
/* #undef MAC_JOYSTICK */

/* are we compiling under Minix-Vmd */
/* #undef MINIXVMD */

/* Define if this version of minix has the recv and send prototypes */
/* #undef MINIX_HAS_RECV_SEND */

/* do we need minix support */
/* #undef MINIX_SUPPORT */

/* Whether struct sockaddr::__ss_family exists */
/* #undef NEED_PREFIXED_SS_FAMILY */

/* NLS local directory. */
/* #define NLS_LOCALEDIR PREFIX"/"DATADIRNAME"/locale" */

/* Enable SCO Openserver 5.x support */
/* #undef OPENSERVER5_COMPILE */

/* Enable SCO Openserver 6.x support */
/* #undef OPENSERVER6_COMPILE */

/* Name of package */
#define PACKAGE "vice"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Where do we want to install the executable? */
#define PREFIX "/usr/local"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* The size of a `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT 4

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* The size of a `unsigned short', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable SCO Unixware 7.x support */
/* #undef UNIXWARE_COMPILE */

/* Enable aix sound support. */
/* #undef USE_AIX_AUDIO */

/* Enable alsa support. */
/* #undef USE_ALSA */

/* Enable aRts support. */
/* #undef USE_ARTS */

/* Enable new color management code. */
#define USE_COLOR_MANAGEMENT 

/* Enable CoreAudio support. */
/* #undef USE_COREAUDIO */

/* Enable sgi sound support. */
/* #undef USE_DMEDIA */

/* Enable esd sound support. */
/* #undef USE_ESD */

/* Use GNOME UI. */
/* #undef USE_GNOMEUI */

/* Enable lamemp3 support. */
/* #undef USE_LAMEMP3 */

/* Use MIDAS Sound System instead of the Allegro library. */
/* #undef USE_MIDAS_SOUND */

/* Enable MITSHM extensions. */
/* #undef USE_MITSHM */

/* Enable oss support. */
/* #define USE_OSS */

/* Enable sdl sound support. */
/* #undef USE_SDL_AUDIO */

/* Enable XF86 extensions. */
/* #undef USE_XF86_EXTENSIONS */

/* Enable XF86 VidMode extensions. */
/* #undef USE_XF86_VIDMODE_EXT */

/* Version number of package */
#define VERSION "2.0"

/* Win32 Version string. */
#define VERSION_RC "$VERSION_RC"

/* Support for The Final Ethernet */
/* #undef VICE_USE_LIBNET_1_1 */

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to 1 if the X Window System is missing or not being used. */
#define X_DISPLAY_MISSING 1

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* ss_family is not defined here, use __ss_family instead */
/* #undef ss_family */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */
