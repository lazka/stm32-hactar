=============
STM32LIB Docs
=============

Documentation
-------------

FIXME

FAQ
---

**Is there a prepackaged IDE including a basic project?**

Have at look in /projects/stm32lib on the AFS server.

**Wich libraries are available, which ones do I need?**

Basically only the standard peripheral library from ST is needed. It includes
a version of the CMSIS library, which is a standardized abstraction layer for
ARM chip producers. Even thought there is a newer CMSIS available at ARM,
the one included in the std periph is a modified version, so don't try
to update it.

The std library also includes a utilities folder which includes drivers for
all ST eval boards.

**Eclipse error: make: \*\*\* No rule to make target `project_name.elf', needed by `all'.  Stop.**

You probably tried to change the toolchain after
creating the project. This seems to be a known bug:
http://sourceforge.net/tracker/index.php?func=detail&aid=3027456&group_id=181918&atid=899126
You have to create a new project to get it working again.

** Eclipse doesn't execute post compile actions like "Create Flash Image", "Create Listing" or "Print Listing".**

Probably a bug because something was changed in the main `C/C++ Build` panel.
Workarround: Add the needed commands manually under `C/C++ Build -> Settings
-> Build Steps -> Post-build steps`

** The GDB server dies everytime I start it, returning -256 or -7 ... **

Hold down the STM32 reset button while starting the GDB server.

** I get "Error: registers may not be the same -- `strexb r0,r0,[r1]'" while compiling **

You are hitting a bug in the CMSIS library (probably while using -Os).
Change "=r" to "=&r" in the functions __STREXB and __STREXH which you
can both find in core_cm3.c
