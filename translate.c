/*
 * translate.c - Global internationalization routines.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "config.h"

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"

#ifdef HAS_TRANSLATION
#include "intl.h"
#endif

#include "lib.h"
#include "log.h"
#include "resources.h"
#include "translate.h"
#include "util.h"

#define countof(array) (sizeof(array) / sizeof((array)[0]))

typedef struct translate_s {
    int resource_id;
    char *text;
} translate_t;

#ifdef HAS_TRANSLATION
char *current_language = NULL;
int current_language_index = 0;

/* The language table is usually duplicated in
   the arch intl.c, make sure they match
   when adding a new language */

static char *language_table[] = {

/* english */
  "en",

/* german */
  "de",

/* french */
  "fr",

/* hungarian */
  "hu",

/* italian */
  "it",

/* dutch */
  "nl",

/* polish */
  "pl",

/* swedish */
  "sv"
};


/* This is the codepage table, which holds the codepage
   used per language to encode the original text */


static int language_cp_table[] = {

/* english */
  28591,	/* ISO 8859-1 */

/* german */
  28591,	/* ISO 8859-1 */

/* french */
  28591,	/* ISO 8859-1 */

/* hungarian */
  28592,	/* ISO 8859-2 */

/* italian */
  28591,	/* ISO 8859-1 */

/* dutch */
  28591,	/* ISO 8859-1 */

/* polish */
  28592,	/* ISO 8859-2 */

/* swedish */
  28591	/* ISO 8859-1 */
};
#endif

/* GLOBAL STRING ID TEXT TABLE */

translate_t string_table[] = {

/* autostart.c */
/* en */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE,    N_("Cannot load snapshot file.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_DE, "Kann Snapshot Datei nicht laden."},
/* fr */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_FR, "Impossible de charger l'instantan�."},
/* hu */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_HU, "Nem siker�lt bet�lteni a pillanatk�p f�jlt."},
/* it */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_IT, "Non � possibile caricare il file di snapshot."},
/* nl */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_NL, "Kan momentopnamebestand niet laden."},
/* pl */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_LOAD_SNAPSHOT_FILE_SV, "Kan inte ladda �gonblicksbildfil."},
#endif

/* debug.c */
/* en */ {IDGS_PLAYBACK_ERROR_DIFFERENT,    N_("Playback error: %s different from line %d of file debug%06d")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_PLAYBACK_ERROR_DIFFERENT_DE, "Wiedergabe Fehler: %s ist unterschiedlich von Zeile %d in Datei debug%06d"},
/* fr */ {IDGS_PLAYBACK_ERROR_DIFFERENT_FR, "Erreur de lecture: %s diff�rent de la ligne %d du fichier debug%06d"},
/* hu */ {IDGS_PLAYBACK_ERROR_DIFFERENT_HU, "Visszaj�tsz�si hiba: %s k�l�nb�zik a %d. sorban a debug%06d f�jl eset�n"},
/* it */ {IDGS_PLAYBACK_ERROR_DIFFERENT_IT, "Errore di riproduzione: %s � differente dalla linea %d del file di debug "
                                            "%06d"},
/* nl */ {IDGS_PLAYBACK_ERROR_DIFFERENT_NL, "Afspeelfout: %s is anders dan regel %d van bestand debug%06d"},
/* pl */ {IDGS_PLAYBACK_ERROR_DIFFERENT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_PLAYBACK_ERROR_DIFFERENT_SV, "�terspelningsfel: %s inte lika som rad %d i filen debug%06d"},
#endif

/* event.c */
/* en */ {IDGS_CANNOT_CREATE_IMAGE,    N_("Cannot create image file!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_CREATE_IMAGE_DE, "Kann Imagedatei nicht erzeugen!"},
/* fr */ {IDGS_CANNOT_CREATE_IMAGE_FR, "Impossible de cr�er le fichier image!"},
/* hu */ {IDGS_CANNOT_CREATE_IMAGE_HU, "Nem siker�lt a k�pm�s f�jlt l�trehozni!"},
/* it */ {IDGS_CANNOT_CREATE_IMAGE_IT, "Non � possibile creare il file immagine!"},
/* nl */ {IDGS_CANNOT_CREATE_IMAGE_NL, "Kan bestand niet maken!"},
/* pl */ {IDGS_CANNOT_CREATE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_CREATE_IMAGE_SV, "Kan inte skapa avbildningsfil!"},
#endif

/* event.c */
/* en */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S,    N_("Cannot write image file %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_DE, "Kann Datei %s nicht schreiben"},
/* fr */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_FR, "Impossible d'�crire le fichier image %s"},
/* hu */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_HU, "Nem siker�lt a %s k�pm�s f�jlba �rni"},
/* it */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_IT, "Non � possibile scrivere l'immagine %s"},
/* nl */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_NL, "Kan niet schrijven naar bestand %s"},
/* pl */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_WRITE_IMAGE_FILE_S_SV, "Kan inte skriva avbildningsfil %s"},
#endif

/* event.c */
/* en */ {IDGS_CANNOT_FIND_MAPPED_NAME_S,    N_("Cannot find mapped name for %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_DE, "Kann zugeordneten Namen f�r `%s' nicht finden."},
/* fr */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_FR, "Impossible de trouver le nom correspondant � %s"},
/* hu */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_HU, "A VICE %s lek�pzett nev�t nem tal�lja"},
/* it */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_IT, "Non � possibile trovare il nome mappato per %s"},
/* nl */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_NL, "Kan vertaalde naam voor %s niet vinden"},
/* pl */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_FIND_MAPPED_NAME_S_SV, "Kan inte kopplat namn f�r %s"},
#endif

/* event.c */
/* en */ {IDGS_CANT_CREATE_START_SNAP_S,    N_("Could not create start snapshot file %s.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANT_CREATE_START_SNAP_S_DE, "Kann Start Snapshot Datei nicht erzeugen: %s"},
/* fr */ {IDGS_CANT_CREATE_START_SNAP_S_FR, "Impossible de cr�er l'instantan� de d�part %s."},
/* hu */ {IDGS_CANT_CREATE_START_SNAP_S_HU, "Nem siker�lt a %s kezdeti pillanatk�p f�jlt l�trehozni."},
/* it */ {IDGS_CANT_CREATE_START_SNAP_S_IT, "Non � possibile creare il file di inizio snapshot %s."},
/* nl */ {IDGS_CANT_CREATE_START_SNAP_S_NL, "Kon het start momentopnamebestand %s niet maken."},
/* pl */ {IDGS_CANT_CREATE_START_SNAP_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANT_CREATE_START_SNAP_S_SV, "Kunde inte skapa start�gonblicksbildfilen %s."},
#endif

/* event.c */
/* en */ {IDGS_ERROR_READING_END_SNAP_S,    N_("Error reading end snapshot file %s.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ERROR_READING_END_SNAP_S_DE, "Kann Ende Snapshot Datei nicht lesen: %s"},
/* fr */ {IDGS_ERROR_READING_END_SNAP_S_FR, "Erreur de lecture de l'instantan� de fin %s."},
/* hu */ {IDGS_ERROR_READING_END_SNAP_S_HU, "Hiba az utols� %s pillanatk�p f�jl �r�sakor."},
/* it */ {IDGS_ERROR_READING_END_SNAP_S_IT, "Errore durante la lettura del file di fine snapshot %s."},
/* nl */ {IDGS_ERROR_READING_END_SNAP_S_NL, "Fout bij het lezen van het eind van het momentopnamebestand %s."},
/* pl */ {IDGS_ERROR_READING_END_SNAP_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ERROR_READING_END_SNAP_S_SV, "Fel vid l�sning av slut�gonblicksbildfilen %s."},
#endif

/* event.c */
/* en */ {IDGS_CANT_CREATE_END_SNAP_S,    N_("Could not create end snapshot file %s.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANT_CREATE_END_SNAP_S_DE, "Kann Ende Snapshot Datei nicht erzeugen: %s"},
/* fr */ {IDGS_CANT_CREATE_END_SNAP_S_FR, "Impossible de cr�er l'instantan� de fin %s."},
/* hu */ {IDGS_CANT_CREATE_END_SNAP_S_HU, "Nem siker�lt a %s befejez� pillanatk�p f�jlt l�trehozni."},
/* it */ {IDGS_CANT_CREATE_END_SNAP_S_IT, "Non � possibile creare il file di fine snapshot %s."},
/* nl */ {IDGS_CANT_CREATE_END_SNAP_S_NL, "Kon het eind momentopnamebestand %s niet maken."},
/* pl */ {IDGS_CANT_CREATE_END_SNAP_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANT_CREATE_END_SNAP_S_SV, "Kunde inte skapa slut�gonblicksbildfilen %s."},
#endif

/* event.c */
/* en */ {IDGS_CANT_OPEN_END_SNAP_S,    N_("Could not open end snapshot file %s.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANT_OPEN_END_SNAP_S_DE, "Kann Ende Snapshot Datei nicht �ffnen: %s"},
/* fr */ {IDGS_CANT_OPEN_END_SNAP_S_FR, "Impossible d'ouvrir l'instantan� de fin %s."},
/* hu */ {IDGS_CANT_OPEN_END_SNAP_S_HU, "Nem siker�lt az utols� %s pillanatk�p f�jlt megnyitni."},
/* it */ {IDGS_CANT_OPEN_END_SNAP_S_IT, "Non � possibile aprire il file di fine snapshot %s."},
/* nl */ {IDGS_CANT_OPEN_END_SNAP_S_NL, "Kon het eind momentopnamebestand %s niet openen."},
/* pl */ {IDGS_CANT_OPEN_END_SNAP_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANT_OPEN_END_SNAP_S_SV, "Kunde inte �ppna slut�gonblicksbildfilen %s."},
#endif

/* event.c */
/* en */ {IDGS_CANT_FIND_SECTION_END_SNAP,    N_("Could not find event section in end snapshot file.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANT_FIND_SECTION_END_SNAP_DE, "Kann Sektion in Ende Snapshotdatei nicht finden."},
/* fr */ {IDGS_CANT_FIND_SECTION_END_SNAP_FR, "Impossible de trouver la section event dans l'instantan� de fin."},
/* hu */ {IDGS_CANT_FIND_SECTION_END_SNAP_HU, "Nem tal�ltam esem�ny r�szt az utols� pillanatk�p f�jlban."},
/* it */ {IDGS_CANT_FIND_SECTION_END_SNAP_IT, "Non � possibile trovare la sezione eventi nel file di fine snapshot."},
/* nl */ {IDGS_CANT_FIND_SECTION_END_SNAP_NL, "Kon de gebeurtenis sectie in eind momentopnamebestand niet vinden."},
/* pl */ {IDGS_CANT_FIND_SECTION_END_SNAP_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANT_FIND_SECTION_END_SNAP_SV, "Kunde inte hinna h�ndelsedelen i slut�gonblicksbilden."},
#endif

/* event.c */
/* en */ {IDGS_ERROR_READING_START_SNAP_TRIED,    N_("Error reading start snapshot file. Tried %s and %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ERROR_READING_START_SNAP_TRIED_DE, "Fehler beim Lesen der Start Snapshot Datei."
                                                  " Versuch gescheitert bei %s und %s."},
/* fr */ {IDGS_ERROR_READING_START_SNAP_TRIED_FR, "Erreur de lecture de l'instantan� de d�part. Essay� %s et %s"},
/* hu */ {IDGS_ERROR_READING_START_SNAP_TRIED_HU, "Nem siker�lt az els� pillanatk�p f�jl olvas�sa. Kipr�b�lva: %s �s %s"},
/* it */ {IDGS_ERROR_READING_START_SNAP_TRIED_IT, "Errore durante la lettura del file di inizio snapshot. Ho provato %s e "
                                                  "%s"},
/* nl */ {IDGS_ERROR_READING_START_SNAP_TRIED_NL, "Fout bij het lezen van het start momentopnamebestand. Heb %s en %s "
                                                  "geprobeerd"},
/* pl */ {IDGS_ERROR_READING_START_SNAP_TRIED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ERROR_READING_START_SNAP_TRIED_SV, "Fel vid l�sning av start�gonblicksbildfil. F�rs�kte med %s och %s"},
#endif

/* event.c */
/* en */ {IDGS_ERROR_READING_START_SNAP,    N_("Error reading start snapshot file.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ERROR_READING_START_SNAP_DE, "Fehler beim Lesen der Start Snapshot Datei."},
/* fr */ {IDGS_ERROR_READING_START_SNAP_FR, "Erreur de lecture de l'instantan� de d�part."},
/* hu */ {IDGS_ERROR_READING_START_SNAP_HU, "Nem siker�lt az ind�t� pillanatk�p f�jl olvas�sa."},
/* it */ {IDGS_ERROR_READING_START_SNAP_IT, "Errore durante la lettura del file di inizio snapshot."},
/* nl */ {IDGS_ERROR_READING_START_SNAP_NL, "Fout bij het lezen van het start momentopnamebestand."},
/* pl */ {IDGS_ERROR_READING_START_SNAP_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ERROR_READING_START_SNAP_SV, "Fel vid l�sning av start�gonblicksbildfil."},
#endif

/* screenshot.c */
/* en */ {IDGS_SORRY_NO_MULTI_RECORDING,    N_("Sorry. Multiple recording is not supported.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SORRY_NO_MULTI_RECORDING_DE, "Eine Aufnahme ist zur Zeit aktiv. Mehrfache Aufnahme ist nicht m�glich."},
/* fr */ {IDGS_SORRY_NO_MULTI_RECORDING_FR, "D�sol�. De multiples enregistrements ne sont pas support�."},
/* hu */ {IDGS_SORRY_NO_MULTI_RECORDING_HU, "Sajn�lom: t�bbsz�r�s felv�tel nem t�mogatott."},
/* it */ {IDGS_SORRY_NO_MULTI_RECORDING_IT, "Le registrazioni multiple non sono supportate."},
/* nl */ {IDGS_SORRY_NO_MULTI_RECORDING_NL, "Sorry. Meerdere opnamen wordt niet ondersteund."},
/* pl */ {IDGS_SORRY_NO_MULTI_RECORDING_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SORRY_NO_MULTI_RECORDING_SV, "Endast en inspelning kan g�ras �t g�ngen."},
#endif

/* sound.c */
/* en */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED,    N_("write to sound device failed.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_DE, "Schreiben auf Ger�t Sound ist fehlgeschlagen."},
/* fr */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_FR, "Impossible d'�criture sur le p�riph�rique de son."},
/* hu */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_HU, "Nem siker�lt a hangeszk�zre �rni"},
/* it */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_IT, "scrittura sulla scheda audio fallita."},
/* nl */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_NL, "schrijfactie naar geluidsapparaat is mislukt."},
/* pl */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_WRITE_TO_SOUND_DEVICE_FAILED_SV, "misslyckades att skriva till ljudenhet."},
#endif

/* sound.c */
/* en */ {IDGS_CANNOT_OPEN_SID_ENGINE,    N_("Cannot open SID engine")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_OPEN_SID_ENGINE_DE, "Kann SID Engine nicht �ffnen"},
/* fr */ {IDGS_CANNOT_OPEN_SID_ENGINE_FR, "Erreur de chargement de l'engin de son SID"},
/* hu */ {IDGS_CANNOT_OPEN_SID_ENGINE_HU, "Nem siker�lt a SID motort megnyitni"},
/* it */ {IDGS_CANNOT_OPEN_SID_ENGINE_IT, "Non � possibile aprire il motore SID"},
/* nl */ {IDGS_CANNOT_OPEN_SID_ENGINE_NL, "Kan de SID-kern niet openen"},
/* pl */ {IDGS_CANNOT_OPEN_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_OPEN_SID_ENGINE_SV, "Kan inte �ppna SID-motor"},
#endif

/* sound.c */
/* en */ {IDGS_CANNOT_INIT_SID_ENGINE,    N_("Cannot initialize SID engine")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_INIT_SID_ENGINE_DE, "Kann SID Engine nicht initialisieren"},
/* fr */ {IDGS_CANNOT_INIT_SID_ENGINE_FR, "Erreur d'initialisation de l'engin de son SID"},
/* hu */ {IDGS_CANNOT_INIT_SID_ENGINE_HU, "Nem siker�lt a SID motort elind�tani"},
/* it */ {IDGS_CANNOT_INIT_SID_ENGINE_IT, "Non � possibile inizializzare il motore SID"},
/* nl */ {IDGS_CANNOT_INIT_SID_ENGINE_NL, "Kan de SID-kern niet initialiseren"},
/* pl */ {IDGS_CANNOT_INIT_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_INIT_SID_ENGINE_SV, "Kan inte initiera SID-motor"},
#endif

/* sound.c */
/* en */ {IDGS_INIT_FAILED_FOR_DEVICE_S,    N_("initialization failed for device `%s'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_INIT_FAILED_FOR_DEVICE_S_DE, "Initialisierung von Ger�t `%s' fehlgeschlagen."},
/* fr */ {IDGS_INIT_FAILED_FOR_DEVICE_S_FR, "erreur d'initialisation du p�riph�rique `%s'."},
/* hu */ {IDGS_INIT_FAILED_FOR_DEVICE_S_HU, "A `%s' eszk�zt nem siker�lt elind�tani."},
/* it */ {IDGS_INIT_FAILED_FOR_DEVICE_S_IT, "inizializzazione fallita per il device `%s'."},
/* nl */ {IDGS_INIT_FAILED_FOR_DEVICE_S_NL, "initialisatie voor apparaat `%s' is mislukt."},
/* pl */ {IDGS_INIT_FAILED_FOR_DEVICE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_INIT_FAILED_FOR_DEVICE_S_SV, "initiering misslyckades f�r enhet \"%s\"."},
#endif

/* sound.c */
/* en */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT,    N_("device '%s' not found or not supported.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_DE, "Ger�t '%s' konnte nicht gefunden werden oder ist nicht unterst�tzt."},
/* fr */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_FR, "p�riph�rique '%s' non trouv� ou non support�."},
/* hu */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_HU, "A '%s' eszk�z nem l�tezik, vagy nem t�mogatott."},
/* it */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_IT, "il device '%s' non � stato trovato oppure non � supportato."},
/* nl */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_NL, "apparaat '%s' niet gevonden of wordt niet ondersteund"},
/* pl */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_DEVICE_S_NOT_FOUND_SUPPORT_SV, "enheten \"%s\" hittades eller st�ds ej."},
#endif

/* sound.c */
/* en */ {IDGS_RECORD_DEVICE_S_NOT_EXIST,    N_("Recording device %s doesn't exist!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_DE, "Aufnahmeger�t %s existiert nicht!"},
/* fr */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_FR, "Le p�riph�rique d'enregistrement %s n'existe pas!"},
/* hu */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_HU, "A %s felvev� eszk�z nem l�tezik!"},
/* it */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_IT, "Il device di registrazione %s non esiste!"},
/* nl */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_NL, "Opnameapparaat %s bestaat niet!"},
/* pl */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RECORD_DEVICE_S_NOT_EXIST_SV, "Inspelningsenhet %s finns inte!"},
#endif

/* sound.c */
/* en */ {IDGS_RECORD_DIFFERENT_PLAYBACK,    N_("Recording device must be different from playback device")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RECORD_DIFFERENT_PLAYBACK_DE, "Aufnahmeger�t mu� unteschiedlich vom Abspielger�t sein"},
/* fr */ {IDGS_RECORD_DIFFERENT_PLAYBACK_FR, "Le p�riph�rique de son d'enregistrement doivent �tre diff�rents"},
/* hu */ {IDGS_RECORD_DIFFERENT_PLAYBACK_HU, "A felvev� �s lej�tsz� eszk�z�knek k�l�nb�znie kell"},
/* it */ {IDGS_RECORD_DIFFERENT_PLAYBACK_IT, "Il device di registrazione deve essere differente da quello di "
                                             "riproduzione"},
/* nl */ {IDGS_RECORD_DIFFERENT_PLAYBACK_NL, "Opnameapparaat moet anders zijn dan afspeelapparaat"},
/* pl */ {IDGS_RECORD_DIFFERENT_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RECORD_DIFFERENT_PLAYBACK_SV, "Inspelningsenhet och �terspelningsenhet kan inte vara samma"},
#endif

/* sound.c */
/* en */ {IDGS_WARNING_RECORDING_REALTIME,    N_("Warning! Recording device %s seems to be a realtime device!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_WARNING_RECORDING_REALTIME_DE, "Warnung! Aufnahmeger�t %s scheint ein Echtzeitger�t zu sein!"},
/* fr */ {IDGS_WARNING_RECORDING_REALTIME_FR, "Attention! Le p�riph�rique d'enregistrement %s semble �tre un "
                                              "p�riph�rique en temps r�el"},
/* hu */ {IDGS_WARNING_RECORDING_REALTIME_HU, "Figyelem! A %s felvev� eszk�z �gy t�nik, val�s idej� eszk�z!"},
/* it */ {IDGS_WARNING_RECORDING_REALTIME_IT, "Attenzione! Il device di registrazione %s sembra essere un dispositivo "
                                              "realtime!"},
/* nl */ {IDGS_WARNING_RECORDING_REALTIME_NL, "Waarschuwing! Opnameapparaat %s lijkt een realtime apparaat te zijn!"},
/* pl */ {IDGS_WARNING_RECORDING_REALTIME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_WARNING_RECORDING_REALTIME_SV, "Varning! Inspelningsenheten %s verkar vara en realtidsenhet!"},
#endif

/* sound.c */
/* en */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR,    N_("The recording device doesn't support current sound parameters")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_DE, "Aufnahmeger�t unterst�tz derzeitige Soundeinstellungen nicht"},
/* fr */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_FR, "Le p�riph�rique d'enregistrement ne supporte pas les param�tres actuels"},
/* hu */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_HU, "A felvev� eszk�z nem t�mogatja az aktu�lis hangbe�ll�t�sokat"},
/* it */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_IT, "Il device di registrazione non supporta i parametri attuali"},
/* nl */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_NL, "Het opname apparaat ondersteunt de huidige geluid opties niet"},
/* pl */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RECORD_NOT_SUPPORT_SOUND_PAR_SV, "Inspelningsenheten st�der inte aktuella ljudinst�llningar"},
#endif

/* sound.c */
/* en */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE,    N_("Sound buffer overflow (cycle based)")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_DE, "Sound Puffer �berlauf (Zyklus basiert)"},
/* fr */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_FR, "Erreur de d�passement de tampon son (cycle based)"},
/* hu */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_HU, "Hangpuffer t�lcsordul�s (ciklus alap�)"},
/* it */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_IT, "Overflow del buffer sonoro (cycle based)"},
/* nl */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_NL, "Geluidsbuffer overloop (cyclus gebaseerd)"},
/* pl */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SOUND_BUFFER_OVERFLOW_CYCLE_SV, "F�r mycket data i ljudbufferten (cykelbaserad)"},
#endif

/* sound.c */
/* en */ {IDGS_SOUND_BUFFER_OVERFLOW,    N_("Sound buffer overflow.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SOUND_BUFFER_OVERFLOW_DE, "Sound Puffer �berlauf."},
/* fr */ {IDGS_SOUND_BUFFER_OVERFLOW_FR, "Erreur de d�passement de tampon son."},
/* hu */ {IDGS_SOUND_BUFFER_OVERFLOW_HU, "Hangpuffer t�lcsordul�s."},
/* it */ {IDGS_SOUND_BUFFER_OVERFLOW_IT, "Overflow del buffer sonoro."},
/* nl */ {IDGS_SOUND_BUFFER_OVERFLOW_NL, "Geluidsbuffer overloop."},
/* pl */ {IDGS_SOUND_BUFFER_OVERFLOW_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SOUND_BUFFER_OVERFLOW_SV, "F�r mycket data i ljudbufferten."},
#endif

/* sound.c */
/* en */ {IDGS_CANNOT_FLUSH,    N_("cannot flush.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_FLUSH_DE, "Entleerung nicht m�glich."},
/* fr */ {IDGS_CANNOT_FLUSH_FR, "impossible de vider."},
/* hu */ {IDGS_CANNOT_FLUSH_HU, "nem siker�lt �r�teni a puffert."},
/* it */ {IDGS_CANNOT_FLUSH_IT, "non � possibile svuotare."},
/* nl */ {IDGS_CANNOT_FLUSH_NL, "kan niet spoelen."},
/* pl */ {IDGS_CANNOT_FLUSH_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_FLUSH_SV, "kan inte t�mma."},
#endif

/* sound.c */
/* en */ {IDGS_FRAGMENT_PROBLEMS,    N_("fragment problems.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_FRAGMENT_PROBLEMS_DE, "Fragmentierungsproblem."},
/* fr */ {IDGS_FRAGMENT_PROBLEMS_FR, "probl�mes de fragments."},
/* hu */ {IDGS_FRAGMENT_PROBLEMS_HU, "t�red�kr�sz probl�m�k."},
/* it */ {IDGS_FRAGMENT_PROBLEMS_IT, "problemi di frammentazione."},
/* nl */ {IDGS_FRAGMENT_PROBLEMS_NL, "fragment problemen."},
/* pl */ {IDGS_FRAGMENT_PROBLEMS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_FRAGMENT_PROBLEMS_SV, "fragmentprogram."},
#endif

/* sound.c */
/* en */ {IDGS_RUNNING_TOO_SLOW,    N_("running too slow.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RUNNING_TOO_SLOW_DE, "Ablauf zu langsam."},
/* fr */ {IDGS_RUNNING_TOO_SLOW_FR, "l'ex�cution est trop lente."},
/* hu */ {IDGS_RUNNING_TOO_SLOW_HU, "t�l lassan megy."},
/* it */ {IDGS_RUNNING_TOO_SLOW_IT, "esecuzione troppo lenta."},
/* nl */ {IDGS_RUNNING_TOO_SLOW_NL, "draait te langzaam."},
/* pl */ {IDGS_RUNNING_TOO_SLOW_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RUNNING_TOO_SLOW_SV, "g�r f�r l�ngsamt."},
#endif

/* sound.c */
/* en */ {IDGS_STORE_SOUNDDEVICE_FAILED,    N_("store to sounddevice failed.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_STORE_SOUNDDEVICE_FAILED_DE, "Speichern auf Sound Ger�t ist fehlgeschlagen."},
/* fr */ {IDGS_STORE_SOUNDDEVICE_FAILED_FR, "erreur d'enregistrement sur le p�riph�rique de son."},
/* hu */ {IDGS_STORE_SOUNDDEVICE_FAILED_HU, "a hangeszk�z�n t�rt�n� t�rol�s nem siker�lt."},
/* it */ {IDGS_STORE_SOUNDDEVICE_FAILED_IT, "memorizzazione sulla scheda audio fallita."},
/* nl */ {IDGS_STORE_SOUNDDEVICE_FAILED_NL, "opslag naar geluidsapparaat is mislukt."},
/* pl */ {IDGS_STORE_SOUNDDEVICE_FAILED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_STORE_SOUNDDEVICE_FAILED_SV, "misslyckades spara i ljudenheten."},
#endif

/* c64/c64export.c */
/* en */ {IDGS_RESOURCE_S_BLOCKED_BY_S,    N_("Resource %s blocked by %s.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RESOURCE_S_BLOCKED_BY_S_DE, "Resource %s wird von %s blockiert."},
/* fr */ {IDGS_RESOURCE_S_BLOCKED_BY_S_FR, "La ressource %s est bloqu�e par %s."},
/* hu */ {IDGS_RESOURCE_S_BLOCKED_BY_S_HU, "A %s er�forr�st %s fogja."},
/* it */ {IDGS_RESOURCE_S_BLOCKED_BY_S_IT, "Risorsa %s bloccata da %s."},
/* nl */ {IDGS_RESOURCE_S_BLOCKED_BY_S_NL, "Bron %s geblokeerd door %s."},
/* pl */ {IDGS_RESOURCE_S_BLOCKED_BY_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RESOURCE_S_BLOCKED_BY_S_SV, "Resursen %s blockeras av %s."},
#endif

#ifdef HAVE_NETWORK
/* network.c */
/* en */ {IDGS_TESTING_BEST_FRAME_DELAY,    N_("Testing best frame delay...")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_TESTING_BEST_FRAME_DELAY_DE, "Teste optimale Framerate..."},
/* fr */ {IDGS_TESTING_BEST_FRAME_DELAY_FR, "Recherche du meilleur d�lai d'image..."},
/* hu */ {IDGS_TESTING_BEST_FRAME_DELAY_HU, "Legjobb k�pkocka k�sleltet�s tesztel�se..."},
/* it */ {IDGS_TESTING_BEST_FRAME_DELAY_IT, "Rileva il miglior ritardo tra frame..."},
/* nl */ {IDGS_TESTING_BEST_FRAME_DELAY_NL, "Beste framevertraging aan het testen..."},
/* pl */ {IDGS_TESTING_BEST_FRAME_DELAY_PL, ""},  /* fuzzy */
/* sv */ {IDGS_TESTING_BEST_FRAME_DELAY_SV, "Testar b�sta ramf�rdr�jning..."},
#endif

/* network.c */
/* en */ {IDGS_USING_D_FRAMES_DELAY,    N_("Using %d frames delay.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_USING_D_FRAMES_DELAY_DE, "Benutze %d Frameverz�gerung."},
/* fr */ {IDGS_USING_D_FRAMES_DELAY_FR, "Utilisation d'un d�lai de %d images."},
/* hu */ {IDGS_USING_D_FRAMES_DELAY_HU, "%d k�pkocka k�sleltet�s haszn�lata."},
/* it */ {IDGS_USING_D_FRAMES_DELAY_IT, "Utilizzo %d frame di ritardo."},
/* nl */ {IDGS_USING_D_FRAMES_DELAY_NL, "%d frames vertraging in gebruik."},
/* pl */ {IDGS_USING_D_FRAMES_DELAY_PL, ""},  /* fuzzy */
/* sv */ {IDGS_USING_D_FRAMES_DELAY_SV, "Anv�nder %d ramars f�rdr�jning."},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER,    N_("Cannot load snapshot file for transfer")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_DE, "Kann Snapshot Datei f�r Transfer nicht laden"},
/* fr */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_FR, "Impossible de charger l'instantan� pour le transfer"},
/* hu */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_HU, "Nem siker�lt az �tk�ldend� pillanatk�p f�jlt bet�lteni"},
/* it */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_IT, "Non � possibile caricare il file di snapshot per il trasferimento"},
/* nl */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_NL, "Kan het momentopnamebestand voor versturen niet laden"},
/* pl */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_LOAD_SNAPSHOT_TRANSFER_SV, "Kan inte l�sa in �gonblicksbildfil f�r �verf�ring."},
#endif

/* network.c */
/* en */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT,    N_("Sending snapshot to client...")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_DE, "Sende Snapshot zu Client..."},
/* fr */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_FR, "Envoi de l'instantan� au client..."},
/* hu */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_HU, "Pillanatk�p k�ld�se a kliensnek..."},
/* it */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_IT, "Invio dell'immagine dello snapshot al client in corso..."},
/* nl */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_NL, "Momentopname wordt naar de andere computer verstuurd..."},
/* pl */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SENDING_SNAPSHOT_TO_CLIENT_SV, "S�nder �gonblicksbild till klient..."},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT,    N_("Cannot send snapshot to client")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_DE, "Kann Snapshot Datei nicht senden"},
/* fr */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_FR, "Impossible d'envoyer l'instantan� au client"},
/* hu */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_HU, "Nem siker�lt pillanatk�pet elk�ldeni a klienshez"},
/* it */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_IT, "Non � possibile inviare lo snapshot al client"},
/* nl */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_NL, "Kan de momentopname niet naar de andere computer versturen"},
/* pl */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_SEND_SNAPSHOT_TO_CLIENT_SV, "Kan inte s�nda �gonblicksbild till klient"},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S,    N_("Cannot create snapshot file %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_DE, "Kann Snapshot Datei `%s' nicht erzeugen"},
/* fr */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_FR, "Impossible de cr�er l'instantan� %s"},
/* hu */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_HU, "Nem siker�lt a pillanatk�pf�jlt l�trehozni (%s)"},
/* it */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_IT, "Non � possibile creare il file di snapshot %s"},
/* nl */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_NL, "Kan het momentopnamebestand %s niet maken"},
/* pl */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_CREATE_SNAPSHOT_FILE_S_SV, "Kan inte skapa �gonblicksbildfil %s"},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S,    N_("Cannot open snapshot file %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_DE, "Kann Snapshot Datei `%s' nicht �ffnen"},
/* fr */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_FR, "Impossible d'ouvrir le fichier %s"},
/* hu */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_HU, "Nem siker�lt a pillanatk�pf�jlt megnyitni (%s)"},
/* it */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_IT, "Non � possibile aprire il file di snapshot %s"},
/* nl */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_NL, "Kan het momentopnamebestand %s niet openen"},
/* pl */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_OPEN_SNAPSHOT_FILE_S_SV, "Kan inte �ppna �gonblicksbildfil %s"},
#endif

/* network.c */
/* en */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT,    N_("Server is waiting for a client...")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_DE, "Server wartet auf Client..."},
/* fr */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_FR, "Le serveur attend la connexion d'un client..."},
/* hu */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_HU, "A kiszolg�l� kliensre v�r..."},
/* it */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_IT, "Il server � in attesa di un client..."},
/* nl */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_NL, "De server wacht op de andere computer..."},
/* pl */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SERVER_IS_WAITING_FOR_CLIENT_SV, "Servern v�ntar p� en klient..."},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT,    N_("Cannot create snapshot file. Select different history directory!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_DE, "Kann Snapshot Datei nicht erzeugen. Verwende ein anderes "
                                                   "Verzeichnis!"},
/* fr */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_FR, "Impossible de cr�er le fichier d'instantan�. S�lectionnez un "
                                                   "r�pertoire d'historique diff�rent"},
/* hu */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_HU, "Nem siker�lt pillanatk�p f�jlt l�trehozni. K�rem v�lasszon m�s "
                                                   "k�nyvt�rat!"},
/* it */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_IT, "Non � possibile creare il file di snapshot. Seleziona una directory "
                                                   "diversa per la cronologia."},
/* nl */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_NL, "Kan het momentopnamebestand niet maken. Selecteer een andere "
                                                   "geschiedenisdirectory!"},
/* pl */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_CREATE_SNAPSHOT_S_SELECT_SV, "Kan inte skapa �gonblicksbildfil. V�lj en annan historikkatalog!"},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_RESOLVE_S,    N_("Cannot resolve %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_RESOLVE_S_DE, "Kann %s nicht aufl�sen"},
/* fr */ {IDGS_CANNOT_RESOLVE_S_FR, "Impossible de r�soudre %s"},
/* hu */ {IDGS_CANNOT_RESOLVE_S_HU, "Nem siker�lt %s-t feloldani"},
/* it */ {IDGS_CANNOT_RESOLVE_S_IT, "Non � possibile risolvere %s"},
/* nl */ {IDGS_CANNOT_RESOLVE_S_NL, "Onbekende host %s"},
/* pl */ {IDGS_CANNOT_RESOLVE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_RESOLVE_S_SV, "Kan inte sl� upp %s"},
#endif

/* network.c */
/* en */ {IDGS_CANNOT_CONNECT_TO_S,    N_("Cannot connect to %s (no server running on port %d).")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_CONNECT_TO_S_DE, "Kann zu %s nicht verbinden (Kein Server aktiv auf Port %d)."},
/* fr */ {IDGS_CANNOT_CONNECT_TO_S_FR, "Impossible de se connecter � %s (aucun serveur sur le port %d)."},
/* hu */ {IDGS_CANNOT_CONNECT_TO_S_HU, "Nem siker�lt %s-hez kapcsol�dni (a %d porton nem fut kiszolg�l�)"},
/* it */ {IDGS_CANNOT_CONNECT_TO_S_IT, "Non � possibile connettersi a %s (nessun server � attivo sulla porta %d)."},
/* nl */ {IDGS_CANNOT_CONNECT_TO_S_NL, "Kan geen verbinding maken met %s (er is geen server aanwezig op poort %d)."},
/* pl */ {IDGS_CANNOT_CONNECT_TO_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_CONNECT_TO_S_SV, "Kan inte ansluta till %s (ingen server ig�ng p� port %d)."},
#endif

/* network.c */
/* en */ {IDGS_RECEIVING_SNAPSHOT_SERVER,    N_("Receiving snapshot from server...")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RECEIVING_SNAPSHOT_SERVER_DE, "Empfange Snapshot von Server..."},
/* fr */ {IDGS_RECEIVING_SNAPSHOT_SERVER_FR, "R�ception de l'instantan� du serveur..."},
/* hu */ {IDGS_RECEIVING_SNAPSHOT_SERVER_HU, "Pillanatk�p fogad�sa a szerverr�l..."},
/* it */ {IDGS_RECEIVING_SNAPSHOT_SERVER_IT, "Ricezione dello snapshot dal server in corso..."},
/* nl */ {IDGS_RECEIVING_SNAPSHOT_SERVER_NL, "Momentopname van de server wordt ontvangen..."},
/* pl */ {IDGS_RECEIVING_SNAPSHOT_SERVER_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RECEIVING_SNAPSHOT_SERVER_SV, "Tar emot �goblicksbild fr�n server..."},
#endif

/* network.c */
/* en */ {IDGS_NETWORK_OUT_OF_SYNC,    N_("Network out of sync - disconnecting.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_NETWORK_OUT_OF_SYNC_DE, "Netzwerksynchronisationsproblem - Trenne Verbindung."},
/* fr */ {IDGS_NETWORK_OUT_OF_SYNC_FR, "Le r�seau n'est pas synchronis� (out of sync error). D�connexion."},
/* hu */ {IDGS_NETWORK_OUT_OF_SYNC_HU, "A h�l�zat kiesett a szinkronb�l - sz�tkapcsol�d�s"},
/* it */ {IDGS_NETWORK_OUT_OF_SYNC_IT, "Rete non sincronizzata - disconnesione in corso."},
/* nl */ {IDGS_NETWORK_OUT_OF_SYNC_NL, "Netwerk niet synchroon - verbinding wordt verbroken."},
/* pl */ {IDGS_NETWORK_OUT_OF_SYNC_PL, ""},  /* fuzzy */
/* sv */ {IDGS_NETWORK_OUT_OF_SYNC_SV, "N�tverket ur synk - kopplar fr�n."},
#endif

/* network.c */
/* en */ {IDGS_REMOTE_HOST_DISCONNECTED,    N_("Remote host disconnected.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_REMOTE_HOST_DISCONNECTED_DE, "Entfernter Rechner getrennt."},
/* fr */ {IDGS_REMOTE_HOST_DISCONNECTED_FR, "L'h�te distant s'est d�connect�."},
/* hu */ {IDGS_REMOTE_HOST_DISCONNECTED_HU, "A t�voli sz�m�tog�p lekapcsol�dott."},
/* it */ {IDGS_REMOTE_HOST_DISCONNECTED_IT, "Disconnesso dall'host remoto."},
/* nl */ {IDGS_REMOTE_HOST_DISCONNECTED_NL, "Andere computer heeft de verbinding verbroken."},
/* pl */ {IDGS_REMOTE_HOST_DISCONNECTED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_REMOTE_HOST_DISCONNECTED_SV, "Fr�nkopplad fr�n fj�rrv�rd."},
#endif

/* network.c */
/* en */ {IDGS_REMOTE_HOST_SUSPENDING,    N_("Remote host suspending...")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_REMOTE_HOST_SUSPENDING_DE, "Entfernter Rechner angehaltedn..."},
/* fr */ {IDGS_REMOTE_HOST_SUSPENDING_FR, "L'h�te distant � suspendu..."},
/* hu */ {IDGS_REMOTE_HOST_SUSPENDING_HU, "A t�voli sz�m�t�g�p felf�ggeszt�st k�rt..."},
/* it */ {IDGS_REMOTE_HOST_SUSPENDING_IT, "Sospesione dell'host remoto in corso..."},
/* nl */ {IDGS_REMOTE_HOST_SUSPENDING_NL, "Andere computer halt de netplay..."},
/* pl */ {IDGS_REMOTE_HOST_SUSPENDING_PL, ""},  /* fuzzy */
/* sv */ {IDGS_REMOTE_HOST_SUSPENDING_SV, "FJ�rrv�rden pausar..."},
#endif

#ifdef HAVE_IPV6
/* network.c */
/* en */ {IDGS_CANNOT_SWITCH_IPV4_IPV6,    N_("Cannot switch IPV4/IPV6 while netplay is active.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_DE, "Kann zwischen IPV4/IPV6 nicht wechseln, solange netplay aktiv ist."},
/* fr */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_FR, "Ne peut alterner IPV4/IPV6 pendant que netplay est actif."},
/* hu */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_HU, "Nem v�lthat IPV4/IPV6 k�z�tt, am�g a h�l�zati j�t�k akt�v"},
/* it */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_IT, "Non � possibile commutare tra IPV4 e IPV6 mentre netplay � attivo."},
/* nl */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_NL, "Kan niet schakelen tussen IPV4/IPV6 omdat netplay actief is."},
/* pl */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CANNOT_SWITCH_IPV4_IPV6_SV, "Kan inte v�xla mellan IPV4/IPV6 medan n�tverksspel �r aktivt."},
#endif
#endif
#endif

/* gfxoutputdrv/ffmpegdrv.c */
/* en */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM,    N_("ffmpegdrv: Cannot open video stream")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_DE, "ffmpegdrv: Kann video stream nicht �ffnen"},
/* fr */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_FR, "ffmpegdrv: Impossible d'ouvrir le flux vid�o"},
/* hu */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_HU, "ffmpegdrv: Nem siker�lt megnyitni a vide� adatfolyamot"},
/* it */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_IT, "ffmpegdrv: Non � possibile aprire il flusso video"},
/* nl */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_NL, "ffmpegdrv: Kan de videostroom niet openen"},
/* pl */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_PL, ""},  /* fuzzy */
/* sv */ {IDGS_FFMPEG_CANNOT_OPEN_VSTREAM_SV, "ffmpegdrv: Kan inte �ppna videostr�m"},
#endif

/* gfxoutputdrv/ffmpegdrv.c */
/* en */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM,    N_("ffmpegdrv: Cannot open audio stream")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_DE, "ffmpegdrv: Kann audio stream nicht �ffnen"},
/* fr */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_FR, "ffmpegdrv: Impossible d'ouvrir le flux audio"},
/* hu */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_HU, "ffmpegdrv: Nem siker�lt megnyitni a audi� adatfolyamot"},
/* it */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_IT, "ffmpegdrv: Non � possibile aprire il flusso audio"},
/* nl */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_NL, "ffmpegdrv: Kan de audiostroom niet openen"},
/* pl */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_PL, ""},  /* fuzzy */
/* sv */ {IDGS_FFMPEG_CANNOT_OPEN_ASTREAM_SV, "ffmpegdrv: Kan inte �ppna ljudstr�m"},
#endif

/* gfxoutputdrv/ffmpegdrv.c */
/* en */ {IDGS_FFMPEG_CANNOT_OPEN_S,    N_("ffmpegdrv: Cannot open %s")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_FFMPEG_CANNOT_OPEN_S_DE, "ffmpegdrv: �ffnen von %s fehlgeschlagen"},
/* fr */ {IDGS_FFMPEG_CANNOT_OPEN_S_FR, "ffmpegdrv: Impossible d'ouvrir %s"},
/* hu */ {IDGS_FFMPEG_CANNOT_OPEN_S_HU, "ffmpegdrv: Nem siker�lt megnyitni: %s"},
/* it */ {IDGS_FFMPEG_CANNOT_OPEN_S_IT, "ffmpegdrv: Non � possibile aprire %s"},
/* nl */ {IDGS_FFMPEG_CANNOT_OPEN_S_NL, "ffmpegdrv: Kan %s niet openen"},
/* pl */ {IDGS_FFMPEG_CANNOT_OPEN_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_FFMPEG_CANNOT_OPEN_S_SV, "ffmpegdrv: Kan inte �ppna %s"},
#endif

/* c64/c64io.c */
/* en */ {IDGS_IO_READ_COLL_AT_X_FROM,    N_("I/O read collision at %X from ")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_IO_READ_COLL_AT_X_FROM_DE, "I/O Lese Kollision bei %X von "},
/* fr */ {IDGS_IO_READ_COLL_AT_X_FROM_FR, "Collision lors de la lecture E/S at %X � partir de "},
/* hu */ {IDGS_IO_READ_COLL_AT_X_FROM_HU, "I/O olvas�s �tk�z�s %X-n�l, hely: "},
/* it */ {IDGS_IO_READ_COLL_AT_X_FROM_IT, "Collisione di I/O in lettura a %X da "},
/* nl */ {IDGS_IO_READ_COLL_AT_X_FROM_NL, "I/O lees botsing op %X van "},
/* pl */ {IDGS_IO_READ_COLL_AT_X_FROM_PL, ""},  /* fuzzy */
/* sv */ {IDGS_IO_READ_COLL_AT_X_FROM_SV, "I/O-l�skollision vid %X fr�n "},
#endif

/* c64/c64io.c */
/* en */ {IDGS_AND,    N_(" and ")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_AND_DE, " und "},
/* fr */ {IDGS_AND_FR, " et "},
/* hu */ {IDGS_AND_HU, " �s "},
/* it */ {IDGS_AND_IT, " e "},
/* nl */ {IDGS_AND_NL, " en "},
/* pl */ {IDGS_AND_PL, ""},  /* fuzzy */
/* sv */ {IDGS_AND_SV, " och "},
#endif

/* c64/c64io.c */
/* en */ {IDGS_ALL_DEVICES_DETACHED,    N_(".\nAll the named devices will be detached.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ALL_DEVICES_DETACHED_DE, ".\nAlle genannten Ger�te werden entfernt."},
/* fr */ {IDGS_ALL_DEVICES_DETACHED_FR, ".\nTous les p�riph�riques nomm�s vont �tre d�tach�s."},
/* hu */ {IDGS_ALL_DEVICES_DETACHED_HU, ".\nMinden megnevezett eszk�z le lesz v�lasztva."},
/* it */ {IDGS_ALL_DEVICES_DETACHED_IT, ".\nTutti i device specificati sarranno rimossi."},
/* nl */ {IDGS_ALL_DEVICES_DETACHED_NL, ".\nAlle genoemde apparaten zullen worden ontkoppelt."},
/* pl */ {IDGS_ALL_DEVICES_DETACHED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ALL_DEVICES_DETACHED_SV, ".\nAlla namngivna enheter kommer kopplas fr�n."},
#endif

/* c64/digimax.c */
/* en */ {IDGS_DIGIMAX_NOT_WITH_RESID,    N_("Digimax cannot be used with ReSID\nPlease switch SID Engine to FastSID")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_DIGIMAX_NOT_WITH_RESID_DE, "Digimax kann mit ReSID nicht benutzt werden\nBitte SID Engine auf FastSID "
                                          "setzen"},
/* fr */ {IDGS_DIGIMAX_NOT_WITH_RESID_FR, "Digimax ne peut �tre utilis� avec ReSID\nVeuillez utiliser l�engin SID "
                                          "FastSID"},
/* hu */ {IDGS_DIGIMAX_NOT_WITH_RESID_HU, "A Digimax nem haszn�lhat� ReSID-del\nK�rem v�ltsa a SID motort FastSID-re"},
/* it */ {IDGS_DIGIMAX_NOT_WITH_RESID_IT, "Digimax non pu� essere usato con il ReSID\nCambia il motore SID a FastSID"},
/* nl */ {IDGS_DIGIMAX_NOT_WITH_RESID_NL, "Digimax kan niet gebruikt worden met ReSID\nschakel de SID-kern over naar "
                                          "FastSID"},
/* pl */ {IDGS_DIGIMAX_NOT_WITH_RESID_PL, ""},  /* fuzzy */
/* sv */ {IDGS_DIGIMAX_NOT_WITH_RESID_SV, "Digimax kan inte anv�ndas med ReSID\nByt SID-motor till FastSID"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDGS_NO_FILENAME,    N_("No filename!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_NO_FILENAME_DE, "Dateiname fehlt!"},
/* fr */ {IDGS_NO_FILENAME_FR, ""},  /* fuzzy */
/* hu */ {IDGS_NO_FILENAME_HU, "Nincs f�jln�v!"},
/* it */ {IDGS_NO_FILENAME_IT, "Nessun nome file!"},
/* nl */ {IDGS_NO_FILENAME_NL, "Geen bestandsnaam!"},
/* pl */ {IDGS_NO_FILENAME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_NO_FILENAME_SV, "Filnamn saknas!"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDGS_ILLEGAL_FILENAME,    N_("Illegal filename!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ILLEGAL_FILENAME_DE, "Ung�ltiger Dateiname!"},
/* fr */ {IDGS_ILLEGAL_FILENAME_FR, ""},  /* fuzzy */
/* hu */ {IDGS_ILLEGAL_FILENAME_HU, "�rv�nytelen f�jln�v!"},
/* it */ {IDGS_ILLEGAL_FILENAME_IT, "Nome file non valido!"},
/* nl */ {IDGS_ILLEGAL_FILENAME_NL, "Bestandsnaam ongeldig!"},
/* pl */ {IDGS_ILLEGAL_FILENAME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ILLEGAL_FILENAME_SV, "Ogiltigt filnamn!"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDGS_ERROR_CREATING_FILE_S,    N_("Error creating file %s!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ERROR_CREATING_FILE_S_DE, "Fehler beim Schreiben: %s."},
/* fr */ {IDGS_ERROR_CREATING_FILE_S_FR, ""},  /* fuzzy */
/* hu */ {IDGS_ERROR_CREATING_FILE_S_HU, "Hiba %s f�jl l�trehoz�sa k�zben!"},
/* it */ {IDGS_ERROR_CREATING_FILE_S_IT, "Errore durante la creazione del file %s!"},
/* nl */ {IDGS_ERROR_CREATING_FILE_S_NL, "Fout bij het maken van bestand %s!"},
/* pl */ {IDGS_ERROR_CREATING_FILE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ERROR_CREATING_FILE_S_SV, "Fel n�r filen \"%s\" skulle skapas!"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDGS_ERROR_WRITING_TO_FILE_S,    N_("Error writing to file %s!")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ERROR_WRITING_TO_FILE_S_DE, "Fehler bei Schreibzugriff: %s!"},
/* fr */ {IDGS_ERROR_WRITING_TO_FILE_S_FR, ""},  /* fuzzy */
/* hu */ {IDGS_ERROR_WRITING_TO_FILE_S_HU, "Hiba %s f�jlba �r�s k�zben!"},
/* it */ {IDGS_ERROR_WRITING_TO_FILE_S_IT, "Errore durante la scrittura sul file %s!"},
/* nl */ {IDGS_ERROR_WRITING_TO_FILE_S_NL, "Fout bij het schrijven naar bestand %s!"},
/* pl */ {IDGS_ERROR_WRITING_TO_FILE_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ERROR_WRITING_TO_FILE_S_SV, "Fel vid skrivning till filen %s!"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDGS_DTV_ROM_CREATED,    N_("DTV ROM image created successfully")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_DTV_ROM_CREATED_DE, "DTV ROM Image erfolgreich erzeugt"},
/* fr */ {IDGS_DTV_ROM_CREATED_FR, ""},  /* fuzzy */
/* hu */ {IDGS_DTV_ROM_CREATED_HU, "DTV ROM k�pm�s sikeresen l�trehozva"},
/* it */ {IDGS_DTV_ROM_CREATED_IT, "Immagine ROM DTV creata con successo"},
/* nl */ {IDGS_DTV_ROM_CREATED_NL, "DTV ROM bestand gemaakt!"},
/* pl */ {IDGS_DTV_ROM_CREATED_PL, ""},  /* fuzzy */
/* sv */ {IDGS_DTV_ROM_CREATED_SV, "DTV-ROM-avbildning skapades framg�ngsrikt"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_COMMAND_S_UNKNOWN,    N_("Command `%s' unknown.\n")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_COMMAND_S_UNKNOWN_DE, "Kommand `%s' ist unbekannt.\n"},
/* fr */ {IDGS_COMMAND_S_UNKNOWN_FR, ""},  /* fuzzy */
/* hu */ {IDGS_COMMAND_S_UNKNOWN_HU, "`%s' parancs ismeretlen.\n"},
/* it */ {IDGS_COMMAND_S_UNKNOWN_IT, "Comando `%s' sconosciuto.\n"},
/* nl */ {IDGS_COMMAND_S_UNKNOWN_NL, "Commando %s' is onbekent.\n"},
/* pl */ {IDGS_COMMAND_S_UNKNOWN_PL, ""},  /* fuzzy */
/* sv */ {IDGS_COMMAND_S_UNKNOWN_SV, "Kommandot \"%s\" �r ok�nt.\n"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_NO_HELP_AVAILABLE_FOR_S,    N_("No help available for `%s'\n")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_NO_HELP_AVAILABLE_FOR_S_DE, "Es ist keine Hilfe f�r `%s' verf�gbar\n"},
/* fr */ {IDGS_NO_HELP_AVAILABLE_FOR_S_FR, ""},  /* fuzzy */
/* hu */ {IDGS_NO_HELP_AVAILABLE_FOR_S_HU, "Nincs s�g� a `%s' parancshoz\n"},
/* it */ {IDGS_NO_HELP_AVAILABLE_FOR_S_IT, "Nessun help disponibile per `%s'\n"},
/* nl */ {IDGS_NO_HELP_AVAILABLE_FOR_S_NL, "Geen help beschikbaar voor `%s'\n"},
/* pl */ {IDGS_NO_HELP_AVAILABLE_FOR_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_NO_HELP_AVAILABLE_FOR_S_SV, "Hj�lp saknas f�r \"%s\"\n"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_SYNTAX_S_S,    N_("\nSyntax: %s %s\n")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SYNTAX_S_S_DE, "\nSyntax: %s %s\n"},
/* fr */ {IDGS_SYNTAX_S_S_FR, ""},  /* fuzzy */
/* hu */ {IDGS_SYNTAX_S_S_HU, "\nSzintaxis: %s %s\n"},
/* it */ {IDGS_SYNTAX_S_S_IT, "\nSintassi: %s %s\n"},
/* nl */ {IDGS_SYNTAX_S_S_NL, "\nSyntax: %s %s\n"},
/* pl */ {IDGS_SYNTAX_S_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SYNTAX_S_S_SV, "\nSyntax: %s %s\n"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_ABBREVIATION_S,    N_("Abbreviation: %s\n")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ABBREVIATION_S_DE, "Abk�rzung: %s\n"},
/* fr */ {IDGS_ABBREVIATION_S_FR, ""},  /* fuzzy */
/* hu */ {IDGS_ABBREVIATION_S_HU, "R�vid�t�s: %s\n"},
/* it */ {IDGS_ABBREVIATION_S_IT, "Abbreviazione: %s\n"},
/* nl */ {IDGS_ABBREVIATION_S_NL, "Afkorting: %s\n"},
/* pl */ {IDGS_ABBREVIATION_S_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ABBREVIATION_S_SV, "F�rkortning: %s\n"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_AVAILABLE_COMMANDS_ARE,    N_("Available commands are:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_AVAILABLE_COMMANDS_ARE_DE, "Zul�ssige Kommandos sind:"},
/* fr */ {IDGS_AVAILABLE_COMMANDS_ARE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_AVAILABLE_COMMANDS_ARE_HU, "Lehets�ges parancsok:"},
/* it */ {IDGS_AVAILABLE_COMMANDS_ARE_IT, "I comandi disponibili sono:"},
/* nl */ {IDGS_AVAILABLE_COMMANDS_ARE_NL, "Beschikbare commandos zijn:"},
/* pl */ {IDGS_AVAILABLE_COMMANDS_ARE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_AVAILABLE_COMMANDS_ARE_SV, "Tillg�ngliga kommandon:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MACHINE_STATE_COMMANDS,    N_("Machine state commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MACHINE_STATE_COMMANDS_DE, "Maschinen Zustandskommandos:"},
/* fr */ {IDGS_MACHINE_STATE_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MACHINE_STATE_COMMANDS_HU, "G�p �llapot parancsok:"},
/* it */ {IDGS_MACHINE_STATE_COMMANDS_IT, "Comandi di stato del computer:"},
/* nl */ {IDGS_MACHINE_STATE_COMMANDS_NL, "Machine staat commandos:"},
/* pl */ {IDGS_MACHINE_STATE_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MACHINE_STATE_COMMANDS_SV, "Kommandon f�r maskinstatus:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MEMSPACE_R_BANKNAME,    N_("[<memspace>] [bankname]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MEMSPACE_R_BANKNAME_DE, "[<Speicherbereich>] [Speicherbank]"},
/* fr */ {IDGS_RP_MEMSPACE_R_BANKNAME_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MEMSPACE_R_BANKNAME_HU, "[<mem�ria t�r>] [lap n�v]"},
/* it */ {IDGS_RP_MEMSPACE_R_BANKNAME_IT, "[<spazio di memoria>] [nome banco]"},
/* nl */ {IDGS_RP_MEMSPACE_R_BANKNAME_NL, "[<geheugenruimte>] [banknaam]"},
/* pl */ {IDGS_RP_MEMSPACE_R_BANKNAME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MEMSPACE_R_BANKNAME_SV, "[<minnesomr�de>] [banknamn]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BANK_DESCRIPTION,    N_("If bankname is not given, print the possible banks for the memspace.\n"
                                           "If bankname is given set the current bank in the memspace to the given\n"
                                           "bank.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BANK_DESCRIPTION_DE, "Falls `bankname' nicht angegeben wird, Ausgabe der m�glichen B�nke in `memspace'.\n"
                                        "Falls `bankname' angegeben wird, setze aktuelle Bank in `memspace' mit angegebener Bank."},
/* fr */ {IDGS_MON_BANK_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BANK_DESCRIPTION_HU, "Ha a lap sz�ma nincs megadva, ki�rja a lehets�ges lapokat a mem�ria t�rb�l.\n"
                                        "Ha meg van adva, az aktu�lis lap sz�mot a mem�ria t�rben a megadott lapra �ll�tja."},
/* it */ {IDGS_MON_BANK_DESCRIPTION_IT, "Se non viene specificato il nome del banco, stampa tutti i banchi\n"
                                        "possibili per lo spazio di memoria.\n"
                                        "Se viene specificato il nome del banco, imposta il banco attuale nello\n"
                                        "spazio di memoria specificato."},
/* nl */ {IDGS_MON_BANK_DESCRIPTION_NL, "Wanneer de banknaam niet is opgegeven, toon de mogelijke banken voor de geheugen-\n"
                                        "ruimte. Wanneer de banknaam is opgegeven, zet de huidige bank in de geheugen-\n"
                                        "ruimte naar de opgegeven bank."},
/* pl */ {IDGS_MON_BANK_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BANK_DESCRIPTION_SV, "Om banknamn inte anges, skriv ut m�jliga bankar f�r minnesomr�det.\n"
                                        "Om banknamn anges, s�tt aktuell bank i minnesomr�det till den givna\n"
                                        "banken."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BACKTRACE_DESCRIPTION,    N_("Print JSR call chain (most recent call first). Stack offset\n"
                                                "relative to SP+1 is printed in parentheses. This is a best guess\n"
                                                "only.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BACKTRACE_DESCRIPTION_DE, "Zeige JSR Aufrufkette (letzter Aufruf zuerst). Stack offset\n"
                                             "wird relative zu SP+1 in Klammern gezeigt. Dies ist jedoch nur\n"
                                             "eine Sch�tzung."},
/* fr */ {IDGS_MON_BACKTRACE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BACKTRACE_DESCRIPTION_HU, "JSR h�v�s lista ki�r�sa (legut�bbi el�sz�r). A verem SP+1\n"
                                             "helyen l�v� tartalma z�r�jelben. Ez csak\n"
                                             "egy legjobb becsl�s."},
/* it */ {IDGS_MON_BACKTRACE_DESCRIPTION_IT, "Stampa la catena di chiamate JSR (la chiamata pi� recente per prima).\n"
                                             "L'offset dello stack relativo a SP+1 � stampato tra parentesi. Questo �\n"
                                             "solo l'ipotesi migliore."},
/* nl */ {IDGS_MON_BACKTRACE_DESCRIPTION_NL, "Toon JSR aanvraag ketting (meest recente aanvraag eerst). De stack offset\n"
                                             "relatief ten opzichte van SP+1 zal binnen aanhalingstekens worden getoond.\n"
                                             "Dit is alleen de beste schatting"},
/* pl */ {IDGS_MON_BACKTRACE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BACKTRACE_DESCRIPTION_SV, "Skriv JSR-anropskedja (senaste anrop f�rst). Stackavst�nd\n"
                                             "relativt till SP+1 visas inom parantes. Detta �r bara en\n"
                                             "gissning."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_CPU_DESCRIPTION,    N_("Specify the type of CPU currently used (6502/z80).")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_CPU_DESCRIPTION_DE, "CPU Typ setzen (6502/z80)"},
/* fr */ {IDGS_MON_CPU_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_CPU_DESCRIPTION_HU, "A processzor t�pus�nak megad�sa (6502/z80)."},
/* it */ {IDGS_MON_CPU_DESCRIPTION_IT, "Specifica il tipo di CPU utilizzata al momento (6502/z80)."},
/* nl */ {IDGS_MON_CPU_DESCRIPTION_NL, "Geef het soort van de huidige CPU (6502/z80)."},
/* pl */ {IDGS_MON_CPU_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_CPU_DESCRIPTION_SV, "Ange typ av CPU som anv�nds f�r n�rvarande (6502/z80)."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_COUNT,    N_("[<count>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_COUNT_DE, "[<Z�hler>]"},
/* fr */ {IDGS_RP_COUNT_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_COUNT_HU, "[<darab>]"},
/* it */ {IDGS_RP_COUNT_IT, "[<contatore>]"},
/* nl */ {IDGS_RP_COUNT_NL, "[<aantal>]"},
/* pl */ {IDGS_RP_COUNT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_COUNT_SV, "[<antal>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_CPUHISTORY_DESCRIPTION,    N_("Show <count> last executed commands.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_CPUHISTORY_DESCRIPTION_DE, "Zeige [<Z�hler>] zuletzt ausgef�hrte Kommandos"},
/* fr */ {IDGS_MON_CPUHISTORY_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_CPUHISTORY_DESCRIPTION_HU, "<darab> utolj�ra v�grehajtott utas�t�s mutat�sa."},
/* it */ {IDGS_MON_CPUHISTORY_DESCRIPTION_IT, "Mostra gli ultimi <contatore> comandi eseguiti."},
/* nl */ {IDGS_MON_CPUHISTORY_DESCRIPTION_NL, "Toon laatst <aantal> uitgevoerde commandos."},
/* pl */ {IDGS_MON_CPUHISTORY_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_CPUHISTORY_DESCRIPTION_SV, "Visa <antal> senast exekverade kommandon."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME,    N_("\"<filename>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_DE, "\"<Dateiname>\""},
/* fr */ {IDGS_QP_FILENAME_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_HU, "\"<f�jln�v>\""},
/* it */ {IDGS_QP_FILENAME_IT, "\"<nome del file>\""},
/* nl */ {IDGS_QP_FILENAME_NL, "\"<bestandsnaam>\""},
/* pl */ {IDGS_QP_FILENAME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_SV, "\"<filnamn>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DUMP_DESCRIPTION,    N_("Write a snapshot of the machine into the file specified.\n"
                                           "This snapshot is compatible with a snapshot written out by the UI.\n"
                                           "Note: No ROM images are included into the dump.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DUMP_DESCRIPTION_DE, "Schreibe eine Snapsho Datei der Maschine auf die angebene Datei.\n"
                                        "Dieser Snapshot ist kompatible zu einem Snapshot initiiert aus dem Men�.\n"
                                        "Hinweis: Die ROM Images werden im Snapshot nicht inkludiert."},
/* fr */ {IDGS_MON_DUMP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DUMP_DESCRIPTION_HU, "Pillanatk�p �r�sa a g�pr�l a megadott f�jlba. A pillanatk�p\n"
                                        "kompatibilis azzal, amelyet a grafikus fel�let hoz l�tre.\n"
                                        "Megjegyz�s: a ROM k�pm�sok nem ker�lnek bele."},
/* it */ {IDGS_MON_DUMP_DESCRIPTION_IT, "Scrive uno snapshot del computer sul file specificato.\n"
                                        "Questo snapshot � compatibile con uno snapshot scritto dall'IU.\n"
                                        "Nota: Le immagini ROM non sono incluse nel dump."},
/* nl */ {IDGS_MON_DUMP_DESCRIPTION_NL, "Schrijf een momentopname van de machine naar de opgegeven bestand.\n"
                                        "Deze momentopname is compatibel met een momentopname zoals geschreven van\n"
                                        "uit het menu.\n"
                                        "NB: ROM bestanden zijn niet opgenomen in de momentopname."},
/* pl */ {IDGS_MON_DUMP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DUMP_DESCRIPTION_SV, "Skriv en �gonblicksbild av maskinen till den angivna filen.\n"
                                        "�gonblicksbilden �r kompatibel med de som skapas fr�n menyn.\n"
                                        "Observera: ROM-avbildningar inkluderas inte i bilden."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_ADDRESS,    N_("<address>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_ADDRESS_DE, "<Adresse>"},
/* fr */ {IDGS_P_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_ADDRESS_HU, "<c�m>"},
/* it */ {IDGS_P_ADDRESS_IT, "<indirizzo>"},
/* nl */ {IDGS_P_ADDRESS_NL, "<Adres"},
/* pl */ {IDGS_P_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_ADDRESS_SV, "<adress>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_GOTO_DESCRIPTION,    N_("Change the PC to ADDRESS and continue execution")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_GOTO_DESCRIPTION_DE, "Setze PC auf ADRESSE und setze Ausf�hrung fort"},
/* fr */ {IDGS_MON_GOTO_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_GOTO_DESCRIPTION_HU, "A PC �ll�t�sa ADDRESS c�mre �s folytat�s"},
/* it */ {IDGS_MON_GOTO_DESCRIPTION_IT, "Cambia il PC in INDIRIZZO e continua l'esecuzione"},
/* nl */ {IDGS_MON_GOTO_DESCRIPTION_NL, "Zet de PC naar ADRES en ga door met de uitvoering van instructies."},
/* pl */ {IDGS_MON_GOTO_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_GOTO_DESCRIPTION_SV, "�ndra PC till ADRESS och forts�tt exekveringen"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_IO_DESCRIPTION,    N_("Print out the I/O area of the emulated machine.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_IO_DESCRIPTION_DE, "Drucke den I/O Bereich der emuliertern Maschine."},
/* fr */ {IDGS_MON_IO_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_IO_DESCRIPTION_HU, "Az emul�lt g�p I/O tartom�ny�nak mutat�sa."},
/* it */ {IDGS_MON_IO_DESCRIPTION_IT, "Stampa l'area di I/O del computer emulato."},
/* nl */ {IDGS_MON_IO_DESCRIPTION_NL, "Toon de I/O geheugenruimte van de huidige machine."},
/* pl */ {IDGS_MON_IO_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_IO_DESCRIPTION_SV, "Skriv I/O-omr�det f�r den emulerade maskinen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_NEXT_DESCRIPTION,    N_("Advance to the next instruction.  Subroutines are treated as\n"
                                           "a single instruction.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_NEXT_DESCRIPTION_DE, "Fahre bis zur n�chsten Instruktion fort.\n"
                                        "Unterroutinen werden wie eine einzelne Instruktion behandelt."},
/* fr */ {IDGS_MON_NEXT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_NEXT_DESCRIPTION_HU, "A k�vetkez� utas�t�sra l�p�s.  A szubrutinokat egy\n"
                                        "utas�t�snak tekinti."},
/* it */ {IDGS_MON_NEXT_DESCRIPTION_IT, "Passa alla prossima istruzione. Le subrutine sono trattate come\n"
                                        "una singola istruzione."},
/* nl */ {IDGS_MON_NEXT_DESCRIPTION_NL, "Ga naar de volgende instructie. Subroutines worden behandled als een\n"
                                        "enkele instructie."},
/* pl */ {IDGS_MON_NEXT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_NEXT_DESCRIPTION_SV, "G� till n�sta instruktion. Underrutiner hanteras som en\n"
                                        "enda instruktion."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_REG_NAME_P_NUMBER,    N_("[<reg_name> = <number> [, <reg_name> = <number>]*]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_REG_NAME_P_NUMBER_DE, "[<Register Name> = <Zahl> [, <Register Name> = <Zahl>]*]"},
/* fr */ {IDGS_RP_REG_NAME_P_NUMBER_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_REG_NAME_P_NUMBER_HU, "[<reg_n�v> = <sz�m> [, <reg_n�v> = <sz�m>]*]"},
/* it */ {IDGS_RP_REG_NAME_P_NUMBER_IT, "[<nome registro> = <numero> [, <nome registro> = <numero>]*]"},
/* nl */ {IDGS_RP_REG_NAME_P_NUMBER_NL, "[<Register_naam> = <nummer> [, <register_naam> = <nummer>]*]"},
/* pl */ {IDGS_RP_REG_NAME_P_NUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_REG_NAME_P_NUMBER_SV, "[<regnamn> = <tal> [, <regnamn> = <tal>]*]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_REGISTERS_DESCRIPTION,    N_("Assign respective registers.  With no parameters, display register\n"
                                                "values.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_REGISTERS_DESCRIPTION_DE, "Zuweisung auf entsprechende Register. Ohne Parameter, zeige Register Werte."},
/* fr */ {IDGS_MON_REGISTERS_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_REGISTERS_DESCRIPTION_HU, "Megadott regiszterek be�ll�t�sa. Param�terek n�lk�l, a regiszterek\n"
                                             "tartalm�nak ki�r�sa."},
/* it */ {IDGS_MON_REGISTERS_DESCRIPTION_IT, "Assegna i rispettivi registri. Senza parametri, mostra i valori dei\n"
                                             "registri."},
/* nl */ {IDGS_MON_REGISTERS_DESCRIPTION_NL, "Vul opgegeven registers met de toebehorende waarde. Zonder parameter, toon\n"
                                             "de huidige waarden in de registers."},
/* pl */ {IDGS_MON_REGISTERS_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_REGISTERS_DESCRIPTION_SV, "Tilldela respektive register. Utan parametrar visas inneh�llet\n"
                                             "i registren."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_TYPE,    N_("[<type>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_TYPE_DE, "[<Typ>]"},
/* fr */ {IDGS_RP_TYPE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_TYPE_HU, "[<t�pus>]"},
/* it */ {IDGS_RP_TYPE_IT, "[<typo>]"},
/* nl */ {IDGS_RP_TYPE_NL, "[<soort>]"},
/* pl */ {IDGS_RP_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_TYPE_SV, "[<typ>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RESET_DESCRIPTION,    N_("Reset the machine or drive. Type: 0 = soft, 1 = hard, 8-11 = drive.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RESET_DESCRIPTION_DE, "Reset der Maschine oder des Laufwerks. Typ: 0 = soft, 1 = hart, 8-11 = Laufwerk."},
/* fr */ {IDGS_MON_RESET_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RESET_DESCRIPTION_HU, "A sz�m�t�g�p vagy meghajt� �jraind�t�sa. T�pus: 0 = meleg, 1 = hideg, 8-11 = meghajt�."},
/* it */ {IDGS_MON_RESET_DESCRIPTION_IT, "Resetta il computer o il drive. Tipo: 0 = soft, 1 = hard, 8-11 = drive."},
/* nl */ {IDGS_MON_RESET_DESCRIPTION_NL, "Reset de machine of drive. Soort: 0 = zacht, 1 = hard, 8-11 = drive."},
/* pl */ {IDGS_MON_RESET_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RESET_DESCRIPTION_SV, "�terst�ll maskinen eller diskettstationen. Typ: 0 = mjuk, 1 = h�rd,\n"
                                         "8-11 = diskettstation."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RETURN_DESCRIPTION,    N_("Continues execution and returns to the monitor just before the next\n"
                                             "RTS or RTI is executed.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RETURN_DESCRIPTION_DE, "Setzt Ausf�hrung fort und retourniert bevor der n�chsten RTS oder RTI instruction."},
/* fr */ {IDGS_MON_RETURN_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RETURN_DESCRIPTION_HU, "Folytatja a v�grehajt�st �s visszat�r a monitorba a k�vetkez�\n"
                                          "RTS vagy RTI utas�t�s el�tt."},
/* it */ {IDGS_MON_RETURN_DESCRIPTION_IT, "Continua l'esecuzione e ritorna al monitor subito prima della prossima\n"
                                          "esecuzione di RTS o RTI."},
/* nl */ {IDGS_MON_RETURN_DESCRIPTION_NL, "Ga door met uitvoeren van instructies en kom terug naar de monitor net voor de\n"
                                          "volgende RTS of RTI wordt uitgevoerd."},
/* pl */ {IDGS_MON_RETURN_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RETURN_DESCRIPTION_SV, "Forts�tt exekveringen och �terg� till monitorn precis innan n�sta\n"
                                          "RTS eller RTI exekveras."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SCREEN_DESCRIPTION,    N_("Displays the contents of the screen.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SCREEN_DESCRIPTION_DE, "Zeigt Bildschirminhalt."},
/* fr */ {IDGS_MON_SCREEN_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SCREEN_DESCRIPTION_HU, "A k�perny� tartalm�nak mutat�sa."},
/* it */ {IDGS_MON_SCREEN_DESCRIPTION_IT, "Mostra il contenuto dello schermo."},
/* nl */ {IDGS_MON_SCREEN_DESCRIPTION_NL, "Toont de inhoud van het scherm."},
/* pl */ {IDGS_MON_SCREEN_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SCREEN_DESCRIPTION_SV, "Visar sk�rmens inneh�ll."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_STEP_DESCRIPTION,    N_("Single-step through instructions.  COUNT allows stepping\n"
                                           "more than a single instruction at a time.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_STEP_DESCRIPTION_DE, "Einzelschritt Ausf�hrung durch Instruktionen. COUNT erlaubt die Ausf�hrung\n"
                                        "mehrerer Instruktionen auf einmal."},
/* fr */ {IDGS_MON_STEP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_STEP_DESCRIPTION_HU, "L�pked�s egyes�vel az utas�t�sokon.  A DARAB-bal be�ll�that�,\n"
                                        "ha t�bb utas�t�st kell ugrani egyszerre."},
/* it */ {IDGS_MON_STEP_DESCRIPTION_IT, "Modalit� single step attraverso le istruzioni. Il CONTATORE permette\n"
                                        "di attraversare pi� di una singola istruzione per volta."},
/* nl */ {IDGS_MON_STEP_DESCRIPTION_NL, "Enkel-stap door de instructies. AANTAL zorgt ervoor dat er meerdere\n"
                                        "instructies worden uitgevoerd in een keer."},
/* pl */ {IDGS_MON_STEP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_STEP_DESCRIPTION_SV, "Stega genom instruktioner. ANTAL g�r det m�jligt att stega\n"
                                        "genom mer �n en enkel instruktion i varje steg."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_UNDUMP_DESCRIPTION,    N_("Read a snapshot of the machine from the file specified.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_UNDUMP_DESCRIPTION_DE, "Lese einen Snapshot der Maschine aus der angegebenen Datei."},
/* fr */ {IDGS_MON_UNDUMP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_UNDUMP_DESCRIPTION_HU, "A pillanatk�p bet�lt�se a megadott f�jlb�l."},
/* it */ {IDGS_MON_UNDUMP_DESCRIPTION_IT, "Legge lo snapshot del computer dal file specificato."},
/* nl */ {IDGS_MON_UNDUMP_DESCRIPTION_NL, "Lees een momentopname voor de machine van het opgegeven bestand."},
/* pl */ {IDGS_MON_UNDUMP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_UNDUMP_DESCRIPTION_SV, "L�s en �gonblicksbild f�r maskinen fr�n den angivna filen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_SYMBOL_TABLE_COMMANDS,    N_("Symbol table commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_SYMBOL_TABLE_COMMANDS_DE, "Symbol Tabellen Kommandos:"},
/* fr */ {IDGS_SYMBOL_TABLE_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_SYMBOL_TABLE_COMMANDS_HU, "Szimb�lum t�bl�zat parancsok:"},
/* it */ {IDGS_SYMBOL_TABLE_COMMANDS_IT, "Comandi tabella dei simboli:"},
/* nl */ {IDGS_SYMBOL_TABLE_COMMANDS_NL, "Symbool tabel commandos:"},
/* pl */ {IDGS_SYMBOL_TABLE_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_SYMBOL_TABLE_COMMANDS_SV, "Kommandon f�r symboltabell:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL,    N_("[<memspace>] <address> <label>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_DE, "[<Speicherbereich>] <Adresse> <Bezeichner>"},
/* fr */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_HU, "[<mem�ria t�r>] <c�m> <c�mke>"},
/* it */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_IT, "[<spazio di memoria>] <indirizzo> <etichetta>"},
/* nl */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_NL, "[<geheugenruimte>] <adres> <label>"},
/* pl */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MEMSPACE_P_ADDRESS_P_LABEL_SV, "[<minnesomr�de>] <adress> <etikett>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_ADD_LABEL_DESCRIPTION,    N_("<memspace> is one of: C: 8: 9: 10: 11:\n"
                                                "<address>  is the address which should get the label.\n"
                                                "<label>    is the name of the label; it must start with a dot (\".\").\n\n"
                                                "Map a given address to a label.  This label can be used when entering\n"
                                                "assembly code and is shown during disassembly.  Additionally, it can\n"
                                                "be used whenever an address must be specified.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_ADD_LABEL_DESCRIPTION_DE, "<Speicherbereich> ist aus: C: 8: 9: 10: 11:\n"
                                             "<Adresse>         bezeichnet die Adresse welche mit dem Bezeichner assoziert werden soll.\n"
                                             "<Bezeichner>      Name des Bezeichners; Mu� mit einem Punkt (\".\") starten.\n"
                                             "\n"
                                             "Assoziiert einen Bezeichner mit einer Adresse. Dieser Bezeichner kann \n"
                                             "bei der Eingabe von Asssmblercode benutzt werden und wird auch beim\n"
                                             "Disassemblieren gezeigt. Dar�ber hinaus kann der Bezeichner �berall\n"
                                             "dort genutzt werden, wo eine Adresseingabe n�tig ist."},
/* fr */ {IDGS_MON_ADD_LABEL_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_ADD_LABEL_DESCRIPTION_HU, "A <mem�ria t�r> ezek k�z�l: C: 8: 9: 10: 11:\n"
                                             "<c�m>  amelyik c�mhez a c�mk�t rendelje.\n"
                                             "<c�mke>    a c�mke neve; ponttal kell kezd�dj�n (\".\").\n"
                                             "\n"
                                             "Az adott c�m rendel�se a c�mk�hez.  Ez a c�mke haszn�lhat� assembly\n"
                                             "program be�r�sakor, �s visszafejt�skor megjelenik.  Mindig haszn�lhat�,\n"
                                             "amikor valamilyen c�met kell megadni."},
/* it */ {IDGS_MON_ADD_LABEL_DESCRIPTION_IT, "<spazio di memoria> � uno tra: C: 8: 9: 10: 11:\n"
                                             "<indirizzo> � l'indirizzo a cui deve essere associata l'etichettal.\n"
                                             "<etichetta> � il nome dell'etichetta; deve iniziare con un punto (\".\").\n"
                                             "\n"
                                             "Associa l'indirizzo specificato all'etichetta. Questa etichetta pu�\n"
                                             "essere usata quando si inserisce codice assembly ed � mostrata\n"
                                             "nella disassemblazione. In aggiunta, pu� essere usata ogni volta che\n"
                                             "deve essere specificato un indirizzo."},
/* nl */ {IDGS_MON_ADD_LABEL_DESCRIPTION_NL, "<geheugenruimte> is een van: C: 8: 9: 10: 11:\n"
                                             "<adres>          is het adres waaraan de label gekoppelt moet worden.\n"
                                             "<label>          is de naam van de label; de naam moet beginnen met een punt\n"
                                             "                 (\".\").\n\n"
                                             "Koppel een opgegeven adres aan een label. Dit label kan worden gebruikt bij\n"
                                             "het invoeren van assembleer code en wordt getoont bij een disassemblering.\n"
                                             "Tevens kan de label gebruikt worden in plaats van het adres."},
/* pl */ {IDGS_MON_ADD_LABEL_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_ADD_LABEL_DESCRIPTION_SV, "<minnesomr�de> �r en av: C: 8: 9: 10: 11:\n"
                                             "<adress>       �r adressen som ska f� etiketten.\n"
                                             "<etikett>      �r namnet p� etiketten; m�ste starta med punkt (\".\").\n"
                                             "\n"
                                             "Koppla en adress till en etikett. Etiketten kan anges n�r assemblerkod\n"
                                             "matas in och visas vid disassemblering. I till�gg kan den anv�ndas\n"
                                             "n�rhelst en adress m�ste anges."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MEMSPACE_P_LABEL,    N_("[<memspace>] <label>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MEMSPACE_P_LABEL_DE, "[<Speicherbereich>] <Bezeichner>"},
/* fr */ {IDGS_RP_MEMSPACE_P_LABEL_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MEMSPACE_P_LABEL_HU, "[<mem�ria tartom�ny>] <c�mke>"},
/* it */ {IDGS_RP_MEMSPACE_P_LABEL_IT, "[<spazio di memoria>] <etichetta>"},
/* nl */ {IDGS_RP_MEMSPACE_P_LABEL_NL, "[<geheugenruimte>] <label>"},
/* pl */ {IDGS_RP_MEMSPACE_P_LABEL_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MEMSPACE_P_LABEL_SV, "[<minnesomr�de>] <etikett>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DELETE_LABEL_DESCRIPTION,    N_("<memspace> is one of: C: 8: 9: 10: 11:\n"
                                                   "<label>    is the name of the label; it must start with a dot (\".\").\n\n"
                                                   "Delete a previously defined label.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_DE, "<Speicherbereich> ist aus: C: 8: 9: 10: 11:\n"
                                                "<Bezeichner>      Name des Bezeichners; Mu� mit einem Punkt (\\\".\\\") starten.\n"
                                                "\n"
                                                "L�scht einen zuvor definierten Bezeichner."},
/* fr */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_HU, "<mem�ria t�r> egy ezek k�z�l: C: 8: 9: 10: 11:\n"
                                                "<c�mke>    a c�mke neve; ponttal kell kezd�dj�n (\".\").\n"
                                                "\n"
                                                "El�z�leg megadott c�mke t�rl�se."},
/* it */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_IT, "<spazio di memoria> � uno tra: C: 8: 9: 10: 11:\n"
                                                "<etichetta> � il nome dell'etichetta; deve iniziare con un punto (\".\").\n"
                                                "\n"
                                                "Cancella una etichetta definita precedentemente."},
/* nl */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_NL, "<geheugenruimte> is een van: C: 8: 9: 10: 11:\n"
                                                "<label>          is de naam van de label; de naam moet beginnen met een\n"
                                                "punt (\".\").\n\n"
                                                "Verwijder een gekoppelde label."},
/* pl */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DELETE_LABEL_DESCRIPTION_SV, "<minnesomr�de> �r en av: C: 8: 9: 10: 11:\n"
                                                "<etikett>      �r namnet p� etiketten; m�ste starta med punkt (\".\").\n"
                                                "\n"
                                                "Ta bort en tidigare angiven etikett."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MEMSPACE_QP_FILENAME,    N_("[<memspace>] \"<filename>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MEMSPACE_QP_FILENAME_DE, "[<Speicherbereich>] \"<Dateiname>\""},
/* fr */ {IDGS_RP_MEMSPACE_QP_FILENAME_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MEMSPACE_QP_FILENAME_HU, "[<mem�ria tartom�ny>] \"<f�jln�v>\""},
/* it */ {IDGS_RP_MEMSPACE_QP_FILENAME_IT, "[<spazio di memoria>] \"<nome file>\""},
/* nl */ {IDGS_RP_MEMSPACE_QP_FILENAME_NL, "[<geheugenruimte>] \"<bestandsnaam>\""},
/* pl */ {IDGS_RP_MEMSPACE_QP_FILENAME_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MEMSPACE_QP_FILENAME_SV, "[<minnesomr�de>] \"<filnamn>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_LOAD_LABELS_DESCRIPTION,    N_("Load a file containing a mapping of labels to addresses.  If no memory\n"
                                                  "space is specified, the default readspace is used.\n\n" 
                                                  "The format of the file is the one written out by the `save_labels' command;\n" 
                                                  "it consists of some `add_label' commands, written one after the other.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_DE, "Laded eine Datei mit Zuordnung von Adressen zu Bezeichnern. Wenn kein \n"
                                               "Speicherbereich angegeben wurde, wird der Standard Speicherplatz\n"
                                               "verwendet.  Das Format der Datei wird durch das Kommande `save_lables'\n"
                                               "definiert.  Es besteht aus eine Reihe von `add_label' Kommandos,\n"
                                               "hintereinander angeordnet."},
/* fr */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_HU, "C�mk�kr�l c�mekre lek�pez�seket tartalmaz� f�jl bet�lt�se.  Ha nincs mem�ria\n"
                                               "t�r megadva, az alap t�r lesz haszn�lva.\n"
                                               "\n"
                                               "A f�jl form�tuma megegyezik a `save_labels' parancs �ltal ki�rt�val;\n"
                                               "n�h�ny egym�s ut�n �rt `add_label' parancsb�l �ll."},
/* it */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_IT, "Carica un file che contiente l'associazione tra le etichette e gli indirizzi.\n"
                                               "Se non � specificato alcuno spazio di memoria, viene usato lo spazio\n"
                                               "di lettura predefinito.\n"
                                               "\n"
                                               "Il formato del file � quello dato in output dal comando `save_labels';\n"
                                               "� composto da alcuni comandi `add_label', scritti uno di seguito all'altro."},
/* nl */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_NL, "Laad een bestand met de koppelingen tussen labels en adressen. Als er geen\n"
                                               "geheugenruimte is opgegeven zal de standaard geheugenruimte worden gebruikt.\n"
                                               "Het formaat van het bestand is die door het commando 'save_labels' wordt\n"
                                               "geschreven; het bestaat uit een aantal 'add_label' commandos na elkaar."},
/* pl */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_LOAD_LABELS_DESCRIPTION_SV, "L�s in en fil som inneh�ller koppling fr�n etiketter till adresser.\n"
                                               "Om inget minnesomr�de anges anv�ndas standard-l�sadressen.\n"
                                               "\n"
                                               "Formatet p� filen �r samma som skrivs av \"save_labels\"-kommandot;\n"
                                               "det best�r av n�gra \"add_label\"-kommandon efter varandra."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SAVE_LABELS_DESCRIPTION,    N_("Save labels to a file.  If no memory space is specified, all of the\n"
                                                  "labels are saved.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_DE, "Speichere Bezeichner in eine Datei. Wenn kein Speicherbereich\n"
                                               "angegeben wurde, werden alle Bezeichner gespeichert."},
/* fr */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_HU, "C�mk�k ment�se f�jlba.  Ha nincs mem�ria tartom�ny megadva, az\n"
                                               "�sszes c�mk�t menti."},
/* it */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_IT, "Salva le etichette su file. Se non � specificato alcuno spazio di memoria,\n"
                                               "vengono salvate tutte le etichette."},
/* nl */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_NL, "Sla de labels op in een bestand. Als er geen geheugenruimte is opgegeven\n"
                                               "worden alle labels opgeslagen."},
/* pl */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SAVE_LABELS_DESCRIPTION_SV, "Spara etiketter till en fil. Om inget minnesomr�de anges sparas alla\n"
                                               "etiketterna."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MEMSPACE,    N_("[<memspace>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MEMSPACE_DE, "[<Speicherbereich>]"},
/* fr */ {IDGS_RP_MEMSPACE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MEMSPACE_HU, "[<mem�ria tartom�ny>]"},
/* it */ {IDGS_RP_MEMSPACE_IT, "[<spazio di memoria>]"},
/* nl */ {IDGS_RP_MEMSPACE_NL, "[<geheugenruimte>]"},
/* pl */ {IDGS_RP_MEMSPACE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MEMSPACE_SV, "[<minnesomr�de>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SHOW_LABELS_DESCRIPTION,    N_("Display current label mappings.  If no memory space is specified, show\n"
                                                  "all labels.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_DE, "Zeige aktuelle Bezeichner Zuordnungen. Wenn kein Speicherbereich angegeben\n"
                                               "wurde, werden alle Bezeichner gezeigt."},
/* fr */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_HU, "Az aktu�lis c�mke lek�pez�sek mutat�sa.  Ha nincs a mem�ria t�r megadva, az\n"
                                               "�sszeset mutatja."},
/* it */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_IT, "Mostra l'associazione delle etichette correnti. Se non � specificato alcun\n"
                                               "spazio di memoria, verranno mostrate tutte le etichette."},
/* nl */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_NL, "Toon huidige label koppelingen. Als er geen geheugenruimte is opgegeven\n"
                                               "worden all labels getoont."},
/* pl */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SHOW_LABELS_DESCRIPTION_SV, "Visar aktuella etiketter. Om inget minnesomr�de anges visas alla\n"
                                               "etiketter."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS,    N_("Assembler and memory commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_DE, "Assembler und Speicher Kommandos:"},
/* fr */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_HU, "Assembler �s mem�ria parancsok:"},
/* it */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_IT, "Comandi assembler e memoria:"},
/* nl */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_NL, "Assembler en geheugen commandos:"},
/* pl */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_ASSEMBLER_AND_MEMORY_COMMANDS_SV, "Assembler- och minneskommandon:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_ADDRESS_P_DATA_LIST,    N_("[<address>] <data_list>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_ADDRESS_P_DATA_LIST_DE, "[<Adresse>] <Datenliste>"},
/* fr */ {IDGS_RP_ADDRESS_P_DATA_LIST_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_ADDRESS_P_DATA_LIST_HU, "[<c�m>] <adat_lista>"},
/* it */ {IDGS_RP_ADDRESS_P_DATA_LIST_IT, "[<indirizzo>] <lista di dati>"},
/* nl */ {IDGS_RP_ADDRESS_P_DATA_LIST_NL, "[<adres>] <data_lijst>"},
/* pl */ {IDGS_RP_ADDRESS_P_DATA_LIST_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_ADDRESS_P_DATA_LIST_SV, "[<adress>] <datalista>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_WRITE_DESCRIPTION,    N_("Write the specified data at `address'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_WRITE_DESCRIPTION_DE, "Schreibe das angegebene Datum auf `Adresse'."},
/* fr */ {IDGS_MON_WRITE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_WRITE_DESCRIPTION_HU, "A megadott adat `c�m' helyre �r�sa."},
/* it */ {IDGS_MON_WRITE_DESCRIPTION_IT, "Scrive il dato specificato all'indirizzo."},
/* nl */ {IDGS_MON_WRITE_DESCRIPTION_NL, "Schrijf de opgegevem data op `adres'."},
/* pl */ {IDGS_MON_WRITE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_WRITE_DESCRIPTION_SV, "Skriv angiven data p� \"adress\"."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_ADDRESS_RP_INSTRUCTION,    N_("<address> [ <instruction> [: <instruction>]* ]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_ADDRESS_RP_INSTRUCTION_DE, "<Adresse> [ <Instruktion> [: <Instruktion>]* ]"},
/* fr */ {IDGS_P_ADDRESS_RP_INSTRUCTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_ADDRESS_RP_INSTRUCTION_HU, "<c�m> [ <utas�t�s> [: <utas�t�s>]* ]"},
/* it */ {IDGS_P_ADDRESS_RP_INSTRUCTION_IT, "<indirizzo> [ <istruzione> [: <istruzione>]* ]"},
/* nl */ {IDGS_P_ADDRESS_RP_INSTRUCTION_NL, "<adres> [ <instructie> [: <instructie>]* ]"},
/* pl */ {IDGS_P_ADDRESS_RP_INSTRUCTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_ADDRESS_RP_INSTRUCTION_SV, "<adress> [ <instruktion> [: <instruktion>]* ]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_ASSEMBLE_DESCRIPTION,    N_("Assemble instructions to the specified address.  If only one\n"
                                               "instruction is specified, enter assembly mode (enter an empty line to\n"
                                               "exit assembly mode).")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_ASSEMBLE_DESCRIPTION_DE, "Assmbler Instruktionen zur angebenen Adresse. Wenn nur eine Instruktion\n"
                                            "angegeben wurde, wird der assemblier Modus aktiviert (Eingabe einer leeren\n"
                                            "Zeile beendet den assemblier Modus)."},
/* fr */ {IDGS_MON_ASSEMBLE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_ASSEMBLE_DESCRIPTION_HU, "Utas�t�sok ford�t�sa a megadott c�mre. Ha csak egy utas�t�s\n"
                                            "van, ford�t� m�dba l�p (�res sort kell be�rni a kil�p�shez)."},
/* it */ {IDGS_MON_ASSEMBLE_DESCRIPTION_IT, "Assembla le istruzioni a partire dall'indirizzo specificato. Se solo una\n"
                                            "istruzione � specificata, entra in modalit� assembly (inserire una\n"
                                            "linea vuota per uscire da questa modalit�)."},
/* nl */ {IDGS_MON_ASSEMBLE_DESCRIPTION_NL, "Assembleer instructies naar het opgegeven adres. Als er alleen een instructie\n"
                                            "is opgegeven dan wordt de assembleer modus ingeschakelt (geef een lege regel\n"
                                            "in om de assembleer modus te verlaten)."},
/* pl */ {IDGS_MON_ASSEMBLE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_ASSEMBLE_DESCRIPTION_SV, "Assemblera instruktioner till angiven adress. Om endast en\n"
                                            "instruktion angavs, aktivera assemblerl�ge (mata in en tom rad\n"
                                            "f�r att avsluta assemblerl�get)."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS,    N_("<address_range> <address>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_DE, "<Adressbereich> <Adresse>"},
/* fr */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_HU, "<c�mtartom�ny> <c�m>"},
/* it */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_IT, "<intervallo indirizzi> <indirizzo>"},
/* nl */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_NL, "<adres_reeks> <adres>"},
/* pl */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_ADDRESS_RANGE_P_ADDRESS_SV, "<adressomr�de> <adress>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_COMPARE_DESCRIPTION,    N_("Compare memory from the source specified by the address range to the\n"
                                              "destination specified by the address.  The regions may overlap.  Any\n"
                                              "values that miscompare are displayed using the default displaytype.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_COMPARE_DESCRIPTION_DE, "Vergleiche angegebenen Adressbereich mit der Zieladresse.\n"
                                           "Die Bereiche k�nnen �berlappen. Ungleiche Werte werden mit dem\n"
                                           "Standardanzeigetyp gezeigt."},
/* fr */ {IDGS_MON_COMPARE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_COMPARE_DESCRIPTION_HU, "A forr�s mem�riatartom�ny �sszehasonl�t�sa a c�llal megadott\n"
                                           "mem�riatartom�nnyal. A tartom�nyok �tfed�se megengedett.  Minden\n"
                                           "meg nem egyez� �rt�ket ki�r az alap adat t�pussal."},
/* it */ {IDGS_MON_COMPARE_DESCRIPTION_IT, "Confronta la memoria della sorgente specificata dall'intervallo di\n"
                                           "indirizzi con la destinazione specificata dall'indirizzo. Le regioni possono\n"
                                           "sovrapporsi. Qualsiasi valore che non corrisponde � visualizzato\n"
                                           "utilizzando il tipo di dato predefinito."},
/* nl */ {IDGS_MON_COMPARE_DESCRIPTION_NL, "Vergelijk geheugen van de bron opgegeven door de adres reeks met het doel\n"
                                           "opgegeven door het adres. De reeksen mogen overlappen. De data die niet\n"
                                           "gelijk is zal worden getoont volgens de huidige toon soort."},
/* pl */ {IDGS_MON_COMPARE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_COMPARE_DESCRIPTION_SV, "J�mf�r minne fr�n k�llan som anges av adressomr�det mot destinationen\n"
                                           "som anges av adressen. Omr�dena kan vara �verlappande. Alla v�rden\n"
                                           "som skiljer sig vid j�mf�relsen visas med standardvisningss�ttet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_CHECKNUM,    N_("<checknum>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_CHECKNUM_DE, "<Checknummer>"},
/* fr */ {IDGS_P_CHECKNUM_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_CHECKNUM_HU, "<ell_pont_sz�m>"},
/* it */ {IDGS_P_CHECKNUM_IT, "<checknum>"},
/* nl */ {IDGS_P_CHECKNUM_NL, "<checknum>"},
/* pl */ {IDGS_P_CHECKNUM_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_CHECKNUM_SV, "<kontrollnr>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DELETE_DESCRIPTION,    N_("Delete checkpoint `checknum'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DELETE_DESCRIPTION_DE, "L�sche Checkpoint `Checknummer'."},
/* fr */ {IDGS_MON_DELETE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DELETE_DESCRIPTION_HU, "`ell_pont_sz�m' ellen�rz�si pont t�rl�se."},
/* it */ {IDGS_MON_DELETE_DESCRIPTION_IT, "Cancella il punto di controllo `checknum'."},
/* nl */ {IDGS_MON_DELETE_DESCRIPTION_NL, "Verwijder checkpunt `checknum'."},
/* pl */ {IDGS_MON_DELETE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DELETE_DESCRIPTION_SV, "Ta bort kontrollpunkt \"kontrollnr\"."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_ADDRESS_RP_ADDRESS,    N_("[<address> [<address>]]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_ADDRESS_RP_ADDRESS_DE, "[<Adresse> [<Adresse>]]"},
/* fr */ {IDGS_RP_ADDRESS_RP_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_ADDRESS_RP_ADDRESS_HU, "[<c�m> [<c�m>]]"},
/* it */ {IDGS_RP_ADDRESS_RP_ADDRESS_IT, "[<indirizzo> [<indirizzo>]]"},
/* nl */ {IDGS_RP_ADDRESS_RP_ADDRESS_NL, "[<adres> [<adres>]]"},
/* pl */ {IDGS_RP_ADDRESS_RP_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_ADDRESS_RP_ADDRESS_SV, "[<adress> [<adress>]]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DISASS_DESCRIPTION,    N_("Disassemble instructions.  If two addresses are specified, they are\n"
                                             "used as a start and end address.  If only one is specified, it is\n"
                                             "treated as the start address and a default number of instructions are\n"
                                             "disassembled.  If no addresses are specified, a default number of\n"
                                             "instructions are disassembled from the dot address.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DISASS_DESCRIPTION_DE, "Disassembliere Instruktionen. Wenn zwei Adressen angegeben werden,\n"
                                          "werden diese als Start- und Zieladresse interpretiert. Wenn nur eine\n"
                                          "Adress angegeben wird, wird diese asl Startadresse interpretiert und\n"
                                          "die Standardanzahl an Instruktionen werden disassembliert. Wenn keine\n"
                                          "Adresse angegeben wird, wird von der aktuellen Adresse disassembliert."},
/* fr */ {IDGS_MON_DISASS_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DISASS_DESCRIPTION_HU, "Utas�t�sok visszafejt�se. Ha k�t c�m van megadva, els� �s utols�\n"
                                          "c�mk�nt �rtelmezi �ket. Ha csak egy, akkor els� c�mk�nt, �s az alap\n"
                                          "sz�m� utas�t�s ker�l visszafejt�sre. Ha nincs c�m megadva, az aktu�lis\n"
                                          "c�mt�l kezdi a visszafejt�st, �s a szok�sos sz�m� utas�t�st �r ki."},
/* it */ {IDGS_MON_DISASS_DESCRIPTION_IT, "Disassembla le istruzioni. Se sono specificati due indirizzi, questi\n"
                                          "sono utilizzati come indirizzo di inizio e fine. Se ne viene specificato\n"
                                          "solo uno, � usato come indirizzo di partenza e viene disassemblato\n"
                                          "il numero di istruzioni predefinito. Se non viene specificato nessun\n"
                                          "indirizzo, viene disassemblato il numero di istruzioni predefinito\n"
                                          "dall'indirizzo dot."},
/* nl */ {IDGS_MON_DISASS_DESCRIPTION_NL, "Disassembleer instructies. Als er twee adressen zijn opgegeven dan worden\n"
                                          "deze gebruikt als een start en eind adres. Als er maar een adres is\n"
                                          "opgegeven dan wordt deze gebruikt als het start adres en een standaard\n"
                                          "aantal instructies zal worden gedisassembleerd. Als er geen adressen zijn\n"
                                          "opgegeven dan wordt een standaard aantal instructies gedisassembleerd vanaf\n"
                                          "het huidige adres."},
/* pl */ {IDGS_MON_DISASS_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DISASS_DESCRIPTION_SV, "Disassemblera instruktioner. Om tv� adresser anges anv�nds de som\n"
                                          "start- och slutadress. Om endast en anges anv�nds den som startadress\n"
                                          "och ett standardantal instruktioner disassembleras. Om ingen adress\n"
                                          "anges disassembleras ett standardantal instruktioner fr�n punktadressen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST,    N_("<address_range> <data_list>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_DE, "<Adressbereich> <Datenliste>"},
/* fr */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_HU, "<c�m_tartom�ny> <adat_lista>"},
/* it */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_IT, "<intervallo di indirizzi> <lista di dati>"},
/* nl */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_NL, "<adres_reeks> <data_lijst>"},
/* pl */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_ADDRESS_RANGE_P_DATA_LIST_SV, "<adressomr�de> <datalista>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_FILL_DESCRIPTION,    N_("Fill memory in the specified address range with the data in\n"
                                           "<data_list>.  If the size of the address range is greater than the size\n"
                                           "of the data_list, the data_list is repeated.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_FILL_DESCRIPTION_DE, "F�lle den angegebenen Adressbereich mit den Daten aus <Datenliste>.\n"
                                        "Wenn die Gr��e des Adressbereichs gr��er als <Datenliste> ist, werden\n"
                                        "die Daten wiederholt."},
/* fr */ {IDGS_MON_FILL_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_FILL_DESCRIPTION_HU, "A mem�ria kit�lt�se az <adat_lista> param�terben megadott adatokkal.\n"
                                        "Ha a c�mtartom�ny nagyobb, mint a lista, ism�telni fogja azt."},
/* it */ {IDGS_MON_FILL_DESCRIPTION_IT, "Riempe la memoria nell'intervallo di indirizzi specificato con i dati\n"
                                        "presenti in <lista di dati>. Se la dimensione dell'intervallo � maggiore\n"
                                        "della dimensione della lista di dati, la lista di dati � ripetuta."},
/* nl */ {IDGS_MON_FILL_DESCRIPTION_NL, "Vul het geheugen van de opgegeven adres reeks op met de data in <data_lijst>.\n"
                                        "Als de grootte van de adres reeks groter is dan de grootte van de data_lijst\n"
                                        "dan zal de data_lijst worder herhaald."},
/* pl */ {IDGS_MON_FILL_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_FILL_DESCRIPTION_SV, "Fyll minnet i det angivna omr�det med data i <datalista>.\n"
                                        "Om storleken p� adressomr�det �r st�rre �n storleken p� datalistan\n"
                                        "repeteras inneh�llet i datalistan."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_HUNT_DESCRIPTION,    N_("Hunt memory in the specified address range for the data in\n"
                                           "<data_list>.  If the data is found, the starting address of the match\n"
                                           "is displayed.  The entire range is searched for all possible matches.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_HUNT_DESCRIPTION_DE, "Suche in angebenen Adressbereich nach <Datenliste>. Wenn die Daten gefunden\n"
                                        "wurden, wird die Startadresse gezeigt. Der komplette Bereich wird alle\n"
                                        "Vorkommnisse von <Datenliste> untersucht."},
/* fr */ {IDGS_MON_HUNT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_HUNT_DESCRIPTION_HU, "Az <adat_lista> adat keres�se a mem�ri�ban. Ha megtal�lja a sorozatot,\n"
                                        "a kezd�c�m�t ki�rja. Az eg�sz tartom�nyt v�gign�zi az �sszes lehets�ges\n"
                                        "tal�lat felfedez�s�hez."},
/* it */ {IDGS_MON_HUNT_DESCRIPTION_IT, "Cerca nell'intervallo di indirizzi specificato i dati nella <lista di dati>.\n"
                                        "Se il dato � trovato, viene visualizzato l'indirizzo iniziale della\n"
                                        "corrispondenza. Tutto l'intervallo viene ricercato per tutte le possibili\n"
                                        "corrispondenze."},
/* nl */ {IDGS_MON_HUNT_DESCRIPTION_NL, "Zoek het adres bereik voor de data in <data_lijst>. Als de data is gevonden\n"
                                        "dan wordt het start adres getoont. Er zal door de hele reeks worden gezocht."},
/* pl */ {IDGS_MON_HUNT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_HUNT_DESCRIPTION_SV, "S�k i minnet i det angivna omr�det med data efter <datalista>.\n"
                                        "Om data p�tr�ffas visas startadressen f�r tr�ffen. Hela\n"
                                        "omr�det genoms�ks f�r att hitta alla m�jliga tr�ffar."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_ADDRESS_OPT_RANGE,    N_("<address_opt_range>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_ADDRESS_OPT_RANGE_DE, "<Optionaler Adressbereich>"},
/* fr */ {IDGS_P_ADDRESS_OPT_RANGE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_ADDRESS_OPT_RANGE_HU, "<c�mtartom�ny>"},
/* it */ {IDGS_P_ADDRESS_OPT_RANGE_IT, "<intervallo di indirizzi opzionale>"},
/* nl */ {IDGS_P_ADDRESS_OPT_RANGE_NL, "<optionele_adres_reeks>"},
/* pl */ {IDGS_P_ADDRESS_OPT_RANGE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_ADDRESS_OPT_RANGE_SV, "<adress_ev_omr�de>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_I_DESCRIPTION,    N_("Display memory contents as PETSCII text.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_I_DESCRIPTION_DE, "Zeige Speicherinhalt als PETSCII Text."},
/* fr */ {IDGS_MON_I_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_I_DESCRIPTION_HU, "Mem�ria tartalom mutat�sa PETSCII sz�vegk�nt."},
/* it */ {IDGS_MON_I_DESCRIPTION_IT, "Mostra il contenuto della memoria come testo PETSCII."},
/* nl */ {IDGS_MON_I_DESCRIPTION_NL, "Toon geheugen inhoud als PETSCII tekst."},
/* pl */ {IDGS_MON_I_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_I_DESCRIPTION_SV, "Visa minnesinneh�ll som PETSCII-text."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_II_DESCRIPTION,    N_("Display memory contents as screen code text.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_II_DESCRIPTION_DE, "Zeige Speicherinhalt als Bildschirmcode Text."},
/* fr */ {IDGS_MON_II_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_II_DESCRIPTION_HU, "Mem�ria tartalom megjelen�t�se k�perny� k�d sz�vegk�nt."},
/* it */ {IDGS_MON_II_DESCRIPTION_IT, "Mostra il contenuto della memoria come testo screen code."},
/* nl */ {IDGS_MON_II_DESCRIPTION_NL, "Toon geheugen inhoud als scherm code tekst."},
/* pl */ {IDGS_MON_II_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_II_DESCRIPTION_SV, "Visa minnesinneh�ll som sk�rmkodstext."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE,    N_("[<data_type>] [<address_opt_range>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_DE, "[<Datentyp>] [<Optionaler Adressbereich>]"},
/* fr */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_HU, "[<adat_t�pus>] [<c�mtartom�ny>]"},
/* it */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_IT, "[<dati>] [<intervallo di indirizzi opzionale>]"},
/* nl */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_NL, "[<data_soort>] [<optionele_adres_reeks>]"},
/* pl */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_DATA_TYPE_RP_ADDRESS_OPT_RANGE_SV, "[<datatyp>] [<adress_ev_omr�de>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEM_DESCRIPTION,    N_("Display the contents of memory.  If no datatype is given, the default\n"
                                          "is used.  If only one address is specified, the length of data\n"
                                          "displayed is based on the datatype.  If no addresses are given, the\n"
                                          "'dot' address is used.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEM_DESCRIPTION_DE, "Zeige Speicherinhalt. Wenn kein Datentyp angegeben wurde, wird der\n"
                                       "Standarddatentyp verwendet. Wenn nur eine Adresse angegeben wird, h�ngt\n"
                                       "die L�nge der Daten vom Datentyp ab. Wenn keine Adresse angeben wurde, \n"
                                       "wird die aktuelle Adresse verwendet."},
/* fr */ {IDGS_MON_MEM_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEM_DESCRIPTION_HU, "A mem�ria tartalm�nak megjelen�t�se. Ha nincs adat t�pus megadva,\n"
                                       "az alapbe�ll�t�sthaszn�lja. Ha egy c�m van megadva, a megjelen�tett adat\n"
                                       "m�rete az adat t�pust�l f�gg. Ha nincs c�m megadva, a 'pont' c�met haszn�lja."},
/* it */ {IDGS_MON_MEM_DESCRIPTION_IT, "Mostra il contenuto della memoria. Se non � specificato alcun tipo di\n"
                                       "dato, verr� utilizzato quello predefinito. Se � specificato solo un indirizzo,\n"
                                       "la lunghezza del dato visualizato � basato sul tipo. Se non � specificato\n"
                                       "nessun indirizzo, varr� utilizzato l'indirizzo 'dot'."},
/* nl */ {IDGS_MON_MEM_DESCRIPTION_NL, "Toon de geheugen inhoud. Als er geen datasoort is opgegeven dan zal de standaard\n"
                                       "worden gebruikt. Als er maar een adres is opgegeven dan zal de lengte van de\n"
                                       "getoonde data gebaseerd worden op de datasoort. Als er geen adres is opgegeven\n"
                                       "dan zal het huidige adres worden gebruikt."},
/* pl */ {IDGS_MON_MEM_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEM_DESCRIPTION_SV, "Visa minnets inneh�ll. Om ingen datatyp anges, anv�nds standard.\n"
                                       "Om endast en adress anges, justeras l�ngden p� visad data baserat\n"
                                       "p� datatypen. Om ingen adress anges anv�nds \"punkt\"-adressen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEMCHAR_DESCRIPTION,    N_("Display the contents of memory as character data.  If only one address\n"
                                              "is specified, only one character is displayed.  If no addresses are\n"
                                              "given, the ``dot'' address is used.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEMCHAR_DESCRIPTION_DE, "Zeige den Inhalt von Speicher als Character Daten. Wenn nur eine Adresse\n"
                                           "angegeben wurde, wird nur ein Zeichen gezeigt. Wenn keine Adresse angegeben\n"
                                           "wurde, wird die aktuelle Adresse verwendet."},
/* fr */ {IDGS_MON_MEMCHAR_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEMCHAR_DESCRIPTION_HU, "A mem�ria tartalm�nak megjelen�t�se karakter grafikak�nt.  Ha csak egy c�m\n"
                                           "adott, egyetlen karakter rajzol�dik ki.  Ha nincs c�m megadva,\n"
                                           "a ``pont'' c�met haszn�lja."},
/* it */ {IDGS_MON_MEMCHAR_DESCRIPTION_IT, "Mosta il contenuto della memoria come dati caratteri. Se � specificato\n"
                                           "solo un indirizzo, verr� visualizzato solo un carattere. Se non �\n"
                                           "specificato alcun indirizzo, varr� utilizzato l'indirizzo ``dot'' ."},
/* nl */ {IDGS_MON_MEMCHAR_DESCRIPTION_NL, "Toon de geheugen inhoud als karakter data. Als er maar een adres is\n"
                                           "opgegeven dan zal er maar een karakter worden weergegeven. Als er geen\n"
                                           "adres is opgegeven dan zal het huidige adres gebruikt worden."},
/* pl */ {IDGS_MON_MEMCHAR_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEMCHAR_DESCRIPTION_SV, "Visa minnets inneh�ll som teckendata. Om endast en adress\n"
                                           "anges, visas endast ett tecken. Om ingen adress anges anv�nds\n"
                                           "\"punkt\"-adressen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME_P_FORMAT,    N_("\"<filename>\" <format>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_P_FORMAT_DE, "\"<Dateiname>\" <Format>"},
/* fr */ {IDGS_QP_FILENAME_P_FORMAT_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_P_FORMAT_HU, "\"<f�jln�v>\" <form�tum>"},
/* it */ {IDGS_QP_FILENAME_P_FORMAT_IT, "\"<nome del file>\" <formato>"},
/* nl */ {IDGS_QP_FILENAME_P_FORMAT_NL, "\"<betandsnaam>\" <formaat>"},
/* pl */ {IDGS_QP_FILENAME_P_FORMAT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_P_FORMAT_SV, "\"<filnamn>\" <format>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION,    N_("Save the memmap as a picture. Format is:\n"
                                                 "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_DE, "Speicher die memmap als Bilddaten. Formate:\n"
                                              "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* fr */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_HU, "Mem�ria t�rk�p ment�se k�pk�nt. A form�tum:\n"
                                              "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* it */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_IT, "Salva la mappa di memoria come immagine. I Formati sono:\n"
                                              "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* nl */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_NL, "Sla de memmap op als een afbeelding. Formaat is:\n"
                                                 "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* pl */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEMMAPSAVE_DESCRIPTION_SV, "Sparar minneskartan som en bild. Format:\n"
                                              "0 = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE,    N_("[<mask>] [<address_opt_range>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_DE, "[<Maske>] [<Optionaler Adressbereich>]"},
/* fr */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_HU, "[<maszk>] [<c�mtartom�ny>]"},
/* it */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_IT, "[<maschera>] [<intervallo di indirizzi opzionale>]"},
/* nl */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_NL, "[<masker>] [<optionele_adres_reeks>]"},
/* pl */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_MASK_RP_ADDRESS_OPT_RANGE_SV, "[<mask>] [<adress_ev_omr�de>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION,    N_("Show the memmap. The mask can be specified to show only those\n"
                                                 "locations with accesses of certain type(s). The mask is a number\n"
                                                 "with the bits \"ioRWXrwx\", where RWX are for ROM and rwx for RAM.\n"
                                                 "Optionally, an address range can be specified.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_DE, "Zeige die memmap. Die Maske kann so definiert werden, dass nur jene Orte\n"
                                              "mit Zugriff auf spezifische Typ(en) gezeigt werden. Die Maske ist eine Nummer\n"
                                              "mit Bits \"ioRWXrwx\", wobei RWX f�r ROM und rwx f�r RAM steht.\n"
                                              "Optional kann ein Adressbereich definiert werden."},
/* fr */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_HU, "A mem�ria t�rk�p mutat�sa. A maszk �gy �ll�that�, hogy csak azokat a\n"
                                              "helyeket mutassa, amelyek a megadott el�r�ssel rendelkeznek. A maszk\n"
                                              "egy sz�m az \"ioRWXrwx\" bitekkel, ahol az RWX a ROM-ra, az rwx a RAM-ra\n"
                                              "vonatkozik. Tetsz�s szerint egy c�mtartom�ny is megadhat�."},
/* it */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_IT, "Mostra la mappa di memoria. La maschera pu� essere specificata per\n"
                                              "mostrare solo quelle locazioni con accessi un un certo tipo. La maschera\n"
                                              "� un numero con i bit \"ioRWXrwx\", dove RWX sono per la ROM e\n"
                                              "rwx per la RAM.\n"
                                              "Opzionalmente, pu� essere specificato anche un intervallo di indirizzi."},
/* nl */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_NL, "Toon de memmap. Het masker kan worden opgegeven zodat er alleen de\n"
                                              "adressen met toegrijpingen van een bepaalde soort(en) zal worden getoond.\n"
                                              "Het masker is een nummer met de bits \"ioRWXrwx\", waar RWX is voor ROM,\n"
                                              "en rwx voor RAM. Er kan optioneel een adres bereik worden opgegeven."},
/* pl */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEMMAPSHOW_DESCRIPTION_SV, "Visa minneskartan. Masken kan anges f�r att endast visa de\n"
                                              "celler som g�ller specifik(a) typ(er). Masken �r ett tal\n"
                                              "med bitarna \"ioRWXrwx\", d�r RWX �r f�r ROM och rwx �r f�r\n"
                                              "RAM. Ett minnesomr�de kan eventuellt anges."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEMMAPZAP_DESCRIPTION,    N_("Clear the memmap.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_DE, "L�sche die memmap."},
/* fr */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_HU, "A mem�ria t�rk�p t�rl�se."},
/* it */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_IT, "Cancella la mappa di meoria."},
/* nl */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_NL, "Veeg de memmap schoon."},
/* pl */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEMMAPZAP_DESCRIPTION_SV, "T�m minneskartan."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MEMSPRITE_DESCRIPTION,    N_("Display the contents of memory as sprite data.  If only one address is\n"
                                                "specified, only one sprite is displayed.  If no addresses are given,\n"
                                                "the ``dot'' address is used.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MEMSPRITE_DESCRIPTION_DE, "Zeige den Speicher als Sprite Daten. Wenn nur eine Adresse angegeben wurde,\n"
                                             "wird nur ein Sprite gezeitg. Wenn keine Adresse angegeben wurde, wird\n"
                                             "die aktuelle Adresse verwendet."},
/* fr */ {IDGS_MON_MEMSPRITE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MEMSPRITE_DESCRIPTION_HU, "A mem�ria tartalm�nak megjelen�t�se sprite adatk�ne. Ha csak egy c�m\n"
                                             "van megadva, egyetlen sprite-ot mutat.  Ha egy�ltal�n nincs c�m megadva,\n"
                                             "az aktu�lis c�met haszn�lja."},
/* it */ {IDGS_MON_MEMSPRITE_DESCRIPTION_IT, "Mostra il contenuto della memoria come dati sprite. Se � specificato\n"
                                             "solo un indirizzo, verr� mostrato solo uno sprite. Se non � specificato\n"
                                             "alcun indirizzo, varr� utilizzato l'indirizzo ``dot''."},
/* nl */ {IDGS_MON_MEMSPRITE_DESCRIPTION_NL, "Toon de geheugen inhoud als sprite data. Als er maar een adres is opgegeven\n"
                                             "dan zal er maar een sprite worden getoond. Als er geen adres is opgegeven\n"
                                             "dan zal het huidige adres worden gebruikt."},
/* pl */ {IDGS_MON_MEMSPRITE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MEMSPRITE_DESCRIPTION_SV, "Visa minnets inneh�ll som spritedata. Om endast en adress\n"
                                             "anges, visas endast en sprite. Om ingen adress anges anv�nds\n"
                                             "\"punkt\"-adressen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_MOVE_DESCRIPTION,    N_("Move memory from the source specified by the address range to\n"
                                           "the destination specified by the address.  The regions may overlap.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_MOVE_DESCRIPTION_DE, "Verschiebe Speicher von Startadresse definiert durch den Adressbereich zum\n"
                                        "Zielbereich definiert durch die Adresse. Die Bereiche k�nnen �berlappend sein."},
/* fr */ {IDGS_MON_MOVE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_MOVE_DESCRIPTION_HU, "Mem�ria m�sol�sa a forr�s ter�letr�l a c�l c�m �ltal meghat�rozott\n"
                                        "ter�letre. A tartom�nyok �tfed�se megengedett."},
/* it */ {IDGS_MON_MOVE_DESCRIPTION_IT, "Sposta la memoria dalla sorgente specificata dall'intervallo di indirizzi\n"
                                        "alla destinazione specificata dall'indirizzo. Le regioni possono\n"
                                        "sovrapporsi."},
/* nl */ {IDGS_MON_MOVE_DESCRIPTION_NL, "Verplaats data van de opgegeven bron adres reeks naar het doel adres. De\n"
                                        "reeksen kunnen overlappen."},
/* pl */ {IDGS_MON_MOVE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_MOVE_DESCRIPTION_SV, "Flytta minne fr�n k�lladressen som anges av minnesomr�det\n"
                                        "till destinationen som anges av adressen. Regionerna kan vara\n"
                                        "�verlappande."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_CHECKPOINT_COMMANDS,    N_("Checkpoint commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_CHECKPOINT_COMMANDS_DE, "Checkpoint Kommandos"},
/* fr */ {IDGS_CHECKPOINT_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_CHECKPOINT_COMMANDS_HU, "Ellen�rz�si pont parancsok:"},
/* it */ {IDGS_CHECKPOINT_COMMANDS_IT, "Comandi punti di controllo:"},
/* nl */ {IDGS_CHECKPOINT_COMMANDS_NL, "Checkpunt commandos:"},
/* pl */ {IDGS_CHECKPOINT_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_CHECKPOINT_COMMANDS_SV, "Kommandon f�r brytpunkter:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_ADDRESS_RP_COND_EXPR,    N_("[<address> [if <cond_expr>] ]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_ADDRESS_RP_COND_EXPR_DE, "[<Adresse> [if <bedingter Ausdruck>] ]"},
/* fr */ {IDGS_RP_ADDRESS_RP_COND_EXPR_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_ADDRESS_RP_COND_EXPR_HU, "[<c�m> [if <felt_kif>] ]"},
/* it */ {IDGS_RP_ADDRESS_RP_COND_EXPR_IT, "[<indirizzo> [if <espressione condizione>] ]"},
/* nl */ {IDGS_RP_ADDRESS_RP_COND_EXPR_NL, "[<adres> [ald <cond_expr>] ]"},
/* pl */ {IDGS_RP_ADDRESS_RP_COND_EXPR_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_ADDRESS_RP_COND_EXPR_SV, "[<adress> [om <villkorsuttryck>] ]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BREAK_DESCRIPTION,    N_("If no address is given, the currently valid watchpoints are printed.\n"
                                            "If an address is given, a breakpoint is set for that address and the\n"
                                            "breakpoint number is printed.  A conditional expression can also be\n"
                                            "specified for the breakpoint.  For more information on conditions, see\n"
                                            "the CONDITION command.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BREAK_DESCRIPTION_DE, "Wenn keine Adresse angegeben wurde, werden die aktuellen watchpoints gezeigt.\n"
                                         "Wenn eine Adresse angegebene wurde, wird ein breakpoint auf diese Adresse\n"
                                         "gesetzt und die breakpoint Nummer wird ausgegeben. Ein bedingter Ausdruck\n"
                                         "can zu dem breakpoint zugeordnet werden. F�r mehr Informations �ber \n"
                                         "bedingte Ausdr�cke, bitte Kommando CONDITION ansehen."},
/* fr */ {IDGS_MON_BREAK_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BREAK_DESCRIPTION_HU, "Ha nincs c�m megadva, az aktu�lisan �rv�nyes figyel�si pontokat\n"
                                         "list�zza. Ha van c�m a param�terben, t�r�spontot helyez el az adott\n"
                                         "c�mre, �s a sz�m�t ki�rja. Egy felt�tel kifejez�s is megadhat� a t�r�sponthoz.\n"
                                         "A felt�telekr�l t�bb inform�ci�hoz l�sd a CONDITION parancs s�g�j�t!"},
/* it */ {IDGS_MON_BREAK_DESCRIPTION_IT, "Se non viene specificato alcun indirizzo, i watchpoint attuali vengono\n"
                                         "stampati.Se viene specificato un indirizzo, viene impostato un breakpoint\n"
                                         "per tale indirizzo e viene stampato il suo numero. Pu� anche essere\n"
                                         "specificata una condizione per il breakpoint. Per ulteriori informazioni,\n"
                                         "vedere il comando CONDITION."},
/* nl */ {IDGS_MON_BREAK_DESCRIPTION_NL, "Als er geen adres is opgegeven dan zullen de huidige geldige kijkpunten\n"
                                         "geprint. Als er een adres is opgegeven dan zal er een breekpunt gezet worden\n"
                                         "voor dat adres en de nummer van het breekpunt zal worden getoond. Een\n"
                                         "conditie kan worden opgegeven voor het breekpunt. Voor meer informatie over\n"
                                         "condities zie het CONDITION commando."},
/* pl */ {IDGS_MON_BREAK_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BREAK_DESCRIPTION_SV, "Om ingen adress anges visas de �vervakningspunkter som �r giltiga.\n"
                                         "Om en adress anges s�tts en brytpunkt p� den adressen och dess\n"
                                         "nummer visas. Ett villkorsuttryck kan ocks� anges f�r brytpunkten.\n"
                                         "F�r mer information om villkor, se kommandot CONDITION."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_CHECKNUM_QP_COMMAND,    N_("<checknum> \"<command>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_CHECKNUM_QP_COMMAND_DE, "<Checknum> \"<Kommando>\""},
/* fr */ {IDGS_P_CHECKNUM_QP_COMMAND_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_CHECKNUM_QP_COMMAND_HU, "<ell_pont_sz�m> \"<parancs>\""},
/* it */ {IDGS_P_CHECKNUM_QP_COMMAND_IT, "<checknum> \"<comando>\""},
/* nl */ {IDGS_P_CHECKNUM_QP_COMMAND_NL, "<checknum> \"<commando>\""},
/* pl */ {IDGS_P_CHECKNUM_QP_COMMAND_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_CHECKNUM_QP_COMMAND_SV, "<kontrollnr> \"<kommando>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_COMMAND_DESCRIPTION,    N_("Specify `command' as the command to execute when checkpoint `checknum'\n"
                                              "is hit.  Note that the `x' command is not yet supported as a\n"
                                              "command argument.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_COMMAND_DESCRIPTION_DE, "Definiere `Kommando', welches bei Checkpoint `Checknum' ausgef�hrt wird.\n"
                                           "Achtung Kommando `x' wird zurzeit noch nicht als Kommando Argument\n"
                                           "unterst�tzt."},
/* fr */ {IDGS_MON_COMMAND_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_COMMAND_DESCRIPTION_HU, "`parancs' megad�sa egy `ell_pont_sz�m' sorsz�m�val adott ellen�rz�si pont\n"
                                           "el�r�sekor t�rt�n� v�grehajt�sra.  Az `x' parancs egyel�re nem haszn�lhat�\n"
                                           "itt param�terk�nt."},
/* it */ {IDGS_MON_COMMAND_DESCRIPTION_IT, "Specifica `comando' come il comando da eseguire quando\n"
                                           "viene incontrato il punto di controllo `checknum'. Nota che \n"
                                           "il comando `x' non � ancora supportato come parametro."},
/* nl */ {IDGS_MON_COMMAND_DESCRIPTION_NL, "Geef `commando' als het command om uit te voeren wanneer checkpunt `checknum'\n"
                                           "wordt uitgevoerd. NB: het `x' commando is op dit moment geen geldige\n"
                                           "parameter."},
/* pl */ {IDGS_MON_COMMAND_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_COMMAND_DESCRIPTION_SV, "Ange \"kommando\" som kommando att exekvera n�r vid tr�ff p�\n"
                                           "kontrollpunkten�\"kontrollnr\". Observera att kommandot \"x\" �nnu\n"
                                           "inte st�ds som kommandoparameter."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR,    N_("<checknum> if <cond_expr>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_DE, "<Checknum> if <bedingter Ausdruck>"},
/* fr */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_HU, "<ell_pont_sz�m> if <kifejez�s>"},
/* it */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_IT, "<checknum> if <espressione condizione>"},
/* nl */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_NL, "<checknum> als <cond_expr>"},
/* pl */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_CHECKNUM_IF_P_COND_EXPR_SV, "<kontrollnum> if <villkorsuttryck>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_CONDITION_DESCRIPTION,    N_("Each time the specified checkpoint is examined, the condition is\n"
                                                "evaluated.  If it evalutes to true, the checkpoint is activated.\n"
                                                "Otherwise, it is ignored.  If registers are specified in the expression,\n"
                                                "the values used are those at the time the checkpoint is examined, not\n"
                                                "when the condition is set.\n"
                                                "The condition can make use of registers (.A, .X, .Y, .PC, .SP) and\n"
                                                "compare them (==, !=, <, >, <=, >=) again other registers or constants.\n"
                                                "Registers can be the registers of other devices; this is denoted by\n"
                                                "a memspace prefix (i.e., c:, 8:, 9:, 10:, 11:\n"
                                                "Examples: .A == 0, .X == .Y, 8:.X == .X\n")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_CONDITION_DESCRIPTION_DE, "Bei jeder �berpr�fung des Checkpoints, wird die Bedingung evaluiert.\n"
                                             "Ist das Ergebnis `Wahr', wird der Checkpoint aktiviert, sonst ingnoriert.\n"
                                             "Wenn Register benutzt werden, werden deren Werte zum Zeitpunkt der \n"
                                             "�berpr�fung herangezogen.\n"
                                             "Die Bedingung kann Register benutzen (.A, .X, .Y, .PC, .SP) und gegen \n"
                                             "Register oder Konstanten vergleichen (==, !=, <, >, <=, >=).\n"
                                             "Register k�nnen auch Ger�teregister sein; dazu wird die memspace Prefix-syntax \n"
                                             "verwendet: c:, 8:, 9:, 10:, 11:\n"
                                             "Beispiel: .A == 0, .X == .Y, 8:.X == .X\n"},
/* fr */ {IDGS_MON_CONDITION_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_CONDITION_DESCRIPTION_HU, "Minden alkalommal, amikor egy adott ellen�rz�si pontot megvizsg�l, a\n"
                                             "felt�telt ki�rt�keli. Ha az eredm�ny igaz, a t�r�spont aktiv�l�dik.\n"
                                             "Egy�bk�nt, figyelmen k�v�l hagyja. Ha regiszterek szerepelnek a kifejez�sben,\n"
                                             "az ellen�rz�s pillanat�ban vett �rt�k�k sz�m�t, nem pedig a be�ll�t�skori.\n"
                                             "A felt�tel haszn�lhatja a regisztereket ( .A, .X, .Y, .PC, .SP) �s �ssze is\n"
                                             "hasonl�thatja �ket (==, !=, <, >, <=, >=) m�s regiszterekkel.\n"
                                             "A regiszterek lehetnek m�s eszk�z�k regiszterei is; azokat egy mem�ria t�r\n"
                                             "el�taggal kell ell�tni (vagyis c:, 8:, 9:, 10:, 11:).\n"
                                             "P�ld�k: .A == 0, .X == .Y, 8:.X == .X\n"},
/* it */ {IDGS_MON_CONDITION_DESCRIPTION_IT, "Ogni volta che si esamina il punto di controllo specificato, viene valutata\n"
                                             "la condizione. Se � vera, il punto di controllo � attivato.\n"
                                             "Altrimenti viene ignorato. Se nell'espressione sono specificati i registri,\n"
                                             "questi valori sono quelli usati quando viene esaminato il punto di controllo,\n"
                                             "non quando viene impostata la condizione.\n"
                                             "La condizione pu� fare uso dei registri (.A, .X, .Y, .PC, .SP) e confontarli\n"
                                             "(==, !=, <, >, <=, >=) con altri registri o costanti.\n"
                                             "I registri possono essere registri di altre periferiche; questo � denotato da\n"
                                             "un prefisso che specifica lo spazio di memoria (cio�, c:, 8:, 9:, 10:, 11:)\n"
                                             "Alcuni esempi: .A == 0, .X == .Y, 8:.X == .X\n"},
/* nl */ {IDGS_MON_CONDITION_DESCRIPTION_NL, "Elke keer de opgegeven checkpunt wordt onderzocht zal de conditie worden\n"
                                             "geevalueerd. Als de evaluatie positief is dan zal de checkpunt geactiveerd\n"
                                             "worden, bij negatief zal er niets worden gedaan. Als er registers zijn\n"
                                             "opgegeven in de expressie dan zullen de waarden in die registers gebruikt\n"
                                             "worden op het moment dat de checkpunt wordt onderzocht, niet op het moment\n"
                                             "dat de conditie is opgegeven.\n"
                                             "De conditie kan gebruik maken van registers (.A, .X, .Y, .PC, .SP) en\n"
                                             "kan ze vergelijken (==, !=, <, >, <=, >=) met andere registers of constanten\n"
                                             "Registers kunnen de registers van andere apparaten zijn; de manier van opgeven\n"
                                             "is door een geheugenruimte prefix (bijvoorbeeld: c:, 8:, 9:, 10:, 11:\n"
                                             "Voorbeelden: .A == 0, .X == .Y, 8:.X == .X\n"},
/* pl */ {IDGS_MON_CONDITION_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_CONDITION_DESCRIPTION_SV, "Villkoret utv�rderas varje g�ng den givna kontrollpunkten\n"
                                             "unders�ks. Om det utv�rderas till sant aktiveras kontrollpunkten.\n"
                                             "I annat fall ignoreras den. Om uttrycket inneh�ller register\n"
                                             "anv�nds de v�rden de har d� kontrollpunkten utv�rderas, inte\n"
                                             "n�r villkoret sattes.\n"
                                             "Villkoret kan anv�nda register (.A, .X, .Y, .PC, .SP) och\n"
                                             "j�mf�ra dem (==, !=, <, >, <=, >=) mot andra register eller\n"
                                             "konstanter. Register kan vara register i andra enheter, detta\n"
                                             "anges som ett minnesomr�desprefix (dvs., c:, 8:, 9:, 10:, 11:)\n"
                                             "Exempel: .A == 0, .X == .Y, 8:.X == .X\n"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DISABLE_DESCRIPTION,    N_("Disable checkpoint `checknum'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DISABLE_DESCRIPTION_DE, "Checkpoint `Checknum' deaktivieren."},
/* fr */ {IDGS_MON_DISABLE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DISABLE_DESCRIPTION_HU, "`ell_pont_sz�m' jel� ellen�rz�si pont tilt�sa."},
/* it */ {IDGS_MON_DISABLE_DESCRIPTION_IT, "Disattiva il punto di controllo `checknum'."},
/* nl */ {IDGS_MON_DISABLE_DESCRIPTION_NL, "Checkpunt `checknum' uitschakelen."},
/* pl */ {IDGS_MON_DISABLE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DISABLE_DESCRIPTION_SV, "Inaktivera kontrollpunkt \"kontrollnr\"."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_ENABLE_DESCRIPTION,    N_("Enable checkpoint `checknum'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_ENABLE_DESCRIPTION_DE, "Checkpoint `Checknum' aktivieren."},
/* fr */ {IDGS_MON_ENABLE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_ENABLE_DESCRIPTION_HU, "`ell_pont_sz�m' jel� ellen�rz�si pont enged�lyez�se."},
/* it */ {IDGS_MON_ENABLE_DESCRIPTION_IT, "Attiva il punto di controllo `checknum'."},
/* nl */ {IDGS_MON_ENABLE_DESCRIPTION_NL, "Activeer checkpunt `checknum'."},
/* pl */ {IDGS_MON_ENABLE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_ENABLE_DESCRIPTION_SV, "Aktivera kontrollpunkt \"kontrollnr\"."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_CHECKNUM_RP_COUNT,    N_("<checknum> [<count>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_CHECKNUM_RP_COUNT_DE, "<Checknum> [<Z�hler>]"},
/* fr */ {IDGS_P_CHECKNUM_RP_COUNT_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_CHECKNUM_RP_COUNT_HU, "<ell_pont_sz�m> [<darab>]"},
/* it */ {IDGS_P_CHECKNUM_RP_COUNT_IT, "<checknum> [<contatore>]"},
/* nl */ {IDGS_P_CHECKNUM_RP_COUNT_NL, "<checknum> [<aantal>]"},
/* pl */ {IDGS_P_CHECKNUM_RP_COUNT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_CHECKNUM_RP_COUNT_SV, "<kontrollnum> [<antal>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_IGNORE_DESCRIPTION,    N_("Ignore a checkpoint a given number of crossings.  If no count is given,\n"
                                             "the default value is 1.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_IGNORE_DESCRIPTION_DE, "Ignoriere den Checkpoint f�r die angegebene Anzahl an Aktivierungen.\n"
                                          "Wenn kein `Z�hler' angegeben wurde, wird der Standardwert 1 verwendet."},
/* fr */ {IDGS_MON_IGNORE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_IGNORE_DESCRIPTION_HU, "Az ellen�rz�si pont figyelmen k�v�l hagy�sa megadott sz�m� �thalad�skor.  Ha nincs\n"
                                          "sz�m megadva, az alap�rtelmezett �rt�k 1."},
/* it */ {IDGS_MON_IGNORE_DESCRIPTION_IT, "Ignora un punto di controllo un numero specificato di occorrenze. Se\n"
                                          "non � specificato alcun valore, quello predefinito � 1."},
/* nl */ {IDGS_MON_IGNORE_DESCRIPTION_NL, "Negeer een checkpunt een opgegeven aantal keer. Als er geen aantal is\n"
                                          "opgegeven dan wordt de standaard waarde van 1 worden gebruikt."},
/* pl */ {IDGS_MON_IGNORE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_IGNORE_DESCRIPTION_SV, "Ignorera en kontrollpunkt det givna antalet g�nger. Om inget antal ges\n"
                                          "�r standardv�rdet 1."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_ADDRESS,    N_("[<address>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_ADDRESS_DE, "[<Adresse>]"},
/* fr */ {IDGS_RP_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_ADDRESS_HU, "[<c�m>]"},
/* it */ {IDGS_RP_ADDRESS_IT, "[<indirizzo>]"},
/* nl */ {IDGS_RP_ADDRESS_NL, "[<adres>]"},
/* pl */ {IDGS_RP_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_ADDRESS_SV, "[<adress>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_UNTIL_DESCRIPTION,    N_("If no address is given, the currently valid breakpoints are printed.\n"
                                            "If an address is given, a temporary breakpoint is set for that address\n"
                                            "and the breakpoint number is printed.  Control is returned to the\n"
                                            "emulator by this command.  The breakpoint is deleted once it is hit.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_UNTIL_DESCRIPTION_DE, "Wenn keine Adresse angegeben wurde, werden die aktuell g�ltigen \n"
                                         "Breakpoints angezeigt. Wenn eine Adresse angegeben wurde, wird ein \n"
                                         "tempor�rer Breakpoint auf dieser Adresse gesetzt und die Breakpoint Nummer wird\n"
                                         "angezeigt. Die Kontrolle wirdd bei diesem Kommando an die Emulation gegeben.\n"
                                         "Der Breakpoint wird gel�scht, wenn er einmal getroffen wurde."},
/* fr */ {IDGS_MON_UNTIL_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_UNTIL_DESCRIPTION_HU, "Ha nincs c�m megadva, a jelenleg �rv�nyes t�r�spontokat �rja ki.\n"
                                         "Ha van adott c�m, egy ideiglenes t�r�spontot helyez el az adott c�mre,\n"
                                         "�s a t�r�spont sz�ma ki�r�dik.  A vez�rl�s visszaker�l az emul�torhoz\n"
                                         "e parancs �ltal. A t�r�spont el�r�sekor t�rl�dik."},
/* it */ {IDGS_MON_UNTIL_DESCRIPTION_IT, "Se non viene specificato alcun indirizzo, vengono stampati i\n"
                                         "breakpoint attualmente validi. Se viene specificato un indirizzo,\n"
                                         "viene impostato un breakpoint temporaneo per tale indirizzo e\n"
                                         "viene stampato il suo numero. Il controllo ritorna all'emulatore\n"
                                         "per mezzo di questo comando. Il breakpoint � cancellato quando\n"
                                         "viene incontrato."},
/* nl */ {IDGS_MON_UNTIL_DESCRIPTION_NL, "Als er geen adres is opgegeven dan zullen de huidig geldige breekpunten\n"
                                         "worden geprint. Als er een adres is opgegeven dan wordt er een tijdelijk\n"
                                         "breekpunt worden gezet voor dat adres en het breekpunt nummer zal worden\n"
                                         "getoond. Na het geven van dit commando zal er terug worden gegaan naar de\n"
                                         "emulator. Het breekpunt zal worden verwijdert na het uitvoeren."},
/* pl */ {IDGS_MON_UNTIL_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_UNTIL_DESCRIPTION_SV, "Om du inte anger n�gon adress visas de f�r n�rvarande giltiga\n"
                                         "brytpunkterna. Om en adress anges s�tts en tempor�r brytpunkt f�r\n"
                                         "den adressen och numret p� den nya brytpunkten visas. Styrningen\n"
                                         "�terl�mnas till emulatorn f�r det h�r kommandot. Brytpunkten\n"
                                         "tas bort n�r den har n�tts."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS,    N_("[loadstore] [address [address]]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_DE, "[lade/speichere] [Adresse [Adresse]]"},
/* fr */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_HU, "[loadstore] [c�m [c�m]]"},
/* it */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_IT, "[loadstore] [indirizzo [indirizzo]]"},
/* nl */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_NL, "[laad/opslaan] [adres [adres]]"},
/* pl */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_R_LOADSTORE_R_ADDRESS_R_ADDRESS_SV, "[loadstore] [adress [adress]]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_WATCH_DESCRIPTION,    N_("Set a watchpoint.  If a single address is specified, set a watchpoint\n"
                                            "for that address.  If two addresses are specified, set a watchpoint\n"
                                            "for the memory locations between the two addresses.\n"
                                            "`loadstore' is either `load' or `store' to specify on which operation\n"
                                            "the monitor breaks. If not specified, the monitor breaks on both\n"
                                            "operations.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_WATCH_DESCRIPTION_DE, "Setze einen Watchpoint. Wenn eine einzelne Adresse angegeben wurde, setze \n"
                                         "den Watchpoint f�r diese Adresse. Wenn zwei Adressen angegeben wurden, \n"
                                         "setze den Watchpoint auf die Adressen zwischen den beiden Adressen.\n"
                                         "`loadstore' ist entweder `load' oder `store' und bezeichnet die Operation, \n"
                                         "bei welcher der Monitor anhalten soll. Wenn keine Angabe gemacht wurde, \n"
                                         "h�lt der Monitor bei beiden Operationen an."},
/* fr */ {IDGS_MON_WATCH_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_WATCH_DESCRIPTION_HU, "Figyel�si pont megad�sa.  Ha egy c�m van a param�terben, az adott\n"
                                         "c�mre �ll�tbe figyel�si pontot. Ha k�t c�m adott, akkor a kett� k�z�tti\n"
                                         "�sszes mem�ria helyre be�ll�tja.\n"
                                         "A `loadstore' vagy `load' vagy `store' lehet, azt adja meg, hogy olvas�s\n"
                                         "vagy �r�s hat�s�ra �ll meg a monitor program. Ha nincs megadva,\n"
                                         "mindk�t m�velet hat�s�ra meg�ll."},
/* it */ {IDGS_MON_WATCH_DESCRIPTION_IT, "Imposta un watchpoint. Se viene specificato solo un indirizzo, imposta\n"
                                         "un watchpoint per tale indirizzo. Se vengono specificati due indirizzi,\n"
                                         "imposta un watchpoint per le locazioni di memoria comprese tra i due\n"
                                         "indirizzi.\n"
                                         "`loadstore' pu� essere `load' o `store' per specificare su quale\n"
                                         "operazione il monitor si interrompe. Se non viene specificato, il monitor\n"
                                         "si interrompe su entrambe le operazioni."},
/* nl */ {IDGS_MON_WATCH_DESCRIPTION_NL, "Zet een kijkpunt. Als er een enkel adres is opgegeven dan zal er een\n"
                                         "kijkpunt worden gezet voor dat adres. Als er twee adressen zijn opgegeven\n"
                                         "dan zal er een kijkpunt worden gezet voor de adressen tussen de twee\n"
                                         "adressen.\n"
                                         "laad/opslaan is 'load' of 'store' om aan te geven bij welke operatie de\n"
                                         "monitor zou moeten starten. Als deze niet opgegeven is dan zal de monitor\n"
                                         "starten bij beide operaties."},
/* pl */ {IDGS_MON_WATCH_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_WATCH_DESCRIPTION_SV, "S�tt en �vervakningspunkt. Om du anger en ensam adress s�tts en\n"
                                         "�vervakningspunkt f�r adressen. Om tv� adresser anges s�tts en\n"
                                         "�vervakningspunkt f�r minnesomr�det mellan adresserna.\n"
                                         "\"loadstore\" �r antingen \"load\" eller \"store\" och anger vilken\n"
                                         "operation emulatorn skall stoppa f�r. Om den inte anges stoppar\n"
                                         "emulatorn p� b�gge operationerna."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_R_ADDRESS_R_ADDRESS,    N_("[address [address]]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_R_ADDRESS_R_ADDRESS_DE, "[Adresse [Adresse]]"},
/* fr */ {IDGS_R_ADDRESS_R_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_R_ADDRESS_R_ADDRESS_HU, "[c�m [c�m]]"},
/* it */ {IDGS_R_ADDRESS_R_ADDRESS_IT, "[indirizzo [indirizzo]]"},
/* nl */ {IDGS_R_ADDRESS_R_ADDRESS_NL, "[adres [adres]]"},
/* pl */ {IDGS_R_ADDRESS_R_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_R_ADDRESS_R_ADDRESS_SV, "[adress [adress]]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_TRACE_DESCRIPTION,    N_("Set a tracepoint.  If a single address is specified, set a tracepoint\n"
                                            "for that address.  If two addresses are specified, set a tracepoint\n"
                                            "for the memory locations between the two addresses.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_TRACE_DESCRIPTION_DE, "Setze einen Tracepoint. Wenn eine einzelne Adresse angegeben wurde, setze eine\n"
                                         "Tracepoint f�r diese Adresse. Wenn zwei Adressen angegeben wurden, \n"
                                         "setze Tracepoints f�r die Speicheradressen zwischen diesen beiden Adressen."},
/* fr */ {IDGS_MON_TRACE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_TRACE_DESCRIPTION_HU, "K�vet�si pont be�ll�t�sa.  Ha egyetlen c�m van megadva, a k�vet�si pontot\n"
                                         "ahhoz �ll�tja be.  Ha k�t c�m van megadva, a k�vet�si pontot\n"
                                         "az adott tartom�nyra �ll�tja be."},
/* it */ {IDGS_MON_TRACE_DESCRIPTION_IT, "Imposta un tracepoint. Se viene specificato solo un indirizzo, imposta\n"
                                         "un tracepoint per tale indirizzo. Se vengono specificati due indirizzi,\n"
                                         "imposta un tracepoint per le locazioni di memoria comprese tra i\n"
                                         "due indirizzi."},
/* nl */ {IDGS_MON_TRACE_DESCRIPTION_NL, "Zet een traceerpunt. Als er een enkel adres is opgegeven dan zal er een\n"
                                         "traceerpunt worden gezet voor het adres. Als er twee adressen zijn\n"
                                         "opgegeven dan zal er een traceerpunt worden gezet voor de adressen tussen\n"
                                         "de twee adressen."},
/* pl */ {IDGS_MON_TRACE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_TRACE_DESCRIPTION_SV, "S�tt en sp�rningspunkt. Om du anger en ensam adress s�tts en\n"
                                         "sp�rningspunkt f�r adressen. Om tv� adresser anges s�tts en\n"
                                         "sp�rningspunkt f�r minnesomr�det mellan adresserna."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MONITOR_STATE_COMMANDS,    N_("Monitor state commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MONITOR_STATE_COMMANDS_DE, "Monitor Zustand Kommandos:"},
/* fr */ {IDGS_MONITOR_STATE_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MONITOR_STATE_COMMANDS_HU, "Monitor �llapot parancsok:"},
/* it */ {IDGS_MONITOR_STATE_COMMANDS_IT, "Comandi stato monitor:"},
/* nl */ {IDGS_MONITOR_STATE_COMMANDS_NL, "Monitor status commandos:"},
/* pl */ {IDGS_MONITOR_STATE_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MONITOR_STATE_COMMANDS_SV, "Kommandon f�r monitorstatus:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DEVICE_DESCRIPTION,    N_("Set the default memory device to either the computer `c:' or the\n"
                                             "specified disk drive (`8:', `9:').")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DEVICE_DESCRIPTION_DE, "Setze das aktuelle Speicherger�t entweder auf `c:' \n"
                                          "oder das angegebene Laufwerk (`8:', `9:')."},
/* fr */ {IDGS_MON_DEVICE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DEVICE_DESCRIPTION_HU, "Az alap�rtelmezett mem�ria eszk�z be�ll�t�sa a sz�m�t�g�pre `c:' vagy\n"
                                          "a megadott lemezmeghajt�ra (`8:', `9:')."},
/* it */ {IDGS_MON_DEVICE_DESCRIPTION_IT, "Imposta la periferica di memoria predefinita o sul computer `c:'\n"
                                          "oppure sul disk drive specificato (`8:', `9:')."},
/* nl */ {IDGS_MON_DEVICE_DESCRIPTION_NL, "Zet de standaard geheugen apparaat naar of de computer `c:' of de\n"
                                          "opgegeven disk drive (`8:', `9:')."},
/* pl */ {IDGS_MON_DEVICE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DEVICE_DESCRIPTION_SV, "S�tt f�rvald minnesenhet antingen till datorn \"c:\" eller\n"
                                          "den angivna diskettenheten (\"8:\", \"9:\")."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_EXIT_DESCRIPTION,    N_("Leave the monitor and return to execution.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_EXIT_DESCRIPTION_DE, "Verlasse Monitor und setze Ausf�hrung fort."},
/* fr */ {IDGS_MON_EXIT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_EXIT_DESCRIPTION_HU, "A monitor elhagy�sa �s a v�grehajt�s folytat�sa."},
/* it */ {IDGS_MON_EXIT_DESCRIPTION_IT, "Esce dal monitor e ritorna all'esecuzione."},
/* nl */ {IDGS_MON_EXIT_DESCRIPTION_NL, "Verlaat de monitor en ga terug naar de uitvoering."},
/* pl */ {IDGS_MON_EXIT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_EXIT_DESCRIPTION_SV, "L�mna monitorn och �terg� till programk�rningen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_QUIT_DESCRIPTION,    N_("Exit the emulator immediately.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_QUIT_DESCRIPTION_DE, "Emulator sofort beenden."},
/* fr */ {IDGS_MON_QUIT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_QUIT_DESCRIPTION_HU, "Azonnali kil�p�s az emul�torb�l."},
/* it */ {IDGS_MON_QUIT_DESCRIPTION_IT, "Esci immediatamente dall'emulatore."},
/* nl */ {IDGS_MON_QUIT_DESCRIPTION_NL, "Onmiddelijk de emulator sluiten."},
/* pl */ {IDGS_MON_QUIT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_QUIT_DESCRIPTION_SV, "Avsluta emulatorn omedelbart."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RADIX_DESCRIPTION,    N_("Set the default radix to hex, decimal, octal, or binary.  With no\n"
                                            "argument, the current radix is printed.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RADIX_DESCRIPTION_DE, "Setze die Standardzahlenbasis auf Hexadezimal, Dezimal, Oktal oder Bin�r.\n"
                                         "Bei keinem Argument wird die akteulle Zahlenbasis ausgegeben."},
/* fr */ {IDGS_MON_RADIX_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RADIX_DESCRIPTION_HU, "Az alap sz�mrendszer hexadecim�lis, decim�lis, okt�lis vagy bin�ris.\n"
                                         "Param�ter n�lk�l az aktu�lisat mutatja."},
/* it */ {IDGS_MON_RADIX_DESCRIPTION_IT, "Imposta la radice predefinita a esadecimale, decimale, ottale o\n"
                                         "binaria. Senza parametro viene stampata la radice attuale."},
/* nl */ {IDGS_MON_RADIX_DESCRIPTION_NL, "Zet de standaard radix naar hex, decimaal, octaal of binair. Als er\n"
                                         "geen parameter is opgegeven dan zal de huidige radix worden getoond."},
/* pl */ {IDGS_MON_RADIX_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RADIX_DESCRIPTION_SV, "S�tt standardradix till hex, decimal, oktal eller bin�r. Om du\n"
                                         "inte ger n�got argument visas nuvarande radix."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_R_ON_R_OFF_R_TOGGLE,    N_("[on|off|toggle]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_R_ON_R_OFF_R_TOGGLE_DE, "[on|off|toggle]"},
/* fr */ {IDGS_R_ON_R_OFF_R_TOGGLE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_R_ON_R_OFF_R_TOGGLE_HU, ""},  /* fuzzy */
/* it */ {IDGS_R_ON_R_OFF_R_TOGGLE_IT, "[on|off|toggle]"},
/* nl */ {IDGS_R_ON_R_OFF_R_TOGGLE_NL, "[on|off|toggle]"},
/* pl */ {IDGS_R_ON_R_OFF_R_TOGGLE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_R_ON_R_OFF_R_TOGGLE_SV, "[on|off|toggle]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SIDEFX_DESCRIPTION,    N_("Control how monitor generated reads affect memory locations that have\n"
                                             "read side-effects.  If the argument is 'on' then reads may cause\n"
                                             "side-effects.  If the argument is 'off' then reads don't cause\n"
                                             "side-effects.  If the argument is 'toggle' then the current mode is\n"
                                             "switched.  No argument displays the current state.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SIDEFX_DESCRIPTION_DE, "Steuerung, wie die vom Monitor erzeugten Lesezugriffe die Speicherzellen\n"
                                          "mit Lese-Seiteneffekten betreffen.\n"
                                          "Bei Argument \"on\" werden Lese-Seiteneffekte erzeugt.\n"
                                          "Bei Argument \"off\" werden keine Lese-Seiteneffekte erzeugt.\n"
                                          "Bei Argumenrt \"toggle\" wird das Verhalten vertauscht.\n"
                                          "Bei keinem Argument wird das aktuelle Verhalten ausgegebene."},
/* fr */ {IDGS_MON_SIDEFX_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SIDEFX_DESCRIPTION_HU, "Be�ll�tja, hogyan hatnak a monitorprogram mem�ria olvas�sai azokra a\n"
                                          "mem�riahelyekre, amelyeknek olvas�s-mell�khat�sai vannak. Ha a param�ter\n"
                                          "'on', az olvas�sok okozhatnak mell�khat�sokat. Ha a param�ter 'off', akkor pedig\n"
                                          "nem okoznak. Ha a param�ter 'toggle', az aktu�lis m�dot �tkapcsolja.\n"
                                          "Param�ter n�lk�li h�v�s ki�rja az aktu�lis m�dot."},
/* it */ {IDGS_MON_SIDEFX_DESCRIPTION_IT, "Controlla come le letture generate dal monitor affliggono le locazioni\n"
                                          "di memoria che hanno degli effetti collaterali sulla lettura.\n"
                                          "Se il paremetro � 'on', le letture possono causare effetti collaterali.\n"
                                          "Se il parametro � 'off', le letture non causano effetti collaterali.\n"
                                          "Se il parametro � 'scambia', la modalit� attuale viene invertita.\n"
                                          "Nessun parametro mostra lo stato attuale."},
/* nl */ {IDGS_MON_SIDEFX_DESCRIPTION_NL, "Geef aan hoe de lees acties van de monitor de geheugen locaties met lees\n"
                                          "bij-effecten beinvloed. Als de parameter 'off' is dan zullen lees acties\n"
                                          "geen bij-effecten vertonen. Als de parameter 'on' is dan zullen lees\n"
                                          "acties bij-effecten vertonen. Als de parameter 'toggle' is dan zal de\n"
                                          "huidige modus worden geschakeld. Als er geen parameter is dan zal de\n"
                                          "huidige modus worden getoond."},
/* pl */ {IDGS_MON_SIDEFX_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SIDEFX_DESCRIPTION_SV, "Styr hur l�sningar i monitorn p�verkar minnesomr�den som har\n"
                                          "sidoeffekter vid l�sning. Om argumentet �r \"on\" till�ts l�sningar\n"
                                          "att orsaka sidoeffekter. Om argumentet �r \"off\" till�ts inte\n"
                                          "l�sningar att orsaka sidoeffekter. Om argumentet �r \"toggle\" byts\n"
                                          "det nuvarande l�get. Om du inte anger n�gra argument visas\n"
                                          "nuvarande l�ge."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_DISK_COMMANDS,    N_("Disk commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_DISK_COMMANDS_DE, "Disk Kommandos:"},
/* fr */ {IDGS_DISK_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_DISK_COMMANDS_HU, "Lemez parancsok:"},
/* it */ {IDGS_DISK_COMMANDS_IT, "Comandi del disco:"},
/* nl */ {IDGS_DISK_COMMANDS_NL, "Disk commandos:"},
/* pl */ {IDGS_DISK_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_DISK_COMMANDS_SV, "Diskettkommandon:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_DISK_COMMAND,    N_("<disk command>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_DISK_COMMAND_DE, "<Disk Kommando>"},
/* fr */ {IDGS_P_DISK_COMMAND_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_DISK_COMMAND_HU, "<lemez parancs>"},
/* it */ {IDGS_P_DISK_COMMAND_IT, "<comando disco>"},
/* nl */ {IDGS_P_DISK_COMMAND_NL, "<disk commando>"},
/* pl */ {IDGS_P_DISK_COMMAND_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_DISK_COMMAND_SV, "<diskkommando>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_AT_DESCRIPTION,    N_("Perform a disk command on the currently attached disk image on drive 8.\n"
                                         "The specified disk command is sent to the drive's channel #15.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_AT_DESCRIPTION_DE, "F�hre ein Diskkommando auf dem aktuell eingelegtem Diskimage auf Laufwerk 8\n"
                                      "aus.\n"
                                      "Das angegebene Kommando wird auf dem Laufwerks Kommandokanal #15 geschickt."},
/* fr */ {IDGS_MON_AT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_AT_DESCRIPTION_HU, "Lemez parancs v�grehajt�sa az aktu�lisan csatolt lemezen, a 8-as egys�gen.\n"
                                      "A megadott lemez parancsot a meghajt� #15-�s csatorn�j�ra k�ldi."},
/* it */ {IDGS_MON_AT_DESCRIPTION_IT, "Esegue un comando disco sull'immagine disco attualmente presente nel\n"
                                      "drive 8. Il comando specificato � inviato al canale #15 del drive."},
/* nl */ {IDGS_MON_AT_DESCRIPTION_NL, "Doe een disk commando operatie op de huidig gekoppelde disk bestand in drive\n"
                                      "8, Het opgegeven disk commando zal worden gestuurt naar kanaal #15 van de\n"
                                      "drive."},
/* pl */ {IDGS_MON_AT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_AT_DESCRIPTION_SV, "Utf�r ett diskkommando p� den diskettavbildning som �r ansluten till\n"
                                      "enhet 8. Diskkommandot s�nds p� diskettenhetens kanal nummer 15."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_FILENAME_P_DEVICE,    N_("<filename> <device>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_FILENAME_P_DEVICE_DE, "<Dateiname> <Ger�t>"},
/* fr */ {IDGS_P_FILENAME_P_DEVICE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_FILENAME_P_DEVICE_HU, "<f�jln�v> <eszk�z>"},
/* it */ {IDGS_P_FILENAME_P_DEVICE_IT, "<nome del file> <perifericd>"},
/* nl */ {IDGS_P_FILENAME_P_DEVICE_NL, "<bestandsnaam> <apparaat>"},
/* pl */ {IDGS_P_FILENAME_P_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_FILENAME_P_DEVICE_SV, "<filnamn> <enhet>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_ATTACH_DESCRIPTION,    N_("Attach file to device. (device 32 = cart)")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_ATTACH_DESCRIPTION_DE, "Benutze Datei f�r das Ger�t. (Ger�t 32 = Modul)"},
/* fr */ {IDGS_MON_ATTACH_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_ATTACH_DESCRIPTION_HU, "F�jl csatol�sa az eszk�zh�z. (32-es eszk�z = cart)"},
/* it */ {IDGS_MON_ATTACH_DESCRIPTION_IT, "Seleziona il file per la perifica. (periferica 32 = cartuccia)"},
/* nl */ {IDGS_MON_ATTACH_DESCRIPTION_NL, "Koppel bestand aan apparaat. (apparaat 32 = cart"},
/* pl */ {IDGS_MON_ATTACH_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_ATTACH_DESCRIPTION_SV, "Anslut en fil till en enhet. (enhet 32 = insticksmodul)"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS,    N_("\"<filename>\" <device> <address>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_DE, "\"<Dateiname>\" <Ger�t> <Adresse>"},
/* fr */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_HU, "\"<f�jln�v>\" <eszk�z> <c�m>"},
/* it */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_IT, "\"<nome del file>\" <periferica> <indirizzo>"},
/* nl */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_NL, "\"<bestandsnaam>\" <aparraat> <adres>"},
/* pl */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS_SV, "\"<filnamn>\" <enhet> <adress>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BLOAD_DESCRIPTION,    N_("Load the specified file into memory at the specified address.\n"
                                            "If device is 0, the file is read from the file system.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BLOAD_DESCRIPTION_DE, "Lade angegebene Datei in den Speicher an die angegebene Adresse.\n"
                                         "Bei Ger�t 0, wird die Datei vom Dateisystem gelesen."},
/* fr */ {IDGS_MON_BLOAD_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BLOAD_DESCRIPTION_HU, "A megadott f�jl bet�lt�se a mem�ri�ba a megadott c�mre.\n"
                                         "Ha az eszk�z 0, a f�jlt a f�jlrendszerb�l olvassa."},
/* it */ {IDGS_MON_BLOAD_DESCRIPTION_IT, "Carica il file specificato in memoria all'indirizzo specificato.\n"
                                         "Se la periferica � 0, il file � letto dal file system."},
/* nl */ {IDGS_MON_BLOAD_DESCRIPTION_NL, "Laad het opgegeven bestand naar het geheugen met het opgegeven adres.\n"
                                         "Als het apparaat 0 is dan zal het bestand van het bestandssysteem\n"
                                         "worden geladen."},
/* pl */ {IDGS_MON_BLOAD_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BLOAD_DESCRIPTION_SV, "L�s den angivna filen till minnet p� den angivna adressen.\n"
                                         "Om enheten �r 0 l�ses filen fr�n filsystemet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS,    N_("<track> <sector> [<address>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_DE, "<Spur> <Sektor> [<Adresse>]"},
/* fr */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_HU, "<s�v> <szektor> [<c�m>]"},
/* it */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_IT, "<traccia> <settore> [<indirizzo>]"},
/* nl */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_NL, "<spoor> <sector> [<adres>]"},
/* pl */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_TRACK_P_SECTOR_RP_ADDRESS_SV, "<sp�r> <sektor> [<adress>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BLOCK_READ_DESCRIPTION,    N_("Read the block at the specified track and sector.  If an address is\n"
                                                 "specified, the data is loaded into memory.  If no address is given, the\n"
                                                 "data is displayed using the default datatype.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BLOCK_READ_DESCRIPTION_DE, "Lese den Block von angegebener Spur und Sektor. Wenn eine Adresse\n"
                                              "angegeben wurde, lade in auf die entsprechende Adresse. Wenn keine Adresse\n"
                                              "angegeben ist, zeige die Daten im aktuellen Anzeigemodus f�r Daten."},
/* fr */ {IDGS_MON_BLOCK_READ_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BLOCK_READ_DESCRIPTION_HU, "Blokk beolvas�sa a megadott s�vb�l �s szektorb�l.,  Ha egy c�m is meg\n"
                                              "van adva, az adatot a mem�ri�ba t�lti.  Ha az nincs, az\n"
                                              "adatot az alapbe�ll�t�s szerinti form�ban mutatja."},
/* it */ {IDGS_MON_BLOCK_READ_DESCRIPTION_IT, "Legge il blocco dalla traccia e settore specificati. Se � specificato\n"
                                              "un indirizzo, il dato � caricato in memoria. Se non � specificato alcun\n"
                                              "indirizzo, il dato � mostrato usando il tipo di dato predefinito."},
/* nl */ {IDGS_MON_BLOCK_READ_DESCRIPTION_NL, "Lees het blok van de opgegeven spoor en sector. Als er een adres is\n"
                                              "opgegeven dan zal de data naar dat adres in geheugen worden geladen.\n"
                                              "Als er geen adres is opgegeven dan zal de data worder getoond met de\n"
                                              "huidige datasoort."},
/* pl */ {IDGS_MON_BLOCK_READ_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BLOCK_READ_DESCRIPTION_SV, "L�ser blocket p� det angivna sp�ret och sektorn. Om du anger en\n"
                                              "adress l�ses data till minnet. Om du inte anger n�gon adress visas\n"
                                              "inneh�llet enligt standarddatatypen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2,    N_("\"<filename>\" <device> <address1> <address2>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_DE, "\"<Dateiname>\" <Ger�t> <Adresse1> <Adresse2>"},
/* fr */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_HU, "\"<f�jln�v>\" <eszk�z> <c�m1> <c�m2>"},
/* it */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_IT, "\"<nome del file>\" <periferica> <indirizzo1> <indirizzo2>"},
/* nl */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_NL, "\"<bestandsnaam>\" <aparraat> <adres1> <adres2>"},
/* pl */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_P_DEVICE_P_ADDRESS1_P_ADDRESS2_SV, "\"<filnamn>\" <enhet> <adress1> <adress2>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BSAVE_DESCRIPTION,    N_("Save the memory from address1 to address2 to the specified file.\n"
                                            "If device is 0, the file is written to the file system.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BSAVE_DESCRIPTION_DE, "Speichere den Speicherinhalt von Adresse1 zu Adresse2 in die angegebene Datei.\n"
                                         "Wenn das Ger�t 0 angegeben wurde, speichere im Dateisystem."},
/* fr */ {IDGS_MON_BSAVE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BSAVE_DESCRIPTION_HU, "A mem�ria c�m1 �s c�m2 k�z�tti r�sz�nek f�jlba ment�se.\n"
                                         "Ha az eszk�z 0, a f�jl a f�jlrendszerbe ker�l."},
/* it */ {IDGS_MON_BSAVE_DESCRIPTION_IT, "Salva la memoria compresa tra indirizzo1 e indirizzo2 sul file specificato.\n"
                                         "Se la periferica � 0, il file � scritto sul file system."},
/* nl */ {IDGS_MON_BSAVE_DESCRIPTION_NL, "Sla het geheugen tussen adres1 en adres2 op naar het opgegeven bestand.\n"
                                         "Als het apparaat 0 is dan zal het bestand naar het bestandssysteem worden\n"
                                         "geschreven."},
/* pl */ {IDGS_MON_BSAVE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BSAVE_DESCRIPTION_SV, "Spara minnet fr�n adress1 till adress2 till den angivna filen.\n"
                                         "Om enheten �r 0 skrivs filen till filsystemet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS,    N_("<track> <sector> <address>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_DE, "<Spur> <Sektor> <Adress>"},
/* fr */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_HU, "<s�v> <szektor> <c�m>"},
/* it */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_IT, "<traccia> <settore> <indirizzo>"},
/* nl */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_NL, "<spoor> <sector> <adres>"},
/* pl */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_TRACK_P_SECTOR_P_ADDRESS_SV, "<sp�r> <sektor> <adress>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION,    N_("Write a block of data at `address' on the specified track and sector\n"
                                                  "of disk in drive 8.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_DE, "Schreibe einen Datenblock von `Adresse' auf die angegebene Spure und Sektor\n"
                                               "in die Imagedatei von Laufwerk 8."},
/* fr */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_HU, "Egy adat blokk �r�sa `c�m' helyr�l a megadott s�vba �s szektorba\n"
                                               "a 8-as meghajt�n."},
/* it */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_IT, "Scrive il blocco di dati all'indirizzo sulla traccia e settore specificati\n"
                                               "del disco nel drive 8."},
/* nl */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_NL, "Schrijf een data blok van `adres' naar de opgegeven spoor en sector van\n"
                                               "de disk in drive 8."},
/* pl */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_BLOCK_WRITE_DESCRIPTION_SV, "Skriv ett datablock p� \"adress\" till det angivna sp�ret och sektorn\n"
                                               "p� disken i enhet 8."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_DIRECTORY,    N_("<directory>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_DIRECTORY_DE, "<Verzeichnis>"},
/* fr */ {IDGS_P_DIRECTORY_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_DIRECTORY_HU, "<k�nyvt�r>"},
/* it */ {IDGS_P_DIRECTORY_IT, "<directory>"},
/* nl */ {IDGS_P_DIRECTORY_NL, "<directory>"},
/* pl */ {IDGS_P_DIRECTORY_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_DIRECTORY_SV, "<katalog>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_CD_DESCRIPTION,    N_("Change the working directory.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_CD_DESCRIPTION_DE, "Arbeitsverzeichnis �ndern."},
/* fr */ {IDGS_MON_CD_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_CD_DESCRIPTION_HU, "Munkak�nyvt�r megv�ltoztat�sa."},
/* it */ {IDGS_MON_CD_DESCRIPTION_IT, "Cambia la directory di lavoro."},
/* nl */ {IDGS_MON_CD_DESCRIPTION_NL, "Verander de werkdirectory."},
/* pl */ {IDGS_MON_CD_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_CD_DESCRIPTION_SV, "Byt arbetskatalog."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DETACH_DESCRIPTION,    N_("Detach file from device. (device 32 = cart)")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DETACH_DESCRIPTION_DE, "Entferne Datei von Ger�t. (Ger�t 32 = Modul)"},
/* fr */ {IDGS_MON_DETACH_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DETACH_DESCRIPTION_HU, "F�jl lev�laszt�sa az eszk�zr�l. (32-es eszk�z = cart)"},
/* it */ {IDGS_MON_DETACH_DESCRIPTION_IT, "Rimuovi il file dalla periferica. (periferica 32 = cart)"},
/* nl */ {IDGS_MON_DETACH_DESCRIPTION_NL, "Ontkoppel bestand van apparaat. (apparaat 32 = cart)"},
/* pl */ {IDGS_MON_DETACH_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DETACH_DESCRIPTION_SV, "Koppla fr�n filen fr�n enheten. (enhet 32 = insticksmodul)"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_DIRECTORY,    N_("[<directory>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_DIRECTORY_DE, "[<Verzeichnis>]"},
/* fr */ {IDGS_RP_DIRECTORY_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_DIRECTORY_HU, "[<k�nyvt�r>]"},
/* it */ {IDGS_RP_DIRECTORY_IT, "[<directory>]"},
/* nl */ {IDGS_RP_DIRECTORY_NL, "[<directory>]"},
/* pl */ {IDGS_RP_DIRECTORY_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_DIRECTORY_SV, "[<katalog>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DIR_DESCRIPTION,    N_("Display the directory contents.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DIR_DESCRIPTION_DE, "Zeige den Verzeichnisinhalt."},
/* fr */ {IDGS_MON_DIR_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DIR_DESCRIPTION_HU, "A k�nyvt�r tartalm�nak mutat�sa."},
/* it */ {IDGS_MON_DIR_DESCRIPTION_IT, "Mostra il contenuto della directory."},
/* nl */ {IDGS_MON_DIR_DESCRIPTION_NL, "Toon de directory inhoud."},
/* pl */ {IDGS_MON_DIR_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DIR_DESCRIPTION_SV, "Visa katalogens inneh�ll."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS,    N_("\"<filename>\" <device> [<address>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_DE, "\"<Dateiname>\" <Ger�t> [<Adresse>]"},
/* fr */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_HU, "\"<f�jln�v>\" <eszk�z> [<c�m>]"},
/* it */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_IT, "\"<nome del file>\" <periferica> [<indirizzo>]"},
/* nl */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_NL, "\"<bestandsnaam>\" <apparaat> [<adres>]"},
/* pl */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_P_DEVICE_RP_ADDRESS_SV, "\"<filnamn>\" <enhet> [<adress>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_LOAD_DESCRIPTION,    N_("Load the specified file into memory at the specified address. Set BASIC\n"
                                           "pointers appropriately if loaded into computer memory (not all emulators).\n"
                                           "Use (otherwise ignored) two-byte load address from file if no address\n"
                                           "specified.\n"
                                           "If device is 0, the file is read from the file system.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_LOAD_DESCRIPTION_DE, "Lade die angegebene Datei in den Speicher auf die angegebene Adresse.\n"
                                        "Setze den BASIC Zeiger entsprechend, wenn in den Rechnerspeicher geladen \n"
                                        "wurde (nicht bei allen Emulatoren!).\n"
                                        "Verwende (sonst ignoriert) die zwei-Byte Lade-Adresse aus der Datei, wenn\n"
                                        "keine Adresse angegeben wurde.\n"
                                        "Bei Ger�t 0, wird die Datei vom Dateisystem gelade."},
/* fr */ {IDGS_MON_LOAD_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_LOAD_DESCRIPTION_HU, "A megadott f�jl bet�lt�se a megadott c�mre. A BASIC mutat�kat\n"
                                        "a megfelel� m�don be�ll�tja, ha a sz�m�t�g�p mem�ri�ba t�lti az adatot\n"
                                        "(nem minden emul�torn�l). Ha nincs c�m megadva, az am�gy figyelmen hagyott\n"
                                        "k�t b�jtos bet�lt�si c�met haszn�lja a f�jlb�l.\n"
                                        "Ha az eszk�z 0, a f�jlt a f�jlrendszerr�l olvassa."},
/* it */ {IDGS_MON_LOAD_DESCRIPTION_IT, "Carica il file specificato in memroia all'indirizzo specificato. Imposta\n"
                                        "i puntatori del BASIC in modo appropriato se caricato nella memoria\n"
                                        "del computer (non su tutti gli emulatori).\n"
                                        "Usa (altrimenti ignora) l'indirizzo di caricamento su due byte del file\n"
                                        "se non viene specificato alcun indirizzo.\n"
                                        "Se la periferica � 0, il file viene letto dal file system."},
/* nl */ {IDGS_MON_LOAD_DESCRIPTION_NL, "Laad het opgegeven bestand naar geheugen op het opgegeven adres. Zet de BASIC\n"
                                        "start en eind indicators correct als het bestand naar het computer geheugen\n"
                                        "geladen wordt (niet alle emulatoren). De eerste twee bytes van het bestand\n"
                                        "zullen als laad adres worden gebruikt als er geen adres is opgegeven. Als\n"
                                        "het apparaat 0 is dan zal het bestand van het bestandssysteem worden geladen."},
/* pl */ {IDGS_MON_LOAD_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_LOAD_DESCRIPTION_SV, "L�s in den angivna filen i minnet p� den angivna adressen. S�tter\n"
                                        "motsvarande BASIC-pekare vid l�sning till datorminnet (inte alla\n"
                                        "emulatorer). Om ingen adress anges anv�nds tv�bytesadressen i filen (ignoreras annars).\n"
                                        "Om enheten �r 0 l�ses filen fr�n filsystemet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_PWD_DESCRIPTION,    N_("Show current working directory.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_PWD_DESCRIPTION_DE, "Zeige aktuelles Arbeitsverzeichnis."},
/* fr */ {IDGS_MON_PWD_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_PWD_DESCRIPTION_HU, "Munkak�nyvt�r mutat�sa."},
/* it */ {IDGS_MON_PWD_DESCRIPTION_IT, "Mostra la directory di lavoro attuale."},
/* nl */ {IDGS_MON_PWD_DESCRIPTION_NL, "Toon de huidige werk directory."},
/* pl */ {IDGS_MON_PWD_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_PWD_DESCRIPTION_SV, "Visa aktuell arbetskatalog."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SAVE_DESCRIPTION,    N_("Save the memory from address1 to address2 to the specified file.\n"
                                           "Write two-byte load address.\n"
                                           "If device is 0, the file is written to the file system.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SAVE_DESCRIPTION_DE, "Speichere den Speicher von Adresse1 bis Adresse2 in die angegebene Datei.\n"
                                        "Schreibe eine zwei-Byte Lade Adresse.\n"
                                        "Bei Ger�t 0, wird dei Datei in das Dateisystem geschrieben."},
/* fr */ {IDGS_MON_SAVE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SAVE_DESCRIPTION_HU, "A mem�ria c�m1 �s c�m2 k�z�tti tartom�ny�nak f�jlba ment�se.\n"
                                        "A k�tb�jtos bet�lt�si c�met is ki�rja.\n"
                                        "Ha az eszk�z 0, a f�jlt a f�jlrendszeren hozza l�tre."},
/* it */ {IDGS_MON_SAVE_DESCRIPTION_IT, "Salva la memoria compresa tra indirizzo1 e indirizzo2 sul file\n"
                                        "specificato.\n"
                                        "Scrive l'indirizzo di caricamento su due byte.\n"
                                        "Se la periferica � 0, il file � scritto sul file system."},
/* nl */ {IDGS_MON_SAVE_DESCRIPTION_NL, "Sla het geheugen tussen adres1 en adres2 op naar het opgegeven bestand.\n"
                                        "Schrijf een load adres van twee bytes.\n"
                                        "Als het apparaat 0 is dan zal het bestand geschreven worden naar het\n"
                                        "bestandssysteem."},
/* pl */ {IDGS_MON_SAVE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SAVE_DESCRIPTION_SV, "Spara minnet fr�n adress1 till adress2 till den angivna filen.\n"
                                        "Skriv tv�bytes inl�sningsadress.\n"
                                        "Om enheten �r 0 skrivs filen till filsystemet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_OTHER_COMMANDS,    N_("Other commands:")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_OTHER_COMMANDS_DE, "Andere Kommandos:"},
/* fr */ {IDGS_OTHER_COMMANDS_FR, ""},  /* fuzzy */
/* hu */ {IDGS_OTHER_COMMANDS_HU, "Egy�b parancsok:"},
/* it */ {IDGS_OTHER_COMMANDS_IT, "Altri comandi:"},
/* nl */ {IDGS_OTHER_COMMANDS_NL, "Andere commandos:"},
/* pl */ {IDGS_OTHER_COMMANDS_PL, ""},  /* fuzzy */
/* sv */ {IDGS_OTHER_COMMANDS_SV, "Andra kommandon:"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION,    N_("Display the specified number in decimal, hex, octal and binary.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_DE, "Zeige die angegebene Zahl in dezimal, hexadezimal, oktal oder bin�r."},
/* fr */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_HU, "A megadott sz�m ki�r�sa decim�lisan, hexadecim�lisan, okt�lisan �s bin�risan."},
/* it */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_IT, "Mostra il numero specificato in decimale, esadecimale, ottale e binario."},
/* nl */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_NL, "Toon het opgegeven nummer in decimaal, hex, octaal en binair."},
/* pl */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_DISPLAY_NUMBER_DESCRIPTION_SV, "Visa det angivna talet decimalt, hexadecimalt, oktalt och bin�rt."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_CARTFREEZE_DESCRIPTION,    N_("Use cartridge freeze.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_CARTFREEZE_DESCRIPTION_DE, "Erweiterungsmodul Freeze benutzen."},
/* fr */ {IDGS_MON_CARTFREEZE_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_CARTFREEZE_DESCRIPTION_HU, "Cartridge fagyaszt�s haszn�lata."},
/* it */ {IDGS_MON_CARTFREEZE_DESCRIPTION_IT, "Usa freeze della cartuccia."},
/* nl */ {IDGS_MON_CARTFREEZE_DESCRIPTION_NL, "Gebruik cartridge freeze."},
/* pl */ {IDGS_MON_CARTFREEZE_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_CARTFREEZE_DESCRIPTION_SV, "Anv�nd insticksmodulens frysfunktion."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_RP_COMMAND,    N_("[<command>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_RP_COMMAND_DE, "[<Kommando>]"},
/* fr */ {IDGS_RP_COMMAND_FR, ""},  /* fuzzy */
/* hu */ {IDGS_RP_COMMAND_HU, "[<parancs>]"},
/* it */ {IDGS_RP_COMMAND_IT, "[<commando>]"},
/* nl */ {IDGS_RP_COMMAND_NL, "[<commando>]"},
/* pl */ {IDGS_RP_COMMAND_PL, ""},  /* fuzzy */
/* sv */ {IDGS_RP_COMMAND_SV, "[<kommando>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_HELP_DESCRIPTION,    N_("If no argument is given, prints out a list of all available commands\n" 
                                           "If an argument is given, prints out specific help for that command.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_HELP_DESCRIPTION_DE, "Wenn kein Argement angegeben wurde, werden alle Kommandos ausgegeben.\n"
                                        "Mit Argument, zeige die Hilfe f�r das angegebene Kommand."},
/* fr */ {IDGS_MON_HELP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_HELP_DESCRIPTION_HU, "Ha nincs param�ter, az �sszes l�tez� parancs list�j�t mutatja.\n"
                                        "Ha van, az adott parancshoz mutatja a s�g�t."},
/* it */ {IDGS_MON_HELP_DESCRIPTION_IT, "Se non viene specificato alcun argomeno, stamapa la lista di tutti i\n"
                                        "comandi disponibili.\n"
                                        "Se viene specificato un argomento, stampa l'aiuto relativo a quel\n"
                                        "comando."},
/* nl */ {IDGS_MON_HELP_DESCRIPTION_NL, "Als er geen parameter is opgegeven dan zal een lijst van alle commandos\n"
                                        "worden weergegeven. Als er een parameter is opgegeven dan zal de help voor\n"
                                        "het opgegeven command worden weergegeven."},
/* pl */ {IDGS_MON_HELP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_HELP_DESCRIPTION_SV, "Om du inte anger n�gra argument visas alla tillg�ngliga kommandon.\n"
                                        "Om du anger ett argument visas hj�lp f�r det specifika kommandot."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_STRING,    N_("\"<string>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_STRING_DE, "\"<Zeichenkette>\""},
/* fr */ {IDGS_QP_STRING_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_STRING_HU, "\"<sztring>\""},
/* it */ {IDGS_QP_STRING_IT, "\"<stringa>\""},
/* nl */ {IDGS_QP_STRING_NL, "\"<string>\""},
/* pl */ {IDGS_QP_STRING_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_STRING_SV, "\"<str�ng>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_KEYBUF_DESCRIPTION,    N_("Put the specified string into the keyboard buffer.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_KEYBUF_DESCRIPTION_DE, "Definierte Eingabe in Tastaturpuffer bereitstellen."},
/* fr */ {IDGS_MON_KEYBUF_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_KEYBUF_DESCRIPTION_HU, "A megadott sztring elhelyez�se a billenty�zet pufferben."},
/* it */ {IDGS_MON_KEYBUF_DESCRIPTION_IT, "Metti la stringa specificata nel buffer di tastiera."},
/* nl */ {IDGS_MON_KEYBUF_DESCRIPTION_NL, "Plaats de opgegeven string in de toetsenbordbuffer."},
/* pl */ {IDGS_MON_KEYBUF_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_KEYBUF_DESCRIPTION_SV, "L�gg den angivna str�ngen i tangentbordsbufferten."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_PLAYBACK_DESCRIPTION,    N_("Monitor commands from the specified file are read and executed.  This\n"
                                               "command stops at the end of file or when a STOP command is read.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_PLAYBACK_DESCRIPTION_DE, "Monitor Kommandos aus der angegebenen Datei werden gelesen und ausgef�hrt.\n"
                                            "Dieses Kommando stoppt, wenn das Ende der Datei erreicht wurde, oder das\n"
                                            "Kommando STOP gelesen wurde."},
/* fr */ {IDGS_MON_PLAYBACK_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_PLAYBACK_DESCRIPTION_HU, "A monitor parancsokat beolvassa �s v�grehajtja a megadott f�jlb�l.\n"
                                            "A f�jl v�g�ig halad, vagy egy beolvasott STOP parancsig."},
/* it */ {IDGS_MON_PLAYBACK_DESCRIPTION_IT, "I comandi del monitor sono letti ed eseguiti dal file specificato. Questo\n"
                                            "comando si arresta alla fine del file o quando viene letto il comando\n"
                                            "STOP."},
/* nl */ {IDGS_MON_PLAYBACK_DESCRIPTION_NL, "Monitor commandos van het opgegeven bestand zullen worden gelezen en\n"
                                            "uitgevoerd. Dit commando stopt aan het einde van het bestand of wanneer\n"
                                            "een STOP commando wordt gelezen."},
/* pl */ {IDGS_MON_PLAYBACK_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_PLAYBACK_DESCRIPTION_SV, "Monitorkommandon l�ses in och k�rs fr�n den angivna filen. Kommandot\n"
                                            "stoppar vid filslut eller n�r ett STOP-kommando l�ses."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_EXPRESSION,    N_("<expression>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_EXPRESSION_DE, "<Bedingung>"},
/* fr */ {IDGS_P_EXPRESSION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_EXPRESSION_HU, "<kifejez�s>"},
/* it */ {IDGS_P_EXPRESSION_IT, "<espressione>"},
/* nl */ {IDGS_P_EXPRESSION_NL, "<expressie>"},
/* pl */ {IDGS_P_EXPRESSION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_EXPRESSION_SV, "<uttryck>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_PRINT_DESCRIPTION,    N_("Evaluate the specified expression and output the result.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_PRINT_DESCRIPTION_DE, "Evaluiere den angegebenen Ausdruck und zeige das Ergebnis."},
/* fr */ {IDGS_MON_PRINT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_PRINT_DESCRIPTION_HU, "Ki�rt�keli a megadott kifejez�st �s ki�rja az eredm�nyt."},
/* it */ {IDGS_MON_PRINT_DESCRIPTION_IT, "Calcola l'espressione specificata e mostra il risultato."},
/* nl */ {IDGS_MON_PRINT_DESCRIPTION_NL, "Evalueer de opgegeven expressie en toon het resultaat."},
/* pl */ {IDGS_MON_PRINT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_PRINT_DESCRIPTION_SV, "Utv�rdera det angivna uttrycket och visa resultatet."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RECORD_DESCRIPTION,    N_("After this command, all commands entered are written to the specified\n"
                                             "file until the STOP command is entered.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RECORD_DESCRIPTION_DE, "Nach diesem Kommando werden alle eingebenen Kommands in die angegebene\n"
                                          "Datei gespeicher solang bis das Kommando STOP eingegeben wurde."},
/* fr */ {IDGS_MON_RECORD_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RECORD_DESCRIPTION_HU, "Ezut�n a parancs ut�n az �sszes parancsot a megadott f�jlba\n"
                                          "�rja, am�g STOP parancs nem �rkezik."},
/* it */ {IDGS_MON_RECORD_DESCRIPTION_IT, "Dopo questo comando, tutti i comandi inseriti saranno scritti sul file\n"
                                          "specificato fino a quando verr� inserito il comando STOP."},
/* nl */ {IDGS_MON_RECORD_DESCRIPTION_NL, "Na dit commando zullen alle opgegeven commandos worden geschreven naar\n"
                                          "het opgegeven bestand totdat het STOP command wordt opgegeven."},
/* pl */ {IDGS_MON_RECORD_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RECORD_DESCRIPTION_SV, "Efter detta kommando kommer alla kommandon som anges att skrivas till\n"
                                          "den angivna filen tills ett STOP-kommando anges."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_RESOURCE,    N_("\"<resource>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_RESOURCE_DE, "\"<Ressource>\""},
/* fr */ {IDGS_QP_RESOURCE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_RESOURCE_HU, "\"<er�forr�s>\""},
/* it */ {IDGS_QP_RESOURCE_IT, "\"<risorsa>\""},
/* nl */ {IDGS_QP_RESOURCE_NL, "\"<resource>\""},
/* pl */ {IDGS_QP_RESOURCE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_RESOURCE_SV, "\"<resurs>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RESOURCEGET_DESCRIPTION,    N_("Displays the value of the resource.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RESOURCEGET_DESCRIPTION_DE, "Zeige den Werde der Ressource."},
/* fr */ {IDGS_MON_RESOURCEGET_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RESOURCEGET_DESCRIPTION_HU, "Az er�forr�s �rt�k�nek ki�r�sa."},
/* it */ {IDGS_MON_RESOURCEGET_DESCRIPTION_IT, "Mostra il valore della risorsa."},
/* nl */ {IDGS_MON_RESOURCEGET_DESCRIPTION_NL, "Toont de waarde van de resource."},
/* pl */ {IDGS_MON_RESOURCEGET_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RESOURCEGET_DESCRIPTION_SV, "Visar v�rdet f�r resursen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_RESOURCE_QP_VALUE,    N_("\"<resource>\" \"<value>\"")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_RESOURCE_QP_VALUE_DE, "\"<Ressource>\" \"<Wert>\""},
/* fr */ {IDGS_QP_RESOURCE_QP_VALUE_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_RESOURCE_QP_VALUE_HU, "\"<er�forr�s>\" \"<�rt�k>\""},
/* it */ {IDGS_QP_RESOURCE_QP_VALUE_IT, "\"<risorsa>\" \"<valore>\""},
/* nl */ {IDGS_QP_RESOURCE_QP_VALUE_NL, "\"<resource>\" \"<waarde>\""},
/* pl */ {IDGS_QP_RESOURCE_QP_VALUE_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_RESOURCE_QP_VALUE_SV, "\"<resurs>\" \"<v�rde>\""},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_RESOURCESET_DESCRIPTION,    N_("Sets the value of the resource.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_RESOURCESET_DESCRIPTION_DE, "Setze den Werde der Ressource."},
/* fr */ {IDGS_MON_RESOURCESET_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_RESOURCESET_DESCRIPTION_HU, "Az er�forr�s �rt�k�nek be�ll�t�sa."},
/* it */ {IDGS_MON_RESOURCESET_DESCRIPTION_IT, "Imposta il valore della risorsa."},
/* nl */ {IDGS_MON_RESOURCESET_DESCRIPTION_NL, "Plaats de waarde in de resource."},
/* pl */ {IDGS_MON_RESOURCESET_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_RESOURCESET_DESCRIPTION_SV, "St�ller in ett v�rde f�r resursen."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_STOP_DESCRIPTION,    N_("Stop recording commands.  See `record'.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_STOP_DESCRIPTION_DE, "Stoppe die Aufnahme von Kommandos. Siehe auch `Record'."},
/* fr */ {IDGS_MON_STOP_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_STOP_DESCRIPTION_HU, "A parancsok r�gz�t�s�nek befejez�se.  L�sd `record'"},
/* it */ {IDGS_MON_STOP_DESCRIPTION_IT, "Ferma il comando di registrazione. Guarda `registra'."},
/* nl */ {IDGS_MON_STOP_DESCRIPTION_NL, "Stop de opname van commandos. Zie 'record'."},
/* pl */ {IDGS_MON_STOP_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_STOP_DESCRIPTION_SV, "Sluta spela in kommandon. Se \"record\"."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_QP_FILENAME_RP_FORMAT,    N_("\"<filename>\" [<format>]")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_QP_FILENAME_RP_FORMAT_DE, "\"<Dateiname>\" [<Format>]"},
/* fr */ {IDGS_QP_FILENAME_RP_FORMAT_FR, ""},  /* fuzzy */
/* hu */ {IDGS_QP_FILENAME_RP_FORMAT_HU, "\"<f�jln�v>\" [<form�tum>]"},
/* it */ {IDGS_QP_FILENAME_RP_FORMAT_IT, "\"<nome del file>\" [<formato>]"},
/* nl */ {IDGS_QP_FILENAME_RP_FORMAT_NL, "\"<bestandsnaam>\" [<formaat>]"},
/* pl */ {IDGS_QP_FILENAME_RP_FORMAT_PL, ""},  /* fuzzy */
/* sv */ {IDGS_QP_FILENAME_RP_FORMAT_SV, "\"<filnamn>\" [<format>]"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_SCREENSHOT_DESCRIPTION,    N_("Take a screenshot. Format is:\n"
                                                 "default = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_SCREENSHOT_DESCRIPTION_DE, "Erzeuge Screenshot. Format ist:\n"
                                              "Standard = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* fr */ {IDGS_MON_SCREENSHOT_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_SCREENSHOT_DESCRIPTION_HU, "K�perny�k�p k�sz�t�se. A form�tum lehet:\n"
                                              "alap = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* it */ {IDGS_MON_SCREENSHOT_DESCRIPTION_IT, "Cattura schermo. Il formato �:\n"
                                              "predefinito = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* nl */ {IDGS_MON_SCREENSHOT_DESCRIPTION_NL, "Sla een schermafdruk op. Formaat is:\n"
                                              "standaard = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
/* pl */ {IDGS_MON_SCREENSHOT_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_SCREENSHOT_DESCRIPTION_SV, "Ta en sk�rmbild. Format �r:\n"
                                              "standard = BMP, 1 = PCX, 2 = PNG, 3 = GIF, 4 = IFF."},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_P_COMMAND,    N_("<command>")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_P_COMMAND_DE, "<Kommando>"},
/* fr */ {IDGS_P_COMMAND_FR, ""},  /* fuzzy */
/* hu */ {IDGS_P_COMMAND_HU, "<parancs>"},
/* it */ {IDGS_P_COMMAND_IT, "<commando>"},
/* nl */ {IDGS_P_COMMAND_NL, "<commando>"},
/* pl */ {IDGS_P_COMMAND_PL, ""},  /* fuzzy */
/* sv */ {IDGS_P_COMMAND_SV, "<kommando>"},
#endif

/* monitor/mon_command.c */
/* en */ {IDGS_MON_TAPECTRL_DESCRIPTION,    N_("Control the datasette. Valid commands:\n"
                                               "0 = stop, 1 = start, 2 = forward, 3 = rewind, 4 = record,\n"
                                               "5 = reset, 6 = reset counter.")},
#ifdef HAS_TRANSLATION
/* de */ {IDGS_MON_TAPECTRL_DESCRIPTION_DE, "Steuere die Datasette. G�ltige Kommandos sind:\n"
                                            "0 = Stop, 1 = Start, 2 = Forw�rts, 3 = R�ckw�rts, 4 = Aufnahme,\n"
                                            "5 = Reset, 6 = Reset Z�hler."},
/* fr */ {IDGS_MON_TAPECTRL_DESCRIPTION_FR, ""},  /* fuzzy */
/* hu */ {IDGS_MON_TAPECTRL_DESCRIPTION_HU, "Szalagos egys�g vez�rl�se. �rv�nyes parancsok:\n"
                                            "0 = start, 1 = ind�t, 2 = el�re, 3 = vissza, 4 = felv�tel,\n"
                                            "5 = �jraind�t, 6 = sz�ml�l�t null�z"},
/* it */ {IDGS_MON_TAPECTRL_DESCRIPTION_IT, "Controlla il registratore. Comandi validi:\n"
                                            "0 = ferma, 1 = avvia, 2 = avanti, 3 = indietro, 4 = registra,\n"
                                            "5 = reset, 6 = reset contatore."},
/* nl */ {IDGS_MON_TAPECTRL_DESCRIPTION_NL, "Geef datasetta commandos. Geldige commandos:\n"
                                            "0 = stop, 1 = start, 2 = vooruit, 3 = terug, 4 = opnemen,\n"
                                            "5 = reset, 6 = reset teller."},
/* pl */ {IDGS_MON_TAPECTRL_DESCRIPTION_PL, ""},  /* fuzzy */
/* sv */ {IDGS_MON_TAPECTRL_DESCRIPTION_SV, "Styr bandspelaren. Giltiga kommandon:\n"
                                            "0 = stoppa, 1 = starta, 2 = spola fram�t, 3 = spola bak�t,\n"
                                            "4 = spela in, 5 = �terst�ll, 6 = nollst�ll r�knare."},
#endif

/* ------------------------ COMMAND LINE OPTION STRINGS -------------------- */

/* autostart.c */
/* en */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON,    N_("On autostart, use the 'RUN' command with a colon, i.e., 'RUN:'")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_DE, "Bei Autostart soll 'RUN' mit Doppelpunkt ausgef�hrt werden ('RUN:')"},
/* fr */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_HU, "Automatikus ind�t�sn�l a 'RUN' parancs kett�sponttal, vagyis 'RUN:'"},
/* it */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_IT, "All'avvio automatico, usa il comando 'RUN' con i due punti; cio� 'RUN:'"},
/* nl */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_NL, "Bij het autostarten, gebruik het 'RUN' commando met een dubbele punt, dus "
                                              "'RUN:'"},
/* pl */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_AUTOSTARTWITHCOLON_SV, "Vid autostart, anv�nd \"RUN\"-kommando med kolon, dvs. \"RUN:\""},
#endif

/* autostart.c */
/* en */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON,    N_("On autostart, do not use the 'RUN' command with a colon; i.e., 'RUN'")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_DE, "Bei Autostart soll 'RUN' ohne Doppelpunkt ausgef�hrt werden ('RUN')"},
/* fr */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_HU, "Automatikus ind�t�snak a 'RUN' parancs kett�spont n�lk�l, vagyis 'RUN'"},
/* it */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_IT, "All'avvio automatico, non usare il comando 'RUN' con i due punti; cio� 'RUN'"},
/* nl */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_NL, "Bij het autostarten, gebruik het 'RUN' commando niet met een dubbele "
                                               "punt, dus 'RUN'"},
/* pl */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_AUTOSTARTWITHCOLON_SV, "Vid autostart, anv�nd inte \"RUN\"-kommando med kolon, dvs. \"RUN\""},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   c64/psid.c, plus4/plus4-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_USE_PAL_SYNC_FACTOR,    N_("Use PAL sync factor")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_PAL_SYNC_FACTOR_DE, "PAL Faktor zum Synchronisieren benutzen"},
/* fr */ {IDCLS_USE_PAL_SYNC_FACTOR_FR, "Utiliser PAL"},
/* hu */ {IDCLS_USE_PAL_SYNC_FACTOR_HU, "PAL szinkron faktor haszn�lata"},
/* it */ {IDCLS_USE_PAL_SYNC_FACTOR_IT, "Usa il fattore di sincronizzazione PAL"},
/* nl */ {IDCLS_USE_PAL_SYNC_FACTOR_NL, "Gebruik PAL-synchronisatiefaktor"},
/* pl */ {IDCLS_USE_PAL_SYNC_FACTOR_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_PAL_SYNC_FACTOR_SV, "Anv�nd PAL-synkfaktor"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   c64/psid.c, plus4/plus4-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_USE_NTSC_SYNC_FACTOR,    N_("Use NTSC sync factor")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_NTSC_SYNC_FACTOR_DE, "NTSC Sync Faktor benutzen"},
/* fr */ {IDCLS_USE_NTSC_SYNC_FACTOR_FR, "Utiliser NTSC"},
/* hu */ {IDCLS_USE_NTSC_SYNC_FACTOR_HU, "NTSC szinkron faktor haszn�lata"},
/* it */ {IDCLS_USE_NTSC_SYNC_FACTOR_IT, "Usa il fattore di sincronizzazione NTSC"},
/* nl */ {IDCLS_USE_NTSC_SYNC_FACTOR_NL, "Gebruik NTSC-synchronisatiefaktor"},
/* pl */ {IDCLS_USE_NTSC_SYNC_FACTOR_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_NTSC_SYNC_FACTOR_SV, "Anv�nd NTSC-synkfaktor"},
#endif

/* c128/c128-cmdline-options.c, c128/functionrom.c,
   c64/c64-cmdline-options.c, c64/georam.c,
   c64/ramcart.c, c64/reu.c, c64/cart/c64cart.c,
   c64/cart/ide64.c, cbm2/cbm2-cmdline-options.c,
   drive/iec/iec-cmdline-options.c, drive/iec128dcr/iec128dcr-cmdline-options.c,
   drive/ieee/ieee-cmdline-options.c, drive/tcbm/tcbm-cmdline-options.c,
   fsdevice/fsdevice-cmdline-options.c, pet/pet-cmdline-options.c,
   plus4/plus4-cmdline-options.c, printerdrv/driver-select.c,
   printerdrv/output-select.c, printerdrv/output-text.c,
   rs232drv/rs232drv.c, vic20/vic20-cmdline-options.c,
   vic20/vic20cartridge.c, video/video-cmdline-options.c,
   fliplist.c, initcmdline.c, log.c, sound.c, c64/plus60k.c,
   c64/c64_256k.c, pet/petreu.c, c64/plus256k.c */
/* en */ {IDCLS_P_NAME,    N_("<name>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_NAME_DE, "<Name>"},
/* fr */ {IDCLS_P_NAME_FR, "<nom>"},
/* hu */ {IDCLS_P_NAME_HU, "<n�v>"},
/* it */ {IDCLS_P_NAME_IT, "<nome>"},
/* nl */ {IDCLS_P_NAME_NL, "<naam>"},
/* pl */ {IDCLS_P_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_NAME_SV, "<namn>"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_INT_KERNEL_NAME,    N_("Specify name of international Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_INT_KERNEL_NAME_DE, "Dateiname des internationalen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_INT_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal internationale"},
/* hu */ {IDCLS_SPECIFY_INT_KERNEL_NAME_HU, "Adja meg a nemzetk�zi Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_INT_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal internazionale"},
/* nl */ {IDCLS_SPECIFY_INT_KERNEL_NAME_NL, "Geef de naam van het internationaal Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_INT_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_INT_KERNEL_NAME_SV, "Ange namn p� internationell kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_DE_KERNEL_NAME,    N_("Specify name of German Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_DE_KERNEL_NAME_DE, "Dateiname des deutschen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_DE_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal allemande"},
/* hu */ {IDCLS_SPECIFY_DE_KERNEL_NAME_HU, "Adja meg a n�met Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_DE_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal tedesca"},
/* nl */ {IDCLS_SPECIFY_DE_KERNEL_NAME_NL, "Geef de naam van het Duitse Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_DE_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_DE_KERNEL_NAME_SV, "Ange namn p� tysk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_FI_KERNEL_NAME,    N_("Specify name of Finnish Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FI_KERNEL_NAME_DE, "Dateiname des finnischen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_FI_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal finlandaise"},
/* hu */ {IDCLS_SPECIFY_FI_KERNEL_NAME_HU, "Adja meg a finn Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_FI_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal finlandese"},
/* nl */ {IDCLS_SPECIFY_FI_KERNEL_NAME_NL, "Geef de naam van het Fins Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_FI_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FI_KERNEL_NAME_SV, "Ange namn p� finsk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_FR_KERNEL_NAME,    N_("Specify name of French Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FR_KERNEL_NAME_DE, "Dateiname des franz�sischen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_FR_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal fran�aise"},
/* hu */ {IDCLS_SPECIFY_FR_KERNEL_NAME_HU, "Adja meg a francia Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_FR_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal francese"},
/* nl */ {IDCLS_SPECIFY_FR_KERNEL_NAME_NL, "Geef de naam van het Frans Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_FR_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FR_KERNEL_NAME_SV, "Ange namn p� fransk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_IT_KERNEL_NAME,    N_("Specify name of Italian Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_IT_KERNEL_NAME_DE, "Dateiname des italienischen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_IT_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal italienne"},
/* hu */ {IDCLS_SPECIFY_IT_KERNEL_NAME_HU, "Adja meg az olasz Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_IT_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal italiana"},
/* nl */ {IDCLS_SPECIFY_IT_KERNEL_NAME_NL, "Geef de naam van het Italiaans Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_IT_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_IT_KERNEL_NAME_SV, "Ange namn p� italiensk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_NO_KERNEL_NAME,    N_("Specify name of Norwegian Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_NO_KERNEL_NAME_DE, "Dateiname des norwegischen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_NO_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal norv�gienne"},
/* hu */ {IDCLS_SPECIFY_NO_KERNEL_NAME_HU, "Adja meg a norv�g Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_NO_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal norvegese"},
/* nl */ {IDCLS_SPECIFY_NO_KERNEL_NAME_NL, "Geef de naam van het Noors Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_NO_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_NO_KERNEL_NAME_SV, "Ange namn p� norsk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SV_KERNEL_NAME,    N_("Specify name of Swedish Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SV_KERNEL_NAME_DE, "Dateiname des schwedischen Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_SV_KERNEL_NAME_FR, "Sp�cifier le nom de l'image Kernal su�doise"},
/* hu */ {IDCLS_SPECIFY_SV_KERNEL_NAME_HU, "Adja meg s sv�d Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_SV_KERNEL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal svedese"},
/* nl */ {IDCLS_SPECIFY_SV_KERNEL_NAME_NL, "Geef de naam van het Zweeds Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_SV_KERNEL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SV_KERNEL_NAME_SV, "Ange namn p� svensk kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW,    N_("Specify name of BASIC ROM image (lower part)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_DE, "Dateiname des Basic ROMs (oberer Adressbereich"},
/* fr */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_FR, "Sp�cifier le nom de l'image BASIC ROM (partie basse)"},
/* hu */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_HU, "Adja meg a BASIC ROM k�pm�s nev�t (als� r�sz)"},
/* it */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_IT, "Specifica il nome dell'immagine della ROM del BASIC (parte inferiore)"},
/* nl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_NL, "Geef de naam van het BASIC-ROM-bestand (laag gedeelte)"},
/* pl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BASIC_ROM_NAME_LOW_SV, "Ange namn p� BASIC ROM-avbildning (nedre delen)"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH,    N_("Specify name of BASIC ROM image (higher part)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_DE, "Dateiname des Basic ROMs (unterer Adressbereich)"},
/* fr */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_FR, "Sp�cifier le nom de l'image BASIC ROM (partie haute)"},
/* hu */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_HU, "Adja meg a BASIC ROM k�pm�s nev�t (fels� r�sz)"},
/* it */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_IT, "Specifica il nome dell'immagine della ROM del BASIC (parte superiore)"},
/* nl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_NL, "Geef de naam van het BASIC-ROM-bestand (hoog gedeelte)"},
/* pl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HIGH_SV, "Ange namn p� BASIC ROM-avbildning (�vre delen)"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME,    N_("Specify name of international character generator ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_DE, "Dateiname des internationalen Zeichensatz ROMs"},
/* fr */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_FR, "Sp�cifier le nom de l'image du g�n�rateur de caract�res internationaux"},
/* hu */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_HU, "Adja meg a nemzetk�zi karaktergener�tor ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del generatore di caratteri "
                                                 "internazionale"},
/* nl */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_NL, "Geef de naam van het internationaal CHARGEN-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_INT_CHARGEN_ROM_NAME_SV, "Ange namn p� internationell teckengenerator-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME,    N_("Specify name of German character generator ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_DE, "Dateiname des deutschen Zeichensatz ROMs"},
/* fr */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_FR, "Sp�cifier le nom de l'image du g�n�rateur de caract�res allemands"},
/* hu */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_HU, "Adja meg a n�met karaktergener�tor ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del generatore di caratteri "
                                                "tedesco"},
/* nl */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_NL, "Geef de naam van het Duitse CHARGEN-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_DE_CHARGEN_ROM_NAME_SV, "Ange namn p� tysk teckengenerator-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME,    N_("Specify name of French character generator ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_DE, "Dateiname des franz�sischen Zeichensatz ROMs"},
/* fr */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_FR, "Sp�cifier le nom de l'image du g�n�rateur de caract�res fran�ais"},
/* hu */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_HU, "Adja meg a francia karaktergener�tor ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del generatore di caratteri "
                                                "francese"},
/* nl */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_NL, "Geef de naam van het Franse CHARGEN-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FR_CHARGEN_ROM_NAME_SV, "Ange namn p� fransk teckengenerator-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME,    N_("Specify name of Swedish character generator ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_DE, "Dateiname des swedischen Zeichensatz ROMs"},
/* fr */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_FR, "Sp�cifier le nom de l'image du g�n�rateur de caract�res su�dois"},
/* hu */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_HU, "Adja meg a sv�d karaktergener�tor ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del generatore di caratteri "
                                                "svedese"},
/* nl */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_NL, "Geef de naam van het Zweedse CHARGEN-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SV_CHARGEN_ROM_NAME_SV, "Ange namn p� svensk teckengenerator-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME,    N_("Specify name of C64 mode Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_DE, "Dateiname des Kernal ROMs im C64 Modus"},
/* fr */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_FR, "Sp�cifier le nom de l'image Kernal du mode C64"},
/* hu */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_HU, "Adja meg a C64 m�d� Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal in modalit� C64"},
/* nl */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_NL, "Geef de naam van het Kernal-ROM-bestand voor de C64-modus"},
/* pl */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_C64_MODE_KERNAL_NAME_SV, "Ange namn p� C64-kernal-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME,    N_("Specify name of C64 mode BASIC ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_DE, "Dateiname des Basic ROMs im C64 Modus"}, 
/* fr */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_FR, "Sp�cifier le nom de l'image BASIC ROM du mode C64"},
/* hu */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_HU, "Adja meg a C64 m�d� BASIC ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_IT, "Specifica il nome dell'immagine della ROM in modalit� C64"},
/* nl */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_NL, "Geef de naam van het BASIC-ROM-bestand voor de C64-modus"},
/* pl */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_C64_MODE_BASIC_NAME_SV, "Ange namn p� C64-BASIC-ROM-avbildning"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_ENABLE_EMULATOR_ID,    N_("Enable emulator identification")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_EMULATOR_ID_DE, "Emulatoridentifikation aktivieren"},
/* fr */ {IDCLS_ENABLE_EMULATOR_ID_FR, "Activer l'identification de l'�mulateur"},
/* hu */ {IDCLS_ENABLE_EMULATOR_ID_HU, "Emul�tor azonos�t� enged�lyez�se"},
/* it */ {IDCLS_ENABLE_EMULATOR_ID_IT, "Attiva l'identificazione dell'emulatore"},
/* nl */ {IDCLS_ENABLE_EMULATOR_ID_NL, "Activeer emulatoridentificatie"},
/* pl */ {IDCLS_ENABLE_EMULATOR_ID_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_EMULATOR_ID_SV, "Aktivera emulatoridentifiering"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_DISABLE_EMULATOR_ID,    N_("Disable emulator identification")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_EMULATOR_ID_DE, "Emulatoridentifikation deaktivieren"},
/* fr */ {IDCLS_DISABLE_EMULATOR_ID_FR, "D�sactiver l'identification de l'�mulateur"},
/* hu */ {IDCLS_DISABLE_EMULATOR_ID_HU, "Emul�tor azonos�t� tilt�sa"},
/* it */ {IDCLS_DISABLE_EMULATOR_ID_IT, "Disattiva l'identificazione dell'emulatore"},
/* nl */ {IDCLS_DISABLE_EMULATOR_ID_NL, "Emulatoridentificatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_EMULATOR_ID_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_EMULATOR_ID_SV, "Inaktivera emulatoridentifiering"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_ENABLE_IEE488,    N_("Enable the IEEE488 interface emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEE488_DE, "IEEE488 Schnittstellenemulation aktivieren"},
/* fr */ {IDCLS_ENABLE_IEE488_FR, "Activer l'interface d'�mulation IEEE488"},
/* hu */ {IDCLS_ENABLE_IEE488_HU, "IEEE488 interf�sz emul�ci� enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEE488_IT, "Attiva l'emulazione dell'interfaccia IEEE488"},
/* nl */ {IDCLS_ENABLE_IEE488_NL, "Activeer de IEEE488-interface-emulatie"},
/* pl */ {IDCLS_ENABLE_IEE488_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEE488_SV, "Aktivera emulering av IEEE488-gr�nssnitt"},
#endif

/* c128/c128-cmdline-options.c */
/* en */ {IDCLS_DISABLE_IEE488,    N_("Disable the IEEE488 interface emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEE488_DE, "IEEE488 Schnittstellenemulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEE488_FR, "D�sactiver l'interface d'�mulation IEEE488"},
/* hu */ {IDCLS_DISABLE_IEE488_HU, "IEEE488 interf�sz emul�ci� tilt�sa"},
/* it */ {IDCLS_DISABLE_IEE488_IT, "Disattiva l'emulazione dell'interfaccia IEEE488"},
/* nl */ {IDCLS_DISABLE_IEE488_NL, "De IEEE488-interface-emulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEE488_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEE488_SV, "Inaktivera emulering av IEEE488-gr�nssnitt"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c */
/* en */ {IDCLS_P_REVISION,    N_("<revision>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_REVISION_DE, "<Revision>"},
/* fr */ {IDCLS_P_REVISION_FR, "<revision>"},
/* hu */ {IDCLS_P_REVISION_HU, "<v�ltozat>"},
/* it */ {IDCLS_P_REVISION_IT, "<revisione>"},
/* nl */ {IDCLS_P_REVISION_NL, "<revisie>"},
/* pl */ {IDCLS_P_REVISION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_REVISION_SV, "<utg�va>"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c */
/* en */ {IDCLS_PATCH_KERNAL_TO_REVISION,    N_("Patch the Kernal ROM to the specified <revision>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PATCH_KERNAL_TO_REVISION_DE, "Kernal ROM auf angegenbene Revision �ndern"},
/* fr */ {IDCLS_PATCH_KERNAL_TO_REVISION_FR, "Mettre � jour le ROM Kernal � la <revision> sp�cifi�e"},
/* hu */ {IDCLS_PATCH_KERNAL_TO_REVISION_HU, "A Kernal ROM �t�r�sa adott <v�ltozatra>"},
/* it */ {IDCLS_PATCH_KERNAL_TO_REVISION_IT, "Fai il patch della ROM del Kernal alla <revisione> specificata"},
/* nl */ {IDCLS_PATCH_KERNAL_TO_REVISION_NL, "Verbeter de Kernal-ROM naar de opgegeven <revisie>"},
/* pl */ {IDCLS_PATCH_KERNAL_TO_REVISION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PATCH_KERNAL_TO_REVISION_SV, "Patcha kernal-ROM till angiven <utg�va>"},
#endif

#ifdef HAVE_RS232
/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU,    N_("Enable the $DE** ACIA RS232 interface emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_DE, "$DE** ACIA RS232 Schnittstellenemulation aktivieren"},
/* fr */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_FR, "Activer l'�mulation de l'interface $DE** ACIA RS232"},
/* hu */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_HU, "$DE** ACIA RS232 interf�sz emul�ci� enged�lyez�se"},
/* it */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_IT, "Attiva l'emulazione dell'interfaccia RS232 su ACIA a $DE**"},
/* nl */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_NL, "Activeer de $DE** ACIA RS232-interface-emulatie"},
/* pl */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DEXX_ACIA_RS232_EMU_SV, "Aktivera $DE** ACIA RS232-gr�nssnittsemulering"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU,    N_("Disable the $DE** ACIA RS232 interface emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_DE, "$DE** ACIA RS232 Schnittstellenemulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_FR, "D�sactiver l'�mulation de l'interface $DE** ACIA RS232"},
/* hu */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_HU, "$DE** ACIA RS232 interf�sz emul�ci� tilt�sa"},
/* it */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_IT, "Disattiva l'emulazione dell'interfaccia RS232 su ACIA a $DE**"},
/* nl */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_NL, "De $DE** ACIA RS232 interface emulatie afsluiten"},
/* pl */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DEXX_ACIA_RS232_EMU_SV, "Inaktivera $DE** ACIA-RS232-gr�nssnittsemulering"},
#endif
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   c64/psid.c, cbm2/cbm2-cmdline-options.c,
   pet/pet-cmdline-options.c, plus4/plus4-cmdline-options.c,
   vdc/vdc-cmdline-options.c, vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_P_NUMBER,    N_("<number>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_NUMBER_DE, "<Nummer>"},
/* fr */ {IDCLS_P_NUMBER_FR, "<numero>"},
/* hu */ {IDCLS_P_NUMBER_HU, "<sz�m>"},
/* it */ {IDCLS_P_NUMBER_IT, "<numero>"},
/* nl */ {IDCLS_P_NUMBER_NL, "<nummer>"},
/* pl */ {IDCLS_P_NUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_NUMBER_SV, "<nummer>"},
#endif

#ifdef COMMON_KBD
/* c128/c128-cmdline-options.c, plus4/plus4-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX,    N_("Specify index of keymap file (0=symbol, 1=positional)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_DE, "Aktive Tastaturbelegung (0=symbolisch) (1=positionell)"},
/* fr */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_FR, "Sp�cifier l'index du fichier keymap (0=symboles, 1=position)"},
/* hu */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_HU, "Adja meg a billenty�zet lek�pz�si m�dot (0=szimbolikus, 1=poz�ci� "
                                              "szerinti)"},
/* it */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_IT, "Specifica l'indice del file della mappa della tastiera (0=simbolico, "
                                              "1=posizionale)"},
/* nl */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_NL, "Geef index van het keymapbestand (0=symbool, 1=positioneel)"},
/* pl */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_KEYMAP_FILE_INDEX_SV, "Ange index f�r f�r tangentbordsinst�llningsfil (0=symbolisk, "
                                              "1=positionsriktig)"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   plus4/plus4-cmdline-options.c, vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME,    N_("Specify name of symbolic keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_DE, "Dateiname f�r symbolische Tastaturbelegung"},
/* fr */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier symbolique"},
/* hu */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_HU, "Adja meg a szimbolikus lek�pz�sf�jl nev�t"},
/* it */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_IT, "Specifica il nome del file della mappa simbolica della tastiera"},
/* nl */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_NL, "Geef naam van het symbolisch keymapbestand"},
/* pl */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME_SV, "Ange fil f�r symbolisk tangentbordsemulering"},
#endif

/* c128/c128-cmdline-options.c, c64/c64-cmdline-options.c,
   plus4/plus4-cmdline-options.c, vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME,    N_("Specify name of positional keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_DE, "Dateiname f�r positionelle Tastaturbelegung"},
/* fr */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier positionnel"},
/* hu */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_HU, "Adja meg a poz�ci� szerinti lek�pz�sf�jl nev�t"},
/* it */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_IT, "Specifica il nome del file della mappa posizionale della tastiera"},
/* nl */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_NL, "Geef naam van het positioneel keymapbestand"},
/* pl */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME_SV, "Ange fil f�r positionsriktig tangentbordsemulering"},
#endif
#endif

/* c128/c128mmu.c */
/* en */ {IDCLS_ACTIVATE_40_COL_MODE,    N_("Activate 40 column mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ACTIVATE_40_COL_MODE_DE, "40 Zeichen Modus"},
/* fr */ {IDCLS_ACTIVATE_40_COL_MODE_FR, "Activer le mode 40 colonnes"},
/* hu */ {IDCLS_ACTIVATE_40_COL_MODE_HU, "40 oszlopos m�d aktiv�l�sa"},
/* it */ {IDCLS_ACTIVATE_40_COL_MODE_IT, "Attiva la modalit� a 40 colonne"},
/* nl */ {IDCLS_ACTIVATE_40_COL_MODE_NL, "Activeer 40 kolomsmodus"},
/* pl */ {IDCLS_ACTIVATE_40_COL_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ACTIVATE_40_COL_MODE_SV, "Aktivera 40-kolumnersl�ge"},
#endif

/* c128/c128mmu.c */
/* en */ {IDCLS_ACTIVATE_80_COL_MODE,    N_("Activate 80 column mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ACTIVATE_80_COL_MODE_DE, "80 Zeichen Modus"},
/* fr */ {IDCLS_ACTIVATE_80_COL_MODE_FR, "Activer le mode 80 colonnes"},
/* hu */ {IDCLS_ACTIVATE_80_COL_MODE_HU, "80 oszlopos m�d aktiv�l�sa"},
/* it */ {IDCLS_ACTIVATE_80_COL_MODE_IT, "Attiva la modalit� a 80 colonne"},
/* nl */ {IDCLS_ACTIVATE_80_COL_MODE_NL, "Activeer 80 kolomsmodus"},
/* pl */ {IDCLS_ACTIVATE_80_COL_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ACTIVATE_80_COL_MODE_SV, "Aktivera 80-kolumnersl�ge"},
#endif

/* c128/c128mmu.c */
/* en */ {IDCLS_GO64_MODE,    N_("Always switch to C64 mode on reset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_GO64_MODE_DE, "Immer bei Reset in C64 Modus schalten"},
/* fr */ {IDCLS_GO64_MODE_FR, "Toujours revenir au mode C64 au red�marrage"},
/* hu */ {IDCLS_GO64_MODE_HU, "Mindig C64 m�dban �jraind�t�s ut�n"},
/* it */ {IDCLS_GO64_MODE_IT, "Cambia sempre in modalita C64 al reset"},
/* nl */ {IDCLS_GO64_MODE_NL, "Start altijd op in C64-modus na reset"},
/* pl */ {IDCLS_GO64_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_GO64_MODE_SV, "V�xla alltid till C64-l�ge vid �terst�llning"},
#endif

/* c128/c128mmu.c */
/* en */ {IDCLS_GO128_MODE,    N_("Always switch to C128 mode on reset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_GO128_MODE_DE, "Immer be Reset in C128 Modus schalten"},
/* fr */ {IDCLS_GO128_MODE_FR, "Toujours revenir au mode C128 au red�marrage"},
/* hu */ {IDCLS_GO128_MODE_HU, "Mindig C128 m�d �jraind�t�s ut�n"},
/* it */ {IDCLS_GO128_MODE_IT, "Cambia sempre in modalita C128 al reset"},
/* nl */ {IDCLS_GO128_MODE_NL, "Start altijd op in C128-modus na reset"},
/* pl */ {IDCLS_GO128_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_GO128_MODE_SV, "V�xla alltid till C128-l�ge vid �terst�llning"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME,    N_("Specify name of internal Function ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_DE, "Dateiname des internen Funktions ROMs"},
/* fr */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_FR, "Sp�cifier le nom de l'image ROM des fonctions internes"},
/* hu */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_HU, "Adja meg a bels� Function ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_IT, "Specifica il nome dell'immagine della Function ROM interna"},
/* nl */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_NL, "Geef de naam van het intern functie-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_INT_FUNC_ROM_NAME_SV, "Ange namn p� intern funktions-ROM-avbildning"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME,    N_("Specify name of external Function ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_DE, "Dateiname des externen Funktions ROMs"},
/* fr */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_FR, "Sp�cifier le nom de l'image ROM des fonctions externes"},
/* hu */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_HU, "Adja meg a k�ls� Function ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_IT, "Specifica il nome dell'immagine della Function ROM esterna"},
/* nl */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_NL, "Geef de naam van het extern functie-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_FUNC_ROM_NAME_SV, "Ange namn p� extern funktions-ROM-avbildning"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_ENABLE_INT_FUNC_ROM,    N_("Enable the internal Function ROM")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_INT_FUNC_ROM_DE, "Internes Funktions ROM aktivieren"},
/* fr */ {IDCLS_ENABLE_INT_FUNC_ROM_FR, "Activer l'image ROM des fonctions internes"},
/* hu */ {IDCLS_ENABLE_INT_FUNC_ROM_HU, "Bels� Function ROM enged�lyez�se"},
/* it */ {IDCLS_ENABLE_INT_FUNC_ROM_IT, "Attiva la Function ROM interna"},
/* nl */ {IDCLS_ENABLE_INT_FUNC_ROM_NL, "Activeer de interne functie-ROM"},
/* pl */ {IDCLS_ENABLE_INT_FUNC_ROM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_INT_FUNC_ROM_SV, "Aktivera internt funktions-ROM"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_DISABLE_INT_FUNC_ROM,    N_("Disable the internal Function ROM")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_INT_FUNC_ROM_DE, "Internes Funktions ROM deaktivieren"},
/* fr */ {IDCLS_DISABLE_INT_FUNC_ROM_FR, "D�sactiver l'image ROM des fonctions internes"},
/* hu */ {IDCLS_DISABLE_INT_FUNC_ROM_HU, "Bels� Function ROM tilt�sa"},
/* it */ {IDCLS_DISABLE_INT_FUNC_ROM_IT, "Disattiva la Function ROM interna"},
/* nl */ {IDCLS_DISABLE_INT_FUNC_ROM_NL, "De interne functie-ROM uitschakelen"},
/* pl */ {IDCLS_DISABLE_INT_FUNC_ROM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_INT_FUNC_ROM_SV, "Inaktivera internt funktions-ROM"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_ENABLE_EXT_FUNC_ROM,    N_("Enable the external Function ROM")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_EXT_FUNC_ROM_DE, "Externes Funktions ROM aktivieren"},
/* fr */ {IDCLS_ENABLE_EXT_FUNC_ROM_FR, "Activer l'image ROM des fonctions externes"},
/* hu */ {IDCLS_ENABLE_EXT_FUNC_ROM_HU, "K�ls� Function ROM enged�lyez�se"},
/* it */ {IDCLS_ENABLE_EXT_FUNC_ROM_IT, "Attiva la Function ROM esterna"},
/* nl */ {IDCLS_ENABLE_EXT_FUNC_ROM_NL, "Activeer de externe functie-ROM"},
/* pl */ {IDCLS_ENABLE_EXT_FUNC_ROM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_EXT_FUNC_ROM_SV, "Aktivera externt funktions-ROM"},
#endif

/* c128/functionrom.c */
/* en */ {IDCLS_DISABLE_EXT_FUNC_ROM,    N_("Disable the external Function ROM")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_EXT_FUNC_ROM_DE, "Externes Funktions ROM deaktivieren"},
/* fr */ {IDCLS_DISABLE_EXT_FUNC_ROM_FR, "D�sactiver l'image ROM des fonctions externes"},
/* hu */ {IDCLS_DISABLE_EXT_FUNC_ROM_HU, "K�ls� Function ROM tilt�sa"},
/* it */ {IDCLS_DISABLE_EXT_FUNC_ROM_IT, "Disattiva la Function ROM esterna"},
/* nl */ {IDCLS_DISABLE_EXT_FUNC_ROM_NL, "De externe functie-ROM uitschakelen"},
/* pl */ {IDCLS_DISABLE_EXT_FUNC_ROM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_EXT_FUNC_ROM_SV, "Inaktivera externt funktions-ROM"},
#endif

/* c64/c64-cmdline-options.c, c64/psid.c */
/* en */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR,    N_("Use old NTSC sync factor")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_DE, "NTSC (alt) Faktor zum Synchronisieren benutzen"},
/* fr */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_FR, "Utiliser l'ancien mode NTSC"},
/* hu */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_HU, "R�gi NTSC szinkron faktor haszn�lata"},
/* it */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_IT, "Usa il vecchio fattore di sincronizzazione NTSC"},
/* nl */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_NL, "Gebruik oude NTSC-synchronisatiefactor"},
/* pl */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_OLD_NTSC_SYNC_FACTOR_SV, "Anv�nd gammal NTSC-synkfaktor"},
#endif

/* c64dtv/c64dtv-cmdline-options.c */
/* en */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE,    N_("Select Hummer Userport device (0: None, 1: ADC, 2: Joystick)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_DE, "Hummer Userport Ger�t ausw�hlen (0: Kein, 1: ADC, 2: Joystick)"},
/* fr */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_HU, "Hummer Userport eszk�z megad�sa (0: nincs, 1: ADC, 2: Joystick)"},
/* it */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_IT, "Seleziona il dispositivo sulla userport dell'Hummer (0: Nessuno, 1:"
                                                  " ADC, 2: Joystick)"},
/* nl */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_NL, "Selecteer Hummer Userport apparaat (0: Geen, 1: ADC, 2: Joystick"},
/* pl */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SELECT_HUMMER_USERPORT_DEVICE_SV, "V�lj enhet f�r Hummeranv�ndarport (0: ingen, 1: ADC, 2: styrspak)"},
#endif

/* c64dtv/c64dtv-cmdline-options.c */
/* en */ {IDCLS_P_PORT,    N_("<port>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_PORT_DE, "<Port>"},
/* fr */ {IDCLS_P_PORT_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_P_PORT_HU, "<port>"},
/* it */ {IDCLS_P_PORT_IT, "<porta>"},
/* nl */ {IDCLS_P_PORT_NL, "<poort>"},
/* pl */ {IDCLS_P_PORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_PORT_SV, "<port>"},
#endif

/* c64dtv/c64dtv-cmdline-options.c */
/* en */ {IDCLS_HUMMER_JOY_PORT,    N_("Select which joystick port should be mapped to the userport")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_HUMMER_JOY_PORT_DE, "W�hle welches Joystickport dem Userport zugeordnet werden soll"},
/* fr */ {IDCLS_HUMMER_JOY_PORT_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_HUMMER_JOY_PORT_HU, "V�lassza ki a joystick portot, amely a userport-ra k�pz�dik le"},
/* it */ {IDCLS_HUMMER_JOY_PORT_IT, "Seleziona quali porte joystick devono essere mappate sulla userport"},
/* nl */ {IDCLS_HUMMER_JOY_PORT_NL, "Selecteer welke joystick poort moet worden omgeleidt naar de userport"},
/* pl */ {IDCLS_HUMMER_JOY_PORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_HUMMER_JOY_PORT_SV, "Ange vilken spelport som skall kopplas till anv�ndarporten"},
#endif

/* c64/c64-cmdline-options.c, cbm2/cbm2-cmdline-options.c,
   pet/pet-cmdline-options.c, plus4/plus4-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_KERNAL_ROM_NAME,    N_("Specify name of Kernal ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_DE, "Dateiname des Kernal ROMs"},
/* fr */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_FR, "Sp�cifier le nom de l'image ROM Kernal"},
/* hu */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_HU, "Adja meg a Kernal ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del Kernal"},
/* nl */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_NL, "Geef de naam van het Kernal-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_KERNAL_ROM_NAME_SV, "Ange namn p� kernal-ROM-avbildning"},
#endif

/* c64/c64-cmdline-options.c, cbm2/cbm2-cmdline-options.c,
   pet/pet-cmdline-options.c, plus4/plus4-cmdline-options.c,
   vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BASIC_ROM_NAME,    N_("Specify name of BASIC ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BASIC_ROM_NAME_DE, "Dateiname des Basic ROMs"},
/* fr */ {IDCLS_SPECIFY_BASIC_ROM_NAME_FR, "Sp�cifier le nom de l'image BASIC ROM"},
/* hu */ {IDCLS_SPECIFY_BASIC_ROM_NAME_HU, "Adja meg a BASIC ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_BASIC_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del BASIC"},
/* nl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_NL, "Geef de naam van het BASIC-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_BASIC_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BASIC_ROM_NAME_SV, "Ange namn p� BASIC-ROM-avbildning"},
#endif

/* c64/c64-cmdline-options.c, cbm2/cbm2-cmdline-options.c,
   pet/pet-cmdline-options.c, vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME,    N_("Specify name of character generator ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_DE, "Dateiname des Zeichensatz ROMs"},
/* fr */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_FR, "Sp�cifier le nom de l'image du g�n�rateur de caract�res"},
/* hu */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_HU, "Adja meg a karaktergener�tor ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_IT, "Specifica il nome della ROM del generatore di caratteri"},
/* nl */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_NL, "Geef de naam van het CHARGEN-ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CHARGEN_ROM_NAME_SV, "Ange namn p� teckengenerator-ROM-avbildning"},
#endif

#ifdef COMMON_KBD
/* c64/c64-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2,    N_("Specify index of keymap file (0=sym, 1=symDE, 2=pos)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_DE, "Index f�r Keymap Datei festlegen (0=symbol, 1=symDE, 2=positional)"},
/* fr */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_FR, "Sp�cifier l'index du fichier keymap (0=sym, 1=symDE, 2=pos)"},
/* hu */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_HU, "Adja meg a billenty�zet lek�pz�si f�jl t�pus�t (0=szimbolikus, "
                                                  "1=n�met szimbolikus, 2=poz�ci� szerinti)"},
/* it */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_IT, "Specifica l'indice del file della mappa della tastiera (0=sim, "
                                                  "1=simGER, 2=pos)"},
/* nl */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_NL, "Geef de index van het keymapbestand (0=sym, 1=symDE, 2=pos)"},
/* pl */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_INDEX_KEYMAP_FILE_0_2_SV, "Ange index f�r f�r tangentbordsinst�llningsfil (0=symbolisk, "
                                                  "1=symbolisk tysk, 2=positionsriktig)"},
#endif

/* c64/c64-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP,    N_("Specify name of symbolic German keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_DE, "Name von symbolischer Keymap Datei definieren"},
/* fr */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_FR, "Sp�cifier le nom du fichier symbolique de mappage clavier"},
/* hu */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_HU, "Adja meg a nev�t a n�met billenty�zet lek�pz�s f�jlnak"},
/* it */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_IT, "Specifica il nome del file della mappa simbolica della tastiera tedesca"},
/* nl */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_NL, "Geef de naam van het symbolische Duitse keymapbestand"},
/* pl */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_NAME_SYM_DE_KEYMAP_SV, "Ange fil f�r tysk symbolisk tangentbordsemulering"},
#endif
#endif

/* c64/georam.c */
/* en */ {IDCLS_ENABLE_GEORAM,    N_("Enable the GEORAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_GEORAM_DE, "GEORAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_GEORAM_FR, "Activer l'unit� d'expansion GEORAM"},
/* hu */ {IDCLS_ENABLE_GEORAM_HU, "GEORAM Expansion Unit enged�lyez�se"},
/* it */ {IDCLS_ENABLE_GEORAM_IT, "Attiva l'espansione GEORAM"},
/* nl */ {IDCLS_ENABLE_GEORAM_NL, "Activeer de GEORAM-uitbreidingseenheid"},
/* pl */ {IDCLS_ENABLE_GEORAM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_GEORAM_SV, "Aktivera GEORAM-expansionsenhet"},
#endif

/* c64/georam.c */
/* en */ {IDCLS_DISABLE_GEORAM,    N_("Disable the GEORAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_GEORAM_DE, "GEORAM Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_GEORAM_FR, "D�sactiver l'unit� d'expansion GEORAM"},
/* hu */ {IDCLS_DISABLE_GEORAM_HU, "GEORAM Expansion Unit tilt�sa"},
/* it */ {IDCLS_DISABLE_GEORAM_IT, "Disattiva l'espansione GEORAM"},
/* nl */ {IDCLS_DISABLE_GEORAM_NL, "De GEORAM-uitbreidingseenheid uitschakelen"},
/* pl */ {IDCLS_DISABLE_GEORAM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_GEORAM_SV, "Inaktivera GEORAM-expansionsenhet"},
#endif

/* c64/georam.c */
/* en */ {IDCLS_SPECIFY_GEORAM_NAME,    N_("Specify name of GEORAM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_GEORAM_NAME_DE, "Dateiname f�r GEORAM Erweiterung"},
/* fr */ {IDCLS_SPECIFY_GEORAM_NAME_FR, "Sp�cifier le nom de l'image GEORAM"},
/* hu */ {IDCLS_SPECIFY_GEORAM_NAME_HU, "Adja meg a GEORAM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_GEORAM_NAME_IT, "Specifica il nome dell'immagine GEORAM"},
/* nl */ {IDCLS_SPECIFY_GEORAM_NAME_NL, "Geef de naam van het GEORAM-bestand"},
/* pl */ {IDCLS_SPECIFY_GEORAM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_GEORAM_NAME_SV, "Ange namn p� GEORAM-avbildning"},
#endif

/* c64/georam.c, c64/ramcart.c, c64/reu.c, pet/petreu.c */
/* en */ {IDCLS_P_SIZE_IN_KB,    N_("<size in KB>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_SIZE_IN_KB_DE, "<Gr�sse in KB>"},
/* fr */ {IDCLS_P_SIZE_IN_KB_FR, "<taille en KO>"},
/* hu */ {IDCLS_P_SIZE_IN_KB_HU, "<m�ret KB-ban>"},
/* it */ {IDCLS_P_SIZE_IN_KB_IT, "<dimensione in KB>"},
/* nl */ {IDCLS_P_SIZE_IN_KB_NL, "<grootte in KB>"},
/* pl */ {IDCLS_P_SIZE_IN_KB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_SIZE_IN_KB_SV, "<storlek i KB>"},
#endif

/* c64/georam.c */
/* en */ {IDCLS_GEORAM_SIZE,    N_("Size of the GEORAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_GEORAM_SIZE_DE, "Gr�sse der GEORAM Erweiterung"},
/* fr */ {IDCLS_GEORAM_SIZE_FR, "Taille de l'unit� d'expansion GEORAM"},
/* hu */ {IDCLS_GEORAM_SIZE_HU, "GEORAM Expansion Unit m�rete"},
/* it */ {IDCLS_GEORAM_SIZE_IT, "Dimensione dell'espansione GEORAM"},
/* nl */ {IDCLS_GEORAM_SIZE_NL, "Grootte van de GEORAM-uitbreidingseenheid"},
/* pl */ {IDCLS_GEORAM_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_GEORAM_SIZE_SV, "Storlek p� GEORAM-expansionsenhet"},
#endif

/* pet/petreu.c */
/* en */ {IDCLS_ENABLE_PETREU,    N_("Enable the PET Ram and Expansion Unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_PETREU_DE, "PET RAM Speicher und Erweiterungsmodul aktivieren"},
/* fr */ {IDCLS_ENABLE_PETREU_FR, "Activer l'expansion d'unit� et de RAM du PET"},
/* hu */ {IDCLS_ENABLE_PETREU_HU, "PET RAM and Expansion Unit enged�lyez�se"},
/* it */ {IDCLS_ENABLE_PETREU_IT, "Attiva l'unit� di espansione e della RAM del PET"},
/* nl */ {IDCLS_ENABLE_PETREU_NL, "Activeer de PET-Ram en uitbreidingseenheid"},
/* pl */ {IDCLS_ENABLE_PETREU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_PETREU_SV, "Aktivera PET Ram- och -expansionsenhet"},
#endif

/* pet/petreu.c */
/* en */ {IDCLS_DISABLE_PETREU,    N_("Disable the PET Ram and Expansion Unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_PETREU_DE, "PET RAM Speicher und Erweiterungsmodul dektivieren"},
/* fr */ {IDCLS_DISABLE_PETREU_FR, "D�sactiver l'expansion d'unit� et de RAM du PET"},
/* hu */ {IDCLS_DISABLE_PETREU_HU, "PET RAM and Expansion Unit tilt�sa"},
/* it */ {IDCLS_DISABLE_PETREU_IT, "Disattiva l'unit� di espansione e della RAM del PET"},
/* nl */ {IDCLS_DISABLE_PETREU_NL, "De PET-RAM en uitbreidingseenheid uitschakelen"},
/* pl */ {IDCLS_DISABLE_PETREU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_PETREU_SV, "Inaktivera PET Ram- och -expansionsenhet"},
#endif

/* pet/petreu.c */
/* en */ {IDCLS_SPECIFY_PETREU_NAME,    N_("Specify name of PET Ram and Expansion Unit image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PETREU_NAME_DE, "Namen f�r PET RAM Speicher und Erweiterungsmodul definieren"},
/* fr */ {IDCLS_SPECIFY_PETREU_NAME_FR, "Sp�cifier le nom de l'image d�unit� d'expansion RAM"},
/* hu */ {IDCLS_SPECIFY_PETREU_NAME_HU, "Adja meg a PET RAM and Expansion Unit k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_PETREU_NAME_IT, "Specifica il nome dell'immagine dell'unit� di espansione e della RAM del PET"},
/* nl */ {IDCLS_SPECIFY_PETREU_NAME_NL, "Geef de naam van het PET-RAM en uitbreidingseenheid bestand"},
/* pl */ {IDCLS_SPECIFY_PETREU_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PETREU_NAME_SV, "Ange namn p� PET Ram- och -expansionsenhet"},
#endif

/* pet/petreu.c */
/* en */ {IDCLS_PETREU_SIZE,    N_("Size of the PET Ram and Expansion Unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PETREU_SIZE_DE, "Gr��e des PET RAM Speicher und Erweiterungsmodul"},
/* fr */ {IDCLS_PETREU_SIZE_FR, "Taille de l'unit� d'expansion RAM"},
/* hu */ {IDCLS_PETREU_SIZE_HU, "PET RAM and Expansion Unit m�rete"},
/* it */ {IDCLS_PETREU_SIZE_IT, "Dimensione dell'unit� di espansione e della RAM del PET"},
/* nl */ {IDCLS_PETREU_SIZE_NL, "Grootte van de PET-RAM en uitbreidingseenheid"},
/* pl */ {IDCLS_PETREU_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PETREU_SIZE_SV, "Storlek p� PET Ram- och -expansionsenhet"},
#endif

/* c64/psid.c */
/* en */ {IDCLS_SID_PLAYER_MODE,    N_("SID player mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SID_PLAYER_MODE_DE, "SID player Modus"},
/* fr */ {IDCLS_SID_PLAYER_MODE_FR, "Mode jukebox SID"},
/* hu */ {IDCLS_SID_PLAYER_MODE_HU, "SID lej�tsz� m�d"},
/* it */ {IDCLS_SID_PLAYER_MODE_IT, "Modalit� del SID player"},
/* nl */ {IDCLS_SID_PLAYER_MODE_NL, "SID-playermodus"},
/* pl */ {IDCLS_SID_PLAYER_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SID_PLAYER_MODE_SV, "SID-spelarl�ge"},
#endif

/* c64/psid.c */
/* en */ {IDCLS_OVERWRITE_PSID_SETTINGS,    N_("Override PSID settings for Video standard and SID model")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_OVERWRITE_PSID_SETTINGS_DE, "�berschreiben der PSID Einstellungen bez�glich Videostandard und SID Modell"},
/* fr */ {IDCLS_OVERWRITE_PSID_SETTINGS_FR, "�craser les param�tres PSID pour le standard vid�o et le mod�le SID"},
/* hu */ {IDCLS_OVERWRITE_PSID_SETTINGS_HU, "A vide� szabv�ny �s a SID modell PSID be�ll�t�sainak fel�lb�r�l�sa"},
/* it */ {IDCLS_OVERWRITE_PSID_SETTINGS_IT, "Non tener conto delle impostazioni del PSID per lo standard video e il "
                                            "modello di SID"},
/* nl */ {IDCLS_OVERWRITE_PSID_SETTINGS_NL, "Overschrijf PSID-instellingen voor videostandaard en SID-model"},
/* pl */ {IDCLS_OVERWRITE_PSID_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_OVERWRITE_PSID_SETTINGS_SV, "Ers�tt PSID-inst�llningar f�r videostandard och SID-modell"},
#endif

/* c64/psid.c */
/* en */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER,    N_("Specify PSID tune <number>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_DE, "PSID St�ck <Nummer> definieren"},
/* fr */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_FR, "Sp�cifier le <numero> de piste PSID"},
/* hu */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_HU, "Adja meg a PSID hangzatok <sz�m>"},
/* it */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_IT, "Specifica la melodia <numero> del PSID"},
/* nl */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_NL, "Geef PSID-deuntje <nummer>"},
/* pl */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PSID_TUNE_NUMBER_SV, "Ange PSID-l�t <nummer>"},
#endif

/* c64/ramcart.c */
/* en */ {IDCLS_ENABLE_RAMCART,    N_("Enable the RAMCART expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAMCART_DE, "RAMCART Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_RAMCART_FR, "Activer l'expansion RAMCART"},
/* hu */ {IDCLS_ENABLE_RAMCART_HU, "RAMCART b�v�t�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAMCART_IT, "Attiva l'espansione RAMCART"},
/* nl */ {IDCLS_ENABLE_RAMCART_NL, "Activeer de RAMCART-uitbreiding"},
/* pl */ {IDCLS_ENABLE_RAMCART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAMCART_SV, "Aktivera RAMCART-expansion"},
#endif

/* c64/ramcart.c */
/* en */ {IDCLS_DISABLE_RAMCART,    N_("Disable the RAMCART expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAMCART_DE, "RAMCART Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAMCART_FR, "D�sactiver l'expansion RAMCART"},
/* hu */ {IDCLS_DISABLE_RAMCART_HU, "RAMCART b�v�t�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAMCART_IT, "Disattiva l'espansione RAMCART"},
/* nl */ {IDCLS_DISABLE_RAMCART_NL, "De RAMCART-uitbreiding uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAMCART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAMCART_SV, "Inaktivera RAMCART-expansion"},
#endif

/* c64/ramcart.c */
/* en */ {IDCLS_SPECIFY_RAMCART_NAME,    N_("Specify name of RAMCART image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RAMCART_NAME_DE, "Dateiname f�r RAMCART Erweiterung"},
/* fr */ {IDCLS_SPECIFY_RAMCART_NAME_FR, "Sp�cifier le nom de l'image RAMCART"},
/* hu */ {IDCLS_SPECIFY_RAMCART_NAME_HU, "Adja meg a RAMCART k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_RAMCART_NAME_IT, "Specifica il nome dell'immagine RAMCART"},
/* nl */ {IDCLS_SPECIFY_RAMCART_NAME_NL, "Geef de naam van het RAMCART-bestand"},
/* pl */ {IDCLS_SPECIFY_RAMCART_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RAMCART_NAME_SV, "Ange namn p� RAMCART-avbildning"},
#endif

/* c64/ramcart.c */
/* en */ {IDCLS_RAMCART_SIZE,    N_("Size of the RAMCART expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_RAMCART_SIZE_DE, "Gr�sse der RAMCART Erweiterung"},
/* fr */ {IDCLS_RAMCART_SIZE_FR, "Taille de l'expansion RAMCART"},
/* hu */ {IDCLS_RAMCART_SIZE_HU, "RAMCART b�v�t�s m�rete"},
/* it */ {IDCLS_RAMCART_SIZE_IT, "Dimensione dell'espansione RAMCART"},
/* nl */ {IDCLS_RAMCART_SIZE_NL, "Grootte van de RAMCART-uitbreiding"},
/* pl */ {IDCLS_RAMCART_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_RAMCART_SIZE_SV, "Storlek p� RAMCART-expansion"},
#endif

/* c64/dqbb.c */
/* en */ {IDCLS_ENABLE_DQBB,    N_("Enable Double Quick Brown Box")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DQBB_DE, "Double Quick Brown Box aktivieren"},
/* fr */ {IDCLS_ENABLE_DQBB_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_DQBB_HU, "Double Quick Brown Box enged�lyez�se"},
/* it */ {IDCLS_ENABLE_DQBB_IT, "Attiva Double Quick Brown Box"},
/* nl */ {IDCLS_ENABLE_DQBB_NL, "Activeer de Double Quick Brown Box cartridge"},
/* pl */ {IDCLS_ENABLE_DQBB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DQBB_SV, "Aktivera Double Quick Brown Box"},
#endif

/* c64/dqbb.c */
/* en */ {IDCLS_DISABLE_DQBB,    N_("Disable Double Quick Brown Box")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DQBB_DE, "Double Quick Brown Box Modul deaktivieren"},
/* fr */ {IDCLS_DISABLE_DQBB_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_DQBB_HU, "Double Quick Brown Box tilt�sa"},
/* it */ {IDCLS_DISABLE_DQBB_IT, "Disattiva Double Quick Brown Box"},
/* nl */ {IDCLS_DISABLE_DQBB_NL, "De Double Quick Brown Box cartridge uitschakelen"},
/* pl */ {IDCLS_DISABLE_DQBB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DQBB_SV, "Inaktivera Double Quick Brown Box"},
#endif

/* c64/dqbb.c */
/* en */ {IDCLS_SPECIFY_DQBB_NAME,    N_("Specify Double Quick Brown Box filename")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_DQBB_NAME_DE, "Double Quick Brown Box Dateiname spezifizieren"},
/* fr */ {IDCLS_SPECIFY_DQBB_NAME_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_DQBB_NAME_HU, "Adja meg a Double Quick Brown Box f�jlnevet"},
/* it */ {IDCLS_SPECIFY_DQBB_NAME_IT, "Specifica il nome del file Double Quick Brown Box"},
/* nl */ {IDCLS_SPECIFY_DQBB_NAME_NL, "Geef de naam van het Double Quick Brown Box cartridge-bestand"},
/* pl */ {IDCLS_SPECIFY_DQBB_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_DQBB_NAME_SV, "Ange filnamn f�r Double Quick Brown Box"},
#endif

/* c64/isepic.c */
/* en */ {IDCLS_ENABLE_ISEPIC,    N_("Enable the isepic cart")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_ISEPIC_DE, "Isepic Modul aktivieren"},
/* fr */ {IDCLS_ENABLE_ISEPIC_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_ISEPIC_HU, "Isepic cartridge enged�lyez�se"},
/* it */ {IDCLS_ENABLE_ISEPIC_IT, "Attiva la cartuccia isepic"},
/* nl */ {IDCLS_ENABLE_ISEPIC_NL, "Activeer de isepic cartridge"},
/* pl */ {IDCLS_ENABLE_ISEPIC_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_ISEPIC_SV, "Aktivera isepic-insticksmodul"},
#endif

/* c64/isepic.c */
/* en */ {IDCLS_DISABLE_ISEPIC,    N_("Disable the isepic cart")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_ISEPIC_DE, "Isepic Modul deaktivieren"},
/* fr */ {IDCLS_DISABLE_ISEPIC_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_ISEPIC_HU, "Isepic cartridge tilt�sa"},
/* it */ {IDCLS_DISABLE_ISEPIC_IT, "Disattiva la cartuccia isepic"},
/* nl */ {IDCLS_DISABLE_ISEPIC_NL, "De isepic cartridge uitschakelen"},
/* pl */ {IDCLS_DISABLE_ISEPIC_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_ISEPIC_SV, "Inaktivera isepic-insticksmodul."},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_ENABLE_MMC64,    N_("Enable the MMC64 expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_MMC64_DE, "MMC64 Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_MMC64_FR, "Activer l'expansion MMC64"},
/* hu */ {IDCLS_ENABLE_MMC64_HU, "MMC64 b�v�t�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_MMC64_IT, "Attiva l'espansione MMC64"},
/* nl */ {IDCLS_ENABLE_MMC64_NL, "Activeer de MMC64-uitbreiding"},
/* pl */ {IDCLS_ENABLE_MMC64_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_MMC64_SV, "Aktivera MMC64-expansion"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_DISABLE_MMC64,    N_("Disable the MMC64 expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_MMC64_DE, "MMC64 Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_MMC64_FR, "D�sactiver l'expansion MMC64"},
/* hu */ {IDCLS_DISABLE_MMC64_HU, "MMC64 b�v�t�s tilt�sa"},
/* it */ {IDCLS_DISABLE_MMC64_IT, "Disattiva l'espansione MMC64"},
/* nl */ {IDCLS_DISABLE_MMC64_NL, "De MMC64-uitbreiding uitschakelen"},
/* pl */ {IDCLS_DISABLE_MMC64_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_MMC64_SV, "Inaktivera MMC64-expansion"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_SPECIFY_MMC64_BIOS_NAME,    N_("Specify name of MMC64 BIOS image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_DE, "Name f�r MMC64 BIOS Datei spezifizieren"},
/* fr */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_FR, "Sp�cifier le nom de l'image MMC64 BIOS"},
/* hu */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_HU, "Adja meg az MMC64 BIOS k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_IT, "Specifica il nome dell'immagine del BIOS MMC64"},
/* nl */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_NL, "Geef de naam van het MMC64-BIOS-bestand"},
/* pl */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_MMC64_BIOS_NAME_SV, "Ange namn p� MMC64-BIOS-avbildning"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME,    N_("Specify name of MMC64 image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_DE, "Name der MMC64 Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_FR, "Sp�cifier le nom de l'image MMC64"},
/* hu */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_HU, "Adja meg az MMC64 k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_IT, "Specifica il nome dell'immagine MMC64"},
/* nl */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_NL, "Geef de naam van het MMC64-bestand"},
/* pl */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_MMC64_IMAGE_NAME_SV, "Ange namn p� MMC64-avbildning"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_MMC64_READONLY,    N_("Set the MMC64 card to read-only")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_MMC64_READONLY_DE, "MMC64 Karte Schreibschutz"},
/* fr */ {IDCLS_MMC64_READONLY_FR, "D�finir la carte MMC64 en lecture seule"},
/* hu */ {IDCLS_MMC64_READONLY_HU, "MMC64 k�rtya csak olvashat�"},
/* it */ {IDCLS_MMC64_READONLY_IT, "Imposta la cartuccia MMC64 in sola lettura"},
/* nl */ {IDCLS_MMC64_READONLY_NL, "Zet de MMC64 card als alleen lezen"},
/* pl */ {IDCLS_MMC64_READONLY_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_MMC64_READONLY_SV, "Skrivskydda MMC64-kort"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_MMC64_READWRITE,    N_("Set the MMC64 card to read/write")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_MMC64_READWRITE_DE, "MMC64 Karte Schreibzugriff"},
/* fr */ {IDCLS_MMC64_READWRITE_FR, "D�finir la carte MMC64 en lecture/�criture"},
/* hu */ {IDCLS_MMC64_READWRITE_HU, "MMC64 k�rtya �rhat�/olvashat�"},
/* it */ {IDCLS_MMC64_READWRITE_IT, "Imposta la cartuccia MMC64 in lettura/scrittura"},
/* nl */ {IDCLS_MMC64_READWRITE_NL, "Zet de MMC64 card als lezen/schrijven"},
/* pl */ {IDCLS_MMC64_READWRITE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_MMC64_READWRITE_SV, "Till�t skrivning p� MMC64-kort"},
#endif

/* c64/mmc64.c */
/* en */ {IDCLS_MMC64_BIOS_WRITE,    N_("Save the MMC64 bios when changed")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_MMC64_BIOS_WRITE_DE, "MMC64 BIOS speichern bei �nderung"},
/* fr */ {IDCLS_MMC64_BIOS_WRITE_FR, "Sauvegarder le BIOS MMC64 lorsque modifi�"},
/* hu */ {IDCLS_MMC64_BIOS_WRITE_HU, "MMC64 BIOS ment�se v�ltoz�s ut�n"},
/* it */ {IDCLS_MMC64_BIOS_WRITE_IT, "Salva il bios MMC64 al cambio"},
/* nl */ {IDCLS_MMC64_BIOS_WRITE_NL, "Sla de MMC64-bios op als er veranderingen zijn gemaakt"},
/* pl */ {IDCLS_MMC64_BIOS_WRITE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_MMC64_BIOS_WRITE_SV, "Spara MMC64-BIOS vid �ndring"},
#endif

/* c64/reu.c */
/* en */ {IDCLS_ENABLE_REU,    N_("Enable the RAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_REU_DE, "REU Speichererweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_REU_FR, "Activer l'unit� d'expansion RAM"},
/* hu */ {IDCLS_ENABLE_REU_HU, "RAM Expansion Unit enged�lyez�se"},
/* it */ {IDCLS_ENABLE_REU_IT, "Attiva l'espansione di RAM"},
/* nl */ {IDCLS_ENABLE_REU_NL, "Activeer de RAM-uitbreidingseenheid (REU)"},
/* pl */ {IDCLS_ENABLE_REU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_REU_SV, "Aktivera RAM-expansionsenhet"},
#endif

/* c64/reu.c */
/* en */ {IDCLS_DISABLE_REU,    N_("Disable the RAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_REU_DE, "REU Speichererweiterung deaktivieren"}, 
/* fr */ {IDCLS_DISABLE_REU_FR, "D�sactiver l'unit� d'expansion RAM"},
/* hu */ {IDCLS_DISABLE_REU_HU, "RAM Expansion Unit tilt�sa"},
/* it */ {IDCLS_DISABLE_REU_IT, "Disattiva l'espansione di RAM"},
/* nl */ {IDCLS_DISABLE_REU_NL, "De RAM-uitbreidingseenheid (REU) uitschakelen"},
/* pl */ {IDCLS_DISABLE_REU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_REU_SV, "Inaktivera RAM-expansionsenhet"},
#endif

/* c64/reu.c */
/* en */ {IDCLS_SPECIFY_REU_NAME,    N_("Specify name of REU image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_REU_NAME_DE, "Dateiname f�r REU Speichererweiterung"},
/* fr */ {IDCLS_SPECIFY_REU_NAME_FR, "Sp�cifier le nom de l'image REU"},
/* hu */ {IDCLS_SPECIFY_REU_NAME_HU, "Adja meg a REU k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_REU_NAME_IT, "Specifica il nome dell'immagine REU"},
/* nl */ {IDCLS_SPECIFY_REU_NAME_NL, "Geef de naam van het REU-bestand"},
/* pl */ {IDCLS_SPECIFY_REU_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_REU_NAME_SV, "Ange namn p� REU-avbildning"},
#endif

/* c64/reu.c */
/* en */ {IDCLS_REU_SIZE,    N_("Size of the RAM expansion unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_REU_SIZE_DE, "Gr�sse der REU Speichererweiterung"},
/* fr */ {IDCLS_REU_SIZE_FR, "Taille de l'unit� d'expansion RAM"},
/* hu */ {IDCLS_REU_SIZE_HU, "RAM Expansion Unit m�rete"},
/* it */ {IDCLS_REU_SIZE_IT, "Dimensione dell'espansione di RAM"},
/* nl */ {IDCLS_REU_SIZE_NL, "Grootte van de RAM-uitbreidingseenheid (REU)"},
/* pl */ {IDCLS_REU_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_REU_SIZE_SV, "Storlek p� RAM-expansionsenhet"},
#endif

#ifdef HAVE_TFE
/* c64/tfe.c */
/* en */ {IDCLS_ENABLE_TFE,    N_("Enable the TFE (\"The Final Ethernet\") unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_TFE_DE, "TFE Ethernetemulation aktivieren"},
/* fr */ {IDCLS_ENABLE_TFE_FR, "Activer l'unit� TFE (\"The Final Ethernet\")"},
/* hu */ {IDCLS_ENABLE_TFE_HU, "TFE (\"the final ethernet\") egys�g enged�lyez�se"},
/* it */ {IDCLS_ENABLE_TFE_IT, "Attiva l'unit� TFE (\"The Final Ethernet\")"},
/* nl */ {IDCLS_ENABLE_TFE_NL, "Activeer de TFE (\"The Final Ethernet\")-eenheid"},
/* pl */ {IDCLS_ENABLE_TFE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_TFE_SV, "Aktivera TFE-enhet (\"The Final Ethernet\")"},
#endif

/* c64/tfe.c */
/* en */ {IDCLS_DISABLE_TFE,    N_("Disable the TFE (\"The Final Ethernet\") unit")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_TFE_DE, "TFE Ethernetemulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_TFE_FR, "D�sactiver l'unit� TFE"},
/* hu */ {IDCLS_DISABLE_TFE_HU, "TFE (\"The Final Ethernet\") egys�g tilt�sa"},
/* it */ {IDCLS_DISABLE_TFE_IT, "Disattiva l'unit� TFE (\"The Final Ethernet\")"},
/* nl */ {IDCLS_DISABLE_TFE_NL, "De TFE (\"The Final Ethernet\")-eenheid uitschakelen"},
/* pl */ {IDCLS_DISABLE_TFE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_TFE_SV, "Inaktivera TFE-enhet (\"The Final Ethernet\")"},
#endif

/* c64/tfe.c */
/* en */ {IDCLS_TFE_INTERFACE,    N_("Set the system ethernet interface for TFE emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TFE_INTERFACE_DE, "Setze die System Ethernet Schnittstelle f�r die TFE Emulation"},
/* fr */ {IDCLS_TFE_INTERFACE_FR, "D�finir l�interface ethernet pour l��mulation TFE"},
/* hu */ {IDCLS_TFE_INTERFACE_HU, "Rendszer ethernet eszk�z be�ll�t�sa TFE emul�ci�hoz"},
/* it */ {IDCLS_TFE_INTERFACE_IT, "Imposta l'interfaccia ethernet per l'emulazione TFE"},
/* nl */ {IDCLS_TFE_INTERFACE_NL, "Zet de systeem ethernetinterface voor de TFE-emulatie"},
/* pl */ {IDCLS_TFE_INTERFACE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TFE_INTERFACE_SV, "St�ll in systemets ethernetgr�nssnitt f�r TFE-emulering"},
#endif

/* c64/tfe.c */
/* en */ {IDCLS_ENABLE_TFE_AS_RRNET,    N_("Enable RRNet mode of TFE emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_TFE_AS_RRNET_DE, "RRNet Modus der TFE Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_TFE_AS_RRNET_FR, "Activer le mode RRNet de l��mulation TFE"},
/* hu */ {IDCLS_ENABLE_TFE_AS_RRNET_HU, "RRNet m�d enged�lyez�se TFE emul�ci�hoz"},
/* it */ {IDCLS_ENABLE_TFE_AS_RRNET_IT, "Attiva la modalit� RRNet dell'emulazione TFE"},
/* nl */ {IDCLS_ENABLE_TFE_AS_RRNET_NL, "Activeer RRNet-modus van de TFE-emulatie"},
/* pl */ {IDCLS_ENABLE_TFE_AS_RRNET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_TFE_AS_RRNET_SV, "Aktivera RRNet-l�ge p� TFE-emulering"},
#endif

/* c64/tfe.c */
/* en */ {IDCLS_DISABLE_TFE_AS_RRNET,    N_("Disable RRNet mode of TFE emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_TFE_AS_RRNET_DE, "RRNet Modus der TFE Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_TFE_AS_RRNET_FR, "D�sactiver le mode RRNet de l��mulation TFE"},
/* hu */ {IDCLS_DISABLE_TFE_AS_RRNET_HU, "RRNet m�d tilt�sa TFE emul�ci�hoz"},
/* it */ {IDCLS_DISABLE_TFE_AS_RRNET_IT, "Disattiva la modalit� RRNet dell'emulazione TFE"},
/* nl */ {IDCLS_DISABLE_TFE_AS_RRNET_NL, "RRNet-modus van de TFE-emulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_TFE_AS_RRNET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_TFE_AS_RRNET_SV, "Inaktivera RRNet-l�ge p� TFE-emulering"},
#endif
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_CART_ATTACH_DETACH_RESET,    N_("Reset machine if a cartridge is attached or detached")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_CART_ATTACH_DETACH_RESET_DE, "Reset der Maschine wenn ein Erweiterungsmodul eingelegt oder entfernt wird"},
/* fr */ {IDCLS_CART_ATTACH_DETACH_RESET_FR, "R�initialiser si une cartouche est ins�r�e ou retir�e"},
/* hu */ {IDCLS_CART_ATTACH_DETACH_RESET_HU, "Reset a cartridge csatol�sakorgy lev�laszt�sakor"},
/* it */ {IDCLS_CART_ATTACH_DETACH_RESET_IT, "Fare il reset della macchina se una cartuccia � collegata o rimossa"},
/* nl */ {IDCLS_CART_ATTACH_DETACH_RESET_NL, "Reset machine als een cartridge wordt aangekoppeld of ontkoppeld"},
/* pl */ {IDCLS_CART_ATTACH_DETACH_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_CART_ATTACH_DETACH_RESET_SV, "Nollst�ll maskinen om en insticksmodul kopplas till eller fr�n"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_CART_ATTACH_DETACH_NO_RESET,    N_("Do not reset machine if a cartridge is attached or detached")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_DE, "Kein Reset der Maschine wenn ein Erweiterungsmodul eingelegt oder "
                                                "entfernt wird"},
/* fr */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_FR, "Ne pas r�initialiser si une cartouche est ins�r�e ou retir�e"},
/* hu */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_HU, "Nincs reset a cartridge csatol�sakorgy lev�laszt�sakor"},
/* it */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_IT, "Non fare il reset della macchina se una cartuccia � collegata o rimossa"},
/* nl */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_NL, "Reset machine niet als een cartridge wordt aangekoppeld of ontkoppeld"},
/* pl */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_CART_ATTACH_DETACH_NO_RESET_SV, "Nollst�ll inte maskinen om en insticksmodul kopplas till eller fr�n"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_CRT_CART,    N_("Attach CRT cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_CRT_CART_DE, "CRT Erweiterungsmodul Image einlegen"},
/* fr */ {IDCLS_ATTACH_CRT_CART_FR, "Ins�rer une cartouche CRT"},
/* hu */ {IDCLS_ATTACH_CRT_CART_HU, "CRT cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_CRT_CART_IT, "Seleziona l'immagine di una cartuccia CRT"},
/* nl */ {IDCLS_ATTACH_CRT_CART_NL, "Koppel CRT-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_CRT_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_CRT_CART_SV, "Anslut CRT-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_GENERIC_8KB_CART,    N_("Attach generic 8KB cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_GENERIC_8KB_CART_DE, "Generisches 8K Erweiterungsmodul Image einlegen"},
/* fr */ {IDCLS_ATTACH_GENERIC_8KB_CART_FR, "Ins�rer une cartouche g�n�rique 8KO"},
/* hu */ {IDCLS_ATTACH_GENERIC_8KB_CART_HU, "�ltal�nos 8KB cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_GENERIC_8KB_CART_IT, "Seleziona l'immagine di una cartuccia generica di 8KB"},
/* nl */ {IDCLS_ATTACH_GENERIC_8KB_CART_NL, "Koppel generiek 8KB-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_GENERIC_8KB_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_GENERIC_8KB_CART_SV, "Anslut vanlig 8KB-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_GENERIC_16KB_CART,    N_("Attach generic 16KB cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_GENERIC_16KB_CART_DE, "Generisches 16K Erweiterungsmodul Image einlegen"},
/* fr */ {IDCLS_ATTACH_GENERIC_16KB_CART_FR, "Ins�rer une cartouche g�n�rique 16KO"},
/* hu */ {IDCLS_ATTACH_GENERIC_16KB_CART_HU, "�ltal�nos 16KB cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_GENERIC_16KB_CART_IT, "Seleziona l'immagine di una cartuccia generica di 16KB"},
/* nl */ {IDCLS_ATTACH_GENERIC_16KB_CART_NL, "Koppel generiek 16KB-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_GENERIC_16KB_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_GENERIC_16KB_CART_SV, "Anslut vanlig 16KB-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_STARDOS_CART,    N_("Attach raw 16KB StarDOS cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_STARDOS_CART_DE, "16KB StarDOS Erweiterungsmodul Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_STARDOS_CART_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ATTACH_RAW_STARDOS_CART_HU, "Nyers 16KB-os StarDOS cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_STARDOS_CART_IT, "Seleziona l'immagine di una cartuccia StarDOS di 16KB"},
/* nl */ {IDCLS_ATTACH_RAW_STARDOS_CART_NL, "Koppel binair 16KB StarDOS-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_STARDOS_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_STARDOS_CART_SV, "Anslut r� 16KB StarDOS-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART,    N_("Attach raw 32KB Action Replay IV cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_DE, "32K Action Replay Image IV einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_HU, "32KB-os Action Replay IV cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_IT, "Seleziona l'immagine di una cartuccia Action Replay IV di 32KB"},
/* nl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_NL, "Koppel binair 32KB Action-Replay-IV-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_ACTION_REPLAY4_CART_SV, "Anslut r� 32KB-Action Replay IV-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART,    N_("Attach raw 16KB Action Replay III cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_DE, "16KB Action Replay Image III (raw) einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_HU, "16KB-os Action Replay III cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_IT, "Seleziona l'immagine di una cartuccia Action Replay III di 16KB"},
/* nl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_NL, "Koppel binair 16KB Action-Replay-III-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_ACTION_REPLAY3_CART_SV, "Anslut r� 16KB-Action Replay III-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART,    N_("Attach raw 32KB Action Replay cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_DE, "32K Action Replay Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_FR, "Ins�rer une cartouche Action Replay 32KO"},
/* hu */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_HU, "32KB-os Action Replay cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_IT, "Seleziona l'immagine di una cartuccia Action Replay di 32KB"},
/* nl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_NL, "Koppel binair 32KB Action-Replay-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_ACTION_REPLAY_CART_SV, "Anslut r� 32KB-Action Replay-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART,    N_("Attach raw 64KB Retro Replay cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_DE, "64K Retro Replay Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_FR, "Ins�rer une cartouche Retro Replay 64KO"},
/* hu */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_HU, "64KB-os Retro Replay cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_IT, "Seleziona l'immagine di una cartuccia Retro Replay di 64KB"},
/* nl */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_NL, "Koppel binair 64KB Retro-Replay-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_RETRO_REPLAY_CART_SV, "Anslut r� 64KB-Retro Replay-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_IDE64_CART,    N_("Attach raw 64KB IDE64 cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_IDE64_CART_DE, "64KB IDE64 Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_IDE64_CART_FR, "Ins�rer une cartouche IDE64 64KO"},
/* hu */ {IDCLS_ATTACH_RAW_IDE64_CART_HU, "64KB-os IDE64 cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_IDE64_CART_IT, "Seleziona l'immagine di una cartuccia IDE64 di 64KB"},
/* nl */ {IDCLS_ATTACH_RAW_IDE64_CART_NL, "Koppel binair 64KB IDE64-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_IDE64_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_IDE64_CART_SV, "Anslut r� 64KB IDE64-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART,    N_("Attach raw 32KB Atomic Power cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_DE, "32KB Atomic Power Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_FR, "Ins�rer une cartouche Atomic Power 32KO"},
/* hu */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_HU, "32KB-os Atomic Power cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_IT, "Seleziona l'immagine di una cartuccia Atomic Power di 32KB"},
/* nl */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_NL, "Koppel binair 32KB Atomic-Power-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_ATOMIC_POWER_CART_SV, "Anslut r� 32KB Atomic Power-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART,    N_("Attach raw 8KB Epyx fastload cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_DE, "8KB Epyx Fastload Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_FR, "Ins�rer une cartouche Epyx FastLoad 8KO"},
/* hu */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_HU, "8KB-os Epyx gyorst�lt� cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_IT, "Seleziona l'immagine di una cartuccia Epyx fastload di 8KB"},
/* nl */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_NL, "Koppel binair 8KB Epyx-fastload-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_EPYX_FASTLOAD_CART_SV, "Anslut r� 8KB Epyx fastload-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_SS4_CART,    N_("Attach raw 32KB Super Snapshot cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_SS4_CART_DE, "32KB Super Snapshot (raw) Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_SS4_CART_FR, "Ins�rer une cartouche Super Snapshot 32KO"},
/* hu */ {IDCLS_ATTACH_RAW_SS4_CART_HU, "32KB-os Super Snapshot cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_SS4_CART_IT, "Seleziona l'immagine di una cartuccia Super Snapshot di 32KB"},
/* nl */ {IDCLS_ATTACH_RAW_SS4_CART_NL, "Koppel binair 32KB Super-Snapshot-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_SS4_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_SS4_CART_SV, "Anslut r� 32KB Super Snapshot-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_SS5_CART,    N_("Attach raw 64KB Super Snapshot cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_SS5_CART_DE, "64KB Super Snapshot (raw) Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_SS5_CART_FR, "Ins�rer une cartouche Super Snapshot 64KO"},
/* hu */ {IDCLS_ATTACH_RAW_SS5_CART_HU, "64KB-os Super Snapshot cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_SS5_CART_IT, "Seleziona l'immagine di una cartuccia Super Snapshot di 64KB"},
/* nl */ {IDCLS_ATTACH_RAW_SS5_CART_NL, "Koppel binair 64KB Super-Snapshot-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_SS5_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_SS5_CART_SV, "Anslut r� 64KB-Super Snapshot-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_CBM_IEEE488_CART,    N_("Attach CBM IEEE488 cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_CBM_IEEE488_CART_DE, "CBM IEEE488 Erweiterungsmodul einlegen"},
/* fr */ {IDCLS_ATTACH_CBM_IEEE488_CART_FR, "Ins�rer une cartouche CBM IEEE488"},
/* hu */ {IDCLS_ATTACH_CBM_IEEE488_CART_HU, "CBM IEEE488 cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_CBM_IEEE488_CART_IT, "Seleziona l'immagine di una cartuccia CBM IEE488"},
/* nl */ {IDCLS_ATTACH_CBM_IEEE488_CART_NL, "Koppel CBM IEEE488-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_CBM_IEEE488_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_CBM_IEEE488_CART_SV, "Anslut CBM IEEE488-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_WESTERMANN_CART,    N_("Attach raw 16KB Westermann learning cartridge image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_DE, "16KB Westermann learning (raw) Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_FR, "Ins�rer une cartouche Westermann Learning 16KO"},
/* hu */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_HU, "16KB-os Westermann learning cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_IT, "Seleziona l'immagine di una cartuccia Westermann learning di 16KB"},
/* nl */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_NL, "Koppel binair 16KB Westermann-learning-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_WESTERMANN_CART_SV, "Anslut r� 16KB-Westermann learning-insticksmodulfil"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ENABLE_EXPERT_CART,    N_("Enable expert cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_EXPERT_CART_DE, "Expert Cartridge aktivieren"},
/* fr */ {IDCLS_ENABLE_EXPERT_CART_FR, "Activer les param�tres experts de cartouches"},
/* hu */ {IDCLS_ENABLE_EXPERT_CART_HU, "Expert cartridge enged�lyez�se"},
/* it */ {IDCLS_ENABLE_EXPERT_CART_IT, "Attiva l'Expert Cartridge"},
/* nl */ {IDCLS_ENABLE_EXPERT_CART_NL, "Activeer expertcartridge"},
/* pl */ {IDCLS_ENABLE_EXPERT_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_EXPERT_CART_SV, "Aktivera Expert Cartridge"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_DISABLE_CART,    N_("Disable default cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_CART_DE, "Standard Modul deaktivieren"},
/* fr */ {IDCLS_DISABLE_CART_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_CART_HU, "Alap cartridge tilt�sa"},
/* it */ {IDCLS_DISABLE_CART_IT, "Disattiva la cartuccia predefinita"},
/* nl */ {IDCLS_DISABLE_CART_NL, "De standaard cartridge uitschakelen"},
/* pl */ {IDCLS_DISABLE_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_CART_SV, "Inaktivera standardinsticksmodul"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_SPECIFY_IDE64_NAME,    N_("Specify name of IDE64 image file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_IDE64_NAME_DE, "Namen f�r IDE64 Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_IDE64_NAME_FR, "Sp�cifier le nom de l'image IDE64"},
/* hu */ {IDCLS_SPECIFY_IDE64_NAME_HU, "Adja meg az IDE64 k�pm�s f�jlnev�t"},
/* it */ {IDCLS_SPECIFY_IDE64_NAME_IT, "Specifica il nome del file immagine dell'IDE64"},
/* nl */ {IDCLS_SPECIFY_IDE64_NAME_NL, "Geef de naam van het IDE64-bestand"},
/* pl */ {IDCLS_SPECIFY_IDE64_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_IDE64_NAME_SV, "Ange namn p� IDE64-insticksmodulfil"},
#endif

/* c64/cart/ide64.c, gfxoutputdrv/ffmpegdrv.c, datasette.c,
   debug.c, mouse.c, ram.c, sound.c, vsync.c */
/* en */ {IDCLS_P_VALUE,    N_("<value>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_VALUE_DE, "<Wert>"},
/* fr */ {IDCLS_P_VALUE_FR, "<valeur>"},
/* hu */ {IDCLS_P_VALUE_HU, "<�rt�k>"},
/* it */ {IDCLS_P_VALUE_IT, "<valore>"},
/* nl */ {IDCLS_P_VALUE_NL, "<waarde>"},
/* pl */ {IDCLS_P_VALUE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_VALUE_SV, "<v�rde>"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64,    N_("Set number of cylinders for the IDE64 emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_DE, "Anzahl der Zylinder f�r IDE64 Emulation"},
/* fr */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_FR, "R�gler le nombre de cylindres pour l'�mulation IDE64"},
/* hu */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_HU, "Adja meg a s�vok sz�m�t IDE64 emul�ci�hoz"},
/* it */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_IT, "Imposta il numero di cilindri dell'emulazione IDE64"},
/* nl */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_NL, "Stel het aantal cylinders voor de IDE64-emulatie in"},
/* pl */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_AMOUNT_CYLINDERS_IDE64_SV, "V�lj antal cylindrar f�r IDE64-emuleringen"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_SET_AMOUNT_HEADS_IDE64,    N_("Set number of heads for the IDE64 emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_AMOUNT_HEADS_IDE64_DE, "Anzahl der Laufwerksk�pfe f�r IDE64 Emulation"},
/* fr */ {IDCLS_SET_AMOUNT_HEADS_IDE64_FR, "R�gler le nombre de t�tes pour l'�mulation IDE64"},
/* hu */ {IDCLS_SET_AMOUNT_HEADS_IDE64_HU, "Adja meg a fejek sz�m�t IDE64 emul�ci�hoz"},
/* it */ {IDCLS_SET_AMOUNT_HEADS_IDE64_IT, "Imposta il numero di testine dell'emulazione IDE64"},
/* nl */ {IDCLS_SET_AMOUNT_HEADS_IDE64_NL, "Stel het aantal koppen voor de IDE64-emulatie in"},
/* pl */ {IDCLS_SET_AMOUNT_HEADS_IDE64_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_AMOUNT_HEADS_IDE64_SV, "Ange antal huvuden f�r IDE64-emuleringen"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_SET_AMOUNT_SECTORS_IDE64,    N_("Set number of sectors for the IDE64 emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_DE, "Anzahl der Sektoren f�r IDE64 Emulation"},
/* fr */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_FR, "R�gler le nombre de secteurs pour l'�mulation IDE64"},
/* hu */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_HU, "Adja meg a szektorok sz�m�t IDE64 emul�ci�hoz"},
/* it */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_IT, "Imposta il numero di settori dell'emulazione IDE64"},
/* nl */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_NL, "Stel het aantal sectors voor de IDE64-emulatie in"},
/* pl */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_AMOUNT_SECTORS_IDE64_SV, "Ange antal sektorer f�r IDE64-emuleringen"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_AUTODETECT_IDE64_GEOMETRY,    N_("Autodetect geometry of formatted images")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_DE, "Geometrie von formatierten Image Dateien automatisch erkennen"},
/* fr */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_FR, "D�tecter automatiquement la g�om�trie des images formatt�es"},
/* hu */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_HU, "Form�zott k�pm�sok geometri�j�nak automatikus felismer�se"},
/* it */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_IT, "Rileva automaticamente la geometria delle immagini formattate"},
/* nl */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_NL, "Automatisch de grootte van de geformateerde bestanden detecteren"},
/* pl */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_AUTODETECT_IDE64_GEOMETRY_SV, "Autodetektera geometri fr�n formaterad avbildningsfil"},
#endif

/* c64/cart/ide64.c */
/* en */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY,    N_("Do not autodetect geometry of formatted images")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_DE, "Geometrie von formatierten Image Dateien nicht automatisch erkennen"},
/* fr */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_FR, "Ne pas d�tecter automatiquement la g�om�trie des images formatt�es"},
/* hu */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_HU, "Form�zott k�pm�sok geometri�j�t ne ismerje fel automatikusan"},
/* it */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_IT, "Non rilevare automaticamente la geometrica delle immagini formattate"},
/* nl */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_NL, "Niet automatisch de grootte van de geformateerde bestanden detecteren"},
/* pl */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_NO_AUTODETECT_IDE64_GEOMETRY_SV, "Autodetektera inte geometri fr�n formaterad avbildningsfil"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_P_MODELNUMBER,    N_("<modelnumber>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_MODELNUMBER_DE, "<Modell Nummer>"},
/* fr */ {IDCLS_P_MODELNUMBER_FR, "<numerodemodele>"},
/* hu */ {IDCLS_P_MODELNUMBER_HU, "<modellsz�m>"},
/* it */ {IDCLS_P_MODELNUMBER_IT, "<numero di modello>"},
/* nl */ {IDCLS_P_MODELNUMBER_NL, "<modelnummer>"},
/* pl */ {IDCLS_P_MODELNUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_MODELNUMBER_SV, "<modellnummer>"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CBM2_MODEL,    N_("Specify CBM-II model to emulate")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CBM2_MODEL_DE, "CBM-II Modell f�r Emulation definieren"},
/* fr */ {IDCLS_SPECIFY_CBM2_MODEL_FR, "Sp�cifier le mod�le CBM-II � �muler"},
/* hu */ {IDCLS_SPECIFY_CBM2_MODEL_HU, "Adja meg az emul�land� CBM-II modell sz�m�t"},
/* it */ {IDCLS_SPECIFY_CBM2_MODEL_IT, "Specifica il modello di CBM II da emulare"},
/* nl */ {IDCLS_SPECIFY_CBM2_MODEL_NL, "Geef CBM-II-model om te emuleren"},
/* pl */ {IDCLS_SPECIFY_CBM2_MODEL_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CBM2_MODEL_SV, "Ange CBM-II-modell att emulera"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_TO_USE_VIC_II,    N_("Specify to use VIC-II")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TO_USE_VIC_II_DE, "VIC-II Unterst�tzung aktivieren"},
/* fr */ {IDCLS_SPECIFY_TO_USE_VIC_II_FR, "Sp�cifier l'utilisation de VIC-II"},
/* hu */ {IDCLS_SPECIFY_TO_USE_VIC_II_HU, "VIC-II haszn�lata"},
/* it */ {IDCLS_SPECIFY_TO_USE_VIC_II_IT, "Specifica di utilizzare il VIC-II"},
/* nl */ {IDCLS_SPECIFY_TO_USE_VIC_II_NL, "Gebruik de VIC-II"},
/* pl */ {IDCLS_SPECIFY_TO_USE_VIC_II_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TO_USE_VIC_II_SV, "Ange f�r att anv�nda VIC-II"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_TO_USE_CRTC,    N_("Specify to use CRTC")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TO_USE_CRTC_DE, "CRTC Unterst�tzung aktivieren"},
/* fr */ {IDCLS_SPECIFY_TO_USE_CRTC_FR, "Sp�cifier l'utilisation de CRTC"},
/* hu */ {IDCLS_SPECIFY_TO_USE_CRTC_HU, "CRTC haszn�lata"},
/* it */ {IDCLS_SPECIFY_TO_USE_CRTC_IT, "Specifica di utilizzare il CRTC"},
/* nl */ {IDCLS_SPECIFY_TO_USE_CRTC_NL, "Gebruik de CRTC"},
/* pl */ {IDCLS_SPECIFY_TO_USE_CRTC_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TO_USE_CRTC_SV, "Ange f�r att anv�nda CRTC"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_P_LINENUMBER,    N_("<linenumber>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_LINENUMBER_DE, "<Zeilennummer>"},
/* fr */ {IDCLS_P_LINENUMBER_FR, "<num�rodeligne>"},
/* hu */ {IDCLS_P_LINENUMBER_HU, "<sorsz�m>"},
/* it */ {IDCLS_P_LINENUMBER_IT, "<numero di linea>"},
/* nl */ {IDCLS_P_LINENUMBER_NL, "<lijnnummer>"},
/* pl */ {IDCLS_P_LINENUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_LINENUMBER_SV, "<linjenummer>"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE,    N_("Specify CBM-II model hardware (0=6x0, 1=7x0)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_DE, "CBM-II Hardware Modell definieren (0=6x0, 1=7x0)"},
/* fr */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_FR, "Sp�cifier le mod�le hardware CBM-II (0=6x0, 1=7x0)"},
/* hu */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_HU, "Adja meg a CBM-II hardver modellt (0=6x0, 1=7x0)"},
/* it */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_IT, "Specifica il modello hardware del CBM-II (0=6x0, 1=7x0)"},
/* nl */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_NL, "Geef CBM-II-hardwaremodel (0=6x0, 1=7x0)"},
/* pl */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CBM2_MODEL_HARDWARE_SV, "Ange maskinvara f�r CBM-II-modell (0=6x0, 1=7x0)"},
#endif

/* cbm2/cbm2-cmdline-options.c, plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_P_RAMSIZE,    N_("<ramsize>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_RAMSIZE_DE, "<RAM Gr��e>"},
/* fr */ {IDCLS_P_RAMSIZE_FR, "<tailleram>"},
/* hu */ {IDCLS_P_RAMSIZE_HU, "<ramm�ret>"},
/* it */ {IDCLS_P_RAMSIZE_IT, "<dimensione della ram>"},
/* nl */ {IDCLS_P_RAMSIZE_NL, "<geheugengrootte>"},
/* pl */ {IDCLS_P_RAMSIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_RAMSIZE_SV, "<ramstorlek>"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIZE_OF_RAM,    N_("Specify size of RAM (64/128/256/512/1024 kByte)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIZE_OF_RAM_DE, "RAM Gr��e definieren (64/128/256/512/1024 kByte)"},
/* fr */ {IDCLS_SPECIFY_SIZE_OF_RAM_FR, "Sp�cifier la taille de la RAM (64/128/256/512/1024 KO)"},
/* hu */ {IDCLS_SPECIFY_SIZE_OF_RAM_HU, "Adja meg a RAM m�ret�t (64/128/256/512/1024 kB�jt)"},
/* it */ {IDCLS_SPECIFY_SIZE_OF_RAM_IT, "Specifica la dimensione della RAM (4/128/256/512/1024 kByte)"},
/* nl */ {IDCLS_SPECIFY_SIZE_OF_RAM_NL, "Geef geheugengrootte (64/128/256/512/1024 kByte)"},
/* pl */ {IDCLS_SPECIFY_SIZE_OF_RAM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIZE_OF_RAM_SV, "Ange storlek p� RAM (64/128/256/512/1024 kByte)"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_ROM_1000_NAME,    N_("Specify name of cartridge ROM image for $1000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_DE, "Name f�r ROM Module Image Datei ($1000)"},
/* fr */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_FR, "Sp�cifier le nom de l�image de cartouche ROM pour $1000"},
/* hu */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_HU, "Adja meg a $1000 c�m� cartridge ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_IT, "Specifica il nome dell'immagine della cartuccia ROM a $1000"},
/* nl */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_NL, "Geef de naam van het cartridge-ROM-bestand voor $1000"},
/* pl */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_ROM_1000_NAME_SV, "Ange namn p� insticksmodul-ROM-avbildning f�r $1000"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_ROM_2000_NAME,    N_("Specify name of cartridge ROM image for $2000-$3fff")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_DE, "Name f�r ROM Module Image Datei ($2000-3fff)"},
/* fr */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_FR, "Sp�cifier le nom de l�image de cartouche ROM pour $2000-$3fff"},
/* hu */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_HU, "Adja meg a $2000-$3fff c�m� cartridge ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_IT, "Specifica il nome dell'immagine della ROM della cartuccia per "
                                               "$2000-$3fff"},
/* nl */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_NL, "Geef de naam van het cartridge-ROM-bestand voor $2000-$3fff"},
/* pl */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_ROM_2000_NAME_SV, "Ange namn p� insticksmodul-ROM-avbildning f�r $2000-$3fff"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_ROM_4000_NAME,    N_("Specify name of cartridge ROM image for $4000-$5fff")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_DE, "Name f�r ROM Module Image Datei ($4000-5fff)"},
/* fr */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_FR, "Sp�cifier le nom de l�image de cartouche ROM pour $4000-$5fff"},
/* hu */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_HU, "Adja meg a $4000-$5fff c�m� cartridge ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_IT, "Specifica il nome dell'immagine della ROM della cartuccia per "
                                               "$4000-$5fff"},
/* nl */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_NL, "Geef de naam van het cartridge-ROM-bestand voor $4000-$5fff"},
/* pl */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_ROM_4000_NAME_SV, "Ange namn p� insticksmodul-ROM-avbildning f�r $4000-$5fff"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_ROM_6000_NAME,    N_("Specify name of cartridge ROM image for $6000-$7fff")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_DE, "Name f�r ROM Module Image Datei ($6000-7fff)"},
/* fr */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_FR, "Sp�cifier le nom de l�image de cartouche ROM pour $6000-$7fff"},
/* hu */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_HU, "Adja meg a $6000-$7fff c�m� cartridge ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_IT, "Specifica il nome dell'immagine della ROM della cartuccia per "
                                               "$6000-$7fff"},
/* nl */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_NL, "Geef de naam van het cartridge-ROM-bestand voor $6000-$7fff"},
/* pl */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_ROM_6000_NAME_SV, "Ange namn p� insticksmodul-ROM-avbildning f�r $6000-$7fff"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800,    N_("Enable RAM mapping in $0800-$0FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_DE, "RAM Zuordnung in $0800-$0FFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_FR, "Activer le mappage RAM dans $0800-$0FFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_HU, "$0800-$0FFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_IT, "Attiva il mappaggio della RAM a $0800-$0FFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_NL, "Activeer RAM op adres $0800-$0FFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_0800_SV, "Aktivera RAM-mappning p� $0800-$0FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800,    N_("Disable RAM mapping in $0800-$0FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_DE, "RAM Zuordnung in $0800-$0FFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_FR, "D�sactiver le mappage RAM dans $0800-$0FFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_HU, "$0800-$0FFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_IT, "Disattiva il mappaggio della RAM a $0800-$0FFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_NL, "RAM op adres $0800-$0FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_0800_SV, "Inaktivera RAM-mappning p� $0800-$0FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000,    N_("Enable RAM mapping in $1000-$1FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_DE, "RAM Zuordnung in $1000-$1FFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_FR, "Activer le mappage RAM dans $1000-$1FFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_HU, "$1000-$1FFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_IT, "Attiva il mappaggio della RAM a $1000-$1FFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_NL, "Activeer RAM op adres $1000-$1FFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_1000_SV, "Aktivera RAM-mappning p� $1000-$1FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000,    N_("Disable RAM mapping in $1000-$1FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_DE, "RAM Zuordnung in $1000-$1FFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_FR, "D�sactiver le mappage RAM dans $1000-$1FFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_HU, "$1000-$1FFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_IT, "Disattiva il mappaggio della RAM a $1000-$1FFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_NL, "RAM op adres $1000-$1FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_1000_SV, "Inaktivera RAM-mappning p� $1000-$1FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000,    N_("Enable RAM mapping in $2000-$3FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_DE, "RAM Zuordnung in $2000-$3FFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_FR, "Activer le mappage RAM dans $2000-$3FFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_HU, "$2000-$3FFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_IT, "Attiva il mappaggio della RAM a $2000-$3FFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_NL, "Activeer RAM op adres $2000-$3FFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_2000_SV, "Aktivera RAM-mappning p� $2000-$3FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000,    N_("Disable RAM mapping in $2000-$3FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_DE, "RAM Zuordnung in $2000-$3FFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_FR, "D�sactiver le mappage RAM dans $2000-$3FFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_HU, "$2000-$3FFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_IT, "Disattiva il mappaggio della RAM a $2000-$3FFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_NL, "RAM op adres $2000-$3FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_2000_SV, "Inaktivera RAM-mappning p� $2000-$3FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000,    N_("Enable RAM mapping in $4000-$5FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_DE, "RAM Zuordnung in $4000-$5FFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_FR, "Activer le mappage RAM dans $4000-$5FFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_HU, "$4000-$5FFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_IT, "Attiva il mappaggio della RAM a $4000-$5FFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_NL, "Activeer RAM op adres $4000-$5FFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_4000_SV, "Aktivera RAM-mappning p� $4000-$5FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000,    N_("Disable RAM mapping in $4000-$5FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_DE, "RAM Zuordnung in $4000-$5FFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_FR, "D�sactiver le mappage RAM dans $4000-$5FFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_HU, "$4000-$5FFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_IT, "Disattiva il mappaggio della RAM a $4000-$5FFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_NL, "RAM op adres $4000-$5FFF uitchakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_4000_SV, "Inaktivera RAM-mappning p� $4000-$5FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000,    N_("Enable RAM mapping in $6000-$7FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_DE, "RAM Zuordnung in $6000-$7FFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_FR, "Activer le mappage RAM dans $6000-$7FFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_HU, "$6000-$7FFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_IT, "Attiva il mappaggio della RAM a $6000-$7FFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_NL, "Activeer RAM op adres $6000-$7FFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_6000_SV, "Aktivera RAM-mappning p� $6000-$7FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000,    N_("Disable RAM mapping in $6000-$7FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_DE, "RAM Zuordnung in $6000-$7FFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_FR, "D�sactiver le mappage RAM dans $6000-$7FFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_HU, "$6000-$7FFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_IT, "Disattiva il mappaggio della RAM a $6000-$7FFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_NL, "RAM op adres $6000-$7FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_6000_SV, "Inaktivera RAM-mappning p� $6000-$7FFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000,    N_("Enable RAM mapping in $C000-$CFFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_DE, "RAM Zuordnung in $C000-$CFFF aktivieren"},
/* fr */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_FR, "Activer le mappage RAM dans $C000-$CFFF"},
/* hu */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_HU, "$C000-$CFFF RAM lek�pz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_IT, "Attiva il mappaggio della RAM a $C000-$CFFF"},
/* nl */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_NL, "Activeer RAM op adres $C000-$CFFF"},
/* pl */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RAM_MAPPING_IN_C000_SV, "Aktivera RAM-mappning p� $C000-$CFFF"},
#endif

/* cbm2/cbm2-cmdline-options.c */
/* en */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000,    N_("Disable RAM mapping in $C000-$CFFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_DE, "RAM Zuordnung in $C000-$CFFF deaktivieren"},
/* fr */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_FR, "D�sactiver le mappage RAM dans $C000-$CFFF"},
/* hu */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_HU, "$C000-$CFFF RAM lek�pz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_IT, "Disattiva il mappaggio della RAM a $C000-$CFFF"},
/* nl */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_NL, "RAM op adres $C000-$CFFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RAM_MAPPING_IN_C000_SV, "Inaktivera RAM-mappning p� $C000-$CFFF"},
#endif

#ifdef COMMON_KBD
/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_KEYMAP_INDEX,    N_("Specify index of keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_KEYMAP_INDEX_DE, "Index der Keymap Datei definieren"},
/* fr */ {IDCLS_SPECIFY_KEYMAP_INDEX_FR, "Sp�cifier l'index du fichier de mappage clavier"},
/* hu */ {IDCLS_SPECIFY_KEYMAP_INDEX_HU, "Adja meg a billenty�zet lek�pz�s f�jl t�pus�t"},
/* it */ {IDCLS_SPECIFY_KEYMAP_INDEX_IT, "Specifica l'indice del file della mappa della tastiera"},
/* nl */ {IDCLS_SPECIFY_KEYMAP_INDEX_NL, "Geef de index van het keymapbestand"},
/* pl */ {IDCLS_SPECIFY_KEYMAP_INDEX_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_KEYMAP_INDEX_SV, "Ange index o tangentbordsfil"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME,    N_("Specify name of graphics keyboard symbolic keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_DE, "Keymap Datei f�r graphics keyboard symbolic definieren"},
/* fr */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier symbolique"},
/* hu */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_HU, "Adja meg a grafikus �s szimbolikus billenty�zet lek�pz�si f�jl nev�t."},
/* it */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_IT, "Specifica il nome del file della mappa simbolica della tastiera grafica"},
/* nl */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_NL, "Geef de naam van het symbolisch keymapbestand voor het grafische "
                                                "toetsenbord"},
/* pl */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_GFX_SYM_KEYMAP_NAME_SV, "Ange fil f�r symbolisk tangentbordsemulering f�r "
                                                "\"graphics\"-tangentbord"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME,    N_("Specify name of graphics keyboard positional keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_DE, "Keymap Datei f�r graphics keyboard positional definieren"},
/* fr */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier positionnel"},
/* hu */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_HU, "Adja meg a grafikus �s poz�ci� szerinti billenty�zet lek�pz�si f�jl "
                                                "nev�t."},
/* it */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_IT, "Specifica il nome del file della mappa posizionale della tastiera "
                                                "grafica"},
/* nl */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_NL, "Geef de naam van het positioneel keymapbestand voor het grafische "
                                                "toetsenbord"},
/* pl */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_GFX_POS_KEYMAP_NAME_SV, "Ange fil f�r positionsriktig tangentbordsemulering f�r "
                                                "\"graphics\"-tangentbord"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME,    N_("Specify name of UK business keyboard symbolic keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_DE, "Keymap Datei f�r UK business keyboard symbolic definieren"},
/* fr */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier symbolique UK"},
/* hu */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_HU, "Adja meg az angol, hivatalos, szimbolikus billenty�zet lek�pz�si f�jl "
                                                "nev�t."},
/* it */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_IT, "Specifica il nome del file della mappa simbolica della tastiera UK "
                                                "business"},
/* nl */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_NL, "Geef de naam van het symbolisch keymapbestand voor het "
                                                "UK-businesstoetsenbord"},
/* pl */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BUK_SYM_KEYMAP_NAME_SV, "Ange fil f�r symbolisk tangentbordsemulering f�r brittiskt "
                                                "\"business\"-tangentbord"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME,    N_("Specify name of UK business keyboard positional keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_DE, "Keymap Datei f�r UK business keyboard positional definieren"},
/* fr */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier positionnel UK"},
/* hu */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_HU, "Adja meg a UK hivatalos poz�ci� szerinti billenty�zet lek�pz�si f�jl "
                                                "nev�t."},
/* it */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_IT, "Adja meg az angol, hivatalos, poz�ci� szerinti billenty�zet lek�pz�si "
                                                "f�jl nev�t."},
/* nl */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_NL, "Geef de naam van het positioneel keymapbestand voor het "
                                                "UK-businesstoetsenbord"},
/* pl */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BUK_POS_KEYMAP_NAME_SV, "Ange fil f�r positionsriktig tangentbordsemulering f�r brittiskt "
                                                "\"business\"-tangentbord"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME,    N_("Specify name of German business keyboard symbolic keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_DE, "Keymap Datei f�r German business keyboard symbolic definieren"},
/* fr */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier symbolique allemand"},
/* hu */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_HU, "Adja meg a n�met, hivatalos, szimbolikus billenty�zet lek�pz�si f�jl "
                                                "nev�t."},
/* it */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_IT, "Specifica il nome del file della mappa simbolica della tastiera "
                                                "business tedesca"},
/* nl */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_NL, "Geef de naam van het symbolisch keymapbestand voor het Duitse "
                                                "businesstoetsenbord"},
/* pl */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BDE_SYM_KEYMAP_NAME_SV, "Ange fil f�r symbolisk tangentbordsemulering f�r tyskt "
                                                "\"business\"-tangentbord"},
#endif

/* cbm2/cbm2-cmdline-options.c, pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME,    N_("Specify name of German business keyboard positional keymap file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_DE, "Keymap Datei f�r German business keyboard positional definieren"},
/* fr */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_FR, "Sp�cifier le nom du fichier de mappage clavier positionnel allemand"},
/* hu */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_HU, "Adja meg a n�met, hivatalos, poz�ci� szerinti billenty�zet lek�pz�si "
                                                "f�jl nev�t."},
/* it */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_IT, "Specifica il nome del file della mappa posizionale della tastiera "
                                                "business tedesca"},
/* nl */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_NL, "Geef de naam van het positioneel keymapbestand voor het Duitse "
                                                "businesstoetsenbord"},
/* pl */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_BDE_POS_KEYMAP_NAME_SV, "Ange fil f�r positionsriktig tangentbordsemulering f�r tyskt "
                                                "\"business\"-tangentbord"},
#endif
#endif

/* drive/drive-cmdline-options.c */
/* en */ {IDCLS_ENABLE_TRUE_DRIVE,    N_("Enable hardware-level emulation of disk drives")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_TRUE_DRIVE_DE, "Hardware Emulation f�r Disklaufwerke aktivieren"},
/* fr */ {IDCLS_ENABLE_TRUE_DRIVE_FR, "Activer l'�mulation de disques au niveau physique"},
/* hu */ {IDCLS_ENABLE_TRUE_DRIVE_HU, "Lemezegys�gek hardver szint� emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_TRUE_DRIVE_IT, "Attiva l'emulazione hardware dei disk drive"},
/* nl */ {IDCLS_ENABLE_TRUE_DRIVE_NL, "Activeer hardwarematige emulatie van diskdrives"},
/* pl */ {IDCLS_ENABLE_TRUE_DRIVE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_TRUE_DRIVE_SV, "Aktivera emulering av diskettstationer p� maskinvaruniv�"},
#endif

/* drive/drive-cmdline-options.c */
/* en */ {IDCLS_DISABLE_TRUE_DRIVE,    N_("Disable hardware-level emulation of disk drives")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_TRUE_DRIVE_DE, "Hardware Emulation f�r Disklaufwerke deaktivieren"},
/* fr */ {IDCLS_DISABLE_TRUE_DRIVE_FR, "D�sactiver l'�mulation de disques au niveau physique"},
/* hu */ {IDCLS_DISABLE_TRUE_DRIVE_HU, "Lemezegys�gek hardver szint� emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_TRUE_DRIVE_IT, "Disattiva l'emulazione hardware dei disk drive"},
/* nl */ {IDCLS_DISABLE_TRUE_DRIVE_NL, "Hardwarematige emulatie van diskdrives uitschakelen"},
/* pl */ {IDCLS_DISABLE_TRUE_DRIVE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_TRUE_DRIVE_SV, "Inaktivera emulering av diskettstationer p� maskinvaruniv�"},
#endif

/* drive/drive-cmdline-options.c, printerdrv/interface-serial.c,
   attach.c, drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_P_TYPE,    N_("<type>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_TYPE_DE, "<Typ>"},
/* fr */ {IDCLS_P_TYPE_FR, "<type>"},
/* hu */ {IDCLS_P_TYPE_HU, "<t�pus>"},
/* it */ {IDCLS_P_TYPE_IT, "<tipo>"},
/* nl */ {IDCLS_P_TYPE_NL, "<soort>"},
/* pl */ {IDCLS_P_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_TYPE_SV, "<typ>"},
#endif

/* drive/drive-cmdline-options.c */
/* en */ {IDCLS_SET_DRIVE_TYPE,    N_("Set drive type (0: no drive)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DRIVE_TYPE_DE, "Setze Laufwerkstyp (0: kein Laufwerk)"},
/* fr */ {IDCLS_SET_DRIVE_TYPE_FR, "Sp�cifier le type de lecteur de disque (0: pas de lecteur)"},
/* hu */ {IDCLS_SET_DRIVE_TYPE_HU, "Adja meg a lemezegys�g t�pus�t (0: nincs)"},
/* it */ {IDCLS_SET_DRIVE_TYPE_IT, "Imposta il tipo di drive (0: nessun drive)"},
/* nl */ {IDCLS_SET_DRIVE_TYPE_NL, "Zet drive soort (0: geen drive)"},
/* pl */ {IDCLS_SET_DRIVE_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DRIVE_TYPE_SV, "Ange enhetstyp (0: ingen enhet)"},
#endif

/* drive/drive-cmdline-options.c,
   drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_P_METHOD,    N_("<method>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_METHOD_DE, "<Methode>"},
/* fr */ {IDCLS_P_METHOD_FR, "<methode>"},
/* hu */ {IDCLS_P_METHOD_HU, "<m�d>"},
/* it */ {IDCLS_P_METHOD_IT, "<metodo>"},
/* nl */ {IDCLS_P_METHOD_NL, "<methode>"},
/* pl */ {IDCLS_P_METHOD_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_METHOD_SV, "<metod>"},
#endif

/* drive/drive-cmdline-options.c */
/* en */ {IDCLS_SET_DRIVE_EXTENSION_POLICY,    N_("Set drive 40 track extension policy (0: never, 1: ask, 2: on access)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_DE, "40 Spur Erweiterungsmethode (0: nie, 1: R�ckfrage, 2: bei Bedarf)"},
/* fr */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_FR, "Sp�cifier la r�gle d'extention 40 pistes (0: jamais, 1: demander, 2: � "
                                               "l'acc�s)"},
/* hu */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_HU, "Adja meg a 40 s�vra kiterjeszt�s terjeszt�s m�dj�t (0: soha, 1: k�rd�s, "
                                               "2: hozz�f�r�skor)"},
/* it */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_IT, "Imposta la politica di estensione a 40 tracce del drive (0: mai, 1: a "
                                               "richiesta, 2: all'accesso)"},
/* nl */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_NL, "Stel beleid voor 40-sporenuitbreiding drive in (0: nooit, 1: vraag, 2: "
                                               "bij toegang)"},
/* pl */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DRIVE_EXTENSION_POLICY_SV, "Ange regel f�r 40-sp�rsut�kning (0: aldrig, 1: fr�ga, 2: vid �tkomst)"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME,    N_("Specify name of 1541 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_DE, "Name der 1541 DOS ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1541 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_HU, "Adja meg az 1541 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1541"},
/* nl */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_NL, "Geef de naam van het 1541 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1541_DOS_ROM_NAME_SV, "Ange namn p� 1541-DOS-ROM-avbildning"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME,    N_("Specify name of 1541-II DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_DE, "Name der 1541-II DOS ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1541-II DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_HU, "Adja meg az 1541-II DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1541-II"},
/* nl */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_NL, "Geef de naam van het 1541-II DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1541_II_DOS_ROM_NAME_SV, "Ange namn p� 1541-II-DOS-ROM-avbildning"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME,    N_("Specify name of 1570 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_DE, "Name der 1570 DOS ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1570 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_HU, "Adja meg az 1570 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1570"},
/* nl */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_NL, "Geef de naam van het 1570 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1570_DOS_ROM_NAME_SV, "Ange namn p� 1570-DOS-ROM-avbildning"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME,    N_("Specify name of 1571 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_DE, "Name der 1571 DOS ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1571 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_HU, "Adja meg az 1571 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1571"},
/* nl */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_NL, "Geef de naam van het 1571 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1571_DOS_ROM_NAME_SV, "Ange namn p� 1571-DOS-ROM-avbildning"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME,    N_("Specify name of 1581 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_DE, "Name der 1581 DOS ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1581 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_HU, "Adja meg az 1581 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1581"},
/* nl */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_NL, "Geef de naam van het 1581 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1581_DOS_ROM_NAME_SV, "Ange namn p� 1581-DOS-ROM-avbildning"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_SET_IDLE_METHOD,    N_("Set drive idling method (0: no traps, 1: skip cycles, 2: trap idle)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_IDLE_METHOD_DE, "Laufwerks idling Methode (0: kein Traps, 1: Zyklen verwerfen, 2: trap idle)"},
/* fr */ {IDCLS_SET_IDLE_METHOD_FR, "Sp�cifier la m�thode d'idle (0: pas de traps, 1: sauter des cycles, 2: trap idle)"},
/* hu */ {IDCLS_SET_IDLE_METHOD_HU, "Lemezegys�g �resj�r�si m�dja (0: folytonos emul�ci�, 1: ciklusok kihagy�sa, 2: �res "
                                    "DOS ciklusok kihagy�sa)"},
/* it */ {IDCLS_SET_IDLE_METHOD_IT, "Imposta il metodo per rilevare l'inattivit� del drive (0: non rilevare, 1: "
                                    "salta cicli, 2: rileva inattivit�)"},
/* nl */ {IDCLS_SET_IDLE_METHOD_NL, "Stel de drive-idlemethode in (0: geen traps, 1: sla cycli over, 2: trap idle)"},
/* pl */ {IDCLS_SET_IDLE_METHOD_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_IDLE_METHOD_SV, "St�ll in v�ntemetod f�r diskettstation (0: f�nta inge, 1: hoppa cykler, 2: f�nga "
                                    "v�ntan)"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DRIVE_RAM_2000,    N_("Enable 8KB RAM expansion at $2000-$3FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DRIVE_RAM_2000_DE, "8KB RAM Erweiterung bei $2000-3fff aktivieren"},
/* fr */ {IDCLS_ENABLE_DRIVE_RAM_2000_FR, "Enable 8KB RAM expansion at $2000-$3FFF"},
/* hu */ {IDCLS_ENABLE_DRIVE_RAM_2000_HU, "8KB RAM kiterjesz�s enged�lyez�se a $2000-$3FFF c�men"},
/* it */ {IDCLS_ENABLE_DRIVE_RAM_2000_IT, "Attiva l'espansione di RAM di 8KB a $2000-$3FFF"},
/* nl */ {IDCLS_ENABLE_DRIVE_RAM_2000_NL, "Activeer 8KB RAM-uitbreiding op adres $2000-$3FFF"},
/* pl */ {IDCLS_ENABLE_DRIVE_RAM_2000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DRIVE_RAM_2000_SV, "Aktivera 8KB RAM-expansion p� $2000-$3FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DRIVE_RAM_2000,    N_("Disable 8KB RAM expansion at $2000-$3FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DRIVE_RAM_2000_DE, "8KB RAM Erweiterung bei $2000-3fff deaktivieren"},
/* fr */ {IDCLS_DISABLE_DRIVE_RAM_2000_FR, "D�sactiver l'exansion de 8KB RAM � $2000-$3FFF"},
/* hu */ {IDCLS_DISABLE_DRIVE_RAM_2000_HU, "8KB RAM kiterjesz�s tilt�sa a $2000-$3FFF c�men"},
/* it */ {IDCLS_DISABLE_DRIVE_RAM_2000_IT, "Disattiva l'espansione di RAM di 8KB a $2000-$3FFF"},
/* nl */ {IDCLS_DISABLE_DRIVE_RAM_2000_NL, "8KB RAM-uitbreiding op adres $2000-$3FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_DRIVE_RAM_2000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DRIVE_RAM_2000_SV, "Inaktivera 8KB RAM-expansion p� $2000-$3FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DRIVE_RAM_4000,    N_("Enable 8KB RAM expansion at $4000-$5FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DRIVE_RAM_4000_DE, "8KB RAM Erweiterung bei $4000-5fff aktivieren"},
/* fr */ {IDCLS_ENABLE_DRIVE_RAM_4000_FR, "Enable 8KB RAM expansion at $4000-$5FFF"},
/* hu */ {IDCLS_ENABLE_DRIVE_RAM_4000_HU, "8KB RAM kiterjesz�s enged�lyez�se a $4000-$5FFF c�men"},
/* it */ {IDCLS_ENABLE_DRIVE_RAM_4000_IT, "Attiva l'espansione di RAM di 8KB a $4000-$5FFF"},
/* nl */ {IDCLS_ENABLE_DRIVE_RAM_4000_NL, "Activeer 8KB RAM-uitbreiding op adres $4000-$5FFF"},
/* pl */ {IDCLS_ENABLE_DRIVE_RAM_4000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DRIVE_RAM_4000_SV, "Aktivera 8KB RAM-expansion p� $4000-$5FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DRIVE_RAM_4000,    N_("Disable 8KB RAM expansion at $4000-$5FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DRIVE_RAM_4000_DE, "8KB RAM Erweiterung bei $4000-5fff deaktivieren"},
/* fr */ {IDCLS_DISABLE_DRIVE_RAM_4000_FR, "D�sactiver l'exansion de 8KB RAM � $4000-$5FFF"},
/* hu */ {IDCLS_DISABLE_DRIVE_RAM_4000_HU, "8KB RAM kiterjesz�s tilt�sa a $4000-$5FFF c�men"},
/* it */ {IDCLS_DISABLE_DRIVE_RAM_4000_IT, "Disattiva l'espansione di RAM di 8KB a $4000-$5FFF"},
/* nl */ {IDCLS_DISABLE_DRIVE_RAM_4000_NL, "8KB RAM-uitbreiding op adres $4000-$5FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_DRIVE_RAM_4000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DRIVE_RAM_4000_SV, "Inaktivera 8KB RAM-expansion p� $4000-$5FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DRIVE_RAM_6000,    N_("Enable 8KB RAM expansion at $6000-$7FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DRIVE_RAM_6000_DE, "8KB RAM Erweiterung bei $6000-7fff aktivieren"},
/* fr */ {IDCLS_ENABLE_DRIVE_RAM_6000_FR, "Enable 8KB RAM expansion at $6000-$7FFF"},
/* hu */ {IDCLS_ENABLE_DRIVE_RAM_6000_HU, "8KB RAM kiterjesz�s enged�lyez�se a $6000-$7FFF c�men"},
/* it */ {IDCLS_ENABLE_DRIVE_RAM_6000_IT, "Attiva l'espansione di RAM di 8KB a $6000-$7FFF"},
/* nl */ {IDCLS_ENABLE_DRIVE_RAM_6000_NL, "Activeer 8KB RAM-uitbreiding op adres $6000-$7FFF"},
/* pl */ {IDCLS_ENABLE_DRIVE_RAM_6000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DRIVE_RAM_6000_SV, "Aktivera 8KB RAM-expansion p� $6000-$7FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DRIVE_RAM_6000,    N_("Disable 8KB RAM expansion at $6000-$7FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DRIVE_RAM_6000_DE, "8KB RAM Erweiterung bei $6000-7fff deaktivieren"},
/* fr */ {IDCLS_DISABLE_DRIVE_RAM_6000_FR, "D�sactiver l'exansion de 8KB RAM � $6000-$7FFF"},
/* hu */ {IDCLS_DISABLE_DRIVE_RAM_6000_HU, "8KB RAM kiterjesz�s tilt�sa a $6000-$7FFF c�men"},
/* it */ {IDCLS_DISABLE_DRIVE_RAM_6000_IT, "Disattiva l'espansione di RAM di 8KB a $6000-$7FFF"},
/* nl */ {IDCLS_DISABLE_DRIVE_RAM_6000_NL, "8KB RAM-uitbreiding op adres $6000-$7FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_DRIVE_RAM_6000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DRIVE_RAM_6000_SV, "Inaktivera 8KB RAM-expansion p� $6000-$7FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DRIVE_RAM_8000,    N_("Enable 8KB RAM expansion at $8000-$9FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DRIVE_RAM_8000_DE, "8KB RAM Erweiterung bei $8000-9fff aktivieren"},
/* fr */ {IDCLS_ENABLE_DRIVE_RAM_8000_FR, "Enable 8KB RAM expansion at $8000-$9FFF"},
/* hu */ {IDCLS_ENABLE_DRIVE_RAM_8000_HU, "8KB RAM kiterjesz�s enged�lyez�se a $8000-$9FFF c�men"},
/* it */ {IDCLS_ENABLE_DRIVE_RAM_8000_IT, "Attiva l'espansione di RAM di 8KB a $8000-$9FFF"},
/* nl */ {IDCLS_ENABLE_DRIVE_RAM_8000_NL, "Activeer 8KB RAM-uitbreiding op adres $8000-$9FFF"},
/* pl */ {IDCLS_ENABLE_DRIVE_RAM_8000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DRIVE_RAM_8000_SV, "Aktivera 8KB RAM-expansion p� $8000-$9FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DRIVE_RAM_8000,    N_("Disable 8KB RAM expansion at $8000-$9FFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DRIVE_RAM_8000_DE, "8KB RAM Erweiterung bei $8000-9fff deaktivieren"},
/* fr */ {IDCLS_DISABLE_DRIVE_RAM_8000_FR, "D�sactiver l'exansion de 8KB RAM � $8000-$9FFF"},
/* hu */ {IDCLS_DISABLE_DRIVE_RAM_8000_HU, "8KB RAM kiterjesz�s tilt�sa a $8000-$9FFF c�men"},
/* it */ {IDCLS_DISABLE_DRIVE_RAM_8000_IT, "Disattiva l'espansione di RAM di 8KB a $8000-$9FFF"},
/* nl */ {IDCLS_DISABLE_DRIVE_RAM_8000_NL, "8KB RAM-uitbreiding op adres $8000-$9FFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_DRIVE_RAM_8000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DRIVE_RAM_8000_SV, "Inaktivera 8KB RAM-expansion p� $8000-$9FFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DRIVE_RAM_A000,    N_("Enable 8KB RAM expansion at $A000-$BFFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DRIVE_RAM_A000_DE, "8KB RAM Erweiterung bei $a000-bfff aktivieren"},
/* fr */ {IDCLS_ENABLE_DRIVE_RAM_A000_FR, "Enable 8KB RAM expansion at $A000-$BFFF"},
/* hu */ {IDCLS_ENABLE_DRIVE_RAM_A000_HU, "8KB RAM kiterjesz�s enged�lyez�se a $A000-$BFFF c�men"},
/* it */ {IDCLS_ENABLE_DRIVE_RAM_A000_IT, "Attiva l'espansione di RAM di 8KB a $A000-$BFFF"},
/* nl */ {IDCLS_ENABLE_DRIVE_RAM_A000_NL, "Activeer 8KB RAM-uitbreiding op adres $A000-$BFFF"},
/* pl */ {IDCLS_ENABLE_DRIVE_RAM_A000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DRIVE_RAM_A000_SV, "Aktivera 8KB RAM-expansion p� $A000-$BFFF"},
#endif

/* drive/iec/iec-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DRIVE_RAM_A000,    N_("Disable 8KB RAM expansion at $A000-$BFFF")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DRIVE_RAM_A000_DE, "8KB RAM Erweiterung bei $a000-bfff deaktivieren"},
/* fr */ {IDCLS_DISABLE_DRIVE_RAM_A000_FR, "D�sactiver l'exansion de 8KB RAM � $A000-$BFFF"},
/* hu */ {IDCLS_DISABLE_DRIVE_RAM_A000_HU, "8KB RAM kiterjesz�s tilt�sa a $A000-$BFFF c�men"},
/* it */ {IDCLS_DISABLE_DRIVE_RAM_A000_IT, "Disattiva l'espansione di RAM di 8KB a $A000-$BFFF"},
/* nl */ {IDCLS_DISABLE_DRIVE_RAM_A000_NL, "8KB RAM-uitbreiding op adres $A000-$BFFF uitschakelen"},
/* pl */ {IDCLS_DISABLE_DRIVE_RAM_A000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DRIVE_RAM_A000_SV, "Inaktivera 8KB RAM-expansion p� $A000-$BFFF"},
#endif

/* drive/iec/c64exp/c64exp-cmdline-options.c */
/* en */ {IDCLS_PAR_CABLE_C64EXP_TYPE,    N_("Set parallel cable type (0: none, 1: standard, 2: Dolphin DOS 3)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PAR_CABLE_C64EXP_TYPE_DE, "Parallelkabel Typ (0: kein, 1: Standard, 2: Dolphin DOS 3)"},
/* fr */ {IDCLS_PAR_CABLE_C64EXP_TYPE_FR, "D�finir le type de c�ble parall�le (0: aucun, 1: standard, 2: Dolphin DOS 3)"},
/* hu */ {IDCLS_PAR_CABLE_C64EXP_TYPE_HU, "Adja meg a p�rhuzamos k�bel t�pus�t (0: nincs, 1: standard, 2: Dolphin DOS 3)"},
/* it */ {IDCLS_PAR_CABLE_C64EXP_TYPE_IT, "Imposta il tipo di cavo parallelo (0: nessuno, 1: standard, 2: Dolphin DOS 3)"},
/* nl */ {IDCLS_PAR_CABLE_C64EXP_TYPE_NL, "Zet parallele-kabelsoort (0: geen, 1: standaard, 2: Dolphin DOS 3)"},
/* pl */ {IDCLS_PAR_CABLE_C64EXP_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PAR_CABLE_C64EXP_TYPE_SV, "V�lj typ av parallellkabel (0: ingen, 1: standard, 2: Dolphin DOS 3)"},
#endif

/* drive/iec/c64exp/c64exp-cmdline-options.c */
/* en */ {IDCLS_ENABLE_PROFDOS,    N_("Enable Professional DOS")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_PROFDOS_DE, "Professional DOS aktivieren"},
/* fr */ {IDCLS_ENABLE_PROFDOS_FR, "Activer le DOS Professionnel"},
/* hu */ {IDCLS_ENABLE_PROFDOS_HU, "Professional DOS enged�lyez�se"},
/* it */ {IDCLS_ENABLE_PROFDOS_IT, "Attiva Professional DOS"},
/* nl */ {IDCLS_ENABLE_PROFDOS_NL, "Activeer Professional DOS"},
/* pl */ {IDCLS_ENABLE_PROFDOS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_PROFDOS_SV, "Aktivera Professional DOS"},
#endif

/* drive/iec/c64exp/c64exp-cmdline-options.c */
/* en */ {IDCLS_DISABLE_PROFDOS,    N_("Disable Professional DOS")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_PROFDOS_DE, "Professional DOS deaktivieren"},
/* fr */ {IDCLS_DISABLE_PROFDOS_FR, "D�sactiver le DOS Professionnel"},
/* hu */ {IDCLS_DISABLE_PROFDOS_HU, "Professional DOS tilt�sa"},
/* it */ {IDCLS_DISABLE_PROFDOS_IT, "Disattiva Professional DOS"},
/* nl */ {IDCLS_DISABLE_PROFDOS_NL, "Professional DOS uitschakelen"},
/* pl */ {IDCLS_DISABLE_PROFDOS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_PROFDOS_SV, "Inaktivera Professional DOS"},
#endif

/* drive/iec/c64exp/c64exp-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME,    N_("Specify name of Professional DOS 1571 ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_DE, "Name von Professional DOS 1571 Datei definieren"},
/* fr */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_HU, "Adja meg a Professional DOS 1571 ROM k�pm�st"},
/* it */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del Professional DOS 1571"},
/* nl */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_NL, "Geef de naam van het Professional DOS 1571 ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PROFDOS_1571_ROM_NAME_SV, "Ange namn p� Professional DOS 1571-ROM-avbildning"},
#endif

/* drive/iec/plus4exp/plus4exp-cmdline-options.c */
/* en */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE,    N_("Set parallel cable type (0: none, 1: standard)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_DE, "Parallelkabel Typ (0: kein, 1: Standard)"},
/* fr */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_FR, "D�finir le type de c�ble parall�le (0: aucun, 1: standard)"},
/* hu */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_HU, "Adja meg a p�rhuzamos k�bel t�pus�t (0: nincs, 1: standard)"},
/* it */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_IT, "Imposta il tipo di cavo parallelo (0: nessuno, 1: standard)"},
/* nl */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_NL, "Zet parallelle-kabelsoort (0: geen, 1: standaard)"},
/* pl */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PAR_CABLE_PLUS4EXP_TYPE_SV, "V�lj typ av parallellkabel (0: ingen, 1: standard)"},
#endif

/* drive/iec128dcr/iec128dcr-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME,    N_("Specify name of 1571CR DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_DE, "Name der 1571CR DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1571CR DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_HU, "Adja meg az 1571CR DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1571CR"},
/* nl */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_NL, "Geef de naam van het 1571CR DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1571CR_DOS_ROM_NAME_SV, "Ange namn p� 1571CR-DOS-ROM-avbildning"},
#endif

/* drive/ieee/ieee-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME,    N_("Specify name of 2031 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_DE, "Name der 2031 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 2031 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_HU, "Adja meg a 2031 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 2031"},
/* nl */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_NL, "Geef de naam van het 2031 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_2031_DOS_ROM_NAME_SV, "Ange namn p� 2031-DOS-ROM-avbildning"},
#endif

/* drive/ieee/ieee-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME,    N_("Specify name of 2040 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_DE, "Name der 2040 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 2040 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_HU, "Adja meg a 2040 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 2040"},
/* nl */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_NL, "Geef de naam van het 2040 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_2040_DOS_ROM_NAME_SV, "Ange namn p� 2040-DOS-ROM-avbildning"},
#endif

/* drive/ieee/ieee-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME,    N_("Specify name of 3040 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_DE, "Name der 3040 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 3040 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_HU, "Adja meg a 3040 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 3040"},
/* nl */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_NL, "Geef de naam van het 3040 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_3040_DOS_ROM_NAME_SV, "Ange namn p� 3040-DOS-ROM-avbildning"},
#endif

/* drive/ieee/ieee-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME,    N_("Specify name of 4040 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_DE, "Name der 4040 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 4040 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_HU, "Adja meg a 4040 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 4040"},
/* nl */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_NL, "Geef de naam van het 4040 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_4040_DOS_ROM_NAME_SV, "Ange namn p� 4040-DOS-ROM-avbildning"},
#endif

/* drive/ieee/ieee-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME,    N_("Specify name of 1001/8050/8250 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_DE, "Name der 1001/8040/8205 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1001/8050/8250 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_HU, "Adja meg az 1001/8050/8250 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1001/8050/8250"},
/* nl */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_NL, "Geef de naam van het 1001/8050/8250 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1001_DOS_ROM_NAME_SV, "Ange namn p� 1001/8050/8250-DOS-ROM-avbildning"},
#endif

/* drive/tcbm/tcbm-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME,    N_("Specify name of 1551 DOS ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_DE, "Name der 4040 DOS ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_FR, "Sp�cifier le nom de l'image 1551 DOS ROM"},
/* hu */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_HU, "Adja meg az 1551 DOS ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM del DOS del 1551"},
/* nl */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_NL, "Geef de naam van het 1551 DOS ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_1551_DOS_ROM_NAME_SV, "Ange namn p� 1551-DOS-ROM-avbildning"},
#endif

/* fsdevice/fsdevice-cmdline-options.c */
/* en */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8,    N_("Use <name> as directory for file system device #8")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_DE, "Benutze <Name> f�r Verzeichnis beim Verzeichniszugriff f�r Ger�t #8"},
/* fr */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_FR, "Utiliser le r�pertoire <nom> comme syst�me de fichiers pour le lecteur "
                                                "#8"},
/* hu */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_HU, "<n�v> k�nyvt�r haszn�lata az #8-as egys�g f�jlrendszer�nek"},
/* it */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_IT, "Una <nome> come direcory per il file system della periferica #8"},
/* nl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_NL, "Gebruik <naam> als directory voor bestandssysteemapparaat #8"},
/* pl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_8_SV, "Ange <namn> som katalog f�r filsystemsbaserad enhet 8"},
#endif

/* fsdevice/fsdevice-cmdline-options.c */
/* en */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9,    N_("Use <name> as directory for file system device #9")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_DE, "Benutze <Name> f�r Verzeichnis beim Verzeichniszugriff f�r Ger�t #9"},
/* fr */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_FR, "Utiliser le r�pertoire <nom> comme syst�me de fichiers pour le lecteur "
                                                "#9"},
/* hu */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_HU, "<n�v> k�nyvt�r haszn�lata az #9-es egys�g f�jlrendszer�nek"},
/* it */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_IT, "Una <nome> come direcory per il file system della periferica #9"},
/* nl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_NL, "Gebruik <naam> als directory voor bestandssysteemapparaat #9"},
/* pl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_9_SV, "Ange <namn> som katalog f�r filsystemsbaserad enhet 9"},
#endif

/* fsdevice/fsdevice-cmdline-options.c */
/* en */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10,    N_("Use <name> as directory for file system device #10")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_DE, "Benutze <Name> f�r Verzeichnis beim Verzeichniszugriff f�r Ger�t #10"},
/* fr */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_FR, "Utiliser le r�pertoire <nom> comme syst�me de fichiers pour le lecteur "
                                                 "#10"},
/* hu */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_HU, "<n�v> k�nyvt�r haszn�lata az #10-es egys�g f�jlrendszer�nek"},
/* it */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_IT, "Una <nome> come direcory per il file system della periferica #10"},
/* nl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_NL, "Gebruik <naam> als directory voor bestandssysteemapparaat #10"},
/* pl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_10_SV, "Ange <namn> som katalog f�r filsystemsbaserad enhet 10"},
#endif

/* fsdevice/fsdevice-cmdline-options.c */
/* en */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11,    N_("Use <name> as directory for file system device #11")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_DE, "Benutze <Name> f�r Verzeichnis beim Verzeichniszugriff f�r Ger�t #11"},
/* fr */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_FR, "Utiliser le r�pertoire <nom> comme syst�me de fichiers pour le lecteur "
                                                 "#11"},
/* hu */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_HU, "<n�v> k�nyvt�r haszn�lata az #11-es egys�g f�jlrendszer�nek"},
/* it */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_IT, "Una <nome> come direcory per il file system della periferica #11"},
/* nl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_NL, "Gebruik <naam> als directory voor bestandssysteemapparaat #11"},
/* pl */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_AS_DIRECTORY_FSDEVICE_11_SV, "Ange <namn> som katalog f�r filsystemsbaserad enhet 11"},
#endif

#ifdef HAVE_FFMPEG
/* gfxoutputdrv/ffmpegdrv.c */
/* en */ {IDCLS_SET_AUDIO_STREAM_BITRATE,    N_("Set bitrate for audio stream in media file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_AUDIO_STREAM_BITRATE_DE, "Bitrate f�r Audio Stream f�r Media Datei setzen"},
/* fr */ {IDCLS_SET_AUDIO_STREAM_BITRATE_FR, "R�gler le bitrate audio du fichier m�dia"},
/* hu */ {IDCLS_SET_AUDIO_STREAM_BITRATE_HU, "Adja meg az audi� bitr�t�t a m�dia f�jlhoz"},
/* it */ {IDCLS_SET_AUDIO_STREAM_BITRATE_IT, "Imposta il bitrate del flusso audio nel file multimediale"},
/* nl */ {IDCLS_SET_AUDIO_STREAM_BITRATE_NL, "Zet de bitrate voor het audiogedeelte van het mediabestand"},
/* pl */ {IDCLS_SET_AUDIO_STREAM_BITRATE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_AUDIO_STREAM_BITRATE_SV, "Ange bithastighet f�r ljudstr�m i mediafil"},
#endif

/* gfxoutputdrv/ffmpegdrv.c */
/* en */ {IDCLS_SET_VIDEO_STREAM_BITRATE,    N_("Set bitrate for video stream in media file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_VIDEO_STREAM_BITRATE_DE, "Bitrate f�r Video Stream f�r Media Datei setzen"},
/* fr */ {IDCLS_SET_VIDEO_STREAM_BITRATE_FR, "R�gler le bitrate vid�o du fichier m�dia"},
/* it */ {IDCLS_SET_VIDEO_STREAM_BITRATE_IT, "Imposta il bitrate per il flusso video nel file multimediale"},
/* hu */ {IDCLS_SET_VIDEO_STREAM_BITRATE_HU, "Adja meg az vide� bitr�t�t a m�dia f�jlhoz"},
/* nl */ {IDCLS_SET_VIDEO_STREAM_BITRATE_NL, "Zet de bitrate voor het videogedeelte van het mediabestand"},
/* pl */ {IDCLS_SET_VIDEO_STREAM_BITRATE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_VIDEO_STREAM_BITRATE_SV, "Ange bithastighet f�r videostr�m i mediafil"},
#endif
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_PET_MODEL,    N_("Specify PET model to emulate")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PET_MODEL_DE, "PET Modell spezifizieren"},
/* fr */ {IDCLS_SPECIFY_PET_MODEL_FR, "Sp�cifier le mod�le PET � �muler"},
/* hu */ {IDCLS_SPECIFY_PET_MODEL_HU, "Adja meg az emul�lt PET modellt"},
/* it */ {IDCLS_SPECIFY_PET_MODEL_IT, "Specifica il modello di PET da emulare"},
/* nl */ {IDCLS_SPECIFY_PET_MODEL_NL, "Geef PET-model om te emuleren"},
/* pl */ {IDCLS_SPECIFY_PET_MODEL_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PET_MODEL_SV, "Ange PET-modell att emulera"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_EDITOR_ROM_NAME,    N_("Specify name of Editor ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_DE, "Name f�r Editor ROM Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_FR, "Sp�cifier le nom de l�image ROM Editor"},
/* hu */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_HU, "Adja meg a szerkeszt� ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_IT, "Specifica il nome dell'immagine della Editor ROM"},
/* nl */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_NL, "Geef naam van het Editor ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EDITOR_ROM_NAME_SV, "Ange namn p� Editor-ROM-avbildning"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME,    N_("Specify 4K extension ROM name at $9***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_DE, "4K Erweiterungs ROM Datei Name bei $9*** definieren"},
/* fr */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_FR, "Sp�cifier le nom de l'extension ROM 4K � $9***"},
/* hu */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_HU, "Adja meg a $9*** c�m� 4K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_IT, "Specifica il nome della ROM di estensione di 4k a $9***"},
/* nl */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_NL, "Geef naam van het 4K uitbreiding ROM-bestand op adres $9***"},
/* pl */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_4K_ROM_9XXX_NAME_SV, "Ange namn p� 4K-ut�knings-ROM p� $9***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME,    N_("Specify 4K extension ROM name at $A***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_DE, "4K Erweiterungs ROM Datei Name bei $A*** definieren"},
/* fr */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_FR, "Sp�cifier le nom de l'extension ROM 4K � $A***"},
/* hu */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_HU, "Adja meg a $A*** c�m� 4K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_IT, "Specifica il nome della ROM di estensione di 4k a $A***"},
/* nl */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_NL, "Geef naam van het 4K uitbreiding ROM-bestand op adres $A***"},
/* pl */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_4K_ROM_AXXX_NAME_SV, "Ange namn p� 4K-ut�knings-ROM p� $A***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME,    N_("Specify 4K extension ROM name at $B***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_DE, "4K Erweiterungs ROM Datei Name bei $B*** definieren"},
/* fr */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_FR, "Sp�cifier le nom de l'extension ROM 4K � $B***"},
/* hu */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_HU, "Adja meg a $B*** c�m� 4K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_IT, "Specifica il nome della ROM di estensione di 4k a $B***"},
/* nl */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_NL, "Geef naam van het 4K uitbreiding ROM-bestand op adres $B***"},
/* pl */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_4K_ROM_BXXX_NAME_SV, "Ange namn p� 4K-ut�knings-ROM p� $B***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000,    N_("Enable PET8296 4K RAM mapping at $9***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_DE, "PET8296 4K RAM Zuordnung bei $9*** aktivieren"},
/* fr */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_FR, "Activer l'extension PET8296 4K � $9***"},
/* hu */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_HU, "PET8296 4K RAM enged�lyez�se a $9*** c�men"},
/* it */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_IT, "Attiva il mapping della RAM di 4K del PET8296 a $9***"},
/* nl */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_NL, "Activeer PET8296 4K RAM op adres $9***"},
/* pl */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_9000_SV, "Aktivera PET8296 4K-RAM-mappning p� $9***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000,    N_("Disable PET8296 4K RAM mapping at $9***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_DE, "PET8296 4K RAM Zuordnung bei $9*** deaktivieren"},
/* fr */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_FR, "D�sactiver l'extension PET8296 4K � $9***"},
/* hu */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_HU, "PET8296 4K RAM tilt�sa a $9*** c�men"},
/* it */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_IT, "Disattiva il mapping della RAM di 4K del PET8296 a $9***"},
/* nl */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_NL, "PET8296 4K RAM op adres $9*** uitschakelen"},
/* pl */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_9000_SV, "Inaktivera PET8296 4K-RAM-mappning p� $9***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000,    N_("Enable PET8296 4K RAM mapping at $A***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_DE, "PET8296 4K RAM Zuordnung bei $A*** aktivieren"},
/* fr */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_FR, "Activer l'extension PET8296 4K � $A***"},
/* hu */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_HU, "PET8296 4K RAM enged�lyez�se a $A*** c�men"},
/* it */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_IT, "Attiva il mapping della RAM di 4K del PET8296 a $A***"},
/* nl */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_NL, "Activeer PET8296 4K RAM op adres $A***"},
/* pl */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PET8296_ENABLE_4K_RAM_AT_A000_SV, "Aktivera PET8296 4K-RAM-mappning p� $A***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000,    N_("Disable PET8296 4K RAM mapping at $A***")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_DE, "PET8296 4K RAM Zuordnung bei $A*** deaktivieren"},
/* fr */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_FR, "D�sactiver l'extension PET8296 4K � $A***"},
/* hu */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_HU, "PET8296 4K RAM tilt�sa a $A*** c�men"},
/* it */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_IT, "Disattiva il mapping della RAM di 4K del PET8296 a $9***"},
/* nl */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_NL, "PET8296 4K RAM op adres $A*** uitschakelen"},
/* pl */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PET8296_DISABLE_4K_RAM_AT_A000_SV, "Inaktivera PET8296 4K-RAM-mappning p� $A***"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SUPERPET_IO,    N_("Enable SuperPET I/O")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SUPERPET_IO_DE, "SuperPET I/O aktivieren"},
/* fr */ {IDCLS_ENABLE_SUPERPET_IO_FR, "Activer les E/S SuperPET"},
/* hu */ {IDCLS_ENABLE_SUPERPET_IO_HU, "SuperPET I/O enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SUPERPET_IO_IT, "Attiva l'I/O del SuperPET"},
/* nl */ {IDCLS_ENABLE_SUPERPET_IO_NL, "Activeer SuperPET-I/O"},
/* pl */ {IDCLS_ENABLE_SUPERPET_IO_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SUPERPET_IO_SV, "Aktivera SuperPET-I/O"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SUPERPET_IO,    N_("Disable SuperPET I/O")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SUPERPET_IO_DE, "SuperPET I/O deaktivieren"},
/* fr */ {IDCLS_DISABLE_SUPERPET_IO_FR, "D�sactiver les E/S SuperPET"},
/* hu */ {IDCLS_DISABLE_SUPERPET_IO_HU, "SuperPET I/O tilt�sa"},
/* it */ {IDCLS_DISABLE_SUPERPET_IO_IT, "Disattiva l'I/O del SuperPET"},
/* nl */ {IDCLS_DISABLE_SUPERPET_IO_NL, "SuperPET-I/O uitschakelen"},
/* pl */ {IDCLS_DISABLE_SUPERPET_IO_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SUPERPET_IO_SV, "Inaktivera SuperPET-I/O"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES,    N_("Enable ROM 1 Kernal patches")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_DE, "ROM 1 Kernal patches aktivieren"},
/* fr */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_FR, "Activer les patches ROM 1 Kernal"},
/* hu */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_HU, "ROM 1 Kernal foltoz�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_IT, "Attiva le patch del Kernal della ROM 1"},
/* nl */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_NL, "Activeer ROM 1 Kernalverbeteringen"},
/* pl */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_ROM_1_KERNAL_PATCHES_SV, "Aktivera ROM 1-Kernalpatchar"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES,    N_("Disable ROM 1 Kernal patches")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_DE, "ROM 1 Kernal patches deaktivieren"},
/* fr */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_FR, "D�sactiver les patches ROM 1 Kernal"},
/* hu */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_HU, "ROM 1 Kernal foltoz�s tilt�sa"},
/* it */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_IT, "Disattiva le patch del Kernal della ROM 1"},
/* nl */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_NL, "ROM 1 Kernalverbeteringen uitschakelen"},
/* pl */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_ROM_1_KERNAL_PATCHES_SV, "Inaktivera ROM 1-Kernalpatchar"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET,    N_("Switch upper/lower case charset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_DE, "Wechsel Gro�-/Kleinschreibung Zeichensatz"},
/* fr */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_FR, "Inverser le charset majuscule et minuscule"},
/* hu */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_HU, "V�lt�s nagy-/kisbet�s karakterk�szletre"},
/* it */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_IT, "Scambia il set di caratteri maiuscolo/minuscolo"},
/* nl */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_NL, "Schakel tussen grote/kleine letters"},
/* pl */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SWITCH_UPPER_LOWER_CHARSET_SV, "V�xla sm�/stora bokst�ver i teckengeneratorn"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET,    N_("Do not switch upper/lower case charset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_DE, "Kein Wechsel Gro�-/Kleinschreibung Zeichensatz"},
/* fr */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_FR, "Ne pas inverser le charset majuscule et minuscule"},
/* hu */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_HU, "Ne v�ltson a nagy-/kisbet�s karakterk�szletre"},
/* it */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_IT, "Non scambiare il set di caratteri maiuscolo/minuscolo"},
/* nl */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_NL, "Schakel niet tussen grote/kleine letters"},
/* pl */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_NO_SWITCH_UPPER_LOWER_CHARSET_SV, "V�xla inte sm�/stora bokst�ver i teckengeneratorn"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_EOI_BLANKS_SCREEN,    N_("EOI blanks screen")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_EOI_BLANKS_SCREEN_DE, "EOI l�scht Bildschirm"},
/* fr */ {IDCLS_EOI_BLANKS_SCREEN_FR, "EOI vide l'�cran"},
/* hu */ {IDCLS_EOI_BLANKS_SCREEN_HU, "EOI t�rli a k�perny�t"},
/* it */ {IDCLS_EOI_BLANKS_SCREEN_IT, "EOI oscura lo schermo"},
/* nl */ {IDCLS_EOI_BLANKS_SCREEN_NL, "EOI maakt het scherm leeg"},
/* pl */ {IDCLS_EOI_BLANKS_SCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_EOI_BLANKS_SCREEN_SV, "EOI t�mmer sk�rmen"},
#endif

/* pet/pet-cmdline-options.c */
/* en */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN,    N_("EOI does not blank screen")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_DE, "EIO l�scht Bildschirm nicht"},
/* fr */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_FR, "EOI ne vide pas l'�cran"},
/* hu */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_HU, "EOI nem t�rli a k�perny�t"},
/* it */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_IT, "EOI non oscura lo schermo"},
/* nl */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_NL, "EOI maakt niet het scherm leeg"},
/* pl */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_EOI_DOES_NOT_BLANK_SCREEN_SV, "EOI t�mmer inte sk�rmen"},
#endif

/* pet/petpia1.c */
/* en */ {IDCLS_ENABLE_USERPORT_DIAG_PIN,    N_("Enable userport diagnostic pin")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_DE, "Userport Diagnose Pin aktivieren"},
/* fr */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_FR, "Activer la pin de diagnostique userport"},
/* hu */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_HU, "Userport diagnosztikai l�b enged�lyez�se"},
/* it */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_IT, "Attiva il pin diagnostico della userport"},
/* nl */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_NL, "Activeer userport diagnostische pin"},
/* pl */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_USERPORT_DIAG_PIN_SV, "Aktivera diagnostik p� anv�ndarporten"},
#endif

/* pet/petpia1.c */
/* en */ {IDCLS_DISABLE_USERPORT_DIAG_PIN,    N_("Disable userport diagnostic pin")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_DE, "Userport Diagnose Pin deaktivieren"},
/* fr */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_FR, "D�sactiver la pin de diagnostique userport"},
/* hu */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_HU, "Userport diagnosztikai l�b tilt�sa"},
/* it */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_IT, "Disattiva il pin diagnostico della userport"},
/* nl */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_NL, "Userport diagnostische pin uitschakelen"},
/* pl */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_USERPORT_DIAG_PIN_SV, "Inaktivera diagnostik p� anv�ndarporten"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME,    N_("Specify name of Function low ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_DE, "Namen f�r Funktions ROM Datei (unterer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_FR, "Sp�cifier le nom de l'image basse ROM Fonction"},
/* hu */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_HU, "Adja meg az als� Function ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_IT, "Specifica il nome dell'immagine della Function ROM bassa"},
/* nl */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_NL, "Geef de naam van het Function low ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FUNCTION_LOW_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r l�gt funktions-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME,    N_("Specify name of Function high ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_DE, "Namen f�r Funktions ROM Datei (oberer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_FR, "Sp�cifier le nom de l'image haute ROM Fonction"},
/* hu */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_HU, "Adja meg a fels� Function ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_IT, "Specifica il nome dell'immagine della Function ROM alta"},
/* nl */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_NL, "Geef de naam van het Function high ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FUNCTION_HIGH_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r h�gt funktions-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME,    N_("Specify name of Cartridge 1 low ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_DE, "Name f�r Erweiterungsmodul 1 ROM Datei (unterer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_FR, "Sp�cifier le nom de l'image basse ROM Cartouche 1"},
/* hu */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_HU, "Adja meg az als� Cartridge 1 ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM Cartridge 1 bassa"},
/* nl */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_NL, "Geef de naam van het Cartridge 1 low ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_1_LOW_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r l�gt insticksmodul 1-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME,    N_("Specify name of Cartridge 1 high ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_DE, "Name f�r Erweiterungsmodul 1 ROM Datei (oberer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_FR, "Sp�cifier le nom de l'image haute ROM Cartouche 1"},
/* hu */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_HU, "Adja meg a fels� Cartridge 1 ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_IT, "Specifica il nomer dell'immagine della ROM Cartridge 1 alta"},
/* nl */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_NL, "Geef de naam van het Cartridge 1 high ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_1_HIGH_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r h�gt insticksmodul 1-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME,    N_("Specify name of Cartridge 2 low ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_DE, "Name f�r Erweiterungsmodul 2 ROM Datei (unterer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_FR, "Sp�cifier le nom de l'image basse ROM Cartouche 2"},
/* hu */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_HU, "Adja meg az als� Cartridge 2 ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_IT, "Specifica il nome dell'immagine della ROM Cartridge 2 bassa"},
/* nl */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_NL, "Geef de naam van het Cartridge 2 low ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_2_LOW_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r l�gt insticksmodul 2-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME,    N_("Specify name of Cartridge 2 high ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_DE, "Name f�r Erweiterungsmodul 2 ROM Datei (oberer Bereich) definieren"},
/* fr */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_FR, "Sp�cifier le nom de l'image haute ROM Cartouche 2"},
/* hu */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_HU, "Adja meg a fels� Cartridge 2 ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_IT, "Specifica il nomer dell'immagine della ROM Cartridge 2 alta"},
/* nl */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_NL, "Geef de naam van het Cartridge 2 high ROM-bestand"},
/* pl */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CART_2_HIGH_ROM_NAME_SV, "Ange namn p� ROM-avbildning f�r h�gt insticksmodul 2-ROM"},
#endif

/* plus4/plus4-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_RAM_INSTALLED,    N_("Specify size of RAM installed in kb (16/32/64)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RAM_INSTALLED_DE, "Gr��e des emulierten RAM Speichers in kb (16/32/64) spezifizieren"},
/* fr */ {IDCLS_SPECIFY_RAM_INSTALLED_FR, "Sp�cifier la taille du RAM install� (16/32/64)"},
/* hu */ {IDCLS_SPECIFY_RAM_INSTALLED_HU, "Adja meg a telep�tett RAM m�ret�t kb-ban (16/32/64)"},
/* it */ {IDCLS_SPECIFY_RAM_INSTALLED_IT, "Specifica la dimensione della RAM installata in kb (16/32/64)"},
/* nl */ {IDCLS_SPECIFY_RAM_INSTALLED_NL, "Geef de hoeveelheid RAM in kb (16/32/64)"},
/* pl */ {IDCLS_SPECIFY_RAM_INSTALLED_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RAM_INSTALLED_SV, "Ange storlek p� installerat RAM i kb (16/32/64)"},
#endif

/* plus4/ted-cmdline-options.c, vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_SATURATION,    N_("Set saturation of internal calculated palette [1000]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_SATURATION_DE, "S�ttigung f�r intern berechneten Palette setzen [1000]"},
/* fr */ {IDCLS_SET_SATURATION_FR, "R�gler la saturation de la palette interne calcul�e [1000]"},
/* hu */ {IDCLS_SET_SATURATION_HU, "A sz�molt paletta tel�tetts�ge [1000]"},
/* it */ {IDCLS_SET_SATURATION_IT, "Imposta la saturazione della palette calcolata internamente [1000]"},
/* nl */ {IDCLS_SET_SATURATION_NL, "Zet de verzadiging van het intern berekend kleurenpalet [1000]"},
/* pl */ {IDCLS_SET_SATURATION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_SATURATION_SV, "St�ll in m�ttnad p� internt ber�knad palett [1000]"},
#endif

/* plus4/ted-cmdline-options.c, vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_CONTRAST,    N_("Set contrast of internal calculated palette [1100]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_CONTRAST_DE, "Kontrast f�r intern berechneten Palette setzen [1100]"},
/* fr */ {IDCLS_SET_CONTRAST_FR, "R�gler le contraste de la palette interne calcul�e [1100]"},
/* hu */ {IDCLS_SET_CONTRAST_HU, "A sz�molt paletta kontraszt �rt�ke [1100]"},
/* it */ {IDCLS_SET_CONTRAST_IT, "Imposta il constrasto della palette calcolata internamente [1100]"},
/* nl */ {IDCLS_SET_CONTRAST_NL, "Zet het contrast van het intern berekend kleurenpalet [1100]"},
/* pl */ {IDCLS_SET_CONTRAST_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_CONTRAST_SV, "St�ll in kontrast p� internt ber�knad palett [1100]"},
#endif

/* plus4/ted-cmdline-options.c, vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_BRIGHTNESS,    N_("Set brightness of internal calculated palette [1100]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_BRIGHTNESS_DE, "Helliogkeit f�r intern berechneten Palette setzen [1100]"},
/* fr */ {IDCLS_SET_BRIGHTNESS_FR, "R�gler la luminosit� de la palette interne calcul�e [1100]"},
/* hu */ {IDCLS_SET_BRIGHTNESS_HU, "A sz�molt paletta f�nyer�ss�ge [1100]"},
/* it */ {IDCLS_SET_BRIGHTNESS_IT, "Imposta la luminanza della palette calcolata internamente [1100]"},
/* nl */ {IDCLS_SET_BRIGHTNESS_NL, "Zet de helderheid van het intern berekend kleurenpalet [1100]"},
/* pl */ {IDCLS_SET_BRIGHTNESS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_BRIGHTNESS_SV, "St�ll in ljusstyrka p� internt ber�knad palett [1100]"},
#endif

/* plus4/ted-cmdline-options.c`, vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_GAMMA,    N_("Set gamma of internal calculated palette [900]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_GAMMA_DE, "Gamma f�r intern berechneten Palette setzen [900]"},
/* fr */ {IDCLS_SET_GAMMA_FR, "R�gler le gamma de la palette interne calcul�e [900]"},
/* hu */ {IDCLS_SET_GAMMA_HU, "A sz�molt paletta gamm�ja [900]"},
/* it */ {IDCLS_SET_GAMMA_IT, "Imposta la gamma della palette calcolata internamente [900]"},
/* nl */ {IDCLS_SET_GAMMA_NL, "Zet de gamma van het intern berekend kleurenpalet [900]"},
/* pl */ {IDCLS_SET_GAMMA_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_GAMMA_SV, "St�ll in gamma p� internt ber�knad palett [900]"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_TINT,    N_("Set tint of internal calculated palette [1000]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_TINT_DE, "Farbton f�r intern berechneten Palette setzen [1000]"},
/* fr */ {IDCLS_SET_TINT_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_TINT_HU, "A sz�molt, bels� paletta tel�tetts�ge [1000]"},
/* it */ {IDCLS_SET_TINT_IT, "Imposta la sfumatura della palette calcolata internamente [1000]"},
/* nl */ {IDCLS_SET_TINT_NL, "Zet de tint van het intern berekend kleuren palette [1000]"},
/* pl */ {IDCLS_SET_TINT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_TINT_SV, "St�ll in ton p� internt ber�knad palett [1000]"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_ODDLINES_PHASE,    N_("Set phase for color carrier in odd lines [1250]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_ODDLINES_PHASE_DE, "Phase f�r Farbtr�ger in ungeraden Zeilen setzen [1250]"},
/* fr */ {IDCLS_SET_ODDLINES_PHASE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_ODDLINES_PHASE_HU, "Sz�n viv� f�zisa a p�ratlan sorokban [1250]"},
/* it */ {IDCLS_SET_ODDLINES_PHASE_IT, "Imposta la fase per la portante colore nelle linee dispari [1250]"},
/* nl */ {IDCLS_SET_ODDLINES_PHASE_NL, "Zet de fase van de kleuren drager in oneven lijnen [1250]"},
/* pl */ {IDCLS_SET_ODDLINES_PHASE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_ODDLINES_PHASE_SV, "V�lj fas f�r f�rgb�rarv�g p� udda rader [1250]"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_ODDLINES_OFFSET,    N_("Set phase offset for color carrier in odd lines [750]")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_ODDLINES_OFFSET_DE, "Phasenoffset f�r Farbtr�ger in ungeraden Zeilen setzen [750]"},
/* fr */ {IDCLS_SET_ODDLINES_OFFSET_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_ODDLINES_OFFSET_HU, "Sz�n viv� f�zis offszet a p�ratlan sorokban [750]"},
/* it */ {IDCLS_SET_ODDLINES_OFFSET_IT, "Imposta l'offset della fase per la portate colore nelle linee dispari [750]"},
/* nl */ {IDCLS_SET_ODDLINES_OFFSET_NL, "Zet de fase compensatie van de kleuren drager in oneven lijnen [750]"},
/* pl */ {IDCLS_SET_ODDLINES_OFFSET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_ODDLINES_OFFSET_SV, "V�lj fasutj�mning f�r f�rgb�rarv�g p� udda rader [1250]"},
#endif

/* printerdrv/driver-select.c */
/* en */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME,    N_("Specify name of printer driver for device #4")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_DE, "Name f�r Druckertreiber Ger�t #4 definieren"},
/* fr */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_FR, "Sp�cifier le nom du pilote imprimante pour le p�riph�rique #4"},
/* hu */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_HU, "Adja meg a #4-es nyomtat�meghajt� nev�t"},
/* it */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_IT, "Specifica il nome del driver di stampa per la periferica #4"},
/* nl */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_NL, "Geef de naam van het stuurprogramma voor apparaat #4"},
/* pl */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PRT_DRIVER_4_NAME_SV, "Ange namn p� skrivardrivrutin f�r enhet 4"},
#endif

/* printerdrv/driver-select.c */
/* en */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME,    N_("Specify name of printer driver for device #5")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_DE, "Name f�r Druckertreiber Ger�t #5 definieren"},
/* fr */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_FR, "Sp�cifier le nom du pilote imprimante pour le p�riph�rique #5"},
/* hu */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_HU, "Adja meg a #5-�s nyomtat�meghajt� nev�t"},
/* it */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_IT, "Specifica il nome del driver di stampa per la periferica #5"},
/* nl */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_NL, "Geef de naam van het stuurprogramma voor apparaat #5"},
/* pl */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PRT_DRIVER_5_NAME_SV, "Ange namn p� skrivardrivrutin f�r enhet 5"},
#endif

/* printerdrv/driver-select.c */
/* en */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME,    N_("Specify name of printer driver for the userport printer")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_DE, "Name f�r Userport Druckertreiber definieren"},
/* fr */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_FR, "Sp�cifier le nom du pilote imprimante pour l'imprimante userport"},
/* hu */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_HU, "Adja meg a userport nyomtat�meghajt� nev�t"},
/* it */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_IT, "Specifica il nome del driver di stampa per la stampante su userport"},
/* nl */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_NL, "Geef de naam van het stuurprogramma voor de userport printer"},
/* pl */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PRT_DRIVER_USR_NAME_SV, "Ange namn p� skrivardrivrutin f�r anv�ndarporten"},
#endif

/* printerdrv/interface-serial.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_4,    N_("Set device type for device #4 (0: NONE, 1: FS, 2: REAL)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_4_DE, "Ger�te Typ f�r Ger�t #4 (0: NONE, 1: FS, 2: REAL) setzen"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_4_FR, "Sp�cifier le type de p�riph�rique pour #4 (0: AUCUN, 1: SF, 2: R�EL)"},
/* hu */ {IDCLS_SET_DEVICE_TYPE_4_HU, "Adja meg a #4-es eszk�z t�pus�t (0:Nincs, 1: F�jlrendszer, 2: Val�di)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_4_IT, "Specifica il tipo di periferica #4 (0 NESSUNA, 1 FS, 2 REALE)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_4_NL, "Zet apparaat soort voor apparaat #4 (0: GEEN, 1: FS, 2: ECHT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_4_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_4_SV, "Ange enhetstyp f�r enhet 4 (0: INGEN, 1: FS, 2: �KTA)"},
#endif

/* printerdrv/interface-serial.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_5,    N_("Set device type for device #5 (0: NONE, 1: FS, 2: REAL)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_5_DE, "Ger�te Typ f�r Ger�t #5 (0: NONE, 1: FS, 2: REAL) setzen"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_5_FR, "Sp�cifier le type de p�riph�rique pour #5 (0: AUCUN, 1: SF, 2: R�EL)"},
/* hu */ {IDCLS_SET_DEVICE_TYPE_5_HU, "Adja meg a #5-�s eszk�z t�pus�t (0:Nincs, 1: F�jlrendszer, 2: Val�di)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_5_IT, "Specifica il tipo di periferica #4 (0 NESSUNA, 1 FS, 2 REALE)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_5_NL, "Zet apparaat soort voor apparaat #5 (0: GEEN, 1: FS, 2: ECHT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_5_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_5_SV, "Ange enhetstyp f�r enhet 5 (0: INGEN, 1: FS, 2: �KTA)"},
#endif

/* printerdrv/interface-userport.c */
/* en */ {IDCLS_ENABLE_USERPORT_PRINTER,    N_("Enable the userport printer emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_USERPORT_PRINTER_DE, "Userport Drucker Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_USERPORT_PRINTER_FR, "Acviver l'�mulation de l'imprimante userport"},
/* hu */ {IDCLS_ENABLE_USERPORT_PRINTER_HU, "Userport nyomtat� emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_USERPORT_PRINTER_IT, "Attiva l'emulazione della stampante su userport"},
/* nl */ {IDCLS_ENABLE_USERPORT_PRINTER_NL, "Activeer de userport printeremulatie"},
/* pl */ {IDCLS_ENABLE_USERPORT_PRINTER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_USERPORT_PRINTER_SV, "Aktivera emulering av skrivare p� anv�ndarporten"},
#endif

/* printerdrv/interface-userport.c */
/* en */ {IDCLS_DISABLE_USERPORT_PRINTER,    N_("Disable the userport printer emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_USERPORT_PRINTER_DE, "Userport Drucker Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_USERPORT_PRINTER_FR, "D�sactiver l'�mulation de l'imprimante userport"},
/* hu */ {IDCLS_DISABLE_USERPORT_PRINTER_HU, "Userport nyomtat� emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_USERPORT_PRINTER_IT, "Disattiva l'emulazione della stampante su userport"},
/* nl */ {IDCLS_DISABLE_USERPORT_PRINTER_NL, "De userport printeremulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_USERPORT_PRINTER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_USERPORT_PRINTER_SV, "Inaktivera emulering av skrivare p� anv�ndarporten"},
#endif

/* printerdrv/output-select.c */
/* en */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME,    N_("Specify name of output device for device #4")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_DE, "Name f�r Druckerausgabe Datei Ger�t #4 definieren"},
/* fr */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_FR, "Sp�cifiez le nom du p�riph�rique de sortie pour le p�riph�rique #4"},
/* hu */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_HU, "Adja meg a kimeneti eszk�z nev�t a #4-es egys�ghez"},
/* it */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_IT, "Specifica il nome del dispositivo di output per la periferica #4"},
/* nl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_NL, "Geef de naam van het uitvoer apparaat voor apparaat #4"},
/* pl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_OUTPUT_DEVICE_4_NAME_SV, "Ange namn p� utskriftsenhet f�r enhet 4"},
#endif

/* printerdrv/output-select.c */
/* en */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME,    N_("Specify name of output device for device #5")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_DE, "Name f�r Druckerausgabe Datei Ger�t #5 definieren"},
/* fr */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_FR, "Sp�cifiez le nom du p�riph�rique de sortie pour le p�riph�rique #5"},
/* hu */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_HU, "Adja meg a kimeneti eszk�z nev�t a #5-�s egys�ghez"},
/* it */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_IT, "Specifica il nome del dispositivo di output per la periferica #5"},
/* nl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_NL, "Geef de naam van het uitvoer apparaat voor apparaat #5"},
/* pl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_OUTPUT_DEVICE_5_NAME_SV, "Ange namn p� utskriftsenhet f�r enhet 5"},
#endif

/* printerdrv/output-select.c */
/* en */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME,    N_("Specify name of output device for the userport printer")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_DE, "Name f�r Druckerausgabe Datei Userport definieren"},
/* fr */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_FR, "Sp�cifiez le nom du p�riph�rique de sortie pour l'imprimante "
                                                   "userport"},
/* hu */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_HU, "Adja meg a kimeneti eszk�z nev�t a userport nyomtat�hoz"},
/* it */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_IT, "Specifica il nome del dispositivo di output per la stampante su "
                                                   "userport"},
/* nl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_NL, "Geef de naam van het uitvoer apparaat voor de userport printer"},
/* pl */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_OUTPUT_DEVICE_USR_NAME_SV, "Ange namn p� utskriftsenhet f�r anv�ndarporten"},
#endif

/* printerdrv/output-text.c */
/* en */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME,    N_("Specify name of printer text device or dump file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_DE, "Name f�r Druckerausgabe Textger�t oder Dump Datei definieren"},
/* fr */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_FR, "Sp�cifier le nom de l'imprimante texte ou \"dump file\""},
/* hu */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_HU, "Adja meg a sz�vegnyomtat� eszk�z nev�t, vagy a kimeneti f�jlt"},
/* it */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_IT, "Specifica il nome del dispositivo testuale di stampa o salva su file"},
/* nl */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_NL, "Geef de naam van het text apparaat of het dumpbestand"},
/* pl */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TEXT_DEVICE_DUMP_NAME_SV, "Ange namn p� textenhet f�r utskrift eller dumpfil"},
#endif

/* printerdrv/output-text.c */
/* en */ {IDCLS_SPECIFY_TEXT_DEVICE_4,    N_("Specify printer text output device for IEC printer #4")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TEXT_DEVICE_4_DE, "IEC Drucker #4 Ger�t f�r Textausgabe definieren"},
/* fr */ {IDCLS_SPECIFY_TEXT_DEVICE_4_FR, "Sp�cifier le p�riph�rique de sortie text pour l'imprimante IEC #4"},
/* hu */ {IDCLS_SPECIFY_TEXT_DEVICE_4_HU, "Adja meg a sz�vegnyomtat� eszk�zt a #4-es IEC nyomtat�hoz"},
/* it */ {IDCLS_SPECIFY_TEXT_DEVICE_4_IT, "Specifica il nome del dispositivo di output testuale di stampa per la "
                                          "stampante IEC #4"},
/* nl */ {IDCLS_SPECIFY_TEXT_DEVICE_4_NL, "Geef het text uitvoer apparaat voor IEC printer #4"},
/* pl */ {IDCLS_SPECIFY_TEXT_DEVICE_4_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TEXT_DEVICE_4_SV, "Ange skrivartextutenhet f�r IEC-skrivare 4"},
#endif

/* printerdrv/output-text.c */
/* en */ {IDCLS_SPECIFY_TEXT_DEVICE_5,    N_("Specify printer text output device for IEC printer #5")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TEXT_DEVICE_5_DE, "IEC Drucker #5 Ger�t f�r Textausgabe definieren"},
/* fr */ {IDCLS_SPECIFY_TEXT_DEVICE_5_FR, "Sp�cifier le p�riph�rique de sortie text pour l'imprimante IEC #5"},
/* hu */ {IDCLS_SPECIFY_TEXT_DEVICE_5_HU, "Adja meg a sz�vegnyomtat� eszk�zt a #5-es IEC nyomtat�hoz"},
/* it */ {IDCLS_SPECIFY_TEXT_DEVICE_5_IT, "Specifica il nome del dispositivo di output testuale di stampa per la "
                                          "stampante IEC #5"},
/* nl */ {IDCLS_SPECIFY_TEXT_DEVICE_5_NL, "Geef het text uitvoer apparaat voor IEC printer #5"},
/* pl */ {IDCLS_SPECIFY_TEXT_DEVICE_5_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TEXT_DEVICE_5_SV, "Ange skrivartextutenhet f�r IEC-skrivare 5"},
#endif

/* printerdrv/output-text.c */
/* en */ {IDCLS_SPECIFY_TEXT_USERPORT,    N_("Specify printer text output device for userport printer")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_TEXT_USERPORT_DE, "IEC Userport Drucker Ger�t f�r Textausgabe definieren"},
/* fr */ {IDCLS_SPECIFY_TEXT_USERPORT_FR, "Sp�cifier le p�riph�rique de sortie text pour l'imprimante userport"},
/* hu */ {IDCLS_SPECIFY_TEXT_USERPORT_HU, "Adja meg a sz�vegnyomtat� eszk�zt a userport nyomtat�hoz"},
/* it */ {IDCLS_SPECIFY_TEXT_USERPORT_IT, "Specifica il nome del dispositivo di output testuale di stampa per la "
                                          "stampante su userport"},
/* nl */ {IDCLS_SPECIFY_TEXT_USERPORT_NL, "Geef het text uitvoer apparaat voor de userport printer"},
/* pl */ {IDCLS_SPECIFY_TEXT_USERPORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_TEXT_USERPORT_SV, "Ange skrivartextutenhet f�r anv�ndarportsskrivare"},
#endif

/* raster/raster-cmdline-options.c */
/* en */ {IDCLS_ENABLE_VIDEO_CACHE,    N_("Enable the video cache")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_VIDEO_CACHE_DE, "Video Cache aktivieren"},
/* fr */ {IDCLS_ENABLE_VIDEO_CACHE_FR, "Activer le cache vid�o"},
/* hu */ {IDCLS_ENABLE_VIDEO_CACHE_HU, "Vide� gyors�t�t�r enged�lyez�se"},
/* it */ {IDCLS_ENABLE_VIDEO_CACHE_IT, "Attiva la cache video"},
/* nl */ {IDCLS_ENABLE_VIDEO_CACHE_NL, "Activeer de videocache"},
/* pl */ {IDCLS_ENABLE_VIDEO_CACHE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_VIDEO_CACHE_SV, "Aktivera videocache"},
#endif

/* raster/raster-cmdline-options.c */
/* en */ {IDCLS_DISABLE_VIDEO_CACHE,    N_("Disable the video cache")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_VIDEO_CACHE_DE, "Video Cache deaktivieren"},
/* fr */ {IDCLS_DISABLE_VIDEO_CACHE_FR, "D�sactiver le cache vid�o"},
/* hu */ {IDCLS_DISABLE_VIDEO_CACHE_HU, "Vide� gyors�t�t�r tilt�sa"},
/* it */ {IDCLS_DISABLE_VIDEO_CACHE_IT, "Disattiva la cache video"},
/* nl */ {IDCLS_DISABLE_VIDEO_CACHE_NL, "De videocache uitschakelen"},
/* pl */ {IDCLS_DISABLE_VIDEO_CACHE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_VIDEO_CACHE_SV, "Inaktivera videocache"},
#endif

#ifdef HAVE_RS232
/* rs232drv/rs232drv.c */
/* en */ {IDCLS_SPECIFY_RS232_1_NAME,    N_("Specify name of first RS232 device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RS232_1_NAME_DE, "Name f�r erstes RS232 Ger�t definieren"},
/* fr */ {IDCLS_SPECIFY_RS232_1_NAME_FR, "Sp�cifier le nom du 1er p�riph�rique RS232"},
/* hu */ {IDCLS_SPECIFY_RS232_1_NAME_HU, "Adja meg az els� RS232-es eszk�z nev�t"},
/* it */ {IDCLS_SPECIFY_RS232_1_NAME_IT, "Specifica il nome della prima RS232"},
/* nl */ {IDCLS_SPECIFY_RS232_1_NAME_NL, "Geef de naam van het eerste RS232 apparaat"},
/* pl */ {IDCLS_SPECIFY_RS232_1_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RS232_1_NAME_SV, "Ange namn p� f�rsta RS232-enheten"},
#endif

/* rs232drv/rs232drv.c */
/* en */ {IDCLS_SPECIFY_RS232_2_NAME,    N_("Specify name of second RS232 device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RS232_2_NAME_DE, "Name f�r zweites RS232 Ger�t definieren"},
/* fr */ {IDCLS_SPECIFY_RS232_2_NAME_FR, "Sp�cifier le nom du 2e p�riph�rique RS232"},
/* hu */ {IDCLS_SPECIFY_RS232_2_NAME_HU, "Adja meg a m�sodik RS232-es eszk�z nev�t"},
/* it */ {IDCLS_SPECIFY_RS232_2_NAME_IT, "Specifica il nome della seconda RS232"},
/* nl */ {IDCLS_SPECIFY_RS232_2_NAME_NL, "Geef de naam van het tweede RS232 apparaat"},
/* pl */ {IDCLS_SPECIFY_RS232_2_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RS232_2_NAME_SV, "Ange namn p� andra RS232-enheten"},
#endif

/* rs232drv/rs232drv.c */
/* en */ {IDCLS_SPECIFY_RS232_3_NAME,    N_("Specify name of third RS232 device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RS232_3_NAME_DE, "Name f�r drittes RS232 Ger�t definieren"},
/* fr */ {IDCLS_SPECIFY_RS232_3_NAME_FR, "Sp�cifier le nom du 3e p�riph�rique RS232"},
/* hu */ {IDCLS_SPECIFY_RS232_3_NAME_HU, "Adja meg a harmadik RS232-es eszk�z nev�t"},
/* it */ {IDCLS_SPECIFY_RS232_3_NAME_IT, "Specifica il nome della terza RS232"},
/* nl */ {IDCLS_SPECIFY_RS232_3_NAME_NL, "Geef de naam van het derde RS232 apparaat"},
/* pl */ {IDCLS_SPECIFY_RS232_3_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RS232_3_NAME_SV, "Ange namn p� tredje RS232-enheten"},
#endif

/* rs232drv/rs232drv.c */
/* en */ {IDCLS_SPECIFY_RS232_4_NAME,    N_("Specify name of fourth RS232 device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RS232_4_NAME_DE, "Name f�r viertes RS232 Ger�t definieren"},
/* fr */ {IDCLS_SPECIFY_RS232_4_NAME_FR, "Sp�cifier le nom du 4e p�riph�rique RS232"},
/* hu */ {IDCLS_SPECIFY_RS232_4_NAME_HU, "Adja meg a negyedik RS232-es eszk�z nev�t"},
/* it */ {IDCLS_SPECIFY_RS232_4_NAME_IT, "Specifica il nome della quarta RS232"},
/* nl */ {IDCLS_SPECIFY_RS232_4_NAME_NL, "Geef de naam van het vierde RS232 apparaat"},
/* pl */ {IDCLS_SPECIFY_RS232_4_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RS232_4_NAME_SV, "Ange namn p� fj�rde RS232-enheten"},
#endif
#endif

/* rs232drv/rsuser.c */
/* en */ {IDCLS_ENABLE_RS232_USERPORT,    N_("Enable RS232 userport emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_RS232_USERPORT_DE, "RS232 Userport Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_RS232_USERPORT_FR, "Activer l'�mulation de p�riph�rique RS232 userport"},
/* hu */ {IDCLS_ENABLE_RS232_USERPORT_HU, "RS232 userport emul�ci� enged�lyez�se"},
/* it */ {IDCLS_ENABLE_RS232_USERPORT_IT, "Attiva l'emulazione della RS232 su userport"},
/* nl */ {IDCLS_ENABLE_RS232_USERPORT_NL, "Activeer RS232-userportemulatie"},
/* pl */ {IDCLS_ENABLE_RS232_USERPORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_RS232_USERPORT_SV, "Aktivera RS232-emulering p� anv�ndarporten"},
#endif

/* rs232drv/rsuser.c */
/* en */ {IDCLS_DISABLE_RS232_USERPORT,    N_("Disable RS232 userport emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_RS232_USERPORT_DE, "RS232 Userport Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_RS232_USERPORT_FR, "D�sactiver l'�mulation de p�riph�rique RS232 userport"},
/* hu */ {IDCLS_DISABLE_RS232_USERPORT_HU, "RS232 userport emul�ci� tilt�sa"},
/* it */ {IDCLS_DISABLE_RS232_USERPORT_IT, "Disattiva l'emulazione della RS232 su userport"},
/* nl */ {IDCLS_DISABLE_RS232_USERPORT_NL, "RS232-userportemulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_RS232_USERPORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_RS232_USERPORT_SV, "Inaktivera RS232-emulering p� anv�ndarporten"},
#endif

/* rs232drv/rsuser.c */
/* en */ {IDCLS_P_BAUD,    N_("<baud>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_BAUD_DE, "<Baud>"},
/* fr */ {IDCLS_P_BAUD_FR, "<baud>"},
/* hu */ {IDCLS_P_BAUD_HU, "<bitr�ta>"},
/* it */ {IDCLS_P_BAUD_IT, "<baud>"},
/* nl */ {IDCLS_P_BAUD_NL, "<baud>"},
/* pl */ {IDCLS_P_BAUD_PL, "<baud>"},
/* sv */ {IDCLS_P_BAUD_SV, "<baud>"},
#endif

/* rs232drv/rsuser.c */
/* en */ {IDCLS_SET_BAUD_RS232_USERPORT,    N_("Set the baud rate of the RS232 userport emulation.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_BAUD_RS232_USERPORT_DE, "Baudrate f�r RS232 Userport Emulation setzen"},
/* fr */ {IDCLS_SET_BAUD_RS232_USERPORT_FR, "Sp�cifiez la vitesse en baud du p�riph�rique RS232 userport"},
/* hu */ {IDCLS_SET_BAUD_RS232_USERPORT_HU, "Bitr�ta megad�sa az RS232 userport emul�ci�hoz."},
/* it */ {IDCLS_SET_BAUD_RS232_USERPORT_IT, "Imposta la velocit� della RS232 su userport (in baud)"},
/* nl */ {IDCLS_SET_BAUD_RS232_USERPORT_NL, "Zet de baud rate van de RS232 userport emulatie."},
/* pl */ {IDCLS_SET_BAUD_RS232_USERPORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_BAUD_RS232_USERPORT_SV, "Ange baudhastighet f�r RS232-emuleringen p� anv�ndarporten."},
#endif

/* rs232drv/rsuser.c */
/* en */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT,    N_("Specify VICE RS232 device for userport")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_DE, "VICE RS232 Ger�t f�r Userport definieren"},
/* fr */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_FR, "Sp�cifier un p�riph�rique VICE RS232 pour le userport"},
/* hu */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_HU, "VICE RS232-es userport eszk�z megad�sa"},
/* it */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_IT, "Specifica il dispositivo RS232 di VICE su userport"},
/* nl */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_NL, "Geef VICE RS232-apparaat voor userport"},
/* pl */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RS232_DEVICE_USERPORT_SV, "Ange VICE RS232-enhet f�r anv�ndarporten"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_4,    N_("Enable IEC device emulation for device #4")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_4_DE, "IEC Ger�te Emulation f�r Ger�t #4 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_4_FR, "Activer l'�mulation IEC pour le p�riph�rique #4"},
/* hu */ {IDCLS_ENABLE_IEC_4_HU, "IEC eszk�z #4 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_4_IT, "Attiva l'emulazione IEC per la periferica #4"},
/* nl */ {IDCLS_ENABLE_IEC_4_NL, "Activeer IEC-apparaat emulatie voor apparaat #4"},
/* pl */ {IDCLS_ENABLE_IEC_4_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_4_SV, "Aktivera IEC-enhetsemulering f�r enhet 4"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_4,    N_("Disable IEC device emulation for device #4")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_4_DE, "IEC Ger�te Emulation f�r Ger�t #4 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_4_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #4"},
/* hu */ {IDCLS_DISABLE_IEC_4_HU, "IEC eszk�z #4 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_4_IT, "Disattiva l'emulazione IEC per la periferica #4"},
/* nl */ {IDCLS_DISABLE_IEC_4_NL, "IEC-apparaat emulatie voor apparaat #4 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_4_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_4_SV, "Inaktivera IEC-enhetsemulering f�r enhet 4"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_5,    N_("Enable IEC device emulation for device #5")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_5_DE, "IEC Ger�te Emulation f�r Ger�t #5 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_5_FR, "Activer l'�mulation IEC pour le p�riph�rique #5"},
/* hu */ {IDCLS_ENABLE_IEC_5_HU, "IEC eszk�z #5 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_5_IT, "Attiva l'emulazione IEC per la periferica #5"},
/* nl */ {IDCLS_ENABLE_IEC_5_NL, "Activeer IEC-apparaat emulatie voor apparaat #5"},
/* pl */ {IDCLS_ENABLE_IEC_5_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_5_SV, "Aktivera IEC-enhetsemulering f�r enhet 5"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_5,    N_("Disable IEC device emulation for device #5")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_5_DE, "IEC Ger�te Emulation f�r Ger�t #5 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_5_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #5"},
/* hu */ {IDCLS_DISABLE_IEC_5_HU, "IEC eszk�z #5 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_5_IT, "Disattiva l'emulazione IEC per la periferica #5"},
/* nl */ {IDCLS_DISABLE_IEC_5_NL, "IEC-apparaat emulatie voor apparaat #5 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_5_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_5_SV, "Inaktivera IEC-enhetsemulering f�r enhet 5"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_8,    N_("Enable IEC device emulation for device #8")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_8_DE, "IEC Ger�te Emulation f�r Ger�t #8 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_8_FR, "Activer l'�mulation IEC pour le p�riph�rique #8"},
/* hu */ {IDCLS_ENABLE_IEC_8_HU, "IEC eszk�z #8 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_8_IT, "Attiva l'emulazione IEC per la periferica #8"},
/* nl */ {IDCLS_ENABLE_IEC_8_NL, "Activeer IEC-apparaat emulatie voor apparaat #8"},
/* pl */ {IDCLS_ENABLE_IEC_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_8_SV, "Aktivera IEC-enhetsemulering f�r enhet 8"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_8,    N_("Disable IEC device emulation for device #8")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_8_DE, "IEC Ger�te Emulation f�r Ger�t #8 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_8_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #8"},
/* hu */ {IDCLS_DISABLE_IEC_8_HU, "IEC eszk�z #8 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_8_IT, "Disattiva l'emulazione IEC per la periferica #8"},
/* nl */ {IDCLS_DISABLE_IEC_8_NL, "IEC-apparaat emulatie voor apparaat #8 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_8_SV, "Inaktivera IEC-enhetsemulering f�r enhet 8"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_9,    N_("Enable IEC device emulation for device #9")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_9_DE, "IEC Ger�te Emulation f�r Ger�t #9 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_9_FR, "Activer l'�mulation IEC pour le p�riph�rique #9"},
/* hu */ {IDCLS_ENABLE_IEC_9_HU, "IEC eszk�z #9 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_9_IT, "Attiva l'emulazione IEC per la periferica #9"},
/* nl */ {IDCLS_ENABLE_IEC_9_NL, "Activeer IEC-apparaat emulatie voor apparaat #9"},
/* pl */ {IDCLS_ENABLE_IEC_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_9_SV, "Aktivera IEC-enhetsemulering f�r enhet 9"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_9,    N_("Disable IEC device emulation for device #9")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_9_DE, "IEC Ger�te Emulation f�r Ger�t #9 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_9_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #9"},
/* hu */ {IDCLS_DISABLE_IEC_9_HU, "IEC eszk�z #9 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_9_IT, "Disattiva l'emulazione IEC per la periferica #9"},
/* nl */ {IDCLS_DISABLE_IEC_9_NL, "IEC-apparaat emulatie voor apparaat #9 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_9_SV, "Inaktivera IEC-enhetsemulering f�r enhet 9"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_10,    N_("Enable IEC device emulation for device #10")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_10_DE, "IEC Ger�te Emulation f�r Ger�t #10 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_10_FR, "Activer l'�mulation IEC pour le p�riph�rique #10"},
/* hu */ {IDCLS_ENABLE_IEC_10_HU, "IEC eszk�z #10 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_10_IT, "Attiva l'emulazione IEC per la periferica #10"},
/* nl */ {IDCLS_ENABLE_IEC_10_NL, "Activeer IEC-apparaat emulatie voor apparaat #10"},
/* pl */ {IDCLS_ENABLE_IEC_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_10_SV, "Aktivera IEC-enhetsemulering f�r enhet 10"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_10,    N_("Disable IEC device emulation for device #10")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_10_DE, "IEC Ger�te Emulation f�r Ger�t #10 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_10_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #10"},
/* hu */ {IDCLS_DISABLE_IEC_10_HU, "IEC eszk�z #10 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_10_IT, "Disattiva l'emulazione IEC per la periferica #10"},
/* nl */ {IDCLS_DISABLE_IEC_10_NL, "IEC-apparaat emulatie voor apparaat #10 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_10_SV, "Inaktivera IEC-enhetsemulering f�r enhet 10"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_ENABLE_IEC_11,    N_("Enable IEC device emulation for device #11")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_IEC_11_DE, "IEC Ger�te Emulation f�r Ger�t #11 aktivieren"},
/* fr */ {IDCLS_ENABLE_IEC_11_FR, "Activer l'�mulation IEC pour le p�riph�rique #11"},
/* hu */ {IDCLS_ENABLE_IEC_11_HU, "IEC eszk�z #11 emul�ci�j�nak enged�lyez�se"},
/* it */ {IDCLS_ENABLE_IEC_11_IT, "Attiva l'emulazione IEC per la periferica #11"},
/* nl */ {IDCLS_ENABLE_IEC_11_NL, "Activeer IEC-apparaat emulatie voor apparaat #11"},
/* pl */ {IDCLS_ENABLE_IEC_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_IEC_11_SV, "Aktivera IEC-enhetsemulering f�r enhet 11"},
#endif

/* serial/serial-iec-device.c */
/* en */ {IDCLS_DISABLE_IEC_11,    N_("Disable IEC device emulation for device #11")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_IEC_11_DE, "IEC Ger�te Emulation f�r Ger�t #11 deaktivieren"},
/* fr */ {IDCLS_DISABLE_IEC_11_FR, "D�sactiver l'�mulation IEC pour le p�riph�rique #11"},
/* hu */ {IDCLS_DISABLE_IEC_11_HU, "IEC eszk�z #11 emul�ci�j�nak tilt�sa"},
/* it */ {IDCLS_DISABLE_IEC_11_IT, "Disattiva l'emulazione IEC per la periferica #11"},
/* nl */ {IDCLS_DISABLE_IEC_11_NL, "IEC-apparaat emulatie voor apparaat #11 uitschakelen"},
/* pl */ {IDCLS_DISABLE_IEC_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_IEC_11_SV, "Inaktivera IEC-enhetsemulering f�r enhet 11"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_P_ENGINE,    N_("<engine>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_ENGINE_DE, "<Engine>"},
/* fr */ {IDCLS_P_ENGINE_FR, "<engin>"},
/* hu */ {IDCLS_P_ENGINE_HU, "<motor>"},
/* it */ {IDCLS_P_ENGINE_IT, "<motore>"},
/* nl */ {IDCLS_P_ENGINE_NL, "<kern>"},
/* pl */ {IDCLS_P_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_ENGINE_SV, "<motor>"},
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 3: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 3: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 3: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 3: HardSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 3: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 3: HardSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 3: HardSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 4: ParSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 3: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 3: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 3: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 3: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 3: HardSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 3: HardSID)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 4: ParSID)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 3: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 3: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 3: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 3: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, "
                                       "3: HardSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 3: HardSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 4:ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, "
                                       "4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 4: ParSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 3: HardSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 3: HardSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 3: HardSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 3: HardSID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 3: HardSID, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 3: HardSID, 4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 3: HardSID, 4: ParSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 3: HardSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 3: HardSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 3: HardSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 3: HardSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 3: HardSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 3: HardSID, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 3: HardSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 4: ParSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 4: ParSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 3: HardSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "3: HardSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 3: HardSID, "
                                       "4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 3: HardSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 3: HardSID, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 3: HardSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 4: ParSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 4: ParSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, 3: "
                                       "HardSID, 4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, 3: "
                                       "HardSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, 4: "
                                       "ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 3: HardSID, 4: "
                                       "ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && !defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: "
                                       "ParSID)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: "
                                       "ParSID)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "3: HardSID, 4: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 3: HardSID, "
                                       "7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, "
                                       "7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "3: HardSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "4: ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 3: HardSID, "
                                       "4: ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: Catweasel, 3: "
                                       "HardSID, 4: ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: Catweasel, 3: HardSID, 4: ParSID, 7: ReSID-FP)"},
#endif
#endif

#if defined(HAVE_RESID) && defined(HAVE_RESID_FP) && defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_ENGINE,    N_("Specify SID engine (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID, 7: ReSID-FP)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* fr */ {IDCLS_SPECIFY_SID_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
/* hu */ {IDCLS_SPECIFY_SID_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: "
                                       "ParSID, 7: ReSID-FP)"},
/* it */ {IDCLS_SPECIFY_SID_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: "
                                       "ParSID, 7: ReSID-FP)"},
/* nl */ {IDCLS_SPECIFY_SID_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: ReSID, 2: Catweasel, "
                                       "3: HardSID, 4: ParSID, 7: ReSID-FP)"},
/* pl */ {IDCLS_SPECIFY_SID_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: ReSID, 2: Catweasel, 3: HardSID, 4: ParSID, "
                                       "7: ReSID-FP)"},
#endif
#endif

#if !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID)"},
#endif
#endif

#if defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 1: Catweasel)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: Catweasel)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: Catweasel)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: Catweasel)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: Catweasel)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: Catweasel)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: Catweasel)"},
#endif
#endif

#if !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 2: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: HardSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: HardSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: HardSID)"},
#endif
#endif

#if !defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 3: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 3: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 3: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 3: ParSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 3: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 3: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 3: ParSID)"},
#endif
#endif

#if defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && !defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 1: Catweasel, 2: HardSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: Catweasel, 2: HardSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: Catweasel, 2: HardSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: Catweasel, 2: HardSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: Catweasel, 2: HardSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: Catweasel, 2: "
                                           "HardSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: Catweasel, 2: HardSID)"},
#endif
#endif

#if defined(HAVE_CATWEASELMKIII) && !defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 1: Catweasel, 3: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: Catweasel, 3: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: Catweasel, 3: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: Catweasel, 3: ParSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: Catweasel, 3: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: Catweasel, 3: "
                                           "ParSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: Catweasel, 3: ParSID)"},
#endif
#endif

#if !defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 2: HardSID, 3: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 2: HardSID, 3: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 2: HardSID, 3: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 2: HardSID, 3: ParSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 2: HardSID, 3: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 2: HardSID, 3: "
                                           "ParSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 2: HardSID, 3: ParSID)"},
#endif
#endif

#if defined(HAVE_CATWEASELMKIII) && defined(HAVE_HARDSID) && defined(HAVE_PARSID)
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SIDCART_ENGINE,    N_("Specify SID engine (0: FastSID, 1: Catweasel, 2: HardSID, 3: ParSID)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SIDCART_ENGINE_DE, "SID Engine w�hlen (0: FastSID, 1: Catweasel, 2: HardSID, 3: ParSID)"},
/* fr */ {IDCLS_SPECIFY_SIDCART_ENGINE_FR, "Sp�cifier l�engin SID (0: FastSID, 1: Catweasel, 2: HardSID, 3: ParSID)"},
/* hu */ {IDCLS_SPECIFY_SIDCART_ENGINE_HU, "Adja meg a SID motort (0: Gyors SID, 1: Catweasel, 2: HardSID, 3: ParSID)"},
/* it */ {IDCLS_SPECIFY_SIDCART_ENGINE_IT, "Specifica il motore SID (0: FastSID, 1: Catweasel, 2: HardSID, 3: ParSID)"},
/* nl */ {IDCLS_SPECIFY_SIDCART_ENGINE_NL, "Geef op welke SID-kern gebruikt moet worden (0: FastSID, 1: Catweasel, 2: "
                                           "HardSID, 3: ParSID)"},
/* pl */ {IDCLS_SPECIFY_SIDCART_ENGINE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SIDCART_ENGINE_SV, "Ange SID-motor (0: FastSID, 1: Catweasel, 2: HardSID, 3: ParSID)"},
#endif
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SECOND_SID,    N_("Enable second SID")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SECOND_SID_DE, "Zweiten SID aktivieren"},
/* fr */ {IDCLS_ENABLE_SECOND_SID_FR, "Activer le second SID"},
/* hu */ {IDCLS_ENABLE_SECOND_SID_HU, "M�sodik SID enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SECOND_SID_IT, "Attiva Secondo SID"},
/* nl */ {IDCLS_ENABLE_SECOND_SID_NL, "Activeer stereo-SID"},
/* pl */ {IDCLS_ENABLE_SECOND_SID_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SECOND_SID_SV, "Aktivera andra SID"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SIDCART,    N_("Enable SID Cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SIDCART_DE, "SID Modul aktivieren"},
/* fr */ {IDCLS_ENABLE_SIDCART_FR, "Activer la cartouche SID"},
/* hu */ {IDCLS_ENABLE_SIDCART_HU, "SID cartridge enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SIDCART_IT, "Attiva cartuccia SID"},
/* nl */ {IDCLS_ENABLE_SIDCART_NL, "Activeer SID-Cartridge"},
/* pl */ {IDCLS_ENABLE_SIDCART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SIDCART_SV, "Aktivera SID-instickskort"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SIDCART,    N_("Disable SID Cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SIDCART_DE, "SID Modul deaktivieren"},
/* fr */ {IDCLS_DISABLE_SIDCART_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_SIDCART_HU, "SID cartridge tilt�sa"},
/* it */ {IDCLS_DISABLE_SIDCART_IT, "Disattiva cartuccia SID"},
/* nl */ {IDCLS_DISABLE_SIDCART_NL, "SID-Cartridge uitschakelen"},
/* pl */ {IDCLS_DISABLE_SIDCART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SIDCART_SV, "Inaktivera SID-instickskort"},
#endif

/* sid/sid-cmdline-options.c, c64/plus60k, c64/c64_256k.c */
/* en */ {IDCLS_P_BASE_ADDRESS,    N_("<base address>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_BASE_ADDRESS_DE, "<Basis Adresse>"},
/* fr */ {IDCLS_P_BASE_ADDRESS_FR, "<adresse de base>"},
/* hu */ {IDCLS_P_BASE_ADDRESS_HU, "<b�zisc�m>"},
/* it */ {IDCLS_P_BASE_ADDRESS_IT, "<indirizzo base>"},
/* nl */ {IDCLS_P_BASE_ADDRESS_NL, "<basisadres>"},
/* pl */ {IDCLS_P_BASE_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_BASE_ADDRESS_SV, "<basadress>"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_2_ADDRESS,    N_("Specify base address for 2nd SID")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_2_ADDRESS_DE, "Basis Adresse f�r zweiten SID definieren"},
/* fr */ {IDCLS_SPECIFY_SID_2_ADDRESS_FR, "Sp�cifier l'adresse de base pour le second SID"},
/* hu */ {IDCLS_SPECIFY_SID_2_ADDRESS_HU, "Adja meg a b�zisc�m�t a m�sodik SID-nek"},
/* it */ {IDCLS_SPECIFY_SID_2_ADDRESS_IT, "Specifica l'indirizzo di base per il secondo SID"},
/* nl */ {IDCLS_SPECIFY_SID_2_ADDRESS_NL, "Geef het basisadres van de 2e SID"},
/* pl */ {IDCLS_SPECIFY_SID_2_ADDRESS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_2_ADDRESS_SV, "Ange basadress f�r andra SID"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_P_MODEL,    N_("<model>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_MODEL_DE, "<Modell>"},
/* fr */ {IDCLS_P_MODEL_FR, "<mod�le>"},
/* hu */ {IDCLS_P_MODEL_HU, "<modell>"},
/* it */ {IDCLS_P_MODEL_IT, "<modello>"},
/* nl */ {IDCLS_P_MODEL_NL, "<model>"},
/* pl */ {IDCLS_P_MODEL_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_MODEL_SV, "<modell>"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_SID_MODEL,    N_("Specify SID model (0: 6581, 1: 8580, 2: 8580 + digi boost, 4: DTV)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_SID_MODEL_DE, "SID Modell definieren (0: 6581, 1: 8580, 2: 8580 + digi boost, 4: DTV)"},
/* fr */ {IDCLS_SPECIFY_SID_MODEL_FR, "Sp�cifier le mod�le SID (0: 6581, 1: 8580, 2: 8580 + Boost digital, 4: DTV)"},
/* hu */ {IDCLS_SPECIFY_SID_MODEL_HU, "Adja meg a SID t�pus�t (0: 6581, 1: 8580, 2: 8580 + digi boost, 4: DTV)"},
/* it */ {IDCLS_SPECIFY_SID_MODEL_IT, "Specifica il modello di SID (0: 6581, 1: 8580, 2: 8580 + digi boost, 4: DTV)"},
/* nl */ {IDCLS_SPECIFY_SID_MODEL_NL, "Geef het SID model (0: 6581, 1: 8580, 2: 8580 + digi boost, 4: DTV)"},
/* pl */ {IDCLS_SPECIFY_SID_MODEL_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SID_MODEL_SV, "Ange SID-modell (0: 6581, 1: 8580, 2: 8580 + digif�rst�rkning, 4: DTV)"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SID_FILTERS,    N_("Emulate SID filters")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SID_FILTERS_DE, "SID Filter Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_SID_FILTERS_FR, "�muler les filtres SID"},
/* hu */ {IDCLS_ENABLE_SID_FILTERS_HU, "SID sz�r�k emul�ci�ja"},
/* it */ {IDCLS_ENABLE_SID_FILTERS_IT, "Emula i filtri del SID"},
/* nl */ {IDCLS_ENABLE_SID_FILTERS_NL, "Emuleer SID-filters"},
/* pl */ {IDCLS_ENABLE_SID_FILTERS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SID_FILTERS_SV, "Emulera SID-filter"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SID_FILTERS,    N_("Do not emulate SID filters")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SID_FILTERS_DE, "SID Filter Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_SID_FILTERS_FR, "Ne pas �muler les filtres SID"},
/* hu */ {IDCLS_DISABLE_SID_FILTERS_HU, "Nem emul�lja a SID sz�r�ket"},
/* it */ {IDCLS_DISABLE_SID_FILTERS_IT, "Non emulare i filtri del SID"},
/* nl */ {IDCLS_DISABLE_SID_FILTERS_NL, "SID-filters niet emuleren"},
/* pl */ {IDCLS_DISABLE_SID_FILTERS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SID_FILTERS_SV, "Emulera inte SID-filter"},
#endif

#ifdef HAVE_RESID
/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_RESID_SAMPLING_METHOD,    N_("reSID sampling method (0: fast, 1: interpolating, 2: resampling, 3: fast "
                                             "resampling)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_RESID_SAMPLING_METHOD_DE, "reSID Sample Methode (0: schnell, 1: interpolierend, 2: resampling, 3: "
                                          "schnelles resampling)"},
/* fr */ {IDCLS_RESID_SAMPLING_METHOD_FR, "M�thode reSID (0: rapide, 1: interpolation, 2: r��chantillonnage, 3: "
                                          "r��chantillonnage rapide)"},
/* hu */ {IDCLS_RESID_SAMPLING_METHOD_HU, "reSID mintav�telez�si m�d (0: gyors, 1: interpol�l�, 2: �jramintav�telez�, 3: "
                                          "gyors �jramintav�telez�)"},
/* it */ {IDCLS_RESID_SAMPLING_METHOD_IT, "Metodo di campionamento del reSID (0: veloce, 1: interpolato, 2: ricampionato "
                                          "3: ricampionamento veloce)"},
/* nl */ {IDCLS_RESID_SAMPLING_METHOD_NL, "reSID-sampling methode (0: snel, 1: interpoleren, 2: resampling, 3: snelle "
                                          "resampling)"},
/* pl */ {IDCLS_RESID_SAMPLING_METHOD_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_RESID_SAMPLING_METHOD_SV, "reSID-samplingsmetod (0: snabb, 1: interpolerande, 2: omsamplning, 3: snabb "
                                          "omsampling)"},
#endif

/* sid/sid-cmdline-options.c, vsync.c */
/* en */ {IDCLS_P_PERCENT,    N_("<percent>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_PERCENT_DE, "<prozent>"},
/* fr */ {IDCLS_P_PERCENT_FR, "<pourcent>"},
/* hu */ {IDCLS_P_PERCENT_HU, "<sz�zal�k>"},
/* it */ {IDCLS_P_PERCENT_IT, "<percento>"},
/* nl */ {IDCLS_P_PERCENT_NL, "<procent>"},
/* pl */ {IDCLS_P_PERCENT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_PERCENT_SV, "<procent>"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_PASSBAND_PERCENTAGE,    N_("reSID resampling passband in percentage of total bandwidth (0 - 90)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PASSBAND_PERCENTAGE_DE, "reSID Resampling Passband Prozentwert der gesamte Bandbreite (0 - 90)\n(0 - 90, "
                                        "niedrig ist schneller, h�her ist besser)"},
/* fr */ {IDCLS_PASSBAND_PERCENTAGE_FR, "Bande passante pour le resampling reSID en pourcentage de la bande totale "
                                        "(0 - 90)"},
/* hu */ {IDCLS_PASSBAND_PERCENTAGE_HU, "reSID �jramintav�telez�si s�vnak a teljes s�vsz�less�gre vonatkoztatott ar�nya "
                                        "(0 - 90)"},
/* it */ {IDCLS_PASSBAND_PERCENTAGE_IT, "Banda passante di ricampionamento del reSID in percentuale di quella totale "
                                        "(0 - 90)"},
/* nl */ {IDCLS_PASSBAND_PERCENTAGE_NL, "reSID-resampling passband in percentage van de totale bandbreedte (0 - 90)"},
/* pl */ {IDCLS_PASSBAND_PERCENTAGE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PASSBAND_PERCENTAGE_SV, "Passband f�r reSID-resampling i procent av total bandbredd (0 - 90)"},
#endif

/* sid/sid-cmdline-options.c */
/* en */ {IDCLS_RESID_GAIN_PERCENTAGE,    N_("reSID gain in percent (90 - 100)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_RESID_GAIN_PERCENTAGE_DE, "reSID Gain in Prozent (90 - 100)"},
/* fr */ {IDCLS_RESID_GAIN_PERCENTAGE_FR, "Gain reSID en pourcent (90 - 100)"},
/* hu */ {IDCLS_RESID_GAIN_PERCENTAGE_HU, "reSID sz�zal�kos er�s�t�s (90 - 100)"},
/* it */ {IDCLS_RESID_GAIN_PERCENTAGE_IT, "Guadagno del reSID in percentuale (90 - 100)"},
/* nl */ {IDCLS_RESID_GAIN_PERCENTAGE_NL, "reSID-versterking procent (90 - 100)"},
/* pl */ {IDCLS_RESID_GAIN_PERCENTAGE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_RESID_GAIN_PERCENTAGE_SV, "Gain f�r reSID i procent (90 - 100)"},
#endif
#endif

/* vdc/vdc-cmdline-options.c */
/* en */ {IDCLS_SET_VDC_MEMORY_16KB,    N_("Set the VDC memory size to 16KB")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_VDC_MEMORY_16KB_DE, "VDC Speichgr��e auf 16KB setzen"},
/* fr */ {IDCLS_SET_VDC_MEMORY_16KB_FR, "R�gler la taille de la m�moire VDC � 16KO"},
/* hu */ {IDCLS_SET_VDC_MEMORY_16KB_HU, "VDC mem�ria m�ret be�ll�t�sa 16KB-ra"},
/* it */ {IDCLS_SET_VDC_MEMORY_16KB_IT, "Imposta la dimensione della memoria del VDC a 16KB"},
/* nl */ {IDCLS_SET_VDC_MEMORY_16KB_NL, "Zet de VDC-geheugengrootte als 16KB"},
/* pl */ {IDCLS_SET_VDC_MEMORY_16KB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_VDC_MEMORY_16KB_SV, "S�tt VDC-minnesstorlek till 16KB"},
#endif

/* vdc/vdc-cmdline-options.c */
/* en */ {IDCLS_SET_VDC_MEMORY_64KB,    N_("Set the VDC memory size to 64KB")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_VDC_MEMORY_64KB_DE, "VDC Speichgr��e auf 64KB setzen"},
/* fr */ {IDCLS_SET_VDC_MEMORY_64KB_FR, "R�gler la taille de la m�moire VDC � 64KO"},
/* hu */ {IDCLS_SET_VDC_MEMORY_64KB_HU, "VDC mem�ria m�ret be�ll�t�sa 64KB-ra"},
/* it */ {IDCLS_SET_VDC_MEMORY_64KB_IT, "Imposta la dimensione della memoria del VDC a 64KB"},
/* nl */ {IDCLS_SET_VDC_MEMORY_64KB_NL, "Zet de VDC-geheugengrootte als 64KB"},
/* pl */ {IDCLS_SET_VDC_MEMORY_64KB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_VDC_MEMORY_64KB_SV, "S�tt VDC-minnesstorlek till 64KB"},
#endif

/* vdc/vdc-cmdline-options.c */
/* en */ {IDCLS_SET_VDC_REVISION,    N_("Set VDC revision (0..2)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_VDC_REVISION_DE, "VDC Revision (0..2) setzen"},
/* fr */ {IDCLS_SET_VDC_REVISION_FR, "R�gler la r�vision VDC (0..2)"},
/* hu */ {IDCLS_SET_VDC_REVISION_HU, "VDC v�ltozatsz�m be�ll�t�sa (0-2)"},
/* it */ {IDCLS_SET_VDC_REVISION_IT, "Imposta la revisione del VDC (0..2)"},
/* nl */ {IDCLS_SET_VDC_REVISION_NL, "Zet de VDC-revisie (0..2)"},
/* pl */ {IDCLS_SET_VDC_REVISION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_VDC_REVISION_SV, "Ange VDC-revision (0..2)"},
#endif

/* vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_P_SPEC,    N_("<spec>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_SPEC_DE, "<Spec>"},
/* fr */ {IDCLS_P_SPEC_FR, "<spec>"},
/* hu */ {IDCLS_P_SPEC_HU, "<spec>"},
/* it */ {IDCLS_P_SPEC_IT, "<spec>"},
/* nl */ {IDCLS_P_SPEC_NL, "<spec>"},
/* pl */ {IDCLS_P_SPEC_PL, "<spec>"},
/* sv */ {IDCLS_P_SPEC_SV, "<spec>"},
#endif

/* vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_MEMORY_CONFIG,    N_("Specify memory configuration")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_MEMORY_CONFIG_DE, "Speicher Konfiguration definieren"},
/* fr */ {IDCLS_SPECIFY_MEMORY_CONFIG_FR, "Sp�cifier la configuration de la m�moire"},
/* hu */ {IDCLS_SPECIFY_MEMORY_CONFIG_HU, "Adja meg a mem�ria konfigur�ci�t"},
/* it */ {IDCLS_SPECIFY_MEMORY_CONFIG_IT, "Specifica la configurazione della memoria"},
/* nl */ {IDCLS_SPECIFY_MEMORY_CONFIG_NL, "Geef geheugenconfiguratie"},
/* pl */ {IDCLS_SPECIFY_MEMORY_CONFIG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_MEMORY_CONFIG_SV, "Ange minneskonfiguration"},
#endif

/* vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_ENABLE_VIC1112_IEEE488,    N_("Enable VIC-1112 IEEE488 interface")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_VIC1112_IEEE488_DE, "VIC-1112 IEEE488 Schnittstelle aktivieren"},
/* fr */ {IDCLS_ENABLE_VIC1112_IEEE488_FR, "Activer l'interface VIC-1112 IEEE488"},
/* hu */ {IDCLS_ENABLE_VIC1112_IEEE488_HU, "VIC-1112 IEEE488 interf�sz enged�lyez�se"},
/* it */ {IDCLS_ENABLE_VIC1112_IEEE488_IT, "Attiva l'interfaccia IEEE488 del VIC-1112"},
/* nl */ {IDCLS_ENABLE_VIC1112_IEEE488_NL, "Activeer VIC-1112 IEEE488-interface"},
/* pl */ {IDCLS_ENABLE_VIC1112_IEEE488_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_VIC1112_IEEE488_SV, "Aktivera VIC-1112-IEEE488-gr�nssnitt"},
#endif

/* vic20/vic20-cmdline-options.c */
/* en */ {IDCLS_DISABLE_VIC1112_IEEE488,    N_("Disable VIC-1112 IEEE488 interface")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_VIC1112_IEEE488_DE, "VIC-1112 IEEE488 Schnittstelle deaktivieren"},
/* fr */ {IDCLS_DISABLE_VIC1112_IEEE488_FR, "D�sactiver l'interface VIC-1112 IEEE488"},
/* hu */ {IDCLS_DISABLE_VIC1112_IEEE488_HU, "VIC-1112 IEEE488 interf�sz tilt�sa"},
/* it */ {IDCLS_DISABLE_VIC1112_IEEE488_IT, "Disattiva l'interfaccia IEEE488 del VIC-1112"},
/* nl */ {IDCLS_DISABLE_VIC1112_IEEE488_NL, "VIC-1112 IEEE488-interface uitschakelen"},
/* pl */ {IDCLS_DISABLE_VIC1112_IEEE488_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_VIC1112_IEEE488_SV, "Inaktivera VIC-1112-IEEE488-gr�nssnitt"},
#endif

/* vic20/vic20cartridge.c */
/* en */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME,    N_("Specify 4/8/16K extension ROM name at $2000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_DE, "4/8/16K Erweiterungs ROM Datei Name f�r $2000 definieren"},
/* fr */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_FR, "Sp�cifier le nom de l'extension ROM 4/8/16K � $2000"},
/* hu */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_HU, "Adja meg a $2000 c�m� 4/8/16K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_IT, "Specifica il nome della ROM di estensione di 4/8/16K a $2000"},
/* nl */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_NL, "Geef de naam van het bestand voor de 4/8/16K-ROM op $2000"},
/* pl */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_ROM_2000_NAME_SV, "Ange namn f�r 4/8/16K-ut�knings-ROM p� $2000"},
#endif

/* vic20/vic20cartridge.c */
/* en */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME,    N_("Specify 4/8/16K extension ROM name at $4000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_DE, "4/8/16K Erweiterungs ROM Datei Name f�r $4000 definieren"},
/* fr */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_FR, "Sp�cifier le nom de l'extension ROM 4/8/16K � $4000"},
/* hu */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_HU, "Adja meg a $4000 c�m� 4/8/16K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_IT, "Specifica il nome della ROM di estensione di 4/8/16K a $4000"},
/* nl */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_NL, "Geef de naam van het bestand voor de 4/8/16K-ROM op $4000"},
/* pl */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_ROM_4000_NAME_SV, "Ange namn f�r 4/8/16K-ut�knings-ROM p� $4000"},
#endif

/* vic20/vic20cartridge.c */
/* en */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME,    N_("Specify 4/8/16K extension ROM name at $6000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_DE, "4/8/16K Erweiterungs ROM Datei Name f�r $6000 definieren"},
/* fr */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_FR, "Sp�cifier le nom de l'extension ROM 4/8/16K � $6000"},
/* hu */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_HU, "Adja meg a $6000 c�m� 4/8/16K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_IT, "Specifica il nome della ROM di estensione di 4/8/16K a $6000"},
/* nl */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_NL, "Geef de naam van het bestand voor de 4/8/16K-ROM op $6000"},
/* pl */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_ROM_6000_NAME_SV, "Ange namn f�r 4/8/16K-ut�knings-ROM p� $6000"},
#endif

/* vic20/vic20cartridge.c */
/* en */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME,    N_("Specify 4/8K extension ROM name at $A000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_DE, "4/8K Erweiterungs ROM Datei Name f�r $A000 definieren"},
/* fr */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_FR, "Sp�cifier le nom de l'extension ROM 4/8K � $A000"},
/* hu */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_HU, "Adja meg a $A000 c�m� 4/8K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_IT, "Specifica il nome della ROM di estensione di 4/8K a $A000"},
/* nl */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_NL, "Geef de naam van het bestand voor de 4/8K-ROM op $A000"},
/* pl */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_ROM_A000_NAME_SV, "Ange namn f�r 4/8K-ut�knings-ROM p� $A000"},
#endif

/* vic20/vic20cartridge.c */
/* en */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME,    N_("Specify 4K extension ROM name at $B000")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_DE, "4K Erweiterungs ROM Datei Name f�r $B000 definieren"},
/* fr */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_FR, "Sp�cifier le nom de l'extension ROM 4K � $B000"},
/* hu */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_HU, "Adja meg a $B000 c�m� 4K ROM kiterjeszt�s nev�t"},
/* it */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_IT, "Specifica il nome della ROM di estensione di 4K a $B000"},
/* nl */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_NL, "Geef de naam van het bestand voor de 4K-ROM op $B000"},
/* pl */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXT_ROM_B000_NAME_SV, "Ange namn f�r 4K-ut�knings-ROM p� $B000"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_SET_BORDER_MODE,    N_("Set VIC-II border display mode (0: normal, 1: full, 2: debug)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_BORDER_MODE_DE, "VIC-II Rahmen Darstellung Modus (0: normal, 1: full, 2: debug)"},
/* fr */ {IDCLS_SET_BORDER_MODE_FR, "R�gler le mode de bordure VIC-II (0: normal, 1: complet, 2: debug)"},
/* hu */ {IDCLS_SET_BORDER_MODE_HU, "VIC-II keret megjelen�t�si m�d (0: norm�l, 1: teljes, 2: hibakeres�s)"},
/* it */ {IDCLS_SET_BORDER_MODE_IT, "Imposta la modalit� di visualizzazione del bordo del VIC-II (0: normale, 1: "
                                    "intero, 2: debug)"},
/* nl */ {IDCLS_SET_BORDER_MODE_NL, "Zet VIC-II border weergavemodus (0: normaal, 1: volledig, 2: debug)"},
/* pl */ {IDCLS_SET_BORDER_MODE_PL, ""}, /* fuzzy */
/* sv */ {IDCLS_SET_BORDER_MODE_SV, "V�lj VIC II-ramvisningsl�ge (0: normal, 1: full, 2: fels�kning)"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SPRITE_BACKGROUND,    N_("Enable sprite-background collision registers")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SPRITE_BACKGROUND_DE, "Sprite-Hintergrund Kollisionen aktivieren"},
/* fr */ {IDCLS_ENABLE_SPRITE_BACKGROUND_FR, "Activer les registres de collisions de sprite avec arri�re-plan"},
/* hu */ {IDCLS_ENABLE_SPRITE_BACKGROUND_HU, "Sprite-h�tt�r �tk�z�si regiszterek enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SPRITE_BACKGROUND_IT, "Attiva i registri di collisione sprite-sfondo"},
/* nl */ {IDCLS_ENABLE_SPRITE_BACKGROUND_NL, "Activeer sprite-achtergrond botsing registers"},
/* pl */ {IDCLS_ENABLE_SPRITE_BACKGROUND_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SPRITE_BACKGROUND_SV, "Aktivera sprite-till-bakgrund-kollisionsregister"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SPRITE_BACKGROUND,    N_("Disable sprite-background collision registers")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SPRITE_BACKGROUND_DE, "Sprite-Hintergrund Kollisionen deaktivieren"},
/* fr */ {IDCLS_DISABLE_SPRITE_BACKGROUND_FR, "D�sactiver les registres de collisions de sprite avec arri�re-plan"},
/* hu */ {IDCLS_DISABLE_SPRITE_BACKGROUND_HU, "Sprite-h�tt�r �tk�z�si regiszterek tilt�sa"},
/* it */ {IDCLS_DISABLE_SPRITE_BACKGROUND_IT, "Disattiva i registri di collisione sprite-sfondo"},
/* nl */ {IDCLS_DISABLE_SPRITE_BACKGROUND_NL, "Sprite-achtergrond botsing registers uitschakelen"},
/* pl */ {IDCLS_DISABLE_SPRITE_BACKGROUND_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SPRITE_BACKGROUND_SV, "Inaktivera sprite-till-bakgrund-kollisionsregister"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SPRITE_SPRITE,    N_("Enable sprite-sprite collision registers")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SPRITE_SPRITE_DE, "Sprite-Sprite Kollisionen aktivieren"},
/* fr */ {IDCLS_ENABLE_SPRITE_SPRITE_FR, "Activer les registres de collisions de sprite avec sprite"},
/* hu */ {IDCLS_ENABLE_SPRITE_SPRITE_HU, "Sprite-sprite �tk�z�si regiszterek enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SPRITE_SPRITE_IT, "Attiva i registri di collisione sprite-sprite"},
/* nl */ {IDCLS_ENABLE_SPRITE_SPRITE_NL, "Activeer sprite-sprite botsing registers"},
/* pl */ {IDCLS_ENABLE_SPRITE_SPRITE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SPRITE_SPRITE_SV, "Aktivera sprite-till-sprite-kollisionsregister"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SPRITE_SPRITE,    N_("Disable sprite-sprite collision registers")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SPRITE_SPRITE_DE, "Sprite-Sprite Kollisionen deaktivieren"},
/* fr */ {IDCLS_DISABLE_SPRITE_SPRITE_FR, "D�sactiver les registres de collisions de sprite avec sprite"},
/* hu */ {IDCLS_DISABLE_SPRITE_SPRITE_HU, "Sprite-sprite �tk�z�si regiszterek tilt�sa"},
/* it */ {IDCLS_DISABLE_SPRITE_SPRITE_IT, "Disattiva i registri di collisione sprite-sprite"},
/* nl */ {IDCLS_DISABLE_SPRITE_SPRITE_NL, "Sprite-sprite botsing registers uitschakelen"},
/* pl */ {IDCLS_DISABLE_SPRITE_SPRITE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SPRITE_SPRITE_SV, "Inaktivera sprite-till-sprite-kollisionsregister"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_USE_NEW_LUMINANCES,    N_("Use new luminances")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_NEW_LUMINANCES_DE, "Neue Helligkeitsemulation"},
/* fr */ {IDCLS_USE_NEW_LUMINANCES_FR, "Utiliser les nouvelles luminescences"},
/* hu */ {IDCLS_USE_NEW_LUMINANCES_HU, "Az �j f�nyer�ket haszn�lja"},
/* it */ {IDCLS_USE_NEW_LUMINANCES_IT, "Usa nuove luminanze"},
/* nl */ {IDCLS_USE_NEW_LUMINANCES_NL, "Gebruik nieuwe kleuren"},
/* pl */ {IDCLS_USE_NEW_LUMINANCES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_NEW_LUMINANCES_SV, "Anv�nd nya ljusstyrkor"},
#endif

/* vicii/vicii-cmdline-options.c */
/* en */ {IDCLS_USE_OLD_LUMINANCES,    N_("Use old luminances")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_OLD_LUMINANCES_DE, "Alte Helligkeitsemulation"},
/* fr */ {IDCLS_USE_OLD_LUMINANCES_FR, "Utiliser les anciennes luminescences"},
/* hu */ {IDCLS_USE_OLD_LUMINANCES_HU, "A r�gi f�nyer�ket haszn�lja"},
/* it */ {IDCLS_USE_OLD_LUMINANCES_IT, "Usa vecchie luminanze"},
/* nl */ {IDCLS_USE_OLD_LUMINANCES_NL, "Gebruik oude kleuren"},
/* pl */ {IDCLS_USE_OLD_LUMINANCES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_OLD_LUMINANCES_SV, "Anv�nd gamla ljusstyrkor"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DOUBLE_SIZE,    N_("Enable double size")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DOUBLE_SIZE_DE, "Doppelte Gr��e aktivieren"},
/* fr */ {IDCLS_ENABLE_DOUBLE_SIZE_FR, "Taille double"},
/* hu */ {IDCLS_ENABLE_DOUBLE_SIZE_HU, "Dupla m�ret enged�lyez�se"},
/* it */ {IDCLS_ENABLE_DOUBLE_SIZE_IT, "Attiva la dimensione doppia"},
/* nl */ {IDCLS_ENABLE_DOUBLE_SIZE_NL, "Activeer dubbele grootte"},
/* pl */ {IDCLS_ENABLE_DOUBLE_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DOUBLE_SIZE_SV, "Aktivera dubbel storlek"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DOUBLE_SIZE,    N_("Disable double size")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DOUBLE_SIZE_DE, "Doppelte Gr��e deaktivieren"},
/* fr */ {IDCLS_DISABLE_DOUBLE_SIZE_FR, "Taille normale"},
/* hu */ {IDCLS_DISABLE_DOUBLE_SIZE_HU, "Dupla m�ret tilt�sa"},
/* it */ {IDCLS_DISABLE_DOUBLE_SIZE_IT, "Disattiva la dimensione doppia"},
/* nl */ {IDCLS_DISABLE_DOUBLE_SIZE_NL, "Dubbele grootte uitschakelen"},
/* pl */ {IDCLS_DISABLE_DOUBLE_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DOUBLE_SIZE_SV, "Inaktivera dubbel storlek"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DOUBLE_SCAN,    N_("Enable double scan")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DOUBLE_SCAN_DE, "Doppelt Scan aktivieren"},
/* fr */ {IDCLS_ENABLE_DOUBLE_SCAN_FR, "Activer le mode double scan"},
/* hu */ {IDCLS_ENABLE_DOUBLE_SCAN_HU, "Dupla p�szt�z�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_DOUBLE_SCAN_IT, "Attiva la scansione doppia"},
/* nl */ {IDCLS_ENABLE_DOUBLE_SCAN_NL, "Activeer dubbele scan"},
/* pl */ {IDCLS_ENABLE_DOUBLE_SCAN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DOUBLE_SCAN_SV, "Aktivera dubbelskanning"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DOUBLE_SCAN,    N_("Disable double scan")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DOUBLE_SCAN_DE, "Doppelt Scan deaktivieren"},
/* fr */ {IDCLS_DISABLE_DOUBLE_SCAN_FR, "D�sactiver le mode double scan"},
/* hu */ {IDCLS_DISABLE_DOUBLE_SCAN_HU, "Dupla p�szt�z�s tilt�sa"},
/* it */ {IDCLS_DISABLE_DOUBLE_SCAN_IT, "Disattiva la scansione doppia"},
/* nl */ {IDCLS_DISABLE_DOUBLE_SCAN_NL, "Dubbele scan uitschakelen"},
/* pl */ {IDCLS_DISABLE_DOUBLE_SCAN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DOUBLE_SCAN_SV, "Inaktivera dubbelskanning"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_HARDWARE_SCALING,    N_("Enable hardware scaling")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_HARDWARE_SCALING_DE, "Hardwareunterst�tzung f�r Skalierung aktivieren"},
/* fr */ {IDCLS_ENABLE_HARDWARE_SCALING_FR, "Activer le \"scaling\" mat�riel"},
/* hu */ {IDCLS_ENABLE_HARDWARE_SCALING_HU, "Hardver �tm�retez�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_HARDWARE_SCALING_IT, "Attiva l'hardware scaling"},
/* nl */ {IDCLS_ENABLE_HARDWARE_SCALING_NL, "Activeer hardwareschaling"},
/* pl */ {IDCLS_ENABLE_HARDWARE_SCALING_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_HARDWARE_SCALING_SV, "Aktivera maskinvaruskalning"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_HARDWARE_SCALING,    N_("Disable hardware scaling")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_HARDWARE_SCALING_DE, "Hardwareunterst�tzung f�r Skalierung deaktivieren"},
/* fr */ {IDCLS_DISABLE_HARDWARE_SCALING_FR, "D�sactiver le \"scaling\" mat�riel"},
/* hu */ {IDCLS_DISABLE_HARDWARE_SCALING_HU, "Hardver �tm�retez�s tilt�sa"},
/* it */ {IDCLS_DISABLE_HARDWARE_SCALING_IT, "Disattiva l'hardware scaling"},
/* nl */ {IDCLS_DISABLE_HARDWARE_SCALING_NL, "Hardwareschaling uitschakelen"},
/* pl */ {IDCLS_DISABLE_HARDWARE_SCALING_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_HARDWARE_SCALING_SV, "Inaktivera maskinvaruskalning"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_SCALE2X,    N_("Enable Scale2x")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_SCALE2X_DE, "Scale2x aktivieren"},
/* fr */ {IDCLS_ENABLE_SCALE2X_FR, "Activer Scale2x"},
/* hu */ {IDCLS_ENABLE_SCALE2X_HU, "�lsim�t�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SCALE2X_IT, "Attiva Scale2x"},
/* nl */ {IDCLS_ENABLE_SCALE2X_NL, "Activeer Schaal2x"},
/* pl */ {IDCLS_ENABLE_SCALE2X_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SCALE2X_SV, "Aktivera Scale2x"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_SCALE2X,    N_("Disable Scale2x")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_SCALE2X_DE, "Scale2x deaktivieren"},
/* fr */ {IDCLS_DISABLE_SCALE2X_FR, "D�sactiver Scale2x"},
/* hu */ {IDCLS_DISABLE_SCALE2X_HU, "�lsim�t�s tilt�sa"},
/* it */ {IDCLS_DISABLE_SCALE2X_IT, "Disattiva Scale2x"},
/* nl */ {IDCLS_DISABLE_SCALE2X_NL, "Schaal2x uitschakelen"},
/* pl */ {IDCLS_DISABLE_SCALE2X_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SCALE2X_SV, "Inaktivera Scale2x"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_USE_INTERNAL_CALC_PALETTE,    N_("Use an internal calculated palette")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_INTERNAL_CALC_PALETTE_DE, "Benutzse intern berechnete Palette"},
/* fr */ {IDCLS_USE_INTERNAL_CALC_PALETTE_FR, "Utiliser une palette interne calcul�e"},
/* hu */ {IDCLS_USE_INTERNAL_CALC_PALETTE_HU, "Bels�leg sz�m�tott paletta haszn�lata"},
/* it */ {IDCLS_USE_INTERNAL_CALC_PALETTE_IT, "Usa una palette interna calcolata"},
/* nl */ {IDCLS_USE_INTERNAL_CALC_PALETTE_NL, "Gebruik een intern berekend kleurenpalet"},
/* pl */ {IDCLS_USE_INTERNAL_CALC_PALETTE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_INTERNAL_CALC_PALETTE_SV, "Anv�nd en internt ber�knad palett"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_USE_EXTERNAL_FILE_PALETTE,    N_("Use an external palette (file)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_DE, "Benutze externe Palette (Datei)"},
/* fr */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_FR, "Utiliser une palette externe (fichier)"},
/* hu */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_HU, "K�ls� paletta haszn�lata (f�jl)"},
/* it */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_IT, "Usa una palette esterna (file)"},
/* nl */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_NL, "Gebruik een extern kleurenpalet (bestand)"},
/* pl */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_EXTERNAL_FILE_PALETTE_SV, "Anv�nd en extern palett (fil)"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME,    N_("Specify name of file of external palette")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_DE, "Dateiname f�r externe Palette definieren"},
/* fr */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_FR, "Sp�cifier le nom du fichier de la palette externe"},
/* hu */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_HU, "Adja meg a k�ls� paletta nev�t"},
/* it */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_IT, "Specifica il nome del file della palette esterna"},
/* nl */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_NL, "Geef de naam van het extern kleurenpaletbestand"},
/* pl */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_EXTERNAL_PALETTE_NAME_SV, "Ange namn p� fil f�r extern palett"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_FULLSCREEN_MODE,    N_("Enable fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_FULLSCREEN_MODE_DE, "Vollbild Modus aktivieren"},
/* fr */ {IDCLS_ENABLE_FULLSCREEN_MODE_FR, "Activer le mode plein �cran"},
/* hu */ {IDCLS_ENABLE_FULLSCREEN_MODE_HU, "Teljesk�perny�s m�d enged�lyez�se"},
/* it */ {IDCLS_ENABLE_FULLSCREEN_MODE_IT, "Visualizza a tutto schermo"},
/* nl */ {IDCLS_ENABLE_FULLSCREEN_MODE_NL, "Activeer volschermmodus"},
/* pl */ {IDCLS_ENABLE_FULLSCREEN_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_FULLSCREEN_MODE_SV, "Aktivera fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_FULLSCREEN_MODE,    N_("Disable fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_FULLSCREEN_MODE_DE, "Vollbild Modus deaktivieren"},
/* fr */ {IDCLS_DISABLE_FULLSCREEN_MODE_FR, "D�sactiver le mode plein �cran"},
/* hu */ {IDCLS_DISABLE_FULLSCREEN_MODE_HU, "Teljesk�perny�s m�d tilt�sa"},
/* it */ {IDCLS_DISABLE_FULLSCREEN_MODE_IT, "Non visualizzare a tutto schermo"},
/* nl */ {IDCLS_DISABLE_FULLSCREEN_MODE_NL, "Volschermmodus uitschakelen"},
/* pl */ {IDCLS_DISABLE_FULLSCREEN_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_FULLSCREEN_MODE_SV, "Inaktivera fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_P_DEVICE,    N_("<device>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_DEVICE_DE, "<Ger�t>"},
/* fr */ {IDCLS_P_DEVICE_FR, "<p�riph�rique>"},
/* hu */ {IDCLS_P_DEVICE_HU, "<eszk�z>"},
/* it */ {IDCLS_P_DEVICE_IT, "<dispositivo>"},
/* nl */ {IDCLS_P_DEVICE_NL, "<apparaat>"},
/* pl */ {IDCLS_P_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_DEVICE_SV, "<enhet>"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_SELECT_FULLSCREEN_DEVICE,    N_("Select fullscreen device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SELECT_FULLSCREEN_DEVICE_DE, "Vollbild Ger�t selektieren"},
/* fr */ {IDCLS_SELECT_FULLSCREEN_DEVICE_FR, "S�lectionner le p�riph�rique plein �cran"},
/* hu */ {IDCLS_SELECT_FULLSCREEN_DEVICE_HU, "Teljesk�perny�s eszk�z kiv�laszt�sa"},
/* it */ {IDCLS_SELECT_FULLSCREEN_DEVICE_IT, "Seleziona il dispositivo per la visualizzazione a tutto schermo"},
/* nl */ {IDCLS_SELECT_FULLSCREEN_DEVICE_NL, "Selecteer volschermapparaat"},
/* pl */ {IDCLS_SELECT_FULLSCREEN_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SELECT_FULLSCREEN_DEVICE_SV, "Ange fullsk�rmsenhet"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN,    N_("Enable double size in fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_DE, "Doppelte Gr��e im Vollbild Modus aktivieren"},
/* fr */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_FR, "Activer \"Taille double\" en plein �cran"},
/* hu */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_HU, "Dupla m�ret enged�lyez�se teljesk�perny�s m�dban"},
/* it */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_IT, "Attiva la dimensione doppia nella visualizzazione a tutto schermo"},
/* nl */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_NL, "Activeer dubbele grootte in volschermmodus"},
/* pl */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DOUBLE_SIZE_FULLSCREEN_SV, "Aktivera dubbel storlek i fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN,    N_("Disable double size in fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_DE, "Doppelte Gro�e im Vollbild Modus deaktivieren"},
/* fr */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_FR, "D�sactiver \"Taille double\" en plein �cran"},
/* hu */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_HU, "Dupla m�ret tilt�sa teljesk�perny�s m�dban"},
/* it */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_IT, "Disattiva la dimensione doppia nella visualizzazione a tutto schermo"},
/* nl */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_NL, "Dubbele grootte in volschermmodus uitschakelen"},
/* pl */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DOUBLE_SIZE_FULLSCREEN_SV, "Inaktivera dubbel storlek i fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN,    N_("Enable double scan in fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_DE, "Doppelt Scan im Vollbild Modus aktivieren"},
/* fr */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_FR, "Activer \"Double scan\" en plein �cran"},
/* hu */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_HU, "Dupla p�szt�z�s enged�lyez�se teljesk�perny�s m�dban"},
/* it */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_IT, "Attiva la scansione doppia nella visualizzazione a tutto schermo"},
/* nl */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_NL, "Activeer dubbele scan in volschermmodus"},
/* pl */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DOUBLE_SCAN_FULLSCREEN_SV, "Aktivera dubbelskanning i fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN,    N_("Disable double scan in fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_DE, "Doppelt Scan im Vollbild Modus deaktivieren"},
/* fr */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_FR, "D�sactiver \"Double scan\" en plein �cran"},
/* hu */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_HU, "Dupla p�szt�z�s tilt�sa teljesk�perny�s m�dban"},
/* it */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_IT, "Disattiva la scansione doppia nella visualizzazione a tutto schermo"},
/* nl */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_NL, "Dubbele scan in volschermmodus uitschakelen"},
/* pl */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DOUBLE_SCAN_FULLSCREEN_SV, "Inaktivera dubbelskanning i fullsk�rmsl�ge"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_P_MODE,    N_("<mode>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_MODE_DE, "<Modus>"},
/* fr */ {IDCLS_P_MODE_FR, "<mode>"},
/* hu */ {IDCLS_P_MODE_HU, "<m�d>"},
/* it */ {IDCLS_P_MODE_IT, "<modalit�>"},
/* nl */ {IDCLS_P_MODE_NL, "<modus>"},
/* pl */ {IDCLS_P_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_MODE_SV, "<l�ge>"},
#endif

/* video/video-cmdline-options.c */
/* en */ {IDCLS_SELECT_FULLSCREEN_MODE,    N_("Select fullscreen mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SELECT_FULLSCREEN_MODE_DE, "Vollbild Modus w�hlen"},
/* fr */ {IDCLS_SELECT_FULLSCREEN_MODE_FR, "S�lectionner le mode plein �cran"},
/* hu */ {IDCLS_SELECT_FULLSCREEN_MODE_HU, "Teljes k�perny�s m�d kiv�laszt�sa"},
/* it */ {IDCLS_SELECT_FULLSCREEN_MODE_IT, "Seleziona la modalit� di visualizzazione a tutto schermo"},
/* nl */ {IDCLS_SELECT_FULLSCREEN_MODE_NL, "Selecteer volschermmodus"},
/* pl */ {IDCLS_SELECT_FULLSCREEN_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SELECT_FULLSCREEN_MODE_SV, "Aktivera fullsk�rmsl�ge"},
#endif

/* aciacore.c */
/* en */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE,    N_("Specify RS232 device this ACIA should work on")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_DE, "RS232 Ger�t f�r welches ACIA funktionieren soll ist zu spezifizieren"},
/* fr */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_FR, "Sp�cifier le p�riph�rique RS232 sur lequel cet ACIA doit fonctionner"},
/* hu */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_HU, "Adja meg az RS232-es egys�get, amivel az ACIA m�k�dni fog"},
/* it */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_IT, "Specifica il dispositivo RS232 con il quale questa ACIA dovrebbe "
                                              "funzionare"},
/* nl */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_NL, "Geef het RS232-apparaat waarmee deze ACIA moet werken"},
/* pl */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_ACIA_RS232_DEVICE_SV, "Ange RS232-enhet denna ACIA skall arbeta p�"},
#endif

/* attach.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_8,    N_("Set device type for device #8 (0: NONE, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                         "BLOCK DEVICE)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_8_DE, "Ger�te Typ f�r Ger�t #8 (0: Kein, 1: Dateisystem, 2: OpenCBM, 3: Block Ger�t)"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_8_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_DEVICE_TYPE_8_HU, "Adja meg a #8-es egys�g t�pus�t (0: Nincs, 1: f�jlrendszer, 2: OPENCBM, 3 "
                                      "blokk eszk�z)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_8_IT, "Imposta il tipo di periferica #8 (0 NESSUNA, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "DISPOSITIVO A BLOCCHI)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_8_NL, "Zet het apparaatsoort voor apparaat #8 (0: GEEN, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "BLOCK APPARAAT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_8_SV, "St�ll in enhetstyp f�r enhet 8 (0: INGEN, 1: FILSYSTEM, 2: OPENCBM, 3: BLOCKENHET)"},
#endif

/* attach.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_9,    N_("Set device type for device #9 (0: NONE, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                         "BLOCK DEVICE)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_9_DE, "Ger�te Typ f�r Ger�t #9 (0: Kein, 1: Dateisystem, 2: OpenCBM, 3: Block Ger�t)"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_9_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_DEVICE_TYPE_9_HU, "Adja meg a #9-es egys�g t�pus�t (0: Nincs, 1: f�jlrendszer, 2: OPENCBM, 3 "
                                      "blokk eszk�z)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_9_IT, "Imposta il tipo di periferica #9 (0 NESSUNA, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "DISPOSITIVO A BLOCCHI)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_9_NL, "Zet het apparaatsoort voor apparaat #9 (0: GEEN, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "BLOCK APPARAAT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_9_SV, "St�ll in enhetstyp f�r enhet 9 (0: INGEN, 1: FILSYSTEM, 2: OPENCBM, 3: BLOCKENHET)"},
#endif

/* attach.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_10,    N_("Set device type for device #10 (0: NONE, 1: FILESYSTEM, 2: OPENCBM, 3: BLOCK "
                                          "DEVICE)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_10_DE, "Ger�te Typ f�r Ger�t #10 (0: Kein, 1: Dateisystem, 2: OpenCBM, 3: Block Ger�t)"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_10_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_DEVICE_TYPE_10_HU, "Adja meg a #10-es egys�g t�pus�t (0: Nincs, 1: f�jlrendszer, 2: OPENCBM, 3 "
                                       "blokk eszk�z)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_10_IT, "Imposta il tipo di periferica #10 (0 NESSUNA, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "DISPOSITIVO A BLOCCHI)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_10_NL, "Zet het apparaatsoort voor apparaat #10 (0: GEEN, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                       "BLOCK APPARAAT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_10_SV, "St�ll in enhetstyp f�r enhet 10 (0: INGEN, 1: FILSYSTEM, 2: OPENCBM, 3: BLOCKENHET)"},
#endif

/* attach.c */
/* en */ {IDCLS_SET_DEVICE_TYPE_11,    N_("Set device type for device #11 (0: NONE, 1: FILESYSTEM, 2: OPENCBM, 3: BLOCK "
                                          "DEVICE)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_DEVICE_TYPE_11_DE, "Ger�te Typ f�r Ger�t #11 (0: Kein, 1: Dateisystem, 2: OpenCBM, 3: Block Ger�t)"},
/* fr */ {IDCLS_SET_DEVICE_TYPE_11_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SET_DEVICE_TYPE_11_HU, "Adja meg a #11-es egys�g t�pus�t (0: Nincs, 1: f�jlrendszer, 2: OPENCBM, 3 "
                                       "blokk eszk�z)"},
/* it */ {IDCLS_SET_DEVICE_TYPE_11_IT, "Imposta il tipo di periferica #11 (0 NESSUNA, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                      "DISPOSITIVO A BLOCCHI)"},
/* nl */ {IDCLS_SET_DEVICE_TYPE_11_NL, "Zet het apparaatsoort voor apparaat #11 (0: GEEN, 1: FILESYSTEM, 2: OPENCBM, 3: "
                                       "BLOCK APPARAAT)"},
/* pl */ {IDCLS_SET_DEVICE_TYPE_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_DEVICE_TYPE_11_SV, "St�ll in enhetstyp f�r enhet 11 (0: INGEN, 1: FILSYSTEM, 2: OPENCBM, 3: BLOCKENHET)"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_ONLY_8,    N_("Attach disk image for drive #8 read only")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_ONLY_8_DE, "Disk Image als Laufwerk #8 benutzen (schreibgesch�tzt)"},
/* fr */ {IDCLS_ATTACH_READ_ONLY_8_FR, "Charger une image de disque pour le lecteur #8 en lecture seule"},
/* hu */ {IDCLS_ATTACH_READ_ONLY_8_HU, "Csak olvashat� k�pm�s csatol�sa a #8-as lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_ONLY_8_IT, "Seleziona l'immagine del disco per il drive #8 in sola lettura"},
/* nl */ {IDCLS_ATTACH_READ_ONLY_8_NL, "Koppel schijfbestand voor drive #8 als alleen-lezen"},
/* pl */ {IDCLS_ATTACH_READ_ONLY_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_ONLY_8_SV, "Anslut diskettavbildningsfil f�r enhet 8 med skrivskydd"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_WRITE_8,    N_("Attach disk image for drive #8 read write (if possible)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_WRITE_8_DE, "Disk Image als Laufwerk #8 benutzen (Schreibzugriff, wenn m�glich)"},
/* fr */ {IDCLS_ATTACH_READ_WRITE_8_FR, "Charger une image de disque pour le lecteur #8 en lecture-�criture (si "
                                        "possible)"},
/* hu */ {IDCLS_ATTACH_READ_WRITE_8_HU, "�rhat�-olvashat� k�pm�s csatol�sa a #8-as lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_WRITE_8_IT, "Seleziona l'immagine del disco per il drive #8 in lettura/scrittura (se "
                                        "possibile)"},
/* nl */ {IDCLS_ATTACH_READ_WRITE_8_NL, "Koppel schijfbestand voor drive #8 als schrijfbaar (indien mogelijk)"},
/* pl */ {IDCLS_ATTACH_READ_WRITE_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_WRITE_8_SV, "Anslut diskettavbildningsfil f�r enhet 8 skrivbar (om m�jligt)"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_ONLY_9,    N_("Attach disk image for drive #9 read only")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_ONLY_9_DE, "Disk Image als Laufwerk #9 benutzen (schreibgesch�tzt)"},
/* fr */ {IDCLS_ATTACH_READ_ONLY_9_FR, "Charger une image de disque pour le lecteur #9 en lecture seule"},
/* hu */ {IDCLS_ATTACH_READ_ONLY_9_HU, "Csak olvashat� k�pm�s csatol�sa a #9-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_ONLY_9_IT, "Seleziona l'immagine del disco per il drive #9 in sola lettura"},
/* nl */ {IDCLS_ATTACH_READ_ONLY_9_NL, "Koppel schijfbestand voor drive #9 als alleen-lezen"},
/* pl */ {IDCLS_ATTACH_READ_ONLY_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_ONLY_9_SV, "Anslut diskettavbildningsfil f�r enhet 9 med skrivskydd"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_WRITE_9,    N_("Attach disk image for drive #9 read write (if possible)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_WRITE_9_DE, "Disk Image als Laufwerk #9 benutzen (Schreibzugriff, wenn m�glich)"},
/* fr */ {IDCLS_ATTACH_READ_WRITE_9_FR, "Charger une image de disque pour le lecteur #9 en lecture-�criture (si "
                                        "possible)"},
/* hu */ {IDCLS_ATTACH_READ_WRITE_9_HU, "�rhat�-olvashat� k�pm�s csatol�sa a #9-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_WRITE_9_IT, "Seleziona l'immagine del disco per il drive #9 in lettura/scrittura (se "
                                        "possibile)"},
/* nl */ {IDCLS_ATTACH_READ_WRITE_9_NL, "Koppel schijfbestand voor drive #9 als schrijfbaar (indien mogelijk)"},
/* pl */ {IDCLS_ATTACH_READ_WRITE_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_WRITE_9_SV, "Anslut diskettavbildningsfil f�r enhet 9 skrivbar (om m�jligt)"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_ONLY_10,    N_("Attach disk image for drive #10 read only")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_ONLY_10_DE, "Disk Image als Laufwerk #10 benutzen (schreibgesch�tzt)"},
/* fr */ {IDCLS_ATTACH_READ_ONLY_10_FR, "Charger une image de disque pour le lecteur #10 en lecture seule"},
/* hu */ {IDCLS_ATTACH_READ_ONLY_10_HU, "Csak olvashat� k�pm�s csatol�sa a #10-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_ONLY_10_IT, "Seleziona l'immagine del disco per il drive #10 in sola lettura"},
/* nl */ {IDCLS_ATTACH_READ_ONLY_10_NL, "Koppel schijfbestand voor drive #10 als alleen-lezen"},
/* pl */ {IDCLS_ATTACH_READ_ONLY_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_ONLY_10_SV, "Anslut diskettavbildningsfil f�r enhet 10 med skrivskydd"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_WRITE_10,    N_("Attach disk image for drive #10 read write (if possible)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_WRITE_10_DE, "Disk Image als Laufwerk #10 benutzen (Schreibzugriff, wenn m�glich)"},
/* fr */ {IDCLS_ATTACH_READ_WRITE_10_FR, "Charger une image de disque pour le lecteur #10 en lecture-�criture (si "
                                         "possible)"},
/* hu */ {IDCLS_ATTACH_READ_WRITE_10_HU, "�rhat�-olvashat� k�pm�s csatol�sa a #10-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_WRITE_10_IT, "Seleziona l'immagine del disco per il drive #10 in lettura/scrittura (se "
                                         "possibile)"},
/* nl */ {IDCLS_ATTACH_READ_WRITE_10_NL, "Koppel schijfbestand voor drive #10 als schrijfbaar (indien mogelijk)"},
/* pl */ {IDCLS_ATTACH_READ_WRITE_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_WRITE_10_SV, "Anslut diskettavbildningsfil f�r enhet 10 skrivbar (om m�jligt)"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_ONLY_11,    N_("Attach disk image for drive #11 read only")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_ONLY_11_DE, "Disk Image als Laufwerk #11 benutzen (schreibgesch�tzt)"},
/* fr */ {IDCLS_ATTACH_READ_ONLY_11_FR, "Charger une image de disque pour le lecteur #11 en lecture seule"},
/* hu */ {IDCLS_ATTACH_READ_ONLY_11_HU, "Csak olvashat� k�pm�s csatol�sa a #11-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_ONLY_11_IT, "Seleziona l'immagine del disco per il drive #11 in sola lettura"},
/* nl */ {IDCLS_ATTACH_READ_ONLY_11_NL, "Koppel schijfbestand voor drive #11 als alleen-lezen"},
/* pl */ {IDCLS_ATTACH_READ_ONLY_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_ONLY_11_SV, "Anslut diskettavbildningsfil f�r enhet 11 med skrivskydd"},
#endif

/* attach.c */
/* en */ {IDCLS_ATTACH_READ_WRITE_11,    N_("Attach disk image for drive #11 read write (if possible)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_READ_WRITE_11_DE, "Disk Image als Laufwerk #11 benutzen (Schreibzugriff, wenn m�glich)"},
/* fr */ {IDCLS_ATTACH_READ_WRITE_11_FR, "Charger une image de disque pour le lecteur #11 en lecture-�criture (si "
                                         "possible)"},
/* hu */ {IDCLS_ATTACH_READ_WRITE_11_HU, "�rhat�-olvashat� k�pm�s csatol�sa a #11-es lemezegys�ghez"},
/* it */ {IDCLS_ATTACH_READ_WRITE_11_IT, "Seleziona l'immagine del disco per il drive #11 in lettura/scrittura (se "
                                         "possibile)"},
/* nl */ {IDCLS_ATTACH_READ_WRITE_11_NL, "Koppel schijfbestand voor drive #11 als schrijfbaar (indien mogelijk)"},
/* pl */ {IDCLS_ATTACH_READ_WRITE_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_READ_WRITE_11_SV, "Anslut diskettavbildningsfil f�r enhet 11 skrivbar (om m�jligt)"},
#endif

/* datasette.c */
/* en */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET,    N_("Enable automatic Datasette-Reset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_DE, "Automatisches Datasette-Reset aktivieren"},
/* fr */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_FR, "Activer le red�marrage automatique du Datasette"},
/* hu */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_HU, "Automatikus magn� Reset enged�lyez�se"},
/* it */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_IT, "Attiva il reset automatico del registratore"},
/* nl */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_NL, "Activeer automatische Datasette-Reset"},
/* pl */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_AUTO_DATASETTE_RESET_SV, "Aktivera automatisk Datasette�terst�llning"},
#endif

/* datasette.c */
/* en */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET,    N_("Disable automatic Datasette-Reset")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_DE, "Automatisches Datasette-Reset deaktivieren"},
/* fr */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_FR, "D�sactiver le red�marrage automatique du Datasette"},
/* hu */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_HU, "Automatikus magn� Reset tilt�sa"},
/* it */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_IT, "Disattiva il reset automatico del registratore"},
/* nl */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_NL, "Automatische Datasette-Reset uitschakelen"},
/* pl */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_AUTO_DATASETTE_RESET_SV, "Inaktivera automatisk Datasette�terst�llning"},
#endif

/* datasette.c */
/* en */ {IDCLS_SET_ZERO_TAP_DELAY,    N_("Set delay in cycles for a zero in the tap")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_ZERO_TAP_DELAY_DE, "Verz�gerung f�r Zero in Zyklen im Tap"},
/* fr */ {IDCLS_SET_ZERO_TAP_DELAY_FR, "Sp�cifier le d�lai en cycles pour un z�ro sur le ruban"},
/* hu */ {IDCLS_SET_ZERO_TAP_DELAY_HU, "Adja meg ciklusokban a legels� szalagr�s hossz�t"},
/* it */ {IDCLS_SET_ZERO_TAP_DELAY_IT, "Imposta il ritardo in cicli per uno zero nel tap"},
/* nl */ {IDCLS_SET_ZERO_TAP_DELAY_NL, "Zet de vertraging in cycli voor een nul in de tap"},
/* pl */ {IDCLS_SET_ZERO_TAP_DELAY_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_ZERO_TAP_DELAY_SV, "Ange v�ntecykler f�r nolla i tap-filen"},
#endif

/* datasette.c */
/* en */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP,    N_("Set number of cycles added to each gap in the tap")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_DE, "Setze Anzahl der Zyklen f�r jedes Loch im Tap"},
/* fr */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_FR, "Sp�cifier le nombre de cycles ajout� a chaque raccord du ruban"},
/* hu */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_HU, "Adja meg ciklusokban a szalagf�jlok k�zti �res r�s hossz�t"},
/* it */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_IT, "Imposta il numero di cicli aggiunti ad ogni gap nel tap"},
/* nl */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_NL, "Zet aantal extra cycli voor elk gat in de tap"},
/* pl */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_CYCLES_ADDED_GAP_TAP_SV, "Ange cykler som l�ggs till varje gap i tap-filen"},
#endif

#ifdef DEBUG
/* debug.c */
/* en */ {IDCLS_TRACE_MAIN_CPU,    N_("Trace the main CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_MAIN_CPU_DE, "Haupt CPU verfolgen"},
/* fr */ {IDCLS_TRACE_MAIN_CPU_FR, "Tracer l'UCT principal"},
/* hu */ {IDCLS_TRACE_MAIN_CPU_HU, "F� CPU nyomk�vet�se"},
/* it */ {IDCLS_TRACE_MAIN_CPU_IT, "Traccia la CPU principale"},
/* nl */ {IDCLS_TRACE_MAIN_CPU_NL, "Traceer de hoofd-CPU"},
/* pl */ {IDCLS_TRACE_MAIN_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_MAIN_CPU_SV, "Sp�ra huvudprocessorn"},
#endif

/* debug.c */
/* en */ {IDCLS_DONT_TRACE_MAIN_CPU,    N_("Do not trace the main CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_TRACE_MAIN_CPU_DE, "Haupt CPU nicht verfolgen"},
/* fr */ {IDCLS_DONT_TRACE_MAIN_CPU_FR, "Ne pas tracer l'UCT principal"},
/* hu */ {IDCLS_DONT_TRACE_MAIN_CPU_HU, "F� CPU nyomk�vet�s�nek tilt�sa"},
/* it */ {IDCLS_DONT_TRACE_MAIN_CPU_IT, "Non tracciare la CPU principale"},
/* nl */ {IDCLS_DONT_TRACE_MAIN_CPU_NL, "Traceer de hoofd-CPU niet"},
/* pl */ {IDCLS_DONT_TRACE_MAIN_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_TRACE_MAIN_CPU_SV, "Sp�ra inte huvudprocessorn"},
#endif

/* debug.c */
/* en */ {IDCLS_TRACE_DRIVE0_CPU,    N_("Trace the drive0 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_DRIVE0_CPU_DE, "Laufwerk0 CPU verfolgen"},
/* fr */ {IDCLS_TRACE_DRIVE0_CPU_FR, "Tracer l'UCT du drive0"},
/* hu */ {IDCLS_TRACE_DRIVE0_CPU_HU, "A 0-s lemezegys�g CPU-j�nak nyomk�vet�se"},
/* it */ {IDCLS_TRACE_DRIVE0_CPU_IT, "Traccia la CPU del drive0"},
/* nl */ {IDCLS_TRACE_DRIVE0_CPU_NL, "Traceer de CPU van drive 0/8"},
/* pl */ {IDCLS_TRACE_DRIVE0_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_DRIVE0_CPU_SV, "Sp�ra processor i diskettstation 0"},
#endif

/* debug.c */
/* en */ {IDCLS_DONT_TRACE_DRIVE0_CPU,    N_("Do not trace the drive0 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_TRACE_DRIVE0_CPU_DE, "Laufwerk0 CPU nicht verfolgen"},
/* fr */ {IDCLS_DONT_TRACE_DRIVE0_CPU_FR, "Ne pas tracer l'UCT du drive0"},
/* hu */ {IDCLS_DONT_TRACE_DRIVE0_CPU_HU, "A 0-s lemezegys�g CPU-ja nyomk�vet�s�nek tilt�sa"},
/* it */ {IDCLS_DONT_TRACE_DRIVE0_CPU_IT, "Non tracciare la CPU del drive0"},
/* nl */ {IDCLS_DONT_TRACE_DRIVE0_CPU_NL, "Traceer de CPU van drive 0/8 niet"},
/* pl */ {IDCLS_DONT_TRACE_DRIVE0_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_TRACE_DRIVE0_CPU_SV, "Sp�ra inte processor i diskettstation 0"},
#endif

/* debug.c */
/* en */ {IDCLS_TRACE_DRIVE1_CPU,    N_("Trace the drive1 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_DRIVE1_CPU_DE, "Laufwerk1 CPU verfolgen"},
/* fr */ {IDCLS_TRACE_DRIVE1_CPU_FR, "Tracer l'UCT du drive1"},
/* hu */ {IDCLS_TRACE_DRIVE1_CPU_HU, "Az 1-es lemezegys�g CPU-j�nak nyomk�vet�se"},
/* it */ {IDCLS_TRACE_DRIVE1_CPU_IT, "Traccia la CPU del drive1"},
/* nl */ {IDCLS_TRACE_DRIVE1_CPU_NL, "Traceer de CPU van drive 1/9"},
/* pl */ {IDCLS_TRACE_DRIVE1_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_DRIVE1_CPU_SV, "Sp�ra processor i diskettstation 1"},
#endif

/* debug.c */
/* en */ {IDCLS_DONT_TRACE_DRIVE1_CPU,    N_("Do not trace the drive1 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_TRACE_DRIVE1_CPU_DE, "Laufwerk1 CPU nicht verfolgen"},
/* fr */ {IDCLS_DONT_TRACE_DRIVE1_CPU_FR, "Ne pas tracer l'UCT du drive1"},
/* hu */ {IDCLS_DONT_TRACE_DRIVE1_CPU_HU, "Az 1-es lemezegys�g CPU-ja nyomk�vet�s�nek tilt�sa"},
/* it */ {IDCLS_DONT_TRACE_DRIVE1_CPU_IT, "Non tracciare la CPU del drive1"},
/* nl */ {IDCLS_DONT_TRACE_DRIVE1_CPU_NL, "Traceer de CPU van drive 1/9 niet"},
/* pl */ {IDCLS_DONT_TRACE_DRIVE1_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_TRACE_DRIVE1_CPU_SV, "Sp�ra inte processor i diskettstation 1"},
#endif

#if DRIVE_NUM > 2
/* debug.c */
/* en */ {IDCLS_TRACE_DRIVE2_CPU,    N_("Trace the drive2 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_DRIVE2_CPU_DE, "Laufwerk2 CPU verfolgen"},
/* fr */ {IDCLS_TRACE_DRIVE2_CPU_FR, "Tracer l'UCT du drive2"},
/* hu */ {IDCLS_TRACE_DRIVE2_CPU_HU, "A 2-es lemezegys�g CPU-j�nak nyomk�vet�se"},
/* it */ {IDCLS_TRACE_DRIVE2_CPU_IT, "Traccia la CPU del drive2"},
/* nl */ {IDCLS_TRACE_DRIVE2_CPU_NL, "Traceer de CPU van drive 2/10"},
/* pl */ {IDCLS_TRACE_DRIVE2_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_DRIVE2_CPU_SV, "Sp�ra processor i diskettstation 2"},
#endif

/* debug.c */
/* en */ {IDCLS_DONT_TRACE_DRIVE2_CPU,    N_("Do not trace the drive2 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_TRACE_DRIVE2_CPU_DE, "Laufwerk2 CPU nicht verfolgen"},
/* fr */ {IDCLS_DONT_TRACE_DRIVE2_CPU_FR, "Ne pas tracer l'UCT du drive2"},
/* hu */ {IDCLS_DONT_TRACE_DRIVE2_CPU_HU, "A 2-es lemezegys�g CPU-ja nyomk�vet�s�nek tilt�sa"},
/* it */ {IDCLS_DONT_TRACE_DRIVE2_CPU_IT, "Non tracciare la CPU del drive2"},
/* nl */ {IDCLS_DONT_TRACE_DRIVE2_CPU_NL, "Traceer de CPU van drive 2/10 niet"},
/* pl */ {IDCLS_DONT_TRACE_DRIVE2_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_TRACE_DRIVE2_CPU_SV, "Sp�ra inte processor i diskettstation 2"},
#endif
#endif

#if DRIVE_NUM > 3
/* debug.c */
/* en */ {IDCLS_TRACE_DRIVE3_CPU,    N_("Trace the drive3 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_DRIVE3_CPU_DE, "Laufwerk3 CPU verfolgen"},
/* fr */ {IDCLS_TRACE_DRIVE3_CPU_FR, "Tracer l'UCT du drive3"},
/* hu */ {IDCLS_TRACE_DRIVE3_CPU_HU, "A 3-as lemezegys�g CPU-j�nak nyomk�vet�se"},
/* it */ {IDCLS_TRACE_DRIVE3_CPU_IT, "Traccia la CPU del drive3"},
/* nl */ {IDCLS_TRACE_DRIVE3_CPU_NL, "Traceer de CPU van drive 3/11"},
/* pl */ {IDCLS_TRACE_DRIVE3_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_DRIVE3_CPU_SV, "Sp�ra processor i diskettstation 3"},
#endif

/* debug.c */
/* en */ {IDCLS_DONT_TRACE_DRIVE3_CPU,    N_("Do not trace the drive3 CPU")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_TRACE_DRIVE3_CPU_DE, "Laufwerk3 CPU nicht verfolgen"},
/* fr */ {IDCLS_DONT_TRACE_DRIVE3_CPU_FR, "Ne pas tracer l'UCT du drive3"},
/* hu */ {IDCLS_DONT_TRACE_DRIVE3_CPU_HU, "A 3-as lemezegys�g CPU-ja nyomk�vet�s�nek tilt�sa"},
/* it */ {IDCLS_DONT_TRACE_DRIVE3_CPU_IT, "Non tracciare la CPU del drive3"},
/* nl */ {IDCLS_DONT_TRACE_DRIVE3_CPU_NL, "Traceer de CPU van drive 3/11 niet"},
/* pl */ {IDCLS_DONT_TRACE_DRIVE3_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_TRACE_DRIVE3_CPU_SV, "Sp�ra inte processor i diskettstation 0"},
#endif
#endif

/* debug.c */
/* en */ {IDCLS_TRACE_MODE,    N_("Trace mode (0=normal 1=small 2=history)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_TRACE_MODE_DE, "Verfolgung Modus (0=normal 1=klein 2=Geschichte)"},
/* fr */ {IDCLS_TRACE_MODE_FR, "Mode de trace (0=normal 1=petit 2=historique)"},
/* hu */ {IDCLS_TRACE_MODE_HU, "Nyomk�vet�si m�d (0=norm�l 1=kicsi 2=el�zm�nyek)"},
/* it */ {IDCLS_TRACE_MODE_IT, "Modalit� di tracciamento (0=normale 1=piccola 2=storica)"},
/* nl */ {IDCLS_TRACE_MODE_NL, "Traceermodus (0=normaal 1=klein 2=geschiedenis)"},
/* pl */ {IDCLS_TRACE_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_TRACE_MODE_SV, "Sp�rningsl�ge (0=normal 1=liten 2=historik)"},
#endif
#endif

/* event.c */
/* en */ {IDCLS_PLAYBACK_RECORDED_EVENTS,    N_("Playback recorded events")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PLAYBACK_RECORDED_EVENTS_DE, "Wiedergabe von aufgenommener Ereignisse"},
/* fr */ {IDCLS_PLAYBACK_RECORDED_EVENTS_FR, "Lire les �v�nements enregistr�s"},
/* hu */ {IDCLS_PLAYBACK_RECORDED_EVENTS_HU, "A felvett esem�nyek visszaj�tsz�sa"},
/* it */ {IDCLS_PLAYBACK_RECORDED_EVENTS_IT, "Riproduzione degli eventi registrati"},
/* nl */ {IDCLS_PLAYBACK_RECORDED_EVENTS_NL, "Afspelen opgenomen gebeurtenissen"},
/* pl */ {IDCLS_PLAYBACK_RECORDED_EVENTS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PLAYBACK_RECORDED_EVENTS_SV, "Spela upp inspelade h�ndelser"},
#endif

/* monitor.c */
/* en */ {IDCLS_EXECUTE_MONITOR_FROM_FILE,    N_("Execute monitor commands from file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_DE, "Monitor Kommandos von Datei ausf�hren"},
/* fr */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_FR, "Ex�cuter des commandes moniteur depuis un fichier"},
/* hu */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_HU, "Monitorprogram parancsok futtat�sa f�jlb�l"},
/* it */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_IT, "Esegui i comandi del monitor da un file"},
/* nl */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_NL, "Uitvoeren van commandos uit bestand"},
/* pl */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_PL, ""}, /* fuzzy */
/* sv */ {IDCLS_EXECUTE_MONITOR_FROM_FILE_SV, "Exekvera monitorkommandon fr�n fil"},
#endif

/* monitor.c */
/* en */ {IDCLS_SET_INITIAL_BREAKPOINT,    N_("Set an initial breakpoint for the monitor")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_INITIAL_BREAKPOINT_DE, "Setze initialen Breakpoint f�r Monitor"},
/* fr */ {IDCLS_SET_INITIAL_BREAKPOINT_FR, "R�gler un point d�arr�t initial pour le moniteur"},
/* hu */ {IDCLS_SET_INITIAL_BREAKPOINT_HU, "Kezdeti t�r�spont megad�sa a monitorprogramnak"},
/* it */ {IDCLS_SET_INITIAL_BREAKPOINT_IT, "Imposta un breakpoint iniziale per il monitor"},
/* nl */ {IDCLS_SET_INITIAL_BREAKPOINT_NL, "Zet een beginnende breekpunt voor de monitor"},
/* pl */ {IDCLS_SET_INITIAL_BREAKPOINT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_INITIAL_BREAKPOINT_SV, "S�tt en ursprunglig brytpunkt f�r monitorn"},
#endif

/* fliplist.c */
/* en */ {IDCLS_SPECIFY_FLIP_LIST_NAME,    N_("Specify name of the flip list file image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_FLIP_LIST_NAME_DE, "Namen f�r Fliplist Datei definieren"},
/* fr */ {IDCLS_SPECIFY_FLIP_LIST_NAME_FR, "Sp�cifier le nom de l'image du fichier de groupement de disques"},
/* hu */ {IDCLS_SPECIFY_FLIP_LIST_NAME_HU, "Adja meg a lemezlista f�jl nev�t"},
/* it */ {IDCLS_SPECIFY_FLIP_LIST_NAME_IT, "Specifica il nome dell'immagine del file della flip list"},
/* nl */ {IDCLS_SPECIFY_FLIP_LIST_NAME_NL, "Geef de naam van het fliplijstbestand"},
/* pl */ {IDCLS_SPECIFY_FLIP_LIST_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_FLIP_LIST_NAME_SV, "Ange namn p� vallistefilavbildningen"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS,    N_("Show a list of the available options and exit normally")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_DE, "Liste von verf�gbaren Optionen zeigen und beenden"},
/* fr */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_FR, "Montrer une liste des options disponibles et terminer normalement"},
/* hu */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_HU, "Kilist�zza a lehets�ges opci�kat �s kil�p"},
/* it */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_IT, "Mostra la lista delle opzioni disponibili ed esce"},
/* nl */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_NL, "Toon een lijst van de beschikbare opties en exit zoals normaal"},
/* pl */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SHOW_COMMAND_LINE_OPTIONS_SV, "Visa lista �ver tillg�ngliga flaggor och avsluta normalt"},
#endif

#if (!defined  __OS2__ && !defined __BEOS__)
/* initcmdline.c */
/* en */ {IDCLS_CONSOLE_MODE,    N_("Console mode (for music playback)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_CONSOLE_MODE_DE, "Konsolenmodus (f�r Musikwiedergabe)"},
/* fr */ {IDCLS_CONSOLE_MODE_FR, "Mode console (pour l'�coute de musique)"},
/* hu */ {IDCLS_CONSOLE_MODE_HU, "Konzol m�d (zene lej�tsz�shoz)"},
/* it */ {IDCLS_CONSOLE_MODE_IT, "Modalit� console (per la riproduzione musicale)"},
/* nl */ {IDCLS_CONSOLE_MODE_NL, "Consolemodus (voor afspelen voor muziek)"},
/* pl */ {IDCLS_CONSOLE_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_CONSOLE_MODE_SV, "Konsoll�ge (f�r musikspelning)"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ALLOW_CORE_DUMPS,    N_("Allow production of core dumps")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ALLOW_CORE_DUMPS_DE, "Core Dumps erm�glichen"},
/* fr */ {IDCLS_ALLOW_CORE_DUMPS_FR, "Permettre la production de \"core dumps\""},
/* hu */ {IDCLS_ALLOW_CORE_DUMPS_HU, "Core dump k�sz�t�s enged�lyez�se"},
/* it */ {IDCLS_ALLOW_CORE_DUMPS_IT, "Consente la produzione di core dump"},
/* nl */ {IDCLS_ALLOW_CORE_DUMPS_NL, "Maken van core dumps toestaan"},
/* pl */ {IDCLS_ALLOW_CORE_DUMPS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ALLOW_CORE_DUMPS_SV, "Till�t skapa minnesutskrift"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_DONT_ALLOW_CORE_DUMPS,    N_("Do not produce core dumps")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_ALLOW_CORE_DUMPS_DE, "Core Dumps verhindern"},
/* fr */ {IDCLS_DONT_ALLOW_CORE_DUMPS_FR, "Ne pas produire de \"core dumps\""},
/* hu */ {IDCLS_DONT_ALLOW_CORE_DUMPS_HU, "Core dump k�sz�t�s tilt�sa"},
/* it */ {IDCLS_DONT_ALLOW_CORE_DUMPS_IT, "Non produce core dump"},
/* nl */ {IDCLS_DONT_ALLOW_CORE_DUMPS_NL, "Maken van core dumps niet toestaan"},
/* pl */ {IDCLS_DONT_ALLOW_CORE_DUMPS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_ALLOW_CORE_DUMPS_SV, "Skapa inte minnesutskrift"},
#endif
#else
/* initcmdline.c */
/* en */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER,    N_("Don't call exception handler")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_DE, "Ausnahmebehandlung vermeiden"},
/* fr */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_FR, "Ne pas utiliser l'assistant d'exception"},
/* hu */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_HU, "Ne h�vja a kiv�tel kezel�t"},
/* it */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_IT, "Richiama il gestore delle eccezioni"},
/* nl */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_NL, "Geen gebruik maken van de exception handler"},
/* pl */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DONT_CALL_EXCEPTION_HANDLER_SV, "Anropa inte undantagshanterare"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_CALL_EXCEPTION_HANDLER,    N_("Call exception handler (default)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_CALL_EXCEPTION_HANDLER_DE, "Ausnahmebehandlung aktivieren (Default)"},
/* fr */ {IDCLS_CALL_EXCEPTION_HANDLER_FR, "Utiliser l'assistant d'exception (par d�faut)"},
/* hu */ {IDCLS_CALL_EXCEPTION_HANDLER_HU, "Kiv�tel kezel� h�v�sa (alap�rtelmez�s)"},
/* it */ {IDCLS_CALL_EXCEPTION_HANDLER_IT, "Richiama il gestore delle eccezioni (predefinito)"},
/* nl */ {IDCLS_CALL_EXCEPTION_HANDLER_NL, "Gebruik maken van de exception handler (standaard)"},
/* pl */ {IDCLS_CALL_EXCEPTION_HANDLER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_CALL_EXCEPTION_HANDLER_SV, "Anropa undantagshanterare (standard)"},
#endif
#endif

/* initcmdline.c */
/* en */ {IDCLS_RESTORE_DEFAULT_SETTINGS,    N_("Restore default (factory) settings")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_RESTORE_DEFAULT_SETTINGS_DE, "Wiederherstellen Standard Einstellungen"},
/* fr */ {IDCLS_RESTORE_DEFAULT_SETTINGS_FR, "R�tablir les param�tres par d�faut (usine)"},
/* hu */ {IDCLS_RESTORE_DEFAULT_SETTINGS_HU, "Alap (gy�ri) be�ll�t�sok vissza�ll�t�sa"},
/* it */ {IDCLS_RESTORE_DEFAULT_SETTINGS_IT, "Ripristina le impostazioni originarie"},
/* nl */ {IDCLS_RESTORE_DEFAULT_SETTINGS_NL, "Herstel standaardinstelling"},
/* pl */ {IDCLS_RESTORE_DEFAULT_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_RESTORE_DEFAULT_SETTINGS_SV, "�terst�ll f�rvalda inst�llningar"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_P_FILE,    N_("<filename>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_FILE_DE, "<Dateiname>"},
/* fr */ {IDCLS_P_FILE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_P_FILE_HU, "<f�jln�v>"},
/* it */ {IDCLS_P_FILE_IT, "<nome del file>"},
/* nl */ {IDCLS_P_FILE_NL, "<bestandsnaam>"},
/* pl */ {IDCLS_P_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_FILE_SV, "<filnamn>"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_SPECIFY_CONFIG_FILE,    N_("Specify config file")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_CONFIG_FILE_DE, "Konfigurationsdateiname definieren"},
/* fr */ {IDCLS_SPECIFY_CONFIG_FILE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_CONFIG_FILE_HU, "Adja meg a konfigur�ci�s f�jl nev�t"},
/* it */ {IDCLS_SPECIFY_CONFIG_FILE_IT, "Specifica il nome del file di configurazione"},
/* nl */ {IDCLS_SPECIFY_CONFIG_FILE_NL, "Geef configuratie bestandsnaam"},
/* pl */ {IDCLS_SPECIFY_CONFIG_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_CONFIG_FILE_SV, "Ange inst�llningsfil"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AND_AUTOSTART,    N_("Attach and autostart tape/disk image <name>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AND_AUTOSTART_DE, "Einlegen und Autostart Disk/Band Image Datei <Name>"},
/* fr */ {IDCLS_ATTACH_AND_AUTOSTART_FR, "Ins�rer et d�marrer l'image de disque/datassette <nom>"},
/* hu */ {IDCLS_ATTACH_AND_AUTOSTART_HU, "<n�v> lemez/szalag k�pm�s csatol�sa �s automatikus elind�t�sa"},
/* it */ {IDCLS_ATTACH_AND_AUTOSTART_IT, "Seleziona ed avvia l'immagine di una cassetta/disco <nome>"},
/* nl */ {IDCLS_ATTACH_AND_AUTOSTART_NL, "Koppel en autostart een tape/diskbestand <naam>"},
/* pl */ {IDCLS_ATTACH_AND_AUTOSTART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AND_AUTOSTART_SV, "Anslut och starta band-/diskettavbildning <namn>"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AND_AUTOLOAD,    N_("Attach and autoload tape/disk image <name>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AND_AUTOLOAD_DE, "Einlegen und Autoload Disk/Band Image Datei <Name>"},
/* fr */ {IDCLS_ATTACH_AND_AUTOLOAD_FR, "Ins�rer et charger l'image de disque/datassette <nom>"},
/* hu */ {IDCLS_ATTACH_AND_AUTOLOAD_HU, "<n�v> lemez/szalag k�pm�s csatol�sa �s automatikus bet�lt�se"},
/* it */ {IDCLS_ATTACH_AND_AUTOLOAD_IT, "Seleziona e carica l'immagine di una cassetta/disco <nome>"},
/* nl */ {IDCLS_ATTACH_AND_AUTOLOAD_NL, "Koppel en autolaad een tape-/diskbestand <naam>"},
/* pl */ {IDCLS_ATTACH_AND_AUTOLOAD_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AND_AUTOLOAD_SV, "Anslut och l�s in band-/diskettavbildning <namn>"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AS_TAPE,    N_("Attach <name> as a tape image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AS_TAPE_DE, "Image Datei <Name> einlegen"},
/* fr */ {IDCLS_ATTACH_AS_TAPE_FR, "Ins�rer <nom> comme image de datassette"},
/* hu */ {IDCLS_ATTACH_AS_TAPE_HU, "<n�v> szalag k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_AS_TAPE_IT, "Seleziona <nome> come un'immagine di una cassetta"},
/* nl */ {IDCLS_ATTACH_AS_TAPE_NL, "Koppel <naam> als een tapebestand"},
/* pl */ {IDCLS_ATTACH_AS_TAPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AS_TAPE_SV, "Anslut <namn> som bandavbildning"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AS_DISK_8,    N_("Attach <name> as a disk image in drive #8")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AS_DISK_8_DE, "Image Datei <Name> im Laufwerk #8 einlegen"},
/* fr */ {IDCLS_ATTACH_AS_DISK_8_FR, "Ins�rer <nom> comme image de disque dans le lecteur #8"},
/* hu */ {IDCLS_ATTACH_AS_DISK_8_HU, "<n�v> lemezk�pm�s csatol�sa #8-as egys�gk�nt"},
/* it */ {IDCLS_ATTACH_AS_DISK_8_IT, "Seleziona <nome> come un'immagine di un disco nel drive #8"},
/* nl */ {IDCLS_ATTACH_AS_DISK_8_NL, "Koppel <naam> aan als een schijfbestand in drive #8"},
/* pl */ {IDCLS_ATTACH_AS_DISK_8_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AS_DISK_8_SV, "Anslut <namn> som diskettavbildning i enhet 8"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AS_DISK_9,    N_("Attach <name> as a disk image in drive #9")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AS_DISK_9_DE, "Image Datei <Name> im Laufwerk #9 einlegen"},
/* fr */ {IDCLS_ATTACH_AS_DISK_9_FR, "Ins�rer <nom> comme image de disque dans le lecteur #9"},
/* hu */ {IDCLS_ATTACH_AS_DISK_9_HU, "<n�v> lemezk�pm�s csatol�sa #9-es egys�gk�nt"},
/* it */ {IDCLS_ATTACH_AS_DISK_9_IT, "Seleziona <nome> come un'immagine di un disco nel drive #9"},
/* nl */ {IDCLS_ATTACH_AS_DISK_9_NL, "Koppel <naam> aan als een schijfbestand in drive #9"},
/* pl */ {IDCLS_ATTACH_AS_DISK_9_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AS_DISK_9_SV, "Anslut <namn> som diskettavbildning i enhet 9"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AS_DISK_10,    N_("Attach <name> as a disk image in drive #10")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AS_DISK_10_DE, "Image Datei <Name> im Laufwerk #10 einlegen"},
/* fr */ {IDCLS_ATTACH_AS_DISK_10_FR, "Ins�rer <nom> comme image de disque dans le lecteur #10"},
/* hu */ {IDCLS_ATTACH_AS_DISK_10_HU, "<n�v> lemezk�pm�s csatol�sa #10-es egys�gk�nt"},
/* it */ {IDCLS_ATTACH_AS_DISK_10_IT, "Seleziona <nome> come un'immagine di un disco nel drive #10"},
/* nl */ {IDCLS_ATTACH_AS_DISK_10_NL, "Koppel <naam> aan als een schijfbestand in drive #10"},
/* pl */ {IDCLS_ATTACH_AS_DISK_10_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AS_DISK_10_SV, "Anslut <namn> som diskettavbildning i enhet 10"},
#endif

/* initcmdline.c */
/* en */ {IDCLS_ATTACH_AS_DISK_11,    N_("Attach <name> as a disk image in drive #11")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ATTACH_AS_DISK_11_DE, "Image Datei <Name> im Laufwerk #11 einlegen"},
/* fr */ {IDCLS_ATTACH_AS_DISK_11_FR, "Ins�rer <nom> comme image de disque dans le lecteur #11"},
/* hu */ {IDCLS_ATTACH_AS_DISK_11_HU, "<n�v> lemezk�pm�s csatol�sa #11-es egys�gk�nt"},
/* it */ {IDCLS_ATTACH_AS_DISK_11_IT, "Seleziona <nome> come un'immagine di un disco nel drive #11"},
/* nl */ {IDCLS_ATTACH_AS_DISK_11_NL, "Koppel <naam> aan als een schijfbestand in drive #11"},
/* pl */ {IDCLS_ATTACH_AS_DISK_11_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_AS_DISK_11_SV, "Anslut <namn> som diskettavbildning i enhet 11"},
#endif

/* kbdbuf.c */
/* en */ {IDCLS_P_STRING,    N_("<string>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_STRING_DE, "<String>"},
/* fr */ {IDCLS_P_STRING_FR, "<chaine>"},
/* hu */ {IDCLS_P_STRING_HU, "<sztring>"},
/* it */ {IDCLS_P_STRING_IT, "<stringa>"},
/* nl */ {IDCLS_P_STRING_NL, "<string>"},
/* pl */ {IDCLS_P_STRING_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_STRING_SV, "<str�ng>"},
#endif

/* kbdbuf.c */
/* en */ {IDCLS_PUT_STRING_INTO_KEYBUF,    N_("Put the specified string into the keyboard buffer.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PUT_STRING_INTO_KEYBUF_DE, "Definierte Eingabe in Tastaturpuffer bereitstellen."},
/* fr */ {IDCLS_PUT_STRING_INTO_KEYBUF_FR, "Placer la cha�ne sp�cifi�e dans le tampon clavier."},
/* hu */ {IDCLS_PUT_STRING_INTO_KEYBUF_HU, "A megadott string bem�sol�sa a billenty�zet pufferbe."},
/* it */ {IDCLS_PUT_STRING_INTO_KEYBUF_IT, "Metti la stringa specificata nel buffer di tastiera."},
/* nl */ {IDCLS_PUT_STRING_INTO_KEYBUF_NL, "Plaats de opgegeven string in de toetsenbordbuffer."},
/* pl */ {IDCLS_PUT_STRING_INTO_KEYBUF_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PUT_STRING_INTO_KEYBUF_SV, "L�gg den angivna str�ngen i tangentbordsbufferten."},
#endif

/* log.c */
/* en */ {IDCLS_SPECIFY_LOG_FILE_NAME,    N_("Specify log file name")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_LOG_FILE_NAME_DE, "Logdateiname definieren"},
/* fr */ {IDCLS_SPECIFY_LOG_FILE_NAME_FR, "Sp�cifier le nom du fichier log"},
/* hu */ {IDCLS_SPECIFY_LOG_FILE_NAME_HU, "Adja meg a napl�f�jl nev�t"},
/* it */ {IDCLS_SPECIFY_LOG_FILE_NAME_IT, "Specifica il nome del file di log"},
/* nl */ {IDCLS_SPECIFY_LOG_FILE_NAME_NL, "Geef de naam van het logbestand"},
/* pl */ {IDCLS_SPECIFY_LOG_FILE_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_LOG_FILE_NAME_SV, "Ange namn p� loggfil"},
#endif

/* mouse.c */
/* en */ {IDCLS_ENABLE_MOUSE_GRAB,    N_("Enable mouse grab")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_MOUSE_GRAB_DE, "Maus aktivieren"},
/* fr */ {IDCLS_ENABLE_MOUSE_GRAB_FR, "Saisir la souris"},
/* hu */ {IDCLS_ENABLE_MOUSE_GRAB_HU, "Eg�r elkap�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_MOUSE_GRAB_IT, "Attiva cattura mouse"},
/* nl */ {IDCLS_ENABLE_MOUSE_GRAB_NL, "Gebruiken van de muis inschakelen"},
/* pl */ {IDCLS_ENABLE_MOUSE_GRAB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_MOUSE_GRAB_SV, "Aktivera f�ngande av mus"},
#endif

/* mouse.c */
/* en */ {IDCLS_DISABLE_MOUSE_GRAB,    N_("Disable mouse grab")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_MOUSE_GRAB_DE, "Maus deaktivieren"},
/* fr */ {IDCLS_DISABLE_MOUSE_GRAB_FR, "Ne pas saisir la souris"},
/* hu */ {IDCLS_DISABLE_MOUSE_GRAB_HU, "Eg�r elkap�s tilt�sa"},
/* it */ {IDCLS_DISABLE_MOUSE_GRAB_IT, "Disattiva cattura mouse"},
/* nl */ {IDCLS_DISABLE_MOUSE_GRAB_NL, "Gebruiken van de muis uitschakelen"},
/* pl */ {IDCLS_DISABLE_MOUSE_GRAB_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_MOUSE_GRAB_SV, "Inaktivera f�ngande av mus"},
#endif

/* mouse.c */
/* en */ {IDCLS_SELECT_MOUSE_JOY_PORT,    N_("Select the joystick port the mouse is attached to")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SELECT_MOUSE_JOY_PORT_DE, "Joystickport f�r Maus ausw�hlen"},
/* fr */ {IDCLS_SELECT_MOUSE_JOY_PORT_FR, "S�lectionner le port sur lequel attacher la souris"},
/* hu */ {IDCLS_SELECT_MOUSE_JOY_PORT_HU, "V�lassza ki a joystick portot, ahov� az eg�r csatolva van"},
/* it */ {IDCLS_SELECT_MOUSE_JOY_PORT_IT, "Seleziona la porta joystick a cui � collegato il mouse"},
/* nl */ {IDCLS_SELECT_MOUSE_JOY_PORT_NL, "Selecteer de joystickpoort waar de muis aan gekoppelt is"},
/* pl */ {IDCLS_SELECT_MOUSE_JOY_PORT_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SELECT_MOUSE_JOY_PORT_SV, "Ange vilken spelport musen �r ansluten till"},
#endif

/* mouse.c */
/* en */ {IDCLS_SELECT_MOUSE_TYPE,    N_("Select the mouse type (0 = 1351, 1 = NEOS, 2 = Amiga)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SELECT_MOUSE_TYPE_DE, "Maus typ w�hlen (0 = 1351, 1 = NEOS, 2 = Amiga)"},
/* fr */ {IDCLS_SELECT_MOUSE_TYPE_FR, "S�lectionner le type de souris (0 = 1351, 1 = NEOS, 2 = Amiga)"},
/* hu */ {IDCLS_SELECT_MOUSE_TYPE_HU, "Eg�r t�pus kiv�laszt�sa (0 = 1351, 1 = NEOS, 2 = Amiga)"},
/* it */ {IDCLS_SELECT_MOUSE_TYPE_IT, "Seleziona il tipo di mouse (0 = 1351, 1 = NEOS, 2 = Amiga)"},
/* nl */ {IDCLS_SELECT_MOUSE_TYPE_NL, "Selecteer de muis soort (0 = 1351, 1 = NEOS, 2 = Amiga)"},
/* pl */ {IDCLS_SELECT_MOUSE_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SELECT_MOUSE_TYPE_SV, "V�lj mustyp (0 = 1351, 1 = NEOS, 2 = Amiga)"},
#endif

/* ps2mouse.c */
/* en */ {IDCLS_ENABLE_PS2MOUSE,    N_("Enable PS/2 mouse on userport")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_PS2MOUSE_DE, "PS/2 Userport Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_PS2MOUSE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_PS2MOUSE_HU, "PS/2 eg�r enged�lyez�se a userporton"},
/* it */ {IDCLS_ENABLE_PS2MOUSE_IT, "Attiva mouse PS/2 su userport"},
/* nl */ {IDCLS_ENABLE_PS2MOUSE_NL, "Aktiveer emulatie van een PS/2 muis op de userport"},
/* pl */ {IDCLS_ENABLE_PS2MOUSE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_PS2MOUSE_SV, "Aktivera PS/2-mus p� anv�ndarport"},
#endif

/* ps2mouse.c */
/* en */ {IDCLS_DISABLE_PS2MOUSE,    N_("Disable PS/2 mouse on userport")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_PS2MOUSE_DE, "PS/2 Userport Emulation deaktivieren\""},
/* fr */ {IDCLS_DISABLE_PS2MOUSE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_PS2MOUSE_HU, "PS/2 eg�r tilt�sa a userporton"},
/* it */ {IDCLS_DISABLE_PS2MOUSE_IT, "Disattiva mouse PS/2 su userport"},
/* nl */ {IDCLS_DISABLE_PS2MOUSE_NL, "Emulatie van een PS/2 muis op de userport afsluiten"},
/* pl */ {IDCLS_DISABLE_PS2MOUSE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_PS2MOUSE_SV, "Inaktivera PS/2-mus p� anv�ndarport"},
#endif

/* ram.c */
/* en */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE,    N_("Set the value for the very first RAM address after powerup")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_DE, "Wert f�r erstes Byte im RAM nach Kaltstart setzen"},
/* fr */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_FR, "Sp�cifier la valeur de la premi�re adresse RAM apr�s la mise sous "
                                                "tension"},
/* hu */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_HU, "Adja meg a legels� RAM c�m �rt�k�t bekapcsol�s ut�n"},
/* it */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_IT, "Imposta il valore del primissimo indirizzo della RAM dopo l'accensione"},
/* nl */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_NL, "Zet de waarde voor het allereerste RAM adres na koude start"},
/* pl */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_FIRST_RAM_ADDRESS_VALUE_SV, "Ange v�rdet p� den allra f�rsta RAM-adressen efter str�mp�slag"},
#endif

/* ram.c */
/* en */ {IDCLS_P_NUM_OF_BYTES,    N_("<num of bytes>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_NUM_OF_BYTES_DE, "<Anzahl an Bytes>"},
/* fr */ {IDCLS_P_NUM_OF_BYTES_FR, "<nombre d'octets>"},
/* hu */ {IDCLS_P_NUM_OF_BYTES_HU, "<b�jtok sz�ma>"},
/* it */ {IDCLS_P_NUM_OF_BYTES_IT, "<numero di byte>"},
/* nl */ {IDCLS_P_NUM_OF_BYTES_NL, "<aantal bytes>"},
/* pl */ {IDCLS_P_NUM_OF_BYTES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_NUM_OF_BYTES_SV, "<antal byte>"},
#endif

/* ram.c */
/* en */ {IDCLS_LENGTH_BLOCK_SAME_VALUE,    N_("Length of memory block initialized with the same value")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_DE, "L�nge des Speicherblocks der mit dem gleichen Wert initialisiert ist"},
/* fr */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_FR, "Longeur du premier bloc m�moire initialis� avec la m�me valeur"},
/* hu */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_HU, "Azonos �rt�kkel felt�lt�tt mem�ria blokkoknak a hossza"},
/* it */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_IT, "Lunghezza del blocco di memoria inizializzato con lo stesso valore"},
/* nl */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_NL, "Geheugenblokgrootte die dezelfde waarde krijgt bij initialisatie"},
/* pl */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_LENGTH_BLOCK_SAME_VALUE_SV, "L�ngd p� minnesblock som initierats med samma v�rde"},
#endif

/* ram.c */
/* en */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN,    N_("Length of memory block initialized with the same pattern")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_DE, "L�nge des Speicherblocks der mit dem gleichen Muster initialisiert ist"},
/* fr */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_FR, "Longeur du premier bloc m�moire initialis� avec le m�me pattern"},
/* hu */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_HU, "Azonos mint�val felt�lt�tt mem�ria blokkoknak a hossza"},
/* it */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_IT, "Lunghezza del blocco di memoria inizializzato con lo stesso pattern"},
/* nl */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_NL, "Geheugenblokgrootte met hetzelfde patroon bij initialisatie"},
/* pl */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_LENGTH_BLOCK_SAME_PATTERN_SV, "L�ngd p� minnesblock som initierats med samma m�nster"},
#endif

/* sound.c */
/* en */ {IDCLS_ENABLE_SOUND_PLAYBACK,    N_("Enable sound playback")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_ENABLE_SOUND_PLAYBACK_DE, "Sound Wiedergaben einschalten"},
/* fr */ {IDCLS_ENABLE_SOUND_PLAYBACK_FR, "Activer le son"},
/* hu */ {IDCLS_ENABLE_SOUND_PLAYBACK_HU, "Hangok enged�lyez�se"},
/* it */ {IDCLS_ENABLE_SOUND_PLAYBACK_IT, "Attiva la riproduzione del suono"},
/* nl */ {IDCLS_ENABLE_SOUND_PLAYBACK_NL, "Activeer geluidsweergave"},
/* pl */ {IDCLS_ENABLE_SOUND_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_SOUND_PLAYBACK_SV, "Aktivera ljud�tergivning"},
#endif

/* sound.c */
/* en */ {IDCLS_DISABLE_SOUND_PLAYBACK,    N_("Disable sound playback")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_DISABLE_SOUND_PLAYBACK_DE, "Sound Wiedergaben ausschalten"},
/* fr */ {IDCLS_DISABLE_SOUND_PLAYBACK_FR, "D�sactiver le son"},
/* hu */ {IDCLS_DISABLE_SOUND_PLAYBACK_HU, "Hangok tilt�sa"},
/* it */ {IDCLS_DISABLE_SOUND_PLAYBACK_IT, "Disattiva la riproduzione del suono"},
/* nl */ {IDCLS_DISABLE_SOUND_PLAYBACK_NL, "Geluid uitschakelen"},
/* pl */ {IDCLS_DISABLE_SOUND_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_SOUND_PLAYBACK_SV, "Inaktivera ljud�tergivning"},
#endif

/* sound.c */
/* en */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ,    N_("Set sound sample rate to <value> Hz")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_DE, "Setze Sound Sample Rate zu <Wert> Hz"},
/* fr */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_FR, "R�gler le taux d'�chantillonage � <valeur> Hz"},
/* hu */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_HU, "Hang mintav�telez�si r�ta be�ll�t�sa <�rt�k> Hz-re"},
/* it */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_IT, "Imposta la velocit� di campionamento del suono a <valore> Hz"},
/* nl */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_NL, "Zet de geluid sample rate naar <waarde> Hz"},
/* pl */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_SAMPLE_RATE_VALUE_HZ_SV, "S�tt ljudsamplingshastighet till <v�rde> Hz"},
#endif

/* sound.c */
/* en */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC,    N_("Set sound buffer size to <value> msec")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_DE, "Setze Source Buffer Gr��e zu <Wert> msek"},
/* fr */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_FR, "R�gler la taille du tampon son � <valeur> ms"},
/* hu */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_HU, "A hangpuffer m�ret�t <�rt�k> mp-re �ll�tja"},
/* it */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_IT, "Imposta la dimensione del buffer del suono a <valore> msec"},
/* nl */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_NL, "Zet de geluidsbuffergrootte naar <waarde> msec"},
/* pl */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_SOUND_BUFFER_SIZE_MSEC_SV, "S�tt ljudbuffertstorlek till <v�rde> ms"},
#endif

/* sound.c */
/* en */ {IDCLS_SPECIFY_SOUND_DRIVER,    N_("Specify sound driver")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SPECIFY_SOUND_DRIVER_DE, "Sound Treiber spezifieren"},
/* fr */ {IDCLS_SPECIFY_SOUND_DRIVER_FR, "Sp�cifier le pilote de son"},
/* hu */ {IDCLS_SPECIFY_SOUND_DRIVER_HU, "Adja meg a hangmodul nev�t"},
/* it */ {IDCLS_SPECIFY_SOUND_DRIVER_IT, "Specifica il driver audio"},
/* nl */ {IDCLS_SPECIFY_SOUND_DRIVER_NL, "Geef geluidsstuurprogramma"},
/* pl */ {IDCLS_SPECIFY_SOUND_DRIVER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SOUND_DRIVER_SV, "Ange ljuddrivrutin"},
#endif

/* sound.c */
/* en */ {IDCLS_P_ARGS,    N_("<args>")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_P_ARGS_DE, "<Argumente>"},
/* fr */ {IDCLS_P_ARGS_FR, "<args>"},
/* hu */ {IDCLS_P_ARGS_HU, "<argumentumok>"},
/* it */ {IDCLS_P_ARGS_IT, "<argomenti>"},
/* nl */ {IDCLS_P_ARGS_NL, "<parameters>"},
/* pl */ {IDCLS_P_ARGS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_ARGS_SV, "<flaggor>"},
#endif

/* sound.c */
/* en */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM,    N_("Specify initialization parameters for sound driver")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_DE, "Initialisierungsparameter des Sound Treibers spezifizieren"},
/* fr */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_FR, "Sp�cifier les param�tres d'initialisation pour le pilote son"},
/* hu */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_HU, "Adja meg a hangmodul indul�si param�tereit"},
/* it */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_IT, "Specifica i parametri di inizializzazione del driver audio"},
/* nl */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_NL, "Geef de initialisatieparameters voor het geluidsstuurprogramma"},
/* pl */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_SOUND_DRIVER_PARAM_SV, "Ange initieringsflaggor f�r ljuddrivrutin"},
#endif

/* sound.c */
/* en */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER,    N_("Specify recording sound driver")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_DE, "Sound Treiber f�r Aufnahme spezifizieren"},
/* fr */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_FR, "Sp�cifier le pilote d'enregistrement"},
/* hu */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_HU, "Adja meg a felv�telhez haszn�lt hangmodult"},
/* it */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_IT, "Specifica il driver di registrazione del suono"},
/* nl */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_NL, "Geef geluidsstuurprogramma voor opname"},
/* pl */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_RECORDING_SOUND_DRIVER_SV, "Ange ljuddrivrutin f�r inspelning"},
#endif

/* sound.c */
/* en */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM,    N_("Specify initialization parameters for recording sound driver")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_DE, "Initialisierungsparameter f�r Aufnahme Sound Treiber spezifieren"},
/* fr */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_FR, "Sp�cifier les param�tres d'initialisation pour le pilote "
                                                   "d'enregistrement"},
/* hu */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_HU, "Adja meg a felv�telhez haszn�lt hangmodul indul�si param�tereit"},
/* it */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_IT, "Specifica i parametri di inizializzazione per il driver audio di "
                                                   "registrazione"},
/* nl */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_NL, "Geef initialisatieparameters voor het geluidsstuurprogramma voor "
                                                   "opname"},
/* pl */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_REC_SOUND_DRIVER_PARAM_SV, "Ange initieringsflaggor f�r ljuddrivrutin f�r inspelning"},
#endif

/* sound.c */
/* en */ {IDCLS_P_SYNC,    N_("<sync>")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_P_SYNC_DE, "<Sync>"},
/* fr */ {IDCLS_P_SYNC_FR, "<sync>"},
/* hu */ {IDCLS_P_SYNC_HU, "<sync>"},
/* it */ {IDCLS_P_SYNC_IT, "<sync>"},
/* nl */ {IDCLS_P_SYNC_NL, "<sync>"},
/* pl */ {IDCLS_P_SYNC_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_SYNC_SV, "<synk>"},
#endif

/* sound.c */
/* en */ {IDCLS_SET_SOUND_SPEED_ADJUST,    N_("Set sound speed adjustment (0: flexible, 1: adjusting, 2: exact)")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_SET_SOUND_SPEED_ADJUST_DE, "Setze Sound Geschwindigkeit Anpassung (0: flexibel, 1: anpassend, 2: exakt)"},
/* fr */ {IDCLS_SET_SOUND_SPEED_ADJUST_FR, "Choisir la m�thode d'ajustement du son (0: flexible, 1: ajust� 2: exact)"},
/* hu */ {IDCLS_SET_SOUND_SPEED_ADJUST_HU, "Adja meg a zene sebess�g igaz�t�s�t (0: rugalmas. 1: igazod�, 2: pontos)"},
/* it */ {IDCLS_SET_SOUND_SPEED_ADJUST_IT, "Imposta il tipo di adattamento della velocit� dell'audio (0: flessibile, "
                                           "1: adattabile, 2: esatta)"},
/* nl */ {IDCLS_SET_SOUND_SPEED_ADJUST_NL, "Zet geluidssnelheid aanpassing (0: flexibel, 1: aanpassend, 2: exact)"},
/* pl */ {IDCLS_SET_SOUND_SPEED_ADJUST_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SET_SOUND_SPEED_ADJUST_SV, "St�ll in ljudhastighetsjustering (0: flexibel, 1: justerande, 2: exakt)"},
#endif

/* sysfile.c */
/* en */ {IDCLS_P_PATH,    N_("<path>")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_P_PATH_DE, "<Pfad>"},
/* fr */ {IDCLS_P_PATH_FR, "<chemin>"},
/* hu */ {IDCLS_P_PATH_HU, "<el�r�si �t>"},
/* it */ {IDCLS_P_PATH_IT, "<percorso>"},
/* nl */ {IDCLS_P_PATH_NL, "<pad>"},
/* pl */ {IDCLS_P_PATH_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_PATH_SV, "<s�kv�g>"},
#endif

/* sysfile.c */
/* en */ {IDCLS_DEFINE_SYSTEM_FILES_PATH,    N_("Define search path to locate system files")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_DE, "Suchpfad f�r Systemdateien definieren"},
/* fr */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_FR, "D�finir le chemin de recherche pour trouver les fichiers syst�mes"},
/* hu */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_HU, "Keres�si �tvonal megad�sa a rendszerf�jlok megtal�l�s�ra"},
/* it */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_IT, "Definisci il path di ricerca per cercare i file di sistema"},
/* nl */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_NL, "Geef het zoek pad waar de systeem bestanden te vinden zijn"},
/* pl */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DEFINE_SYSTEM_FILES_PATH_SV, "Ange s�kv�g f�r att hitta systemfiler"},
#endif

/* traps.c */
/* en */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION,    N_("Enable general mechanisms for fast disk/tape emulation")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_DE, "Allgemeine Mechanismen f�r schnelle Disk/Band Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_FR, "Activer les m�chanismes g�n�raux pour l'�mulation disque/datassette "
                                                "rapide"},
/* hu */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_HU, "A lemez/szalag emul�ci�t gyors�t� mechanizmusok enged�lyez�se"},
/* it */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_IT, "Attiva meccanismo generale per l'emulazione veloce del disco/cassetta"},
/* nl */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_NL, "Activeer algemene methoden voor snelle disk/tape emulatie"},
/* pl */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_TRAPS_FAST_EMULATION_SV, "Aktivera generella mekanismer f�r snabb disk-/bandemulering"},
#endif

/* traps.c */
/* en */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION,    N_("Disable general mechanisms for fast disk/tape emulation")},
#ifdef HAS_TRANSLATE
/* de */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_DE, "Allgemeine Mechanismen f�r schnelle Disk/Band Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_FR, "D�sactiver les m�chanismes g�n�raux pour l'�mulation disque/datassette "
                                                 "rapide"},
/* hu */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_HU, "A lemez/szalag emul�ci�t gyors�t� mechanizmusok tilt�sa"},
/* it */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_IT, "Disattiva meccanismo generale per l'emulazione veloce del "
                                                 "disco/cassetta"},
/* nl */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_NL, "Algemene methoden voor snelle disk/tape emulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_TRAPS_FAST_EMULATION_SV, "Inaktivera generella mekanismer f�r snabb disk-/bandemulering"},
#endif

/* vsync.c */
/* en */ {IDCLS_LIMIT_SPEED_TO_VALUE,    N_("Limit emulation speed to specified value")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_LIMIT_SPEED_TO_VALUE_DE, "Emulationsgeschwindigkeit auf Wert beschr�nken."},
/* fr */ {IDCLS_LIMIT_SPEED_TO_VALUE_FR, "Limiter la vitesse d'�mulation � une valeur specifi�e"},
/* hu */ {IDCLS_LIMIT_SPEED_TO_VALUE_HU, "Emul�ci�s sebess�g lehat�rol�sa adott �rt�kre"},
/* it */ {IDCLS_LIMIT_SPEED_TO_VALUE_IT, "Limita la velocit� di emulazione al valore specificato"},
/* nl */ {IDCLS_LIMIT_SPEED_TO_VALUE_NL, "Beperk emulatiesnelheid tot de opgegeven waarde"},
/* pl */ {IDCLS_LIMIT_SPEED_TO_VALUE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_LIMIT_SPEED_TO_VALUE_SV, "Begr�nsa emuleringshastighet till angivet v�rde"},
#endif

/* vsync.c */
/* en */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES,    N_("Update every <value> frames (`0' for automatic)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_DE, "Jedes <Wert> Bild aktualisieren (`0' f�r Automatik)"},
/* fr */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_FR, "Mise � jour toutes les <valeur> images (`0' pour auto.)"},
/* hu */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_HU, "Friss�tsen minden <�rt�k> k�pkocka eltelt�vel (0 automatikust jelent)"},
/* it */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_IT, "Aggiorna ogni <valore> frame (`0' per automatico)"},
/* nl */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_NL, "Scherm opbouw elke <waarde> frames (`0' voor automatisch)"},
/* pl */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_UPDATE_EVERY_VALUE_FRAMES_SV, "Uppdatera varje <v�rde> ramar (\"0\" f�r automatiskt)"},
#endif

/* vsync.c */
/* en */ {IDCLS_ENABLE_WARP_MODE,    N_("Enable warp mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_WARP_MODE_DE, "Warp Mode Aktivieren"},
/* fr */ {IDCLS_ENABLE_WARP_MODE_FR, "Activer Turbo"},
/* hu */ {IDCLS_ENABLE_WARP_MODE_HU, "Hipergyors m�d enged�lyez�se"},
/* it */ {IDCLS_ENABLE_WARP_MODE_IT, "Attiva la modalit� turbo"},
/* nl */ {IDCLS_ENABLE_WARP_MODE_NL, "Activeer warpmodus"},
/* pl */ {IDCLS_ENABLE_WARP_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_WARP_MODE_SV, "Aktivera warpl�ge"},
#endif

/* vsync.c */
/* en */ {IDCLS_DISABLE_WARP_MODE,    N_("Disable warp mode")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_WARP_MODE_DE, "*Warp Mode Deaktivieren"},
/* fr */ {IDCLS_DISABLE_WARP_MODE_FR, "D�sactiver Turbo"},
/* hu */ {IDCLS_DISABLE_WARP_MODE_HU, "Hipergyors m�d tilt�sa"},
/* it */ {IDCLS_DISABLE_WARP_MODE_IT, "Disattiva la modalit� turbo"},
/* nl */ {IDCLS_DISABLE_WARP_MODE_NL, "Warpmodus uitschakelen"},
/* pl */ {IDCLS_DISABLE_WARP_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_WARP_MODE_SV, "Inaktivera warpl�ge"},
#endif

/* translate.c */
/* en */ {IDCLS_P_ISO_LANGUAGE_CODE,    N_("<iso language code>")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_P_ISO_LANGUAGE_CODE_DE, "<iso Sprachcode>"},
/* fr */ {IDCLS_P_ISO_LANGUAGE_CODE_FR, "<iso language code>"},
/* hu */ {IDCLS_P_ISO_LANGUAGE_CODE_HU, "<iso nyelv k�d>"},
/* it */ {IDCLS_P_ISO_LANGUAGE_CODE_IT, "<codice iso lingua>"},
/* nl */ {IDCLS_P_ISO_LANGUAGE_CODE_NL, "<iso taalcode>"},
/* pl */ {IDCLS_P_ISO_LANGUAGE_CODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_P_ISO_LANGUAGE_CODE_SV, "<iso-spr�kkod>"},
#endif

/* translate.c */
/* en */ {IDCLS_SPECIFY_ISO_LANG_CODE,    N_("Specify the iso code of the language")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_ISO_LANG_CODE_DE, "Iso Sprachcode spezifizieren"},
/* fr */ {IDCLS_SPECIFY_ISO_LANG_CODE_FR, "Sp�cifier le code iso du langage"},
/* hu */ {IDCLS_SPECIFY_ISO_LANG_CODE_HU, "Adja meg a nyelv iso k�dj�t"},
/* it */ {IDCLS_SPECIFY_ISO_LANG_CODE_IT, "Specifica il codice ISO della lingua"},
/* nl */ {IDCLS_SPECIFY_ISO_LANG_CODE_NL, "Geef de isocode van de taal"},
/* pl */ {IDCLS_SPECIFY_ISO_LANG_CODE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_ISO_LANG_CODE_SV, "Ange ISO-koden f�r spr�ket"},
#endif

/* c64/plus256k.c */
/* en */ {IDCLS_ENABLE_PLUS256K_EXPANSION,    N_("Enable the PLUS256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_PLUS256K_EXPANSION_DE, "PLUS256K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_PLUS256K_EXPANSION_FR, "Activer l'expansion PLUS256K RAM"},
/* hu */ {IDCLS_ENABLE_PLUS256K_EXPANSION_HU, "PLUS256K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_PLUS256K_EXPANSION_IT, "Attiva l'espansione PLUS256K RAM"},
/* nl */ {IDCLS_ENABLE_PLUS256K_EXPANSION_NL, "Activeer de PLUS256K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_PLUS256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_PLUS256K_EXPANSION_SV, "Aktivera PLUS256K RAM-expansion"},
#endif

/* c64/plus256k.c */
/* en */ {IDCLS_DISABLE_PLUS256K_EXPANSION,    N_("Disable the PLUS256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_PLUS256K_EXPANSION_DE, "PLUS256K RAM Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_PLUS256K_EXPANSION_FR, "D�sactiver l'expansion PLUS256K RAM"},
/* hu */ {IDCLS_DISABLE_PLUS256K_EXPANSION_HU, "PLUS256K RAM kiterjeszt�s tilt�sa"},
/* it */ {IDCLS_DISABLE_PLUS256K_EXPANSION_IT, "Disattiva l'espansione PLUS256K RAM"},
/* nl */ {IDCLS_DISABLE_PLUS256K_EXPANSION_NL, "De PLUS256K-geheugenuitbreiding uitschakelen"},
/* pl */ {IDCLS_DISABLE_PLUS256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_PLUS256K_EXPANSION_SV, "Inaktivera PLUS256K RAM-expansion"},
#endif

/* c64/plus256k.c */
/* en */ {IDCLS_SPECIFY_PLUS256K_NAME,    N_("Specify name of PLUS256K image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PLUS256K_NAME_DE, "Name der PLUS256K Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_PLUS256K_NAME_FR, "Sp�cifier le nom de l'image PLUS256K RAM"},
/* hu */ {IDCLS_SPECIFY_PLUS256K_NAME_HU, "Adja meg a PLUS256K-s k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_PLUS256K_NAME_IT, "Specifica il nome dell'immagine PLUS256K"},
/* nl */ {IDCLS_SPECIFY_PLUS256K_NAME_NL, "Geef de naam van het PLUS256K-bestand"},
/* pl */ {IDCLS_SPECIFY_PLUS256K_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PLUS256K_NAME_SV, "Ange namn p� PLUS256K-avbildning"},
#endif

/* c64/plus60k.c */
/* en */ {IDCLS_ENABLE_PLUS60K_EXPANSION,    N_("Enable the PLUS60K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_PLUS60K_EXPANSION_DE, "PLUS60K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_PLUS60K_EXPANSION_FR, "Activer l'expansion PLUS60K RAM"},
/* hu */ {IDCLS_ENABLE_PLUS60K_EXPANSION_HU, "PLUS60K RAM b�v�t�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_PLUS60K_EXPANSION_IT, "Attiva l'espansione PLUS60K RAM"},
/* nl */ {IDCLS_ENABLE_PLUS60K_EXPANSION_NL, "Activeer de PLUS60K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_PLUS60K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_PLUS60K_EXPANSION_SV, "Aktivera PLUS60K RAM-expansion"},
#endif

/* c64/plus60k.c */
/* en */ {IDCLS_DISABLE_PLUS60K_EXPANSION,    N_("Disable the PLUS60K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_PLUS60K_EXPANSION_DE, "PLUS60K RAM Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_PLUS60K_EXPANSION_FR, "D�sactiver l'expansion PLUS60K RAM"},
/* hu */ {IDCLS_DISABLE_PLUS60K_EXPANSION_HU, "PLUS60K RAM b�v�t�s tilt�sa"},
/* it */ {IDCLS_DISABLE_PLUS60K_EXPANSION_IT, "Disattiva l'espansione PLUS60K RAM"},
/* nl */ {IDCLS_DISABLE_PLUS60K_EXPANSION_NL, "De PLUS60K-geheugenuitbreiding uitschakelen"},
/* pl */ {IDCLS_DISABLE_PLUS60K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_PLUS60K_EXPANSION_SV, "Inaktivera PLUS60K RAM-expansion"},
#endif

/* c64/plus60k.c */
/* en */ {IDCLS_SPECIFY_PLUS60K_NAME,    N_("Specify name of PLUS60K image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_PLUS60K_NAME_DE, "Name der PLUS60K Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_PLUS60K_NAME_FR, "Sp�cifier le nom de l'image PLUS60K RAM"},
/* hu */ {IDCLS_SPECIFY_PLUS60K_NAME_HU, "Adja meg a nev�t a PLUS60K k�pm�snak"},
/* it */ {IDCLS_SPECIFY_PLUS60K_NAME_IT, "Specifica il nome dell'immagine PLUS60K"},
/* nl */ {IDCLS_SPECIFY_PLUS60K_NAME_NL, "Geef de naam van het PLUS60K-bestand"},
/* pl */ {IDCLS_SPECIFY_PLUS60K_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_PLUS60K_NAME_SV, "Ange namn p� PLUS60K-avbildning"},
#endif

/* c64/plus60k.c */
/* en */ {IDCLS_PLUS60K_BASE,    N_("Base address of the PLUS60K expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_PLUS60K_BASE_DE, "Basis Adresse f�r PLUS60K Erweiterung"},
/* fr */ {IDCLS_PLUS60K_BASE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_PLUS60K_BASE_HU, "A PLUS60K b�v�t�s b�zisc�me"},
/* it */ {IDCLS_PLUS60K_BASE_IT, "Indirizzo base dell'espansione PLUS60K"},
/* nl */ {IDCLS_PLUS60K_BASE_NL, "Basisadres van de PLUS60K-geheugenuitbreiding"},
/* pl */ {IDCLS_PLUS60K_BASE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_PLUS60K_BASE_SV, "Basadress f�r PLUS60K-expansion"},
#endif

/* c64/c64_256k.c */
/* en */ {IDCLS_ENABLE_C64_256K_EXPANSION,    N_("Enable the 256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_C64_256K_EXPANSION_DE, "256K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_C64_256K_EXPANSION_FR, "Activer l'expansion RAM 256K"},
/* hu */ {IDCLS_ENABLE_C64_256K_EXPANSION_HU, "256K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_C64_256K_EXPANSION_IT, "Attiva l'espansione 256K RAM"},
/* nl */ {IDCLS_ENABLE_C64_256K_EXPANSION_NL, "Activeer de 256K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_C64_256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_C64_256K_EXPANSION_SV, "Aktivera 256K RAM-expansion"},
#endif

/* c64/c64_256k.c */
/* en */ {IDCLS_DISABLE_C64_256K_EXPANSION,    N_("Disable the 256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_C64_256K_EXPANSION_DE, "256K RAM Erweiterung deaktivieren"},
/* fr */ {IDCLS_DISABLE_C64_256K_EXPANSION_FR, "D�sactiver l'expansion RAM 256K"},
/* hu */ {IDCLS_DISABLE_C64_256K_EXPANSION_HU, "256K RAM kiterjeszt�s tilt�sa"},
/* it */ {IDCLS_DISABLE_C64_256K_EXPANSION_IT, "Disattiva l'espansione 256K RAM"},
/* nl */ {IDCLS_DISABLE_C64_256K_EXPANSION_NL, "De 256K-geheugenuitbreiding uitschakelen"},
/* pl */ {IDCLS_DISABLE_C64_256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_C64_256K_EXPANSION_SV, "Inaktivera 256K RAM-expansion"},
#endif

/* c64/c64_256k.c */
/* en */ {IDCLS_SPECIFY_C64_256K_NAME,    N_("Specify name of 256K image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_C64_256K_NAME_DE, "Name der 256K Image Datei definieren"},
/* fr */ {IDCLS_SPECIFY_C64_256K_NAME_FR, "Sp�cifier le nom de l'image 256K"},
/* hu */ {IDCLS_SPECIFY_C64_256K_NAME_HU, "Adja meg a 256K-s k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_C64_256K_NAME_IT, "Specifica il nome dell'immagine 256K"},
/* nl */ {IDCLS_SPECIFY_C64_256K_NAME_NL, "Geef de naam van het 256K-bestand"},
/* pl */ {IDCLS_SPECIFY_C64_256K_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_C64_256K_NAME_SV, "Ange namn p� 256K-avbildning"},
#endif

/* c64/c64_256k.c */
/* en */ {IDCLS_C64_256K_BASE,    N_("Base address of the 256K expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_C64_256K_BASE_DE, "Basis Adresse f�r 256K RAM Erweiterung"},
/* fr */ {IDCLS_C64_256K_BASE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_C64_256K_BASE_HU, "A 256K-s RAM kiterjeszt�s b�zisc�me"},
/* it */ {IDCLS_C64_256K_BASE_IT, "Indirizzo base dell'espansione 256K"},
/* nl */ {IDCLS_C64_256K_BASE_NL, "Basisadres van de 256K-geheugenuitbreiding"},
/* pl */ {IDCLS_C64_256K_BASE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_C64_256K_BASE_SV, "Basadress f�r 256K-expansionen"},
#endif

/* c64/cart/c64cart.c */
/* en */ {IDCLS_ATTACH_RAW_STB_CART,    "Attach raw Structured Basic cartridge image"},
/* de */ {IDCLS_ATTACH_RAW_STB_CART_DE, "Structured Basic (raw) Image einlegen"},
/* fr */ {IDCLS_ATTACH_RAW_STB_CART_FR, "Ins�rer une cartouche Raw Structured Basic"},
/* hu */ {IDCLS_ATTACH_RAW_STB_CART_HU, "Structured Basic cartridge k�pm�s csatol�sa"},
/* it */ {IDCLS_ATTACH_RAW_STB_CART_IT, "Seleziona l'immagine di una cartuccia Structured Basic"},
/* nl */ {IDCLS_ATTACH_RAW_STB_CART_NL, "Koppel binair Structured-Basic-cartridgebestand aan"},
/* pl */ {IDCLS_ATTACH_RAW_STB_CART_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ATTACH_RAW_STB_CART_SV, "Anslut r� Strucured Basic-insticksmodulfil"},

/* plus4/plus4memcsory256k.c */
/* en */ {IDCLS_ENABLE_CS256K_EXPANSION,    N_("Enable the CSORY 256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_CS256K_EXPANSION_DE, "CSORY 256K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_CS256K_EXPANSION_FR, "Activer l'expansion RAM 256K CSORY"},
/* hu */ {IDCLS_ENABLE_CS256K_EXPANSION_HU, "CSORY 256K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_CS256K_EXPANSION_IT, "Attiva l'espansione RAM CSORY 256K"},
/* nl */ {IDCLS_ENABLE_CS256K_EXPANSION_NL, "Activeer de CSORY 256K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_CS256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_CS256K_EXPANSION_SV, "Aktivera CSORY 256K RAM-expansion"},
#endif

/* plus4/plus4memhannes256k.c */
/* en */ {IDCLS_ENABLE_H256K_EXPANSION,    N_("Enable the HANNES 256K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_H256K_EXPANSION_DE, "HANNES 256K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_H256K_EXPANSION_FR, "Activer l'expansion RAM 256K HANNES"},
/* hu */ {IDCLS_ENABLE_H256K_EXPANSION_HU, "HANNES 256K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_H256K_EXPANSION_IT, "Attiva l'espansione RAM HANNES 256K"},
/* nl */ {IDCLS_ENABLE_H256K_EXPANSION_NL, "Activeer de HANNES 256K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_H256K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_H256K_EXPANSION_SV, "Aktivera HANNES 256K RAM-expansion"},
#endif

/* plus4/plus4memhannes256k.c */
/* en */ {IDCLS_ENABLE_H1024K_EXPANSION,    N_("Enable the HANNES 1024K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_H1024K_EXPANSION_DE, "HANNES 1024K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_H1024K_EXPANSION_FR, "Activer l'expansion RAM 1024K HANNES"},
/* hu */ {IDCLS_ENABLE_H1024K_EXPANSION_HU, "HANNES 1024K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_H1024K_EXPANSION_IT, "Attiva l'espansione RAM HANNES 1024K"},
/* nl */ {IDCLS_ENABLE_H1024K_EXPANSION_NL, "Activeer de HANNES 1024K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_H1024K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_H1024K_EXPANSION_SV, "Aktivera HANNES 1024K RAM-expansion"},
#endif

/* plus4/plus4memhannes256k.c */
/* en */ {IDCLS_ENABLE_H4096K_EXPANSION,    N_("Enable the HANNES 4096K RAM expansion")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_H4096K_EXPANSION_DE, "HANNES 4096K RAM Erweiterung aktivieren"},
/* fr */ {IDCLS_ENABLE_H4096K_EXPANSION_FR, "Activer l'expansion RAM 4096K HANNES"},
/* hu */ {IDCLS_ENABLE_H4096K_EXPANSION_HU, "HANNES 4096K RAM kiterjeszt�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_H4096K_EXPANSION_IT, "Attiva l'espansione RAM HANNES 4096K"},
/* nl */ {IDCLS_ENABLE_H4096K_EXPANSION_NL, "Activeer de HANNES 4096K-geheugenuitbreiding"},
/* pl */ {IDCLS_ENABLE_H4096K_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_H4096K_EXPANSION_SV, "Aktivera HANNES 4096K RAM-expansion"},
#endif

/* c64dtv/c64dtvblitter.c */
/* en */ {IDCLS_SPECIFY_DTV_REVISION,    N_("Specify DTV Revision (2: DTV2, 3: DTV3)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_DTV_REVISION_DE, "DTV Revision definieren (2: DTV2, 3: DTV3)"},
/* fr */ {IDCLS_SPECIFY_DTV_REVISION_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_DTV_REVISION_HU, "Adja meg a DTV verzi�j�t (2: DTV2, 3: DTV3)"},
/* it */ {IDCLS_SPECIFY_DTV_REVISION_IT, "Specifica la revisione DTV (2: DTV2, 3: DTV3)"},
/* nl */ {IDCLS_SPECIFY_DTV_REVISION_NL, "Geef de DTV revisie (2: DTV2, 3: DTV3)"},
/* pl */ {IDCLS_SPECIFY_DTV_REVISION_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_DTV_REVISION_SV, "Ange DTV-revision (2: DTV2, 3: DTV3)"},
#endif

/* c64dtv/c64dtvblitter.c */
/* en */ {IDCLS_ENABLE_DTV_BLITTER_LOG,    N_("Enable DTV blitter logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DTV_BLITTER_LOG_DE, "DTV Blitter Log aktivieren."},
/* fr */ {IDCLS_ENABLE_DTV_BLITTER_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_DTV_BLITTER_LOG_HU, "DTV blit napl�k enged�lyez�se."},
/* it */ {IDCLS_ENABLE_DTV_BLITTER_LOG_IT, "Attiva log blitter DTV."},
/* nl */ {IDCLS_ENABLE_DTV_BLITTER_LOG_NL, "Aktiveer DTV blitter logs."},
/* pl */ {IDCLS_ENABLE_DTV_BLITTER_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DTV_BLITTER_LOG_SV, "Aktivera DTV-blitterloggar."},
#endif

/* c64dtv/c64dtvblitter.c */
/* en */ {IDCLS_DISABLE_DTV_BLITTER_LOG,    N_("Disable DTV blitter logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DTV_BLITTER_LOG_DE, "DTV Blitter Log deaktivieren."},
/* fr */ {IDCLS_DISABLE_DTV_BLITTER_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_DTV_BLITTER_LOG_HU, "DTV blit napl�k tilt�sa."},
/* it */ {IDCLS_DISABLE_DTV_BLITTER_LOG_IT, "Disattiva log blitter DTV."},
/* nl */ {IDCLS_DISABLE_DTV_BLITTER_LOG_NL, "DTV Blitter logs afsluiten."},
/* pl */ {IDCLS_DISABLE_DTV_BLITTER_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DTV_BLITTER_LOG_SV, "Inaktivera DTV-blitterloggar."},
#endif

/* c64dtv/c64dtvdma.c */
/* en */ {IDCLS_ENABLE_DTV_DMA_LOG,    N_("Enable DTV DMA logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DTV_DMA_LOG_DE, "DTV DMA Logs aktivieren."},
/* fr */ {IDCLS_ENABLE_DTV_DMA_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_DTV_DMA_LOG_HU, "DTV DMA napl�k enged�lyez�se."},
/* it */ {IDCLS_ENABLE_DTV_DMA_LOG_IT, "Attiva log DMA DTV."},
/* nl */ {IDCLS_ENABLE_DTV_DMA_LOG_NL, "Aktiveer DTV DMA logs."},
/* pl */ {IDCLS_ENABLE_DTV_DMA_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DTV_DMA_LOG_SV, "Aktivera DTV-dma-loggar."},
#endif

/* c64dtv/c64dtvdma.c */
/* en */ {IDCLS_DISABLE_DTV_DMA_LOG,    N_("Disable DTV DMA logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DTV_DMA_LOG_DE, "DTV DMA Logs deaktivieren."},
/* fr */ {IDCLS_DISABLE_DTV_DMA_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_DTV_DMA_LOG_HU, "DTV dma napl�k tilt�sa."},
/* it */ {IDCLS_DISABLE_DTV_DMA_LOG_IT, "Disattiva log DMA DTV."},
/* nl */ {IDCLS_DISABLE_DTV_DMA_LOG_NL, "DTV DMA logs afsluiten."},
/* pl */ {IDCLS_DISABLE_DTV_DMA_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DTV_DMA_LOG_SV, "Inaktivera DTV-dma-loggar."},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDCLS_SPECIFY_C64DTVROM_NAME,    N_("Specify name of C64DTV ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_C64DTVROM_NAME_DE, "Name von C64DTV ROM Datei definieren"},
/* fr */ {IDCLS_SPECIFY_C64DTVROM_NAME_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_C64DTVROM_NAME_HU, "Adja meg a C64DTV ROM k�pm�s nev�t"},
/* it */ {IDCLS_SPECIFY_C64DTVROM_NAME_IT, "Specifica il nome dell'immagine ROM DTV"},
/* nl */ {IDCLS_SPECIFY_C64DTVROM_NAME_NL, "Geef de naam van her C64DTV ROM bestand"},
/* pl */ {IDCLS_SPECIFY_C64DTVROM_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_C64DTVROM_NAME_SV, "Ange namn p� C64DTV-ROM-avbildning"},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDCLS_ENABLE_C64DTVROM_RW,    N_("Enable writing to C64DTV ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_C64DTVROM_RW_DE, "Schreibzugriff auf C64DTV ROM Image erlauben"},
/* fr */ {IDCLS_ENABLE_C64DTVROM_RW_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_C64DTVROM_RW_HU, "C64DTV ROM k�pm�sba �r�s enged�lyez�se"},
/* it */ {IDCLS_ENABLE_C64DTVROM_RW_IT, "Attiva la scrittura sull'immagine ROM C64DTV"},
/* nl */ {IDCLS_ENABLE_C64DTVROM_RW_NL, "Aktiveer schrijven naar C64DTV ROM bestand"},
/* pl */ {IDCLS_ENABLE_C64DTVROM_RW_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_C64DTVROM_RW_SV, "Aktivera skrivning till C64DTV-ROM-avbildning."},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDCLS_DISABLE_C64DTVROM_RW,    N_("Disable writing to C64DTV ROM image")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_C64DTVROM_RW_DE, "Schreibzugriff auf C64DTV ROM Image verhindern"},
/* fr */ {IDCLS_DISABLE_C64DTVROM_RW_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_C64DTVROM_RW_HU, "C64DTV ROM k�pm�sba �r�s tilt�sa"},
/* it */ {IDCLS_DISABLE_C64DTVROM_RW_IT, "Disattiva la scrittura sull'immagine ROM C64DTV"},
/* nl */ {IDCLS_DISABLE_C64DTVROM_RW_NL, "Schrijven naar C64DTV ROM bestand afsluiten"},
/* pl */ {IDCLS_DISABLE_C64DTVROM_RW_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_C64DTVROM_RW_SV, "Inaktivera skrivning till C64DTV-ROM-avbildning."},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDCLS_ENABLE_DTV_FLASH_LOG,    N_("Enable DTV flash chip logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DTV_FLASH_LOG_DE, "DTV Flashchip Log aktivieren."},
/* fr */ {IDCLS_ENABLE_DTV_FLASH_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_DTV_FLASH_LOG_HU, "DTV flash chip napl�k enged�lyez�se."},
/* it */ {IDCLS_ENABLE_DTV_FLASH_LOG_IT, "Attiva i log del chip flash DTV."},
/* nl */ {IDCLS_ENABLE_DTV_FLASH_LOG_NL, "Aktiveer DTV flash chip logs."},
/* pl */ {IDCLS_ENABLE_DTV_FLASH_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DTV_FLASH_LOG_SV, "Aktivera loggar f�r DTV-flashkrets."},
#endif

/* c64dtv/c64dtvflash.c */
/* en */ {IDCLS_DISABLE_DTV_FLASH_LOG,    N_("Disable DTV flash chip logs.")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DTV_FLASH_LOG_DE, "DTV Flashchip Log deaktivieren."},
/* fr */ {IDCLS_DISABLE_DTV_FLASH_LOG_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_DTV_FLASH_LOG_HU, "DTV flash chip napl�k tilt�sa."},
/* it */ {IDCLS_DISABLE_DTV_FLASH_LOG_IT, "Disattiva i log del chip flash DTV."},
/* nl */ {IDCLS_DISABLE_DTV_FLASH_LOG_NL, "DTV flash chip logs afsluiten."},
/* pl */ {IDCLS_DISABLE_DTV_FLASH_LOG_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DTV_FLASH_LOG_SV, "Inaktivera loggar f�r DTV-flashkrets."},
#endif

/* c64dtv/flash-trap.c */
/* en */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS,    N_("Use <name> as directory for flash file system device")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_DE, "Benutze <Name> f�r Verzeichnis Ger�t Flash Dateisystem"},
/* fr */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_HU, "<n�v> k�nyvt�r haszn�lata a flash f�jlrendszer eszk�zh�z"},
/* it */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_IT, "Una <nome> come directory per il file system flash"},
/* nl */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_NL, "Gebruik <naam> als directory voor het flash bestandssysteem apparaat"},
/* pl */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_USE_AS_DIRECTORY_FLASH_FS_SV, "Ange <namn> som katalog f�r filsystemsbaserad flashenhet"},
#endif

/* c64dtv/flash-trap.c */
/* en */ {IDCLS_ENABLE_TRUE_FLASH_FS,    N_("Enable true hardware flash file system")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_TRUE_FLASH_FS_DE, "Pr�zises Hardware Flashdateisystem aktivieren"},
/* fr */ {IDCLS_ENABLE_TRUE_FLASH_FS_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_TRUE_FLASH_FS_HU, "Igazi hardver flash f�jlrendszer enged�lyez�se"},
/* it */ {IDCLS_ENABLE_TRUE_FLASH_FS_IT, "Attiva l'emulazione hardware del file system flash"},
/* nl */ {IDCLS_ENABLE_TRUE_FLASH_FS_NL, "Activeer hardwarmatige flash bestandssysteem"},
/* pl */ {IDCLS_ENABLE_TRUE_FLASH_FS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_TRUE_FLASH_FS_SV, "Aktivera maskinvarubaserat flash-filsystem"},
#endif

/* c64dtv/flash-trap.c */
/* en */ {IDCLS_DISABLE_TRUE_FLASH_FS,    N_("Disable true hardware flash file system")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_TRUE_FLASH_FS_DE, "Pr�zises Hardware Flashdateisystem deaktivieren"},
/* fr */ {IDCLS_DISABLE_TRUE_FLASH_FS_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_TRUE_FLASH_FS_HU, "Igazi hardver flash f�jlrendszer tilt�sa"},
/* it */ {IDCLS_DISABLE_TRUE_FLASH_FS_IT, "Disattiva l'emulazione hardware del file system flash"},
/* nl */ {IDCLS_DISABLE_TRUE_FLASH_FS_NL, "Hardwatematig flash bestandssysteem uitschakelen"},
/* pl */ {IDCLS_DISABLE_TRUE_FLASH_FS_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_TRUE_FLASH_FS_SV, "Inaktivera maskinvaruemelring f�r flashfilsystem"},
#endif

#ifdef HAVE_MIDI
/* midi.c */
/* en */ {IDCLS_ENABLE_MIDI_EMU,    N_("Enable MIDI emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_MIDI_EMU_DE, "MIDI Emulation aktivieren"},
/* fr */ {IDCLS_ENABLE_MIDI_EMU_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_ENABLE_MIDI_EMU_HU, "MIDI emul�ci� enged�lyez�se"},
/* it */ {IDCLS_ENABLE_MIDI_EMU_IT, "Attiva l'emulazione MIDI"},
/* nl */ {IDCLS_ENABLE_MIDI_EMU_NL, "Activeer MIDI emulatie"},
/* pl */ {IDCLS_ENABLE_MIDI_EMU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_MIDI_EMU_SV, "Aktivera MIDI-emulering"},
#endif

/* midi.c */
/* en */ {IDCLS_DISABLE_MIDI_EMU,    N_("Disable MIDI emulation")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_MIDI_EMU_DE, "MIDI Emulation deaktivieren"},
/* fr */ {IDCLS_DISABLE_MIDI_EMU_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_DISABLE_MIDI_EMU_HU, "MIDI emul�ci� tilt�sa"},
/* it */ {IDCLS_DISABLE_MIDI_EMU_IT, "Disattiva l'emulazione MIDI"},
/* nl */ {IDCLS_DISABLE_MIDI_EMU_NL, "MIDI emulatie uitschakelen"},
/* pl */ {IDCLS_DISABLE_MIDI_EMU_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_MIDI_EMU_SV, "Inaktivera MIDI-emulering"},
#endif

/* c64/c64-midi.c */
/* en */ {IDCLS_SPECIFY_C64_MIDI_TYPE,    N_("MIDI interface type (0: Sequential, 1: Passport, 2: DATEL, 3: Namesoft, 4: "
                                             "Maplin)")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_SPECIFY_C64_MIDI_TYPE_DE, "MIDI Interface Typ (0: Sequential, 1: Passport, 2: DATEL, 3: Namesoft, 4: Maplin)"},
/* fr */ {IDCLS_SPECIFY_C64_MIDI_TYPE_FR, ""},  /* fuzzy */
/* hu */ {IDCLS_SPECIFY_C64_MIDI_TYPE_HU, "MIDI interf�sz t�pus (0: soros, 1: Passport, 2: DATEL, 3: Namesoft, 4: Maplin)"},
/* it */ {IDCLS_SPECIFY_C64_MIDI_TYPE_IT, "Tipo interfaccia MIDI (0: Sequential, 1: Passport, 2: DATEL, 3: Namesoft, 4: "
                                          "Maplin)"},
/* nl */ {IDCLS_SPECIFY_C64_MIDI_TYPE_NL, "MIDI interface soort (0: Sequential, 1: Passport, 2: DATEL, 3: Namesoft, 4: "
                                          "Maplin)"},
/* pl */ {IDCLS_SPECIFY_C64_MIDI_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_SPECIFY_C64_MIDI_TYPE_SV, "Typ av MIDI-gr�nssnitt (0: sekventiell, 1: Passport, 2: DATEL, 3: Namesoft, "
                                          "4: Maplin)"},
#endif
#endif

/* c64/digimax.c */
/* en */ {IDCLS_ENABLE_DIGIMAX,    N_("Enable the digimax cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_ENABLE_DIGIMAX_DE, "Digimax Cartridge aktivieren"},
/* fr */ {IDCLS_ENABLE_DIGIMAX_FR, "Activer la cartouche Digimax"},
/* hu */ {IDCLS_ENABLE_DIGIMAX_HU, "Digimax cartridge enged�lyez�se"},
/* it */ {IDCLS_ENABLE_DIGIMAX_IT, "Attiva la cartuccia digimax"},
/* nl */ {IDCLS_ENABLE_DIGIMAX_NL, "Activeer de digimaxcartridge"},
/* pl */ {IDCLS_ENABLE_DIGIMAX_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_ENABLE_DIGIMAX_SV, "Aktivera Digimax-cartridge"},
#endif

/* c64/digimax.c */
/* en */ {IDCLS_DISABLE_DIGIMAX,    N_("Disable the digimax cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DISABLE_DIGIMAX_DE, "Digimax Cartridge deaktivieren"},
/* fr */ {IDCLS_DISABLE_DIGIMAX_FR, "D�sactiver la cartouche Digimax"},
/* hu */ {IDCLS_DISABLE_DIGIMAX_HU, "Digimax cartridge tilt�sa"},
/* it */ {IDCLS_DISABLE_DIGIMAX_IT, "Disattiva la cartuccia digimax"},
/* nl */ {IDCLS_DISABLE_DIGIMAX_NL, "De digimaxcartridge uitschakelen"},
/* pl */ {IDCLS_DISABLE_DIGIMAX_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DISABLE_DIGIMAX_SV, "Inaktivera Digimax Cartridge"},
#endif

/* c64/digimax.c */
/* en */ {IDCLS_DIGIMAX_BASE,    N_("Base address of the digimax cartridge")},
#ifdef HAS_TRANSLATION
/* de */ {IDCLS_DIGIMAX_BASE_DE, "Basis Adresse f�r Digimax Erweiterung"},
/* fr */ {IDCLS_DIGIMAX_BASE_FR, "Adresse de base de la cartouche Digimax"},
/* hu */ {IDCLS_DIGIMAX_BASE_HU, "A digimax cartridge b�zisc�me"},
/* it */ {IDCLS_DIGIMAX_BASE_IT, "Indirizzo base della cartuccia digimax"},
/* nl */ {IDCLS_DIGIMAX_BASE_NL, "Basisadres van de digimaxcartridge"},
/* pl */ {IDCLS_DIGIMAX_BASE_PL, ""},  /* fuzzy */
/* sv */ {IDCLS_DIGIMAX_BASE_SV, "Basadress f�r Digimax-cartridge"}
#endif

};

/* --------------------------------------------------------------------- */

static char *get_string_by_id(int id)
{
  unsigned int k;

  for (k = 0; k < countof(string_table); k++)
  {
    if (string_table[k].resource_id==id)
      return string_table[k].text;
  }
  return NULL;
}

#ifdef HAS_TRANSLATION
#include "translate_table.h"

static char *text_table[countof(translate_text_table)][countof(language_table)];

static void translate_text_init(void)
{
  unsigned int i,j;
  char *temp;

  for (i = 0; i < countof(language_table); i++)
  {
    for (j = 0; j < countof(translate_text_table); j++)
    {
      if (translate_text_table[j][i]==0)
        text_table[j][i]=NULL;
      else
      {
        temp=get_string_by_id(translate_text_table[j][i]);
        text_table[j][i]=intl_convert_cp(temp, language_cp_table[i]);
      }
    }
  }
}

char translate_id_error_text[30];

char *translate_text(int en_resource)
{
  unsigned int i;
  char *retval = NULL;

  if (en_resource == IDCLS_UNUSED)
      return NULL;

  if (en_resource == 0)
  {
    log_error(LOG_DEFAULT, "TRANSLATE ERROR: ID 0 was requested.");
    return "ID 0 translate error";
  }

  if (en_resource < 0x10000)
  {
    retval = intl_translate_text(en_resource);
  }
  else
  {
    for (i = 0; i < countof(translate_text_table); i++)
    {
      if (translate_text_table[i][0] == en_resource)
      {
        if (translate_text_table[i][current_language_index]!=0 &&
            text_table[i][current_language_index]!=NULL &&
            strlen(text_table[i][current_language_index])!=0)
          retval = text_table[i][current_language_index];
        else
          retval = text_table[i][0];
      }
    }
  }

  if (retval == NULL)
  {
    log_error(LOG_DEFAULT, "TRANSLATE ERROR: ID %d was requested, and would be returning NULL.",en_resource);
    sprintf(translate_id_error_text,"ID %d translate error",en_resource);
    retval = translate_id_error_text;
  }

  return retval;
}

int translate_res(int en_resource)
{
  return intl_translate_res(en_resource);
}

/* --------------------------------------------------------------------- */

static int set_current_language(const char *lang, void *param)
{
    unsigned int i;

    util_string_set(&current_language, "en");
    current_language_index = 0;

    if (strlen(lang) != 2)
        return 0;

    for (i = 0; i < countof(language_table); i++) {
        if (!strcasecmp(lang,language_table[i])) {
            current_language_index=i;
            util_string_set(&current_language, language_table[i]);
            intl_update_ui();
            return 0;
        }
    }

    return 0;
}

static const resource_string_t resources_string[] = {
    { "Language", "en", RES_EVENT_NO, NULL,
      &current_language, set_current_language, NULL },
    { NULL }
};

int translate_resources_init(void)
{
  intl_init();
  translate_text_init();

  return resources_register_string(resources_string);
}

void translate_resources_shutdown(void)
{
  unsigned int i,j;

  for (i = 0; i < countof(language_table); i++)
  {
    for (j = 0; j < countof(translate_text_table); j++)
    {
      if (text_table[j][i]!=NULL)
        lib_free(text_table[j][i]);
    }
  }
  intl_shutdown();
  lib_free(current_language);
}

static const cmdline_option_t cmdline_options[] =
{
    { "-lang", SET_RESOURCE, 1,
      NULL, NULL, "Language", NULL,
      USE_PARAM_ID, USE_DESCRIPTION_ID,
      IDCLS_P_ISO_LANGUAGE_CODE, IDCLS_SPECIFY_ISO_LANG_CODE,
      NULL, NULL },
    { NULL }
};

int translate_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}

void translate_arch_language_init(void)
{
  char *lang;

  lang=intl_arch_language_init();
  set_current_language(lang, "");
}
#else

char *translate_text(int en_resource)
{
  if (en_resource == IDCLS_UNUSED)
      return NULL;

  if (en_resource == 0)
  {
    log_error(LOG_DEFAULT, "TRANSLATE ERROR: ID 0 was requested.");
    return "ID 0 translate error";
  }

  return _(get_string_by_id(en_resource));
}
#endif
