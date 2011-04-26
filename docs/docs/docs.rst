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
