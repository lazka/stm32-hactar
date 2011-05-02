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
the one included in the std periph is a modificated version, so don't try
to update it.

The std library also includes a utilities folder which includes drivers for
all ST eval boards.

**Eclipse error: make: \*\*\* No rule to make target `project_name.elf',
needed by `all'.  Stop.**

You probably tried to change the toolchain after
creating the project. This seems to be a known bug:
http://sourceforge.net/tracker/index.php?func=detail&aid=3027456&group_id=181918&atid=899126
You have to create a new project to get it working again.

