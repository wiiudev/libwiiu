Build Instructions:
* python build.py <project path>
* python build.py examples/osscreendmo

Notes:
* examples folder contains projects using OSScreen.
* oldexamples don't use OSScreen.

Credits:
* Relys - Complete refactor of exploit framework, build process and scripts. OSResetGame trick to kill threads discovery. Reverse engineering OSScreen functions. New examples
* Hykem - Reverse engineering OSScreen use in OSFatal
* Marionumber1 - Without him, none of this would be possible.
* Comex - Being a Wizard

Original WebKit Exploit Credits:
* Marionumber1 - ROP chain design/implementation
* TheKit - WebKit bug finding, ROP chain implementation
* Hykem - Finding ROP gadgets
* bubba - Testing WebKit bug candidates
* comex - Access to the coreinit and WebKit binaries
* Chadderz - Blind memory dumping on 5.0.0
* Relys - Testing exploits